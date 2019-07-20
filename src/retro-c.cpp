#include "retro-c.h"

#include "coreinfo.h"
#include "data.h"
#include "emulator.h"
#include "imageops.h"
#include "memory.h"
#include "search.h"
#include "script.h"
#include "movie.h"
#include "movie-bk2.h"

#include <map>
#include <unordered_map>
#include <unordered_set>

using namespace Retro;

CEmulator* emulatorCreate(const char* romPath) {
  if (Emulator::isLoaded()) {
    fprintf(
      stderr,
      "Cannot create multiple emulator instances per process, make sure to call "
      "'env.close()' on each environment before creating a new one.");
    return nullptr;
  }
  Emulator* m_re = new Emulator();
  if (!m_re->loadRom(romPath)) {
    fprintf(stderr, "Could not load ROM.");
    delete m_re;
    return nullptr;
  }
  // The following is necessary because otherwise, you get a segfault 
  // when you try to get the screen for the first time.
  m_re->run();
  return new CEmulator {m_re, 0};
}

void emulatorDelete(CEmulator* emulator) {
  delete static_cast<Emulator*>(emulator->emulator);
  delete emulator;
}

void emulatorStep(CEmulator* emulator) {
  static_cast<Emulator*>(emulator->emulator)->run();
}

CBytes* emulatorGetState(CEmulator* emulator) {
  auto* m_re = static_cast<Emulator*>(emulator->emulator);
  size_t numBytes = m_re->serializeSize();
  void* bytes = malloc(numBytes);
  m_re->serialize(bytes, numBytes);
  return new CBytes {reinterpret_cast<const uint8_t*>(bytes), numBytes};
}

void emulatorStateDelete(CBytes* state) {
  delete state->bytes;
  delete state;
}

bool emulatorSetState(CEmulator* emulator, CBytes* state) {
  return static_cast<Emulator*>(emulator->emulator)->unserialize(state->bytes, state->numBytes);
}

CEmulatorScreen* emulatorGetScreen(CEmulator* emulator) {
  auto* m_re = static_cast<Emulator*>(emulator->emulator);
  long w = m_re->getImageWidth();
  long h = m_re->getImageHeight();
  uint8_t* data = new uint8_t[w * h * 3];
  Image out(Image::Format::RGB888, data, w, h, w);
  Image in;
  if (m_re->getImageDepth() == 16) {
    in = Image(Image::Format::RGB565, m_re->getImageData(), w, h, m_re->getImagePitch());
  } else if (m_re->getImageDepth() == 32) {
    in = Image(Image::Format::RGBX888, m_re->getImageData(), w, h, m_re->getImagePitch());
  }
  in.copyTo(&out);
  return new CEmulatorScreen {data, (size_t) w, (size_t) h, 3};
}

void emulatorScreenDelete(CEmulatorScreen* screen) {
  delete screen->values;
  delete screen;
}

double emulatorGetScreenRate(CEmulator* emulator) {
  return static_cast<Emulator*>(emulator->emulator)->getFrameRate();
}

CEmulatorAudio* emulatorGetAudio(CEmulator* emulator) {
  auto* m_re = static_cast<Emulator*>(emulator->emulator);
  size_t numSamples = m_re->getAudioSamples() * 2;
  int16_t* samples = new int16_t[numSamples];
  memcpy(samples, m_re->getAudioData(), numSamples * 2);
  return new CEmulatorAudio {samples, numSamples};
}

void emulatorAudioDelete(CEmulatorAudio* audio) {
  delete audio->samples;
  delete audio;
}

double emulatorGetAudioRate(CEmulator* emulator) {
  return static_cast<Emulator*>(emulator->emulator)->getAudioRate();
}

CEmulatorResolution* emulatorGetResolution(CEmulator* emulator) {
  auto* m_re = static_cast<Emulator*>(emulator->emulator);
  auto w = m_re->getImageWidth();
  auto h = m_re->getImageHeight();
  return new CEmulatorResolution {w, h};
}

void emulatorResolutionDelete(CEmulatorResolution* resolution) {
  delete resolution;
}

void emulatorSetButtonMask(CEmulator* emulator, const uint8_t* mask, size_t maskSize, unsigned int player) {
  if (maskSize > N_BUTTONS) {
    fprintf(stderr, "mask.size() > N_BUTTONS.");
    return;
  }
  if (player >= MAX_PLAYERS) {
    fprintf(stderr, "player >= MAX_PLAYERS.");
    return;
  }
  for (int key = 0; key < maskSize; key++) {
    static_cast<Emulator*>(emulator->emulator)->setKey(player, key, mask[key]);
  }
}

void emulatorAddCheat(CEmulator* emulator, const char* code) {
  static_cast<Emulator*>(emulator->emulator)->setCheat(emulator->cheats, true, code);
  emulator->cheats++;
}

void emulatorClearCheats(CEmulator* emulator) {
  static_cast<Emulator*>(emulator->emulator)->clearCheats();
  emulator->cheats = 0;
}

void emulatorConfigureData(CEmulator* emulator, CGameData* gameData) {
  static_cast<Emulator*>(emulator->emulator)->configureData(static_cast<GameData*>(gameData->data));
}

CMemoryView* memoryViewCreate(void* addressSpace) {
  return new CMemoryView { static_cast<AddressSpace*>(addressSpace) };
}

void memoryViewDelete(CMemoryView* memoryView) {
  delete memoryView;
}

int64_t memoryViewExtract(CMemoryView* memoryView, size_t address, const char* type) {
  return (*static_cast<AddressSpace*>(memoryView->addressSpace))[Variable{ type, address }];
}

void memoryViewAssign(CMemoryView* memoryView, size_t address, const char* type, int64_t value) {
  (*static_cast<AddressSpace*>(memoryView->addressSpace))[Variable{ type, address }] = value;
}

CMemoryBlocks* memoryViewBlocks(CMemoryView* memoryView) {
  auto& internalBlocks = static_cast<AddressSpace*>(memoryView->addressSpace)->blocks();
  auto numBlocks = internalBlocks.size();
  auto* blocks = new CMemoryBlock[numBlocks];
  auto i = 0;
  for (const auto& iter : internalBlocks) {
    blocks[i] = {iter.first, static_cast<const uint8_t*>(iter.second.offset(0)), iter.second.size()};
    i++;
  }
  return new CMemoryBlocks {blocks, numBlocks};
}

void memoryViewBlocksDelete(CMemoryBlocks* memoryBlocks) {
  delete memoryBlocks->blocks;
  delete memoryBlocks;
}

CSearch* searchCreate(const char** types, size_t numTypes) {
  Search* search = nullptr;
  if (numTypes > 0) {
    std::vector<DataType> dtypes;
    for (int i = 0; i < numTypes; i++) {
      dtypes.emplace_back(types[i]);
    }
    search = new Search(dtypes);
  }
  return new CSearch {search, true};
}

CSearch* searchCreateUnmanaged(Retro::Search* search) {
  return new CSearch {search, false};
}

void searchDelete(CSearch* search) {
  if (search->managed) {
    delete static_cast<Search*>(search->search);
  }
  delete search;
}

int searchNumResults(CSearch* search) {
  return static_cast<Search*>(search->search)->numResults();
}

bool searchHasUniqueResult(CSearch* search) {
  return static_cast<Search*>(search->search)->hasUniqueResult();
}

CSearchResult* searchUniqueResult(CSearch* search) {
  TypedSearchResult result = static_cast<Search*>(search->search)->uniqueResult();
  return new CSearchResult {result.address, strdup(result.type.type)};
}

void searchResultDelete(CSearchResult* searchResult) {
  delete searchResult->type;
  delete searchResult;
}

CSearchTypedResults* searchTypedResults(CSearch* search) {
  std::map<SearchResult, std::unordered_set<DataType>> results;
  for (const auto& result : static_cast<Search*>(search->search)->typedResults()) {
    results[static_cast<const SearchResult&>(result)].emplace(result.type);
  }
  CSearchTypedResult* cResults = new CSearchTypedResult[results.size()];
  int i = 0;
  for (const auto& result : results) {
    const char** types = new const char*[result.second.size()];
    int j = 0;
    for (const auto& type : result.second) {
      types[j] = type.type;
      j++;
    }
    cResults[i] = {
      result.first.address,
      result.first.mult,
      result.first.div,
      result.first.bias,
      types,
      result.second.size()};
    i++;
  }
  return new CSearchTypedResults {cResults, results.size()};
}

void searchTypedResultsDelete(CSearchTypedResults* searchTypedResults) {
  for (int i = 0; i < searchTypedResults->numResults; i++) {
    for (int j = 0; j < searchTypedResults->results[i].numTypes; j++) {
      delete searchTypedResults->results[i].types[j];
    }
  }
  delete searchTypedResults->results;
  delete searchTypedResults;
}

CGameData* gameDataCreate() {
  auto* data = new GameData();
  auto* scenario = new Scenario(*data);
  return new CGameData {data, scenario};
}

void gameDataDelete(CGameData* gameData) {
  delete static_cast<GameData*>(gameData->data);
  delete static_cast<Scenario*>(gameData->scenario);
  delete gameData;
}

bool gameDataLoad(CGameData* gameData, const char* dataFilename, const char* scenarioFilename) {
  ScriptContext::reset();

  bool success = true;
  if (dataFilename != nullptr) {
    success = success && static_cast<GameData*>(gameData->data)->load(dataFilename);
  }
  if (scenarioFilename != nullptr) {
    success = success && static_cast<Scenario*>(gameData->scenario)->load(scenarioFilename);
  }
  return success;
}

bool gameDataSave(CGameData* gameData, const char* dataFilename, const char* scenarioFilename) {
  bool success = true;
  if (dataFilename != nullptr) {
    success = success && static_cast<GameData*>(gameData->data)->save(dataFilename);
  }
  if (scenarioFilename != nullptr) {
    success = success && static_cast<Scenario*>(gameData->scenario)->save(scenarioFilename);
  }
  return success;
}

void gameDataReset(CGameData* gameData) {
  auto* m_scen = static_cast<Scenario*>(gameData->scenario);
  m_scen->restart();
  m_scen->reloadScripts();
}

uint16_t gameDataFilterAction(CGameData* gameData, uint16_t action) {
  return static_cast<Scenario*>(gameData->scenario)->filterAction(action);
}

CValidActions* gameDataValidActions(CGameData* gameData) {
  auto* m_scen = static_cast<Scenario*>(gameData->scenario);
  std::map<int, std::set<int>> validActions = m_scen->validActions();
  size_t numActionsOuter = validActions.size();
  int** actions = new int*[numActionsOuter];
  size_t* numActionsInner = new size_t[numActionsOuter];
  int i = 0;
  for (const auto& action : validActions) {
    numActionsInner[i] = action.second.size();
    actions[i] = new int[numActionsInner[i]];
    int j = 0;
    for (const auto& act : action.second) {
      actions[i][j] = act;
      j++;
    }
    i++;
  }
  return new CValidActions {actions, numActionsInner, numActionsOuter};
}

void gameDataValidActionsDelete(CValidActions* validActions) {
  for (int i = 0; i < validActions->numActionsOuter; i++) {
    delete validActions->actions[i];
  }
  delete validActions->numActionsInner;
  delete validActions;
}

void gameDataUpdateRam(CGameData* gameData) {
  static_cast<GameData*>(gameData->data)->updateRam();
  static_cast<Scenario*>(gameData->scenario)->update();
}

bool gameDataLookupBoolValue(CGameData* gameData, const char* name) {
  return bool(static_cast<GameData*>(gameData->data)->lookupValue(name));
}

int64_t gameDataLookupIntValue(CGameData* gameData, const char* name) {
  return int64_t(static_cast<GameData*>(gameData->data)->lookupValue(name));
}

double gameDataLookupDoubleValue(CGameData* gameData, const char* name) {
  return double(static_cast<GameData*>(gameData->data)->lookupValue(name));
}

void gameDataSetBoolValue(CGameData* gameData, const char* name, bool value) {
  static_cast<GameData*>(gameData->data)->setValue(name, Variant(value));
}

void gameDataSetIntValue(CGameData* gameData, const char* name, int64_t value) {
  static_cast<GameData*>(gameData->data)->setValue(name, Variant(value));
}

void gameDataSetDoubleValue(CGameData* gameData, const char* name, double value) {
  static_cast<GameData*>(gameData->data)->setValue(name, Variant(value));
}

CNames* gameDataLookupKeys(CGameData* gameData) {
  auto* m_data = static_cast<GameData*>(gameData->data);
  std::unordered_map<std::string, Datum> allValues = m_data->lookupAll();
  const char** namesArray = new const char*[allValues.size()];
  auto i = 0;
  for (const auto& var : allValues) {
    namesArray[i] = strdup(var.first.c_str());
    i++;
  }
  return new CNames {namesArray, allValues.size()};
}

CVariable* gameDataGetVariable(CGameData* gameData, const char* name) {
  Retro::Variable var = static_cast<GameData*>(gameData->data)->getVariable(name);
  auto size = sizeof(char) * 5;
  auto* typeCopy = malloc(size);
  memcpy(typeCopy, var.type.type, size);
  auto* type = reinterpret_cast<const char*>(typeCopy);
  return new CVariable {name, var.address, type};
}

void gameDataSetVariable(CGameData* gameData, const char* name, CVariable* value) {
  Retro::Variable var { value->type, value->address };
  static_cast<GameData*>(gameData->data)->setVariable(name, var);
}

void gameDataRemoveVariable(CGameData* gameData, const char* name) {
  static_cast<GameData*>(gameData->data)->removeVariable(name);
}

CVariables* gameDataListVariables(CGameData* gameData) {
  const auto& vars = static_cast<GameData*>(gameData->data)->listVariables();
  auto numVariables = vars.size();
  auto* variables = new CVariable[numVariables];
  auto i = 0;
  for (const auto& var : vars) {
    const auto& v = var.second;
    variables[i] = {strdup(var.first.c_str()), v.address, v.type.type};
    i++;
  }
  return new CVariables {variables, numVariables};
}

float gameDataCurrentReward(CGameData* gameData, unsigned int player) {
  return static_cast<Scenario*>(gameData->scenario)->currentReward(player);
}

float gameDataTotalReward(CGameData* gameData, unsigned int player) {
  return static_cast<Scenario*>(gameData->scenario)->totalReward(player);
}

bool gameDataIsDone(CGameData* gameData) {
  return static_cast<Scenario*>(gameData->scenario)->isDone();
}

CCropInfo* gameDataCropInfo(CGameData* gameData, unsigned int player) {
  size_t x = 0;
  size_t y = 0;
  size_t width = 0;
  size_t height = 0;
  static_cast<Scenario*>(gameData->scenario)->getCrop(&x, &y, &width, &height, player);
  return new CCropInfo {x, y, width, height};
}

void gameDataCropInfoDelete(CCropInfo* cropInfo) {
  delete cropInfo;
}

CMemoryView* gameDataMemory(CGameData* gameData) {
  return memoryViewCreate(&static_cast<GameData*>(gameData->data)->addressSpace());
}

void gameDataSearch(CGameData* gameData, const char* name, int64_t value) {
  static_cast<GameData*>(gameData->data)->search(name, value);
}

void gameDataDeltaSearch(CGameData* gameData, const char* name, const char* op, int64_t ref) {
  static_cast<GameData*>(gameData->data)->deltaSearch(name, Scenario::op(op), ref);
}

CSearch* gameDataSearch(CGameData* gameData, const char* name) {
  return searchCreateUnmanaged(static_cast<GameData*>(gameData->data)->getSearch(name));
}

void gameDataRemoveSearch(CGameData* gameData, const char* name) {
  static_cast<GameData*>(gameData->data)->removeSearch(name);
}

CNames* gameDataListSearchNames(CGameData* gameData) {
  std::vector<std::string> names = static_cast<GameData*>(gameData->data)->listSearches();
  const char** namesArray = new const char*[names.size()];
  for (int i = 0; i < names.size(); i++) {
    namesArray[i] = strdup(names[i].c_str());
  }
  return new CNames {namesArray, names.size()};
}

void gameDataNamesDelete(CNames* names) {
  for (int i = 0; i < names->numNames; i++) {
    delete names->names[i];
  }
  delete names->names;
  delete names;
}

void gameDataVariableDelete(CVariable* variable) {
  delete variable->name;
  delete variable->type;
  delete variable;
}

void gameDataVariablesDelete(CVariables* variables) {
  for (int i = 0; i < variables->numVariables; i++) {
    gameDataVariableDelete(&variables->variables[i]);
  }
  delete variables->variables;
  delete variables;
}

CMovie* movieCreate(const char* name, bool record, unsigned int players) {
  Movie* movie;
  if (record) {
    movie = new MovieBK2(name, true, players);
  } else {
    movie = Movie::load(name).release();
  }
  if (!movie) {
    fprintf(stderr, "Could not load movie.");
    return nullptr;
  }
  return new CMovie {movie, record};
}

void movieDelete(CMovie* movie) {
  delete static_cast<Movie*>(movie->movie);
  delete movie;
}

void movieConfigure(CMovie* movie, const char* name, CEmulator* emulator) {
  if (movie->recording) {
    static_cast<MovieBK2*>(movie->movie)->setGameName(name);
    static_cast<MovieBK2*>(movie->movie)->loadKeymap(static_cast<Emulator*>(emulator->emulator)->core());
  }
}

const char* movieGetGameName(CMovie* movie) {
  return strdup(static_cast<Movie*>(movie->movie)->getGameName().c_str());
}

bool movieStep(CMovie* movie) {
  return static_cast<Movie*>(movie->movie)->step();
}

void movieClose(CMovie* movie) {
  static_cast<Movie*>(movie->movie)->close();
}

unsigned int moviePlayers(CMovie* movie) {
  return static_cast<Movie*>(movie->movie)->players();
}

bool movieGetKey(CMovie* movie, int key, unsigned int player) {
  return static_cast<Movie*>(movie->movie)->getKey(key, player);
}

void movieSetKey(CMovie* movie, int key, bool set, unsigned int player) {
  static_cast<Movie*>(movie->movie)->setKey(key, set, player);
}

CBytes* movieGetState(CMovie* movie) {
  std::vector<uint8_t> data;
  static_cast<Movie*>(movie->movie)->getState(&data);
  auto* cData = data.data();
  auto size = sizeof(uint8_t) * data.size();
  auto* copy = malloc(size);
  memcpy(copy, cData, size);
  return new CBytes {reinterpret_cast<const uint8_t*>(copy), data.size()};
}

void movieSetState(CMovie* movie, CBytes* state) {
  static_cast<Movie*>(movie->movie)->setState(state->bytes, state->numBytes);
}

bool retroLoadCoreInfo(const char* json) {
  return Retro::loadCoreInfo(json);
}

const char* retroCorePath(const char* hint) {
  return strdup(Retro::corePath(hint).c_str());
}

const char* retroDataPath(const char* hint) {
  return strdup(Retro::GameData::dataPath(hint).c_str());
}

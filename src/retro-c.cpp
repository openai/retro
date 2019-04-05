#include "retro-c.h"

#include <cmath>
#ifdef _WIN32
// pyconfig.h doesn't seem to like hypot, so we need to work around it.
namespace std {
template<typename T>
static inline T _hypot(T x, T y) {
  return hypot(x, y);
}
}
#endif

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

using std::string;
using namespace Retro;

CEmulator emulatorCreate(const char* romPath) {
  Retro::Emulator emulator;
  if (Emulator::isLoaded()) {
    throw std::runtime_error(
      "Cannot create multiple emulator instances per process, make sure to call "
      "'env.close()' on each environment before creating a new one.");
  }
  if (!emulator.loadRom(romPath)) {
    throw std::runtime_error("Could not load ROM.");
  }
  // The following is necessary because otherwise, you get a segfault 
  // when you try to get the screen for the first time.
  emulator.run();
  return {&emulator, 0};
}

void emulatorDelete(CEmulator* emulator) {
  delete ((Retro::Emulator*) emulator->emulator);
  delete emulator;
}

void emulatorStep(CEmulator* emulator) {
  ((Retro::Emulator*) emulator->emulator)->run();
}

CBytes emulatorGetState(CEmulator* emulator) {
  size_t numBytes = ((Retro::Emulator*) emulator->emulator)->serializeSize();
  void* bytes = malloc(numBytes);
  ((Retro::Emulator*) emulator->emulator)->serialize(bytes, numBytes);
  return {bytes, numBytes};
}

bool emulatorSetState(CEmulator* emulator, CBytes* state) {
  return ((Retro::Emulator*) emulator->emulator)->unserialize(state->bytes, state->numBytes);
}

CEmulatorScreen emulatorGetScreen(CEmulator* emulator) {
  auto* emu = ((Retro::Emulator*) emulator->emulator);
  long w = emu->getImageWidth();
  long h = emu->getImageHeight();
  uint8_t* data = new uint8_t[w * h * 3];
  Image out(Image::Format::RGB888, data, w, h, w);
  Image in;
  if (emu->getImageDepth() == 16) {
    in = Image(Image::Format::RGB565, emu->getImageData(), w, h, emu->getImagePitch());
  } else if (emu->getImageDepth() == 32) {
    in = Image(Image::Format::RGBX888, emu->getImageData(), w, h, emu->getImagePitch());
  }
  in.copyTo(&out);
  return {data, (int) w, (int) h, 3};
}

double emulatorGetScreenRate(CEmulator* emulator) {
  return ((Retro::Emulator*) emulator->emulator)->getFrameRate();
}

CEmulatorAudio emulatorGetAudio(CEmulator* emulator) {
  size_t numSamples = ((Retro::Emulator*) emulator->emulator)->getAudioSamples() * 2;
  int16_t* samples = new int16_t[numSamples];
  memcpy(samples, ((Retro::Emulator*) emulator->emulator)->getAudioData(), numSamples * 2);
  return {samples, numSamples};
}

double emulatorGetAudioRate(CEmulator* emulator) {
  return ((Retro::Emulator*) emulator->emulator)->getAudioRate();
}

CEmulatorResolution emulatorGetResolution(CEmulator* emulator) {
  auto w = ((Retro::Emulator*) emulator->emulator)->getImageWidth();
  auto h = ((Retro::Emulator*) emulator->emulator)->getImageHeight();
  return {w, h};
}

void emulatorSetButtonMask(CEmulator* emulator, uint8_t* mask, size_t maskSize, unsigned int player) {
  if (maskSize > N_BUTTONS) {
    throw std::runtime_error("mask.size() > N_BUTTONS.");
  }
  if (player >= MAX_PLAYERS) {
    throw std::runtime_error("player >= MAX_PLAYERS.");
  }
  for (int key = 0; key < maskSize; key++) {
    ((Retro::Emulator*) emulator->emulator)->setKey(player, key, mask[key]);
  }
}

void emulatorAddCheat(CEmulator* emulator, const char* code) {
  ((Retro::Emulator*) emulator->emulator)->setCheat(emulator->cheats, true, code);
  emulator->cheats++;
}

void emulatorClearCheats(CEmulator* emulator) {
  ((Retro::Emulator*) emulator->emulator)->clearCheats();
  emulator->cheats = 0;
}

void emulatorConfigureData(CEmulator* emulator, CGameData* gameData) {
  ((Retro::Emulator*) emulator->emulator)->configureData(((Retro::GameData*) gameData->data));
}

CMemoryView memoryViewCreate(void* addressSpace) {
  return { (Retro::AddressSpace*) addressSpace };
}

void memoryViewDelete(CMemoryView* memoryView) {
  delete memoryView;
}

int64_t memoryViewExtract(CMemoryView* memoryView, size_t address, const char* type) {
  return (*((Retro::AddressSpace*) memoryView->addressSpace))[Variable{ type, address }];
}

void memoryViewAssign(CMemoryView* memoryView, size_t address, const char* type, int64_t value) {
  (*((Retro::AddressSpace*) memoryView->addressSpace))[Variable{ type, address }] = value;
}

CMemoryBlocks memoryViewBlocks(CMemoryView* memoryView) {
  auto& internalBlocks = ((Retro::AddressSpace*) memoryView->addressSpace)->blocks();
  auto numBlocks = internalBlocks.size();
  auto* blocks = new CMemoryBlock[numBlocks];
  auto i = 0;
  for (const auto& iter : internalBlocks) {
    blocks[i] = {iter.first, static_cast<const char*>(iter.second.offset(0)), iter.second.size()};
    i++;
  }
  return {blocks, numBlocks};
}

CSearch searchCreate(const char** types, size_t numTypes) {
  Retro::Search* search;
  if (numTypes > 0) {
    std::vector<Retro::DataType> dtypes;
    for (int i = 0; i < numTypes; i++) {
      dtypes.emplace_back(types[i]);
    }
    search = new Retro::Search(dtypes);
  } else {
    search = nullptr;
  }
  return {search, true};
}

CSearch searchCreateUnmanaged(Retro::Search* search) {
  return {search, false};
}

void searchDelete(CSearch* search) {
  if (search->managed) {
    delete ((Retro::Search*) search->search);
  }
  delete search;
}

int searchNumResults(CSearch* search) {
  return ((Retro::Search*) search->search)->numResults();
}

bool searchHasUniqueResult(CSearch* search) {
  return ((Retro::Search*) search->search)->hasUniqueResult();
}

CSearchResult searchUniqueResult(CSearch* search) {
  TypedSearchResult result = ((Retro::Search*) search->search)->uniqueResult();
  return {result.address, result.type.type};
}

CSearchTypedResults searchTypedResults(CSearch* search) {
  std::map<SearchResult, std::unordered_set<DataType>> results;
  for (const auto& result : ((Retro::Search*) search->search)->typedResults()) {
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
  return {cResults, results.size()};
}

CGameData gameDataCreate() {
  auto* data = new Retro::GameData();
  auto* scenario = new Retro::Scenario(*data);
  return {data, scenario};
}

void gameDataDelete(CGameData* gameData) {
  delete ((Retro::GameData*) gameData->data);
  delete ((Retro::Scenario*) gameData->scenario);
  delete gameData;
}

bool gameDataLoad(CGameData* gameData, const char* dataFilename, const char* scenarioFilename) {
  ScriptContext::reset();

  bool success = true;
  if (dataFilename != nullptr) {
    success = success && ((Retro::GameData*) gameData->data)->load(dataFilename);
  }
  if (scenarioFilename != nullptr) {
    success = success && ((Retro::Scenario*) gameData->scenario)->load(scenarioFilename);
  }
  return success;
}

bool gameDataSave(CGameData* gameData, const char* dataFilename, const char* scenarioFilename) {
  ScriptContext::reset();

  bool success = true;
  if (dataFilename != nullptr) {
    success = success && ((Retro::GameData*) gameData->data)->save(dataFilename);
  }
  if (scenarioFilename != nullptr) {
    success = success && ((Retro::Scenario*) gameData->scenario)->save(scenarioFilename);
  }
  return success;
}

void gameDataReset(CGameData* gameData) {
  ((Retro::Scenario*) gameData->scenario)->restart();
  ((Retro::Scenario*) gameData->scenario)->reloadScripts();
}

uint16_t gameDataFilterAction(CGameData* gameData, uint16_t action) {
  return ((Retro::Scenario*) gameData->scenario)->filterAction(action);
}

CValidActions gameDataValidActions(CGameData* gameData) {
  std::map<int, std::set<int>> validActions = ((Retro::Scenario*) gameData->scenario)->validActions();
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
  return {actions, numActionsInner, numActionsOuter};
}

void gameDataUpdateRam(CGameData* gameData) {
  ((Retro::GameData*) gameData->data)->updateRam();
  ((Retro::Scenario*) gameData->scenario)->update();
}

bool gameDataLookupBoolValue(CGameData* gameData, const char* name) {
  return bool(((Retro::GameData*) gameData->data)->lookupValue(name));
}

int64_t gameDataLookupIntValue(CGameData* gameData, const char* name) {
  return int64_t(((Retro::GameData*) gameData->data)->lookupValue(name));
}

double gameDataLookupDoubleValue(CGameData* gameData, const char* name) {
  return double(((Retro::GameData*) gameData->data)->lookupValue(name));
}

void gameDataSetBoolValue(CGameData* gameData, const char* name, bool value) {
  ((Retro::GameData*) gameData->data)->setValue(name, Variant(value));
}

void gameDataSetIntValue(CGameData* gameData, const char* name, int64_t value) {
  ((Retro::GameData*) gameData->data)->setValue(name, Variant(value));
}

void gameDataSetDoubleValue(CGameData* gameData, const char* name, double value) {
  ((Retro::GameData*) gameData->data)->setValue(name, Variant(value));
}

CNames gameDataLookupKeys(CGameData* gameData) {
  std::unordered_map<std::string, Datum> allValues = ((Retro::GameData*) gameData->data)->lookupAll();
  const char** cNames = new const char*[allValues.size()];
  auto i = 0;
  for (const auto& var : allValues) {
    cNames[i] = var.first.c_str();
    i++;
  }
  return {cNames, allValues.size()};
}

CVariable gameDataGetVariable(CGameData* gameData, const char* name) {
  Retro::Variable var = ((Retro::GameData*) gameData->data)->getVariable(name);
  return {name, var.address, var.type.type};
}

void gameDataSetVariable(CGameData* gameData, const char* name, CVariable* value) {
  Retro::Variable var { value->type, value->address };
  ((Retro::GameData*) gameData->data)->setVariable(name, var);
}

void gameDataRemoveVariable(CGameData* gameData, const char* name) {
  ((Retro::GameData*) gameData->data)->removeVariable(name);
}

CVariables gameDataListVariables(CGameData* gameData) {
  const auto& vars = ((Retro::GameData*) gameData->data)->listVariables();
  auto numVariables = vars.size();
  auto* variables = new CVariable[numVariables];
  auto i = 0;
  for (const auto& var : vars) {
    const auto& v = var.second;
    variables[i] = {var.first.c_str(), v.address, v.type.type};
    i++;
  }
  return {variables, numVariables};
}

float gameDataCurrentReward(CGameData* gameData, unsigned int player) {
  return ((Retro::Scenario*) gameData->scenario)->currentReward(player);
}

float gameDataTotalReward(CGameData* gameData, unsigned int player) {
  return ((Retro::Scenario*) gameData->scenario)->totalReward(player);
}

bool gameDataIsDone(CGameData* gameData) {
  return ((Retro::Scenario*) gameData->scenario)->isDone();
}

CCropInfo gameDataCropInfo(CGameData* gameData, unsigned int player) {
  size_t x = 0;
  size_t y = 0;
  size_t width = 0;
  size_t height = 0;
  ((Retro::Scenario*) gameData->scenario)->getCrop(&x, &y, &width, &height, player);
  return {x, y, width, height};
}

CMemoryView gameDataMemory(CGameData* gameData) {
  return memoryViewCreate(&((Retro::GameData*) gameData->data)->addressSpace());
}

void gameDataSearch(CGameData* gameData, const char* name, int64_t value) {
  ((Retro::GameData*) gameData->data)->search(name, value);
}

void gameDataDeltaSearch(CGameData* gameData, const char* name, const char* op, int64_t ref) {
  ((Retro::GameData*) gameData->data)->deltaSearch(name, Retro::Scenario::op(op), ref);
}

CSearch gameDataGetSearch(CGameData* gameData, const char* name) {
  return searchCreateUnmanaged(((Retro::GameData*) gameData->data)->getSearch(name));
}

void gameDataRemoveSearch(CGameData* gameData, const char* name) {
  ((Retro::GameData*) gameData->data)->removeSearch(name);
}

CNames gameDataListSearchNames(CGameData* gameData) {
  std::vector<std::string> names = ((Retro::GameData*) gameData->data)->listSearches();
  const char** cNames = new const char*[names.size()];
  for (int i = 0; i < names.size(); i++) {
    cNames[i] = names[i].c_str();
  }
  return {cNames, names.size()};
}

CMovie movieCreate(const char* name, bool record, unsigned int players) {
  Retro::Movie* movie;
  if (record) {
    movie = new MovieBK2(name, true, players);
  } else {
    movie = Movie::load(name).get();
  }
  if (!movie) {
    throw std::runtime_error("Could not load movie");
  }
  return {movie, record};
}

void movieDelete(CMovie* movie) {
  delete (Retro::Movie*) movie->movie;
  delete movie;
}

void movieConfigure(CMovie* movie, const char* name, CEmulator* emulator) {
  if (movie->recording) {
    static_cast<MovieBK2*>(movie->movie)->setGameName(name);
    static_cast<MovieBK2*>(movie->movie)->loadKeymap(((Retro::Emulator*) ((Retro::Emulator*) emulator->emulator))->core());
  }
}

const char* movieGetGameName(CMovie* movie) {
  return ((Retro::Movie*) movie->movie)->getGameName().c_str();
}

bool movieStep(CMovie* movie) {
  return ((Retro::Movie*) movie->movie)->step();
}

void movieClose(CMovie* movie) {
  ((Retro::Movie*) movie->movie)->close();
}

unsigned int moviePlayers(CMovie* movie) {
  return ((Retro::Movie*) movie->movie)->players();
}

bool movieGetKey(CMovie* movie, int key, unsigned int player) {
  return ((Retro::Movie*) movie->movie)->getKey(key, player);
}

void movieSetKey(CMovie* movie, int key, bool set, unsigned int player) {
  ((Retro::Movie*) movie->movie)->setKey(key, set, player);
}

CBytes movieGetState(CMovie* movie) {
  std::vector<uint8_t> data;
  ((Retro::Movie*) movie->movie)->getState(&data);
  return {reinterpret_cast<char*>(data.data()), data.size()};
}

void movieSetState(CMovie* movie, CBytes* state) {
  ((Retro::Movie*) movie->movie)->setState(reinterpret_cast<uint8_t*>(state->bytes), state->numBytes);
}

bool retroLoadCoreInfo(const char* json) {
  return Retro::loadCoreInfo(json);
}

const char* retroCorePath(const char* hint) {
  return Retro::corePath(hint).c_str();
}

const char* retroDataPath(const char* hint) {
  return Retro::GameData::dataPath(hint).c_str();
}

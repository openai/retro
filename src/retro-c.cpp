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
  delete emulator->emulator;
  delete emulator;
}

void emulatorStep(CEmulator* emulator) {
  emulator->emulator->run();
}

CBytes emulatorGetState(CEmulator* emulator) {
  size_t numBytes = emulator->emulator->serializeSize();
  void* bytes = malloc(numBytes);
  emulator->emulator->serialize(bytes, numBytes);
  return {bytes, numBytes};
}

bool emulatorSetState(CEmulator* emulator, CBytes* state) {
  return emulator->emulator->unserialize(state->bytes, state->numBytes);
}

CEmulatorScreen emulatorGetScreen(CEmulator* emulator) {
  auto* emu = emulator->emulator;
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
  return emulator->emulator->getFrameRate();
}

CEmulatorAudio emulatorGetAudio(CEmulator* emulator) {
  size_t numSamples = emulator->emulator->getAudioSamples() * 2;
  int16_t* samples = new int16_t[numSamples];
  memcpy(samples, emulator->emulator->getAudioData(), numSamples * 2);
  return {samples, numSamples};
}

double emulatorGetAudioRate(CEmulator* emulator) {
  return emulator->emulator->getAudioRate();
}

CEmulatorResolution emulatorGetResolution(CEmulator* emulator) {
  auto w = emulator->emulator->getImageWidth();
  auto h = emulator->emulator->getImageHeight();
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
    emulator->emulator->setKey(player, key, mask[key]);
  }
}

void emulatorAddCheat(CEmulator* emulator, const char* code) {
  emulator->emulator->setCheat(emulator->cheats, true, code);
  emulator->cheats++;
}

void emulatorClearCheats(CEmulator* emulator) {
  emulator->emulator->clearCheats();
  emulator->cheats = 0;
}

void emulatorConfigureData(CEmulator* emulator, CGameData* gameData) {
  emulator->emulator->configureData(gameData->data);
}

CMemoryView memoryViewCreate(Retro::AddressSpace* addressSpace) {
  return { addressSpace };
}

void memoryViewDelete(CMemoryView* memoryView) {
  delete memoryView;
}

int64_t memoryViewExtract(CMemoryView* memoryView, size_t address, const char* type) {
  return (*memoryView->addressSpace)[Variable{ type, address }];
}

void memoryViewAssign(CMemoryView* memoryView, size_t address, const char* type, int64_t value) {
  (*memoryView->addressSpace)[Variable{ type, address }] = value;
}

CMemoryBlocks memoryViewBlocks(CMemoryView* memoryView) {
  auto& internalBlocks = memoryView->addressSpace->blocks();
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
    delete search->search;
  }
  delete search;
}

int searchNumResults(CSearch* search) {
  return search->search->numResults();
}

bool searchHasUniqueResult(CSearch* search) {
  return search->search->hasUniqueResult();
}

CSearchResult searchUniqueResult(CSearch* search) {
  TypedSearchResult result = search->search->uniqueResult();
  return {result.address, result.type.type};
}

// TODO: searchTypedResults

CGameData gameDataCreate() {
  auto* data = new Retro::GameData();
  auto* scenario = new Retro::Scenario(*data);
  return {data, scenario};
}

void gameDataDelete(CGameData* gameData) {
  delete gameData->data;
  delete gameData->scenario;
  delete gameData;
}

bool gameDataLoad(CGameData* gameData, const char* dataFilename, const char* scenarioFilename) {
  ScriptContext::reset();

  bool success = true;
  if (dataFilename != nullptr) {
    success = success && gameData->data->load(dataFilename);
  }
  if (scenarioFilename != nullptr) {
    success = success && gameData->scenario->load(scenarioFilename);
  }
  return success;
}

bool gameDataSave(CGameData* gameData, const char* dataFilename, const char* scenarioFilename) {
  ScriptContext::reset();

  bool success = true;
  if (dataFilename != nullptr) {
    success = success && gameData->data->save(dataFilename);
  }
  if (scenarioFilename != nullptr) {
    success = success && gameData->scenario->save(scenarioFilename);
  }
  return success;
}

void gameDataReset(CGameData* gameData) {
  gameData->scenario->restart();
  gameData->scenario->reloadScripts();
}

uint16_t gameDataFilterAction(CGameData* gameData, uint16_t action) {
  return gameData->scenario->filterAction(action);
}

CValidActions gameDataValidActions(CGameData* gameData) {
  std::map<int, std::set<int>> validActions = gameData->scenario->validActions();
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
  gameData->data->updateRam();
  gameData->scenario->update();
}

bool gameDataLookupBoolValue(CGameData* gameData, const char* name) {
  return bool(gameData->data->lookupValue(name));
}

int64_t gameDataLookupIntValue(CGameData* gameData, const char* name) {
  return int64_t(gameData->data->lookupValue(name));
}

double gameDataLookupDoubleValue(CGameData* gameData, const char* name) {
  return double(gameData->data->lookupValue(name));
}

void gameDataSetBoolValue(CGameData* gameData, const char* name, bool value) {
  gameData->data->setValue(name, Variant(value));
}

void gameDataSetIntValue(CGameData* gameData, const char* name, int64_t value) {
  gameData->data->setValue(name, Variant(value));
}

void gameDataSetDoubleValue(CGameData* gameData, const char* name, double value) {
  gameData->data->setValue(name, Variant(value));
}

// TODO: lookupAll

CVariable gameDataGetVariable(CGameData* gameData, const char* name) {
  Retro::Variable var = gameData->data->getVariable(name);
  return {name, var.address, var.type.type};
}

void gameDataSetVariable(CGameData* gameData, const char* name, CVariable* value) {
  Retro::Variable var { value->type, value->address };
  gameData->data->setVariable(name, var);
}

void gameDataRemoveVariable(CGameData* gameData, const char* name) {
  gameData->data->removeVariable(name);
}

CVariables gameDataListVariables(CGameData* gameData) {
  const auto& vars = gameData->data->listVariables();
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
  return gameData->scenario->currentReward(player);
}

float gameDataTotalReward(CGameData* gameData, unsigned int player) {
  return gameData->scenario->totalReward(player);
}

bool gameDataIsDone(CGameData* gameData) {
  return gameData->scenario->isDone();
}

CCropInfo gameDataCropInfo(CGameData* gameData, unsigned int player) {
  size_t x = 0;
  size_t y = 0;
  size_t width = 0;
  size_t height = 0;
  gameData->scenario->getCrop(&x, &y, &width, &height, player);
  return {x, y, width, height};
}

CMemoryView gameDataMemory(CGameData* gameData) {
  return memoryViewCreate(&gameData->data->addressSpace());
}

void gameDataSearch(CGameData* gameData, const char* name, int64_t value) {
  gameData->data->search(name, value);
}

void gameDataDeltaSearch(CGameData* gameData, const char* name, const char* op, int64_t ref) {
  gameData->data->deltaSearch(name, Retro::Scenario::op(op), ref);
}

CSearch gameDataGetSearch(CGameData* gameData, const char* name) {
  return searchCreateUnmanaged(gameData->data->getSearch(name));
}

void gameDataRemoveSearch(CGameData* gameData, const char* name) {
  gameData->data->removeSearch(name);
}

CSearchList gameDataListSearches(CGameData* gameData) {
  std::vector<std::string> names = gameData->data->listSearches();
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
  delete movie->movie;
  delete movie;
}

void movieConfigure(CMovie* movie, const char* name, CEmulator* emulator) {
  if (movie->recording) {
    static_cast<MovieBK2*>(movie->movie)->setGameName(name);
    static_cast<MovieBK2*>(movie->movie)->loadKeymap(emulator->emulator->core());
  }
}

const char* movieGetGameName(CMovie* movie) {
  return movie->movie->getGameName().c_str();
}

bool movieStep(CMovie* movie) {
  return movie->movie->step();
}

void movieClose(CMovie* movie) {
  movie->movie->close();
}

unsigned int moviePlayers(CMovie* movie) {
  return movie->movie->players();
}

bool movieGetKey(CMovie* movie, int key, unsigned int player) {
  return movie->movie->getKey(key, player);
}

void movieSetKey(CMovie* movie, int key, bool set, unsigned int player) {
  movie->movie->setKey(key, set, player);
}

CBytes movieGetState(CMovie* movie) {
  std::vector<uint8_t> data;
  movie->movie->getState(&data);
  return {reinterpret_cast<char*>(data.data()), data.size()};
}

void movieSetState(CMovie* movie, CBytes* state) {
  movie->movie->setState(reinterpret_cast<uint8_t*>(state->bytes), state->numBytes);
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

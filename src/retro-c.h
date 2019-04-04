#include "data.h"
#include "movie.h"

using namespace Retro;

#ifdef __cplusplus
extern "C" {
#endif

typedef struct CEmulator {
  Retro::Emulator* emulator;
  int cheats;
} CEmulator;

typedef struct CEmulatorScreen {
  uint8_t* values;
  int width;
  int height;
  int channels;
} CEmulatorScreen;

typedef struct CEmulatorAudio {
  int16_t* samples;
  size_t numSamples;
} CEmulatorAudio;

typedef struct CEmulatorResolution {
  int width;
  int height;
} CEmulatorResolution;

typedef struct CMemoryView {
  Retro::AddressSpace* addressSpace;
} CMemoryView;

typedef struct CMemoryBlock {
  size_t address;
  const char* bytes;
  size_t numBytes;
} CMemoryBlock;

typedef struct CMemoryBlocks {
  CMemoryBlock* blocks;
  size_t numBlocks;
} CMemoryBlocks;

typedef struct CSearch {
  Retro::Search* search;
  bool managed;
} CSearch;

typedef struct CSearchList {
  const char** names;
  size_t numNames;
} CSearchList;

typedef struct CSearchResult {
  size_t address;
  const char* type;
} CSearchResult;

typedef struct CGameData {
  Retro::GameData* data;
  Retro::Scenario* scenario;
} CGameData;

typedef struct CVariable {
  const char* name;
  size_t address;
  const char* type;
} CVariable;

typedef struct CVariables {
  CVariable* variables;
  size_t numVariables;
} CVariables;

typedef struct CValidActions {
  int** actions;
  size_t* numActionsInner;
  size_t numActionsOuter;
} CValidActions;

typedef struct CMovie {
  Retro::Movie* movie;
  bool recording;
} CMovie;

typedef struct CBytes {
  void* bytes;
  size_t numBytes;
} CBytes;

typedef struct CCropInfo {
  size_t x;
  size_t y;
  size_t width;
  size_t height;
} CCropInfo;

CEmulator emulatorCreate(const char* romPath);
void emulatorDelete(CEmulator* emulator);
void emulatorStep(CEmulator* emulator);
CBytes emulatorGetState(CEmulator* emulator);
bool emulatorSetState(CEmulator* emulator, CBytes* state);
CEmulatorScreen emulatorGetScreen(CEmulator* emulator);
double emulatorGetScreenRate(CEmulator* emulator);
CEmulatorAudio emulatorGetAudio(CEmulator* emulator);
double emulatorGetAudioRate(CEmulator* emulator);
CEmulatorResolution emulatorGetResolution(CEmulator* emulator);
void emulatorSetButtonMask(CEmulator* emulator, uint8_t* mask, size_t maskSize, unsigned int player);
void emulatorAddCheat(CEmulator* emulator, const char* code);
void emulatorClearCheats(CEmulator* emulator);
void emulatorConfigureData(CEmulator* emulator, CGameData* gameData);

CMemoryView memoryViewCreate(Retro::AddressSpace* addressSpace);
void memoryViewDelete(CMemoryView* memoryView);
int64_t memoryViewExtract(CMemoryView* memoryView, size_t address, const char* type);
void memoryViewAssign(CMemoryView* memoryView, size_t address, const char* type, int64_t value);
CMemoryBlocks memoryViewBlocks(CMemoryView* memoryView);

CSearch searchCreate(const char** types, size_t numTypes);
CSearch searchCreateUnmanaged(Retro::Search* search);
void searchDelete(CSearch* search);
int searchNumResults(CSearch* search);
bool searchHasUniqueResult(CSearch* search);
CSearchResult searchUniqueResult(CSearch* search);
// TODO: searchTypedResults

CGameData gameDataCreate();
void gameDataDelete(CGameData* gameData);
bool gameDataLoad(CGameData* gameData, const char* dataFilename, const char* scenarioFilename);
bool gameDataSave(CGameData* gameData, const char* dataFilename, const char* scenarioFilename);
void gameDataReset(CGameData* gameData);
uint16_t gameDataFilterAction(CGameData* gameData, uint16_t action);
CValidActions gameDataValidActions(CGameData* gameData);
void gameDataUpdateRam(CGameData* gameData);
// TODO: lookupValue, setValue, lookupAll
CVariable gameDataGetVariable(CGameData* gameData, const char* name);
void gameDataSetVariable(CGameData* gameData, const char* name, CVariable* value);
void gameDataRemoveVariable(CGameData* gameData, const char* name);
CVariables gameDataListVariables(CGameData* gameData);
float gameDataCurrentReward(CGameData* gameData, unsigned int player);
float gameDataTotalReward(CGameData* gameData, unsigned int player);
bool gameDataIsDone(CGameData* gameData);
CCropInfo gameDataCropInfo(CGameData* gameData, unsigned int player);
CMemoryView gameDataMemory(CGameData* gameData);
void gameDataSearch(CGameData* gameData, const char* name, int64_t value);
void gameDataDeltaSearch(CGameData* gameData, const char* name, const char* op, int64_t ref);
CSearch gameDataGetSearch(CGameData* gameData, const char* name);
void gameDataRemoveSearch(CGameData* gameData, const char* name);
CSearchList gameDataListSearches(CGameData* gameData);

CMovie movieCreate(const char* name, bool record, unsigned int players);
void movieDelete(CMovie* movie);
void movieConfigure(CMovie* movie, const char* name, CEmulator* emulator);
const char* movieGetGameName(CMovie* movie);
bool movieStep(CMovie* movie);
void movieClose(CMovie* movie);
unsigned int moviePlayers(CMovie* movie);
bool movieGetKey(CMovie* movie, int key, unsigned int player);
void movieSetKey(CMovie* movie, int key, bool set, unsigned int player);
CBytes movieGetState(CMovie* movie);
void movieSetState(CMovie* movie, CBytes* state);

bool retroLoadCoreInfo(const char* json);
const char* retroCorePath(const char* hint);
const char* retroDataPath(const char* hint);

#ifdef __cplusplus
} /* extern "C" */
#endif

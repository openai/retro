#include "data.h"
#include "movie.h"

using namespace Retro;

#ifdef __cplusplus
extern "C" {
#endif

typedef struct CSearch {
  Retro::Search* search;
	bool managed;
} CSearch;

typedef struct CGameData {
  Retro::GameData* data;
	Retro::Scenario* scenario;
} CGameData;

typedef struct CMovie {
  Retro::Movie* movie;
	bool recording;
} CMovie;

typedef struct CMovieState {
  char* bytes;
	size_t numBytes;
} CMovieState;

typedef struct CCropInfo {
  size_t x;
	size_t y;
  size_t width;
  size_t height;
} CCropInfo;

CGameData gameDataCreate();
void gameDataDelete(CGameData* gameData);
bool gameDataLoad(CGameData* gameData, const char* dataFilename, const char* scenarioFilename);
bool gameDataSave(CGameData* gameData, const char* dataFilename, const char* scenarioFilename);
void gameDataReset(CGameData* gameData);
uint16_t gameDataFilterAction(CGameData* gameData, uint16_t action);
// TODO: validActions
void gameDataUpdateRam(CGameData* gameData);
// TODO: lookupValue, setValue, lookupAll
// TODO: getVariable, setVariable, removeVariable, listVariables
float gameDataCurrentReward(CGameData* gameData, unsigned int player);
float gameDataTotalReward(CGameData* gameData, unsigned int player);
bool gameDataIsDone(CGameData* gameData);
CCropInfo gameDataCropInfo(CGameData* gameData, unsigned int player);
// TODO: memory
void gameDataSearch(CGameData* gameData, const char* name, int64_t value);
void gameDataDeltaSearch(CGameData* gameData, const char* name, const char* op, int64_t ref);
// TODO: getSearch
void gameDataRemoveSearch(CGameData* gameData, const char* name);
// TODO: listSearches

CMovie movieCreate(const char* name, bool record, unsigned int players);
void movieDelete(CMovie* movie);
// TODO: movieConfigure
const char* movieGetGameName(CMovie* movie);
bool movieStep(CMovie* movie);
void movieClose(CMovie* movie);
unsigned int moviePlayers(CMovie* movie);
bool movieGetKey(CMovie* movie, int key, unsigned int player);
void movieSetKey(CMovie* movie, int key, bool set, unsigned int player);
CMovieState movieGetState(CMovie* movie);
void movieSetState(CMovie* movie, CMovieState* state);

const char* retroCorePath(const char* hint);
const char* retroDataPath(const char* hint);

#ifdef __cplusplus
} /* extern "C" */
#endif

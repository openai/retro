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

const char* retroCorePath(const char* hint) {
	return Retro::corePath(hint).c_str();
}

const char* retroDataPath(const char* hint) {
	return Retro::GameData::dataPath(hint).c_str();
}

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

// TODO: gameDataValidActions

void gameDataUpdateRam(CGameData* gameData) {
	gameData->data->updateRam();
	gameData->scenario->update();
}

// TODO: lookupValue, setValue, lookupAll
// TODO: getVariable, setVariable, removeVariable, listVariables

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

// TODO: gameDataMemory

void gameDataSearch(CGameData* gameData, const char* name, int64_t value) {
	gameData->data->search(name, value);
}

void gameDataDeltaSearch(CGameData* gameData, const char* name, const char* op, int64_t ref) {
	gameData->data->deltaSearch(name, Retro::Scenario::op(op), ref);
}

// TODO: gameDataGetSearch

void gameDataRemoveSearch(CGameData* gameData, const char* name) {
	gameData->data->removeSearch(name);
}

// TODO: gameDataListSearches

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

// TODO: movieConfigure

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

CMovieState movieGetState(CMovie* movie) {
	std::vector<uint8_t> data;
	movie->movie->getState(&data);
	return {reinterpret_cast<char*>(data.data()), data.size()};
}

void movieSetState(CMovie* movie, CMovieState* state) {
	movie->movie->setState(reinterpret_cast<uint8_t*>(state->bytes), state->numBytes);
}

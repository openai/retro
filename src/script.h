#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "memory.h"

namespace Retro {

class GameData;
class Scenario;
class ScriptContext {
public:
	static std::shared_ptr<ScriptContext> get(const std::string& type);
	static std::vector<std::string> listContexts();
	static void reset();

	virtual void setData(GameData*);
	virtual void setScenario(const Scenario*);

	virtual bool init() = 0;
	virtual bool load(const std::string&) = 0;
	virtual bool loadString(const std::string&) = 0;
	virtual Variant callFunction(const std::string&) = 0;
	virtual std::vector<std::string> listFunctions() = 0;

protected:
	GameData* data();
	const Scenario* scenario();

private:
	GameData* m_data;
	const Scenario* m_scen;
};
}

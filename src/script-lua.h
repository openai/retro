#pragma once

#include "script.h"

#include <unordered_set>

#include <lua.hpp>

namespace Retro {

class ScriptLua final : public ScriptContext {
public:
	static std::shared_ptr<ScriptContext> create();

	~ScriptLua();

	void setData(GameData*) override;
	void setScenario(const Scenario*) override;
	bool init() override;
	bool load(const std::string&) override;
	bool loadString(const std::string&) override;
	Variant callFunction(const std::string&) override;
	std::vector<std::string> listFunctions() override;

private:
	lua_State* m_L = nullptr;
	std::unordered_set<std::string> m_blacklist;
};
}

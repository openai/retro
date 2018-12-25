#include "script.h"

#include "data.h"

#include "script-lua.h"

using namespace Retro;
using namespace std;

static unordered_map<string, function<shared_ptr<ScriptContext>()>> s_scriptTypes{
	make_pair("lua", ScriptLua::create),
};

static unordered_map<string, shared_ptr<ScriptContext>> s_scriptContexts;

shared_ptr<ScriptContext> ScriptContext::get(const string& type) {
	if (type.empty() && s_scriptContexts.size() == 1) {
		return s_scriptContexts.begin()->second;
	}
	const auto& foundContext = s_scriptContexts.find(type);
	if (foundContext != s_scriptContexts.end()) {
		return foundContext->second;
	}

	const auto& found = s_scriptTypes.find(type);
	if (found == s_scriptTypes.end()) {
		return nullptr;
	}
	shared_ptr<ScriptContext> context = found->second();
	if (!context->init()) {
		return nullptr;
	}
	s_scriptContexts[type] = context;
	return context;
}

vector<string> ScriptContext::listContexts() {
	vector<string> contexts;
	for (const auto& context : s_scriptContexts) {
		contexts.emplace_back(context.first);
	}
	return contexts;
}

void ScriptContext::reset() {
	s_scriptContexts.clear();
}

void ScriptContext::setData(GameData* data) {
	m_data = data;
}

GameData* ScriptContext::data() {
	return m_data;
}

void ScriptContext::setScenario(const Scenario* scen) {
	m_scen = scen;
}

const Scenario* ScriptContext::scenario() {
	return m_scen;
}

#include "script-lua.h"

#include "data.h"

using namespace Retro;
using namespace std;

shared_ptr<ScriptContext> ScriptLua::create() {
	return make_shared<ScriptLua>();
}

ScriptLua::~ScriptLua() {
	if (m_L) {
		lua_close(m_L);
	}
}

static int _getData(lua_State* L) {
	const GameData* data = static_cast<const GameData*>(lua_touserdata(L, 1));
	int64_t datum;
	if (lua_isnumber(L, 2)) {
		int64_t address = lua_tonumber(L, 2);
		const AddressSpace& as = data->addressSpace();
		if (address < 0 || !as.hasBlock(address)) {
			lua_pushstring(L, "Out of bounds access");
			lua_error(L);
		}
		datum = as[address];
	} else {
		const char* name = lua_tostring(L, 2);
		datum = data->lookupValue(name);
	}

	lua_pushnumber(L, datum);
	return 1;
}

static int _noop(lua_State*) {
	return 0;
}

void ScriptLua::setData(const GameData* data) {
	ScriptContext::setData(data);

	// Make "table"
	lua_pushlightuserdata(m_L, const_cast<void*>(static_cast<const void*>(data)));

	// Make metatable
	lua_createtable(m_L, 0, 2);
	lua_pushcfunction(m_L, _getData);
	lua_setfield(m_L, -2, "__index");

	lua_pushcfunction(m_L, _noop);
	lua_setfield(m_L, -2, "__newindex");

	lua_setmetatable(m_L, -2);
	lua_setglobal(m_L, "data");
};

bool ScriptLua::init() {
	m_L = luaL_newstate();
	if (!m_L) {
		return false;
	}
	luaopen_base(m_L);
	luaopen_table(m_L);
	luaopen_string(m_L);
	luaopen_math(m_L);

	vector<string> functions = listFunctions();
	m_blacklist = { functions.begin(), functions.end() };
	return true;
}

bool ScriptLua::load(const string& filename) {
	return luaL_dofile(m_L, filename.c_str()) == 0;
}

Variant ScriptLua::callFunction(const string& funcName) {
	lua_getglobal(m_L, funcName.c_str());
	int status = lua_pcall(m_L, 0, 1, 0);
	if (status != 0) {
		string error = string("Lua call failed: ") + lua_tostring(m_L, -1);
		lua_pop(m_L, 1);
		throw runtime_error(error);
	}

	Variant v;
	switch (lua_type(m_L, -1)) {
	case LUA_TNUMBER:
		v = lua_tonumber(m_L, -1);
		break;
	case LUA_TBOOLEAN:
		v = static_cast<bool>(lua_toboolean(m_L, -1));
		break;
	default:
		break;
	}
	lua_pop(m_L, 1);
	return v;
}

vector<string> ScriptLua::listFunctions() {
	vector<string> funcs;
	lua_pushvalue(m_L, LUA_GLOBALSINDEX);
	lua_pushnil(m_L);
	while (lua_next(m_L, -2) != 0) {
		if (lua_isfunction(m_L, -1)) {
			lua_pushvalue(m_L, -2);
			const char* str = lua_tostring(m_L, -1);
			if (!m_blacklist.count(str)) {
				funcs.emplace_back(str);
			}
			lua_pop(m_L, 1);
		}
		lua_pop(m_L, 1);
	}
	lua_pop(m_L, 1);
	return funcs;
}

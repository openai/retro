#include "json.hpp"

#include "coreinfo.h"
#include "data.h"
#include "utils.h"

using namespace std;
using nlohmann::json;

namespace Retro {

static string s_coreDirectory;
static unordered_map<string, string> s_extensionToCore;
static unordered_map<string, string> s_coreToLib;
static json s_cores;

string corePath(const string& hint) {
	if (s_coreDirectory.size()) {
		return s_coreDirectory;
	}
	const char* envDir = getenv("RETRO_CORE_PATH");
	if (envDir) {
		s_coreDirectory = envDir;
	} else if (hint.size()) {
		s_coreDirectory = drillUp({ "cores" }, ".", hint);
	} else {
		s_coreDirectory = ".";
	}
	return s_coreDirectory;
}

string libForCore(const string& core) {
	return s_coreToLib[core];
}

string coreForRom(const string& rom) {
	size_t dot = rom.find_last_of('.');
	if (dot == string::npos) {
		return {};
	}
	string extName = rom.substr(dot + 1);
	if (s_extensionToCore.find(extName) == s_extensionToCore.end()) {
		return {};
	}
	return s_extensionToCore[extName];
}

vector<string> buttons(const string& core) {
	vector<string> results;
	for (const auto& button : s_cores[core]["buttons"]) {
		if (!button.is_string()) {
			results.emplace_back();
			continue;
		}
		results.emplace_back(static_cast<const string&>(button));
	}
	return results;
}

vector<string> keybinds(const string& core) {
	vector<string> results;
	for (const auto& button : s_cores[core]["keybinds"]) {
		if (!button.is_string()) {
			results.emplace_back();
			continue;
		}
		results.emplace_back(static_cast<const string&>(button));
	}
	return results;
}

size_t ramBase(const string& core) {
	return s_cores[core].value("rambase", 0);
}

void configureData(GameData* data, const string& core) {
	if (s_cores[core].find("types") != s_cores[core].end()) {
		vector<Retro::DataType> typesVec;
		for (const string& type : s_cores[core]["types"]) {
			typesVec.emplace_back(type);
		}
		data->setTypes(typesVec);
	}
	if (s_cores[core].find("overlay") != s_cores[core].end()) {
		const auto& overlay = s_cores[core]["overlay"];
		data->addressSpace().setOverlay(Retro::MemoryOverlay{
			static_cast<char>(static_cast<const string&>(overlay[0])[0]),
			static_cast<char>(static_cast<const string&>(overlay[1])[0]),
			static_cast<size_t>(overlay[2]) });
	}
	data->setButtons(Retro::buttons(core));
	if (s_cores[core].find("actions") != s_cores[core].end()) {
		data->setActions(static_cast<const vector<vector<vector<string>>>&>(s_cores[core]["actions"]));
	}
}

bool loadCoreInfo(const string& jsonData) {
	json coreInfo;
	istringstream jsonStream(jsonData);
	try {
		jsonStream >> coreInfo;
		s_cores.update(coreInfo);
	} catch (json::exception&) {
		return false;
	}

	for (auto core = coreInfo.cbegin(); core != coreInfo.cend(); ++core) {
		for (auto ext = core->at("ext").cbegin(); ext != core->at("ext").cend(); ++ext) {
			s_extensionToCore[*ext] = core.key();
		}
		s_coreToLib[core.key()] = core->at("lib");
	}
	return true;
}

vector<string> cores() {
	vector<string> c;
	for (const auto& core : s_coreToLib) {
		c.emplace_back(core.first);
	}
	return c;
}

vector<string> extensions() {
	vector<string> e;
	for (const auto& ext : s_extensionToCore) {
		e.emplace_back(ext.first);
	}
	return e;
}
}

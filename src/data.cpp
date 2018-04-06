#include "data.h"

#include "script.h"
#include "utils.h"

#ifdef ERROR
#undef ERROR
#endif

#include "json.hpp"

#include <fstream>

using namespace Retro;
using namespace std;
using nlohmann::json;

static string s_dataDirectory;

template<typename T>
T find(json::const_reference j, const string& key) {
	const auto& iter = j.find(key);
	if (iter == j.end()) {
		return T();
	}
	try {
		T t = *iter;
		return t;
	} catch (domain_error) {
		return T();
	}
}

static void setActions(const vector<string>& buttonList, const vector<vector<vector<string>>>& actionsIn, map<int, set<int>>& actions) {
	actions.clear();

	for (const auto& outer : actionsIn) {
		set<int> sublist;
		int mask = 0;
		for (const auto& middle : outer) {
			int buttons = 0;
			for (const auto& button : middle) {
				const auto& iter = find(buttonList.begin(), buttonList.end(), button);
				buttons |= 1 << (iter - buttonList.begin());
			}
			mask |= buttons;
			sublist.insert(buttons);
		}
		actions.emplace(mask, move(sublist));
	}
}

static unsigned filterAction(unsigned action, const map<int, set<int>>& actions) {
	unsigned newAction = 0;
	for (const auto& actionSet : actions) {
		unsigned maskedAction = action & actionSet.first;
		if (actionSet.second.find(maskedAction) != actionSet.second.end()) {
			newAction |= maskedAction;
		}
	}
	return newAction;
}

Variable::Variable(const DataType& type, size_t address, uint64_t mask)
	: type(type)
	, address(address)
	, mask(mask) {
}

bool Variable::operator==(const Variable& other) const {
	return type == other.type && address == other.address && mask == other.mask;
}

bool GameData::load(const string& filename) {
	ifstream file(filename);
	return load(&file);
}

bool GameData::load(istream* file) {
	json manifest;
	try {
		*file >> manifest;
	} catch (invalid_argument&) {
		return false;
	}

	const auto& info = const_cast<const json&>(manifest).find("info");
	if (info == manifest.cend()) {
		return false;
	}

	unordered_map<std::string, Variable> oldVars;
	oldVars.swap(m_vars);
	for (auto var = info->cbegin(); var != info->cend(); ++var) {
		if (var->find("address") == var->cend() || var->find("type") == var->cend()) {
			oldVars.swap(m_vars);
			return false;
		}
		string dtype = var->at("type");
		Variable v(dtype, var->at("address"), var->value("mask", UINT64_MAX));
		setVariable(var.key(), v);
	}
	return true;
}

bool GameData::save(const string& filename) const {
	ofstream file(filename);
	return save(&file);
}

bool GameData::save(ostream* file) const {
	json manifest;
	json info;

	for (const auto& var : m_vars) {
		json jvar;
		jvar["address"] = var.second.address;
		jvar["type"] = var.second.type.type;
		if (var.second.mask != UINT64_MAX) {
			jvar["mask"] = var.second.mask;
		}
		info[var.first] = jvar;
	}

	manifest["info"] = info;
	try {
		file->width(2);
		*file << manifest;
		*file << endl;
	} catch (invalid_argument&) {
		return false;
	}
	return true;
}

string GameData::dataPath(const string& hint) {
	if (s_dataDirectory.size()) {
		return s_dataDirectory;
	}
	const char* envDir = getenv("RETRO_DATA_PATH");
	if (envDir) {
		s_dataDirectory = envDir;
	} else {
		s_dataDirectory = drillUp({ "data" }, ".", hint);
	}
	return s_dataDirectory;
}

void GameData::reset() {
	m_lastMem.reset();
	m_cloneMem.reset();
	m_vars.clear();
}

void GameData::updateRam() {
	m_lastMem = move(m_cloneMem);
	m_cloneMem.clone(m_mem);
}

void GameData::setTypes(const vector<DataType> types) {
	m_types = vector<DataType>(types);
}

void GameData::setButtons(const vector<string>& buttons) {
	m_buttons = buttons;
}

vector<string> GameData::buttons() const {
	return m_buttons;
}

void GameData::setActions(const vector<vector<vector<string>>>& actions) {
	::setActions(m_buttons, actions, m_actions);
}

map<int, set<int>> GameData::validActions() const {
	return m_actions;
}

unsigned GameData::filterAction(unsigned action) const {
	return ::filterAction(action, m_actions);
}

Datum GameData::lookupValue(const string& name) {
	auto v = m_vars.find(name);
	if (v == m_vars.end()) {
		throw invalid_argument(name);
	}
	return m_mem[v->second];
}

int64_t GameData::lookupValue(const string& name) const {
	auto v = m_vars.find(name);
	if (v == m_vars.end()) {
		throw invalid_argument(name);
	}
	return m_mem[v->second];
}

int64_t GameData::lookupDelta(const string& name) const {
	const auto& v = m_vars.find(name);
	if (v == m_vars.end()) {
		return 0;
	}
	int64_t newVal = m_cloneMem[v->second];

	if (!m_lastMem.ok()) {
		return 0;
	}
	int64_t oldVal = m_lastMem[v->second];

	return newVal - oldVal;
}

unordered_map<string, Datum> GameData::lookupAll() {
	unordered_map<string, Datum> data;
	for (auto var = m_vars.cbegin(); var != m_vars.cend(); ++var) {
		try {
			data.emplace(var->first, m_mem[var->second]);
		} catch (...) {
		}
	}
	return data;
}

unordered_map<string, int64_t> GameData::lookupAll() const {
	unordered_map<string, int64_t> data;
	for (auto var = m_vars.cbegin(); var != m_vars.cend(); ++var) {
		try {
			data.emplace(var->first, m_mem[var->second]);
		} catch (...) {
		}
	}
	return data;
}

Variable GameData::getVariable(const string& name) const {
	const auto& v = m_vars.find(name);
	if (v == m_vars.end()) {
		throw invalid_argument(name);
	}
	return v->second;
}

void GameData::setVariable(const string& name, const Variable& var) {
	removeVariable(name);
	m_vars.emplace(name, var);
}

void GameData::removeVariable(const string& name) {
	auto iter = m_vars.find(name);
	if (iter != m_vars.end()) {
		m_vars.erase(iter);
	}
}

unordered_map<string, Variable> GameData::listVariables() const {
	return m_vars;
}

size_t GameData::numVariables() const {
	return m_vars.size();
}

Scenario::Scenario(const GameData& data)
	: m_data(data) {
}

bool Scenario::load(const string& filename) {
	ifstream file(filename);
	return load(&file, filename);
}

static shared_ptr<Scenario::DoneNode> loadNode(const json& nodeinfo) {
	shared_ptr<Scenario::DoneNode> node = make_unique<Scenario::DoneNode>();

	const auto& vars = nodeinfo.find("variables");
	if (vars != nodeinfo.cend()) {
		for (auto var = vars->cbegin(); var != vars->cend(); ++var) {
			node->vars.emplace(var.key(), Scenario::DoneSpec{
											  Scenario::measurement(find<string>(*var, "measurement"), Scenario::Measurement::ABSOLUTE),
											  Scenario::op(find<string>(*var, "op")),
											  find<int64_t>(*var, "reference"),
										  });
		}
	}

	const auto& nodes = nodeinfo.find("nodes");
	if (nodes != nodeinfo.cend()) {
		for (auto subnode = nodes->cbegin(); subnode != nodes->cend(); ++subnode) {
			node->nodes.emplace(subnode.key(), loadNode(*subnode));
		}
	}

	const auto& condition = nodeinfo.find("condition");
	node->condition = Scenario::DoneCondition::ANY;
	if (condition != nodeinfo.cend() && *condition == "all") {
		node->condition = Scenario::DoneCondition::ALL;
	}
	return node;
}

bool Scenario::load(istream* file, const std::string& path) {
	json manifest;
	try {
		*file >> manifest;
	} catch (invalid_argument&) {
		return false;
	}

	reset();

	const auto& reward = const_cast<const json&>(manifest).find("reward");
	if (reward != manifest.cend()) {
		const auto& vars = reward->find("variables");
		if (vars != reward->cend()) {
			for (auto var = vars->cbegin(); var != vars->cend(); ++var) {
				setRewardVariable(var.key(), { measurement(find<string>(*var, "measurement"), Measurement::DELTA),
												 op(find<string>(*var, "op")),
												 find<int64_t>(*var, "reference"),
												 find<float>(*var, "reward"),
												 find<float>(*var, "penalty") });
			}
		}

		const auto& time = reward->find("time");
		if (time != reward->cend()) {
			m_rewardTime = {
				measurement(find<string>(*time, "measurement"), Measurement::DELTA),
				op(find<string>(*time, "op")),
				find<int64_t>(*time, "reference"),
				find<float>(*time, "reward"),
				find<float>(*time, "penalty")
			};
		}

		const auto& script = reward->find("script");
		if (script != reward->cend()) {
			string func = *script;
			size_t colon = func.find(':');
			if (colon == string::npos) {
				setRewardFunction(func);
			} else {
				setRewardFunction(func.substr(colon + 1), func.substr(0, colon));
			}
		}
	}

	const auto& done = const_cast<const json&>(manifest).find("done");
	if (done != manifest.cend()) {
		const auto& vars = done->find("variables");
		if (vars != done->cend()) {
			for (auto var = vars->cbegin(); var != vars->cend(); ++var) {
				setDoneVariable(var.key(), {
											   measurement(find<string>(*var, "measurement"), Measurement::ABSOLUTE),
											   op(find<string>(*var, "op")),
											   find<int64_t>(*var, "reference"),
										   });
			}
		}

		const auto& nodes = done->find("nodes");
		if (nodes != done->cend()) {
			for (auto node = nodes->cbegin(); node != nodes->cend(); ++node) {
				setDoneNode(node.key(), loadNode(*node));
			}
		}

		const auto& condition = done->find("condition");
		m_doneCondition = DoneCondition::ANY;
		if (condition != done->cend() && *condition == "all") {
			m_doneCondition = DoneCondition::ALL;
		}

		const auto& script = done->find("script");
		if (script != done->cend()) {
			string func = *script;
			size_t colon = func.find(':');
			if (colon == string::npos) {
				setDoneFunction(func);
			} else {
				setDoneFunction(func.substr(colon + 1), func.substr(0, colon));
			}
		}
	}

	const auto& actions = const_cast<const json&>(manifest).find("actions");
	if (actions != manifest.cend()) {
		setActions(static_cast<const vector<vector<vector<string>>>&>(actions.value()));
	}

#ifndef _WIN32
	size_t slash = path.find_last_of('/');
#else
	size_t slash = path.find_last_of("/\\");
#endif
	if (slash != string::npos) {
		m_base = path.substr(0, slash);
	}

	const auto& scripts = const_cast<const json&>(manifest).find("scripts");
	if (scripts != manifest.cend()) {
		for (const auto& script : *scripts) {
			string scriptfile = script;
			size_t dot = scriptfile.find_last_of('.');
			string extName;
			if (dot != string::npos) {
				extName = scriptfile.substr(dot + 1);
			}
			loadScript(scriptfile, extName);
		}
	}

	return true;
}

bool Scenario::save(const string& filename) const {
	ofstream file(filename);
	return save(&file);
}

static json specToJson(const Scenario::RewardSpec& spec) {
	json jvar;
	if (spec.measurement != Scenario::Measurement::DELTA) {
		jvar["measurement"] = Scenario::name(spec.measurement);
	}
	if (spec.op != Operation::NOOP) {
		jvar["op"] = Scenario::name(spec.op);
	}
	if (spec.reference) {
		jvar["reference"] = spec.reference;
	}
	if (spec.reward) {
		jvar["reward"] = spec.reward;
	}
	if (spec.penalty) {
		jvar["penalty"] = spec.penalty;
	}
	return jvar;
}

static json specToJson(const Scenario::DoneSpec& spec) {
	json jvar;
	if (spec.measurement != Scenario::Measurement::ABSOLUTE) {
		jvar["measurement"] = Scenario::name(spec.measurement);
	}
	if (spec.op != Operation::NOOP) {
		jvar["op"] = Scenario::name(spec.op);
	}
	if (spec.reference) {
		jvar["reference"] = spec.reference;
	}
	return jvar;
}

static bool saveNode(json& nodelist, const string& key, const Scenario::DoneNode& node) {
	json done;
	json doneVars;
	json doneNodes;
	for (const auto& var : node.vars) {
		doneVars[var.first] = specToJson(var.second);
	}
	if (doneVars.size()) {
		done["variables"] = doneVars;
	}
	for (const auto& subnode : node.nodes) {
		if (!saveNode(doneNodes, subnode.first, *subnode.second)) {
			return false;
		}
	}
	if (doneNodes.size()) {
		done["nodes"] = doneNodes;
	}
	if (node.condition == Scenario::DoneCondition::ALL) {
		done["condition"] = "all";
	}

	if (done.size()) {
		nodelist.emplace(key, done);
	}
	return true;
}

bool Scenario::save(ostream* file) const {
	json manifest;

	json reward;
	json rewardVars;
	for (const auto& var : m_rewardVars) {
		rewardVars[var.first] = specToJson(var.second);
	}
	if (rewardVars.size()) {
		reward["variables"] = rewardVars;
	}
	if (m_rewardTime.penalty || m_rewardTime.reward) {
		reward["time"] = specToJson(m_rewardTime);
	}
	if (m_rewardFunc.first.size()) {
		if (m_rewardFunc.second.size()) {
			reward["script"] = m_rewardFunc.second + ":" + m_rewardFunc.first;
		} else {
			reward["script"] = m_rewardFunc.first;
		}
	}
	if (reward.size()) {
		manifest["reward"] = reward;
	}

	json done;
	json doneVars;
	json doneNodes;
	for (const auto& var : m_doneVars) {
		doneVars[var.first] = specToJson(var.second);
	}
	if (doneVars.size()) {
		done["variables"] = doneVars;
	}
	if (m_doneFunc.first.size()) {
		if (m_doneFunc.second.size()) {
			done["script"] = m_doneFunc.second + ":" + m_doneFunc.first;
		} else {
			done["script"] = m_doneFunc.first;
		}
	}
	for (const auto& node : m_doneNodes) {
		if (!saveNode(doneNodes, node.first, *node.second)) {
			return false;
		}
	}
	if (doneNodes.size()) {
		done["nodes"] = doneNodes;
	}
	if (m_doneCondition == DoneCondition::ALL) {
		done["condition"] = "all";
	}
	if (done.size()) {
		manifest["done"] = done;
	}

	if (!m_actions.empty()) {
		json actions;
		const auto& buttons = m_data.buttons();
		for (const auto& outer : m_actions) {
			json subactions;
			for (const auto& inner : outer.second) {
				json action(0, string());
				for (int i = 0; i < 16; ++i) {
					if (inner & (1 << i)) {
						action.push_back(buttons[i]);
					}
				}
				subactions.push_back(action);
			}
			actions.push_back(subactions);
		}
		manifest["actions"] = actions;
	}

	json scripts;
	for (const auto& script : m_scripts) {
		size_t dot = script.first.find_last_of('.');
		string extName;
		if (dot != string::npos) {
			extName = script.first.substr(dot + 1);
		}
		if (extName != script.second) {
			scripts.push_back(script.second + ":" + script.first);
		} else {
			scripts.push_back(script.first);
		}
	}
	if (scripts.size()) {
		manifest["scripts"] = scripts;
	}

	try {
		file->width(2);
		*file << manifest;
		*file << endl;
	} catch (invalid_argument&) {
		return false;
	}
	return true;
}

void Scenario::reset() {
	m_rewardVars.clear();
	m_rewardTime = { Measurement::DELTA };
	m_doneVars.clear();
	m_doneCondition = DoneCondition::ANY;
}

bool Scenario::loadScript(const string& filename, const string& scope) {
	auto context = ScriptContext::get(scope);
	if (!context) {
		return false;
	}
	context->setData(&m_data);
	string path = filename;
	if (filename[0] == '/') {
		size_t prefixLength;
		size_t lastSlash = 0;
		for (prefixLength = 0; prefixLength < m_base.length() && prefixLength < filename.length() && m_base[prefixLength] == filename[prefixLength]; ++prefixLength) {
			if (m_base[prefixLength] == '/') {
				lastSlash = prefixLength;
			}
		}
		if (prefixLength == m_base.length()) {
			path = filename.substr(prefixLength + 1);
		} else {
			size_t updirs = count(m_base.begin() + lastSlash, m_base.end(), '/');
			path.clear();
			path.reserve(updirs * 3);
			for (size_t i = 0; i < updirs; ++i) {
				path += "../";
			}
			path += filename.substr(lastSlash + 1);
		}
	}
	if (context->load(m_base + "/" + path)) {
		m_scripts.emplace_back(make_pair(path, scope));
		return true;
	}
	return false;
}

void Scenario::reloadScripts() {
	ScriptContext::reset();

	for (const auto& script : m_scripts) {
		auto context = ScriptContext::get(script.second);
		if (!context) {
			continue;
		}
		context->setData(&m_data);
		context->load(m_base + "/" + script.first);
	}
}

vector<pair<string, string>> Scenario::scripts() const {
	return m_scripts;
}

float Scenario::currentReward() const {
	if (m_rewardFunc.first.size()) {
		return ScriptContext::get(m_rewardFunc.second)->callFunction(m_rewardFunc.first);
	}

	float reward = m_rewardTime.calculate(1, 1);
	for (auto var = m_rewardVars.cbegin(); var != m_rewardVars.cend(); ++var) {
		try {
			reward += var->second.calculate(m_data.lookupValue(var->first), m_data.lookupDelta(var->first));
		} catch (...) {
		}
	}
	return reward;
}

bool Scenario::isDone() const {
	if (m_doneFunc.first.size()) {
		return ScriptContext::get(m_doneFunc.second)->callFunction(m_doneFunc.first);
	}
	for (auto var = m_doneVars.cbegin(); var != m_doneVars.cend(); ++var) {
		try {
			int done = var->second.test(m_data.lookupValue(var->first), m_data.lookupDelta(var->first));
			if (done > 0 && m_doneCondition == DoneCondition::ANY) {
				return true;
			}
			if (done <= 0 && m_doneCondition == DoneCondition::ALL) {
				return false;
			}
		} catch (...) {
		}
	}
	for (auto node = m_doneNodes.cbegin(); node != m_doneNodes.cend(); ++node) {
		int done = isDone(*node->second);
		if (done > 0 && m_doneCondition == DoneCondition::ANY) {
			return true;
		}
		if (done <= 0 && m_doneCondition == DoneCondition::ALL) {
			return false;
		}
	}
	return m_doneCondition == DoneCondition::ALL;
}

bool Scenario::isDone(const DoneNode& subnode) const {
	for (auto var = subnode.vars.cbegin(); var != subnode.vars.cend(); ++var) {
		try {
			int done = var->second.test(m_data.lookupValue(var->first), m_data.lookupDelta(var->first));
			if (done > 0 && subnode.condition == DoneCondition::ANY) {
				return true;
			}
			if (done <= 0 && subnode.condition == DoneCondition::ALL) {
				return false;
			}
		} catch (...) {
		}
	}
	for (auto node = subnode.nodes.cbegin(); node != subnode.nodes.cend(); ++node) {
		int done = isDone(*node->second);
		if (done > 0 && subnode.condition == DoneCondition::ANY) {
			return true;
		}
		if (done <= 0 && subnode.condition == DoneCondition::ALL) {
			return false;
		}
	}
	return subnode.condition == DoneCondition::ALL;
}

void Scenario::setActions(const vector<vector<vector<string>>>& actions) {
	::setActions(m_data.buttons(), actions, m_actions);
}

map<int, set<int>> Scenario::validActions() const {
	if (m_actions.empty()) {
		return m_data.validActions();
	}
	return m_actions;
}

unsigned Scenario::filterAction(unsigned action) const {
	if (m_actions.empty()) {
		return m_data.filterAction(action);
	}
	return ::filterAction(action, m_actions);
}

static const vector<pair<string, Operation>> s_ops{
	make_pair("equal", Operation::EQUAL),
	make_pair("negative-equal", Operation::NEGATIVE_EQUAL),
	make_pair("not-equal", Operation::NOT_EQUAL),
	make_pair("less-than", Operation::LESS_THAN),
	make_pair("greater-than", Operation::GREATER_THAN),
	make_pair("less-or-equal", Operation::LESS_OR_EQUAL),
	make_pair("greater-or-equal", Operation::GREATER_OR_EQUAL),
	make_pair("less-or-equal", Operation::LESS_OR_EQUAL),
	make_pair("nonzero", Operation::NONZERO),
	make_pair("zero", Operation::ZERO),
	make_pair("negative", Operation::NEGATIVE),
	make_pair("positive", Operation::POSITIVE),
	make_pair("sign", Operation::SIGN)
};

Scenario::Measurement Scenario::measurement(const string& name, Scenario::Measurement def) {
	static unordered_map<string, Measurement> names{
		make_pair("absolute", Measurement::ABSOLUTE),
		make_pair("delta", Measurement::DELTA)
	};
	const auto& measurement = names.find(name);
	if (measurement == names.end()) {
		return def;
	}
	return measurement->second;
}

Operation Scenario::op(const string& name) {
	static const unordered_map<string, Operation> names{ s_ops.begin(), s_ops.end() };
	const auto& op = names.find(name);
	if (op == names.end()) {
		return Operation::NOOP;
	}
	return op->second;
}

int64_t Scenario::calculate(Scenario::Measurement measurement, Operation op, int64_t reference, int64_t value, int64_t delta) {
	if (measurement == Measurement::DELTA) {
		value = delta;
	}
	return Retro::calculate(op, reference, value);
}

string Scenario::name(Scenario::Measurement measurement) {
	static unordered_map<std::underlying_type<Measurement>::type, string> names{
		make_pair(static_cast<std::underlying_type<Measurement>::type>(Measurement::ABSOLUTE), "absolute"),
		make_pair(static_cast<std::underlying_type<Measurement>::type>(Measurement::DELTA), "delta")
	};
	return names[static_cast<std::underlying_type<Measurement>::type>(measurement)];
}

string Scenario::name(Operation op) {
	static unordered_map<std::underlying_type<Operation>::type, string> names;
	if (!names.size()) {
		for (const auto& name : s_ops) {
			names[static_cast<std::underlying_type<Operation>::type>(name.second)] = name.first;
		}
	}
	const auto& name = names.find(static_cast<std::underlying_type<Operation>::type>(op));
	if (name == names.end()) {
		return string();
	}
	return name->second;
}

void Scenario::setRewardVariable(const string& name, const RewardSpec& var) {
	m_rewardVars.emplace(name, var);
}

void Scenario::setRewardFunction(const string& name, const string& scope) {
	m_rewardFunc = make_pair(name, scope);
}

void Scenario::setDoneVariable(const string& name, const DoneSpec& var) {
	m_doneVars.emplace(name, var);
}

void Scenario::setDoneNode(const string& name, shared_ptr<DoneNode> node) {
	m_doneNodes.emplace(name, move(node));
}

void Scenario::setDoneCondition(Scenario::DoneCondition condition) {
	m_doneCondition = condition;
}

void Scenario::setDoneFunction(const string& name, const string& scope) {
	m_doneFunc = make_pair(name, scope);
}

unordered_map<string, Scenario::RewardSpec> Scenario::listRewardVariables() const {
	return m_rewardVars;
}

unordered_map<string, Scenario::DoneSpec> Scenario::listDoneVariables() const {
	return m_doneVars;
}

unordered_map<string, shared_ptr<Scenario::DoneNode>> Scenario::listDoneNodes() const {
	return m_doneNodes;
}

float Scenario::RewardSpec::calculate(int64_t value, int64_t delta) const {
	float measured = Scenario::calculate(measurement, op, reference, value, delta);
	if (measured < 0) {
		measured *= penalty;
	} else if (measured > 0) {
		measured *= reward;
	}
	return measured;
}

bool Scenario::DoneSpec::test(int64_t value, int64_t delta) const {
	int64_t measured = Scenario::calculate(measurement, op, reference, value, delta);
	return measured > 0;
}

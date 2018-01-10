#pragma once

#include "memory.h"
#include "utils.h"

#include <map>
#include <memory>
#include <set>
#include <unordered_map>
#include <vector>

#ifdef ABSOLUTE
#undef ABSOLUTE
#endif

namespace Retro {

class GameData {
public:
	bool load(const std::string& filename);
	bool load(std::istream* stream);

	bool save(const std::string& filename) const;
	bool save(std::ostream* stream) const;

	void reset();

	static std::string dataPath(const std::string& hint = ".");

	AddressSpace& addressSpace() { return m_mem; }
	const AddressSpace& addressSpace() const { return m_mem; }
	void updateRam();

	void setTypes(const std::vector<DataType> types);
	void setButtons(const std::vector<std::string>& names);
	std::vector<std::string> buttons() const;

	void setActions(const std::vector<std::vector<std::vector<std::string>>>& actions);
	std::map<int, std::set<int>> validActions() const;
	unsigned filterAction(unsigned) const;

	Datum lookupValue(const std::string& name);
	int64_t lookupValue(const std::string& name) const;
	std::unordered_map<std::string, Datum> lookupAll();
	std::unordered_map<std::string, int64_t> lookupAll() const;

	int64_t lookupDelta(const std::string& name) const;

	Variable getVariable(const std::string& name) const;
	void setVariable(const std::string& name, const Variable&);
	void removeVariable(const std::string& name);

	std::unordered_map<std::string, Variable> listVariables() const;
	size_t numVariables() const;

private:
	AddressSpace m_mem;
	AddressSpace m_cloneMem;
	AddressSpace m_lastMem;
	std::vector<DataType> m_types;

	std::map<int, std::set<int>> m_actions;
	std::vector<std::string> m_buttons;

	std::unordered_map<std::string, Variable> m_vars;
};

class Scenario {
public:
	Scenario(const GameData& data);

	bool load(const std::string& filename);
	bool load(std::istream* stream, const std::string& path = {});

	bool save(const std::string& filename) const;
	bool save(std::ostream* stream) const;

	void reset();

	bool loadScript(const std::string& filename, const std::string& scope);
	void reloadScripts();
	std::vector<std::pair<std::string, std::string>> scripts() const;

	const GameData* data() const { return &m_data; }

	float currentReward() const;
	bool isDone() const;

	void setActions(const std::vector<std::vector<std::vector<std::string>>>& actions);
	std::map<int, std::set<int>> validActions() const;
	unsigned filterAction(unsigned) const;

	enum class Measurement {
		ABSOLUTE,
		DELTA
	};

	enum class DoneCondition {
		ANY,
		ALL
	};

	static Measurement measurement(const std::string&, Measurement);
	static Operation op(const std::string&);

	static std::string name(Measurement);
	static std::string name(Operation);

	static int64_t calculate(Measurement, Operation, int64_t reference, int64_t value, int64_t delta);

	struct RewardSpec {
		Measurement measurement;
		Operation op;
		int64_t reference;
		float reward;
		float penalty;

		float calculate(int64_t value, int64_t delta) const;
	};

	struct DoneSpec {
		Measurement measurement;
		Operation op;
		int64_t reference;

		bool test(int64_t value, int64_t delta) const;
	};

	struct DoneNode {
		std::unordered_map<std::string, DoneSpec> vars;
		std::unordered_map<std::string, std::shared_ptr<DoneNode>> nodes;
		DoneCondition condition = DoneCondition::ANY;
	};

	void setRewardVariable(const std::string& name, const RewardSpec&);
	void setRewardFunction(const std::string& name, const std::string& scope = {});
	void setDoneVariable(const std::string& name, const DoneSpec&);
	void setDoneNode(const std::string& name, std::shared_ptr<DoneNode>);
	void setDoneCondition(DoneCondition);
	void setDoneFunction(const std::string& name, const std::string& scope = {});

	std::unordered_map<std::string, RewardSpec> listRewardVariables() const;
	std::unordered_map<std::string, DoneSpec> listDoneVariables() const;
	std::unordered_map<std::string, std::shared_ptr<DoneNode>> listDoneNodes() const;

	std::pair<std::string, std::string> rewardFunction() const { return m_rewardFunc; }
	std::pair<std::string, std::string> doneFunction() const { return m_doneFunc; }

	DoneCondition doneCondition() const { return m_doneCondition; }

private:
	bool isDone(const DoneNode&) const;

	const GameData& m_data;
	std::string m_base;

	std::vector<std::pair<std::string, std::string>> m_scripts;

	std::unordered_map<std::string, RewardSpec> m_rewardVars;
	RewardSpec m_rewardTime{ Measurement::DELTA };
	std::pair<std::string, std::string> m_rewardFunc;

	std::unordered_map<std::string, DoneSpec> m_doneVars;
	std::unordered_map<std::string, std::shared_ptr<DoneNode>> m_doneNodes;
	DoneCondition m_doneCondition = DoneCondition::ANY;
	std::pair<std::string, std::string> m_doneFunc;

	std::map<int, std::set<int>> m_actions;
};
}

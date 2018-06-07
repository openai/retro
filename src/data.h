#pragma once

#include "emulator.h"
#include "memory.h"
#include "search.h"

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
	void restart();

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
	Variant lookupValue(const std::string& name) const;
	Datum lookupValue(const TypedSearchResult&);
	int64_t lookupValue(const TypedSearchResult&) const;
	std::unordered_map<std::string, Datum> lookupAll();
	std::unordered_map<std::string, int64_t> lookupAll() const;

	void setValue(const std::string& name, int64_t);
	void setValue(const std::string& name, const Variant&);

	int64_t lookupDelta(const std::string& name) const;

	Variable getVariable(const std::string& name) const;
	void setVariable(const std::string& name, const Variable&);
	void removeVariable(const std::string& name);

	std::unordered_map<std::string, Variable> listVariables() const;
	size_t numVariables() const;

	void search(const std::string& name, int64_t value);
	void deltaSearch(const std::string& name, Operation op, int64_t reference);
	size_t numSearches() const;

	std::vector<std::string> listSearches() const;
	Search* getSearch(const std::string& name);
	void removeSearch(const std::string& name);

#ifdef USE_CAPNP
	bool loadSearches(const std::string& filename);
	bool saveSearches(const std::string& filename) const;
#endif

private:
	AddressSpace m_mem;
	AddressSpace m_cloneMem;
	AddressSpace m_lastMem;
	std::vector<DataType> m_types;

	std::map<int, std::set<int>> m_actions;
	std::vector<std::string> m_buttons;

	std::unordered_map<std::string, Variable> m_vars;
	std::unordered_map<std::string, Search> m_searches;
	std::unordered_map<std::string, AddressSpace> m_searchOldMem;
	std::unordered_map<std::string, std::unique_ptr<Variant>> m_customVars;
};

class Scenario {
public:
	Scenario(GameData& data);

	bool load(const std::string& filename);
	bool load(std::istream* stream, const std::string& path = {});

	bool save(const std::string& filename) const;
	bool save(std::ostream* stream) const;

	void reset();

	bool loadScript(const std::string& filename, const std::string& scope);
	void reloadScripts();
	std::vector<std::pair<std::string, std::string>> scripts() const;

	const GameData* data() const { return &m_data; }

	void update();
	void restart();

	float currentReward(unsigned player = 0) const;
	float totalReward(unsigned player = 0) const;
	bool isDone() const;
	uint64_t frame() const;
	uint64_t timestep() const;

	void setCrop(size_t x, size_t y, size_t width, size_t height, unsigned player = 0);
	void getCrop(size_t* x, size_t* y, size_t* width, size_t* height, unsigned player = 0) const;

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

	struct CropInfo {
		size_t x = 0;
		size_t y = 0;
		size_t width = 0;
		size_t height = 0;

		bool operator==(const CropInfo& other) const {
			return x == other.x && y == other.y && width == other.width && height == other.height;
		}
		bool operator!=(const CropInfo& other) const {
			return !(*this == other);
		}
	};

	void setRewardVariable(const std::string& name, const RewardSpec&, unsigned player = 0);
	void setRewardFunction(const std::string& name, const std::string& scope = {}, unsigned player = 0);
	void setRewardTime(const RewardSpec&, unsigned player = 0);
	void setDoneVariable(const std::string& name, const DoneSpec&);
	void setDoneNode(const std::string& name, std::shared_ptr<DoneNode>);
	void setDoneCondition(DoneCondition);
	void setDoneFunction(const std::string& name, const std::string& scope = {});

	std::unordered_map<std::string, RewardSpec> listRewardVariables(unsigned player = 0) const;
	std::unordered_map<std::string, DoneSpec> listDoneVariables() const;
	std::unordered_map<std::string, std::shared_ptr<DoneNode>> listDoneNodes() const;

	std::pair<std::string, std::string> rewardFunction(unsigned player = 0) const { return m_rewardFunc[player]; }
	std::pair<std::string, std::string> doneFunction() const { return m_doneFunc; }

	DoneCondition doneCondition() const { return m_doneCondition; }

private:
	bool isDone(const DoneNode&) const;

	float calculateReward(unsigned player) const;
	bool calculateDone() const;

	GameData& m_data;
	std::string m_base;

	std::vector<std::pair<std::string, std::string>> m_scripts;

	std::unordered_map<std::string, RewardSpec> m_rewardVars[MAX_PLAYERS];
	RewardSpec m_rewardTime[MAX_PLAYERS];
	std::pair<std::string, std::string> m_rewardFunc[MAX_PLAYERS];

	std::unordered_map<std::string, DoneSpec> m_doneVars;
	std::unordered_map<std::string, std::shared_ptr<DoneNode>> m_doneNodes;
	DoneCondition m_doneCondition = DoneCondition::ANY;
	std::pair<std::string, std::string> m_doneFunc;

	std::map<int, std::set<int>> m_actions;

	float m_reward[MAX_PLAYERS] = { 0 };
	float m_totalReward[MAX_PLAYERS] = { 0 };
	bool m_done = false;
	CropInfo m_crops[MAX_PLAYERS]{};
	uint64_t m_frame = 0;
};
}

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "data.h"

#include <sstream>

using namespace std;
using namespace ::testing;

MATCHER_P(ValuesAreMatcher, b, "") {
	int matches = 0;
	for (auto iter = b.cbegin(); iter != b.cend(); ++iter) {
		matches += arg.find(iter->first) != arg.cend() && arg.at(iter->first) == iter->second;
	}
	return matches == arg.size() && matches == b.size();
}

// Sugar so that we don't have to cast always
#define ValuesAre(VALUES) ValuesAreMatcher(vector<pair<string, int64_t>> VALUES)
#define EXPECT_EQI(A, B) EXPECT_EQ(static_cast<int64_t>(A), B)

namespace Retro {

using M = Scenario::Measurement;
using O = Operation;

TEST(GameData, Empty) {
	GameData data;
	EXPECT_THROW(data.getVariable("foo"), invalid_argument);
	EXPECT_THROW(data.lookupValue("foo"), invalid_argument);
	EXPECT_THROW(const_cast<const GameData&>(data).lookupValue("foo"), invalid_argument);
	EXPECT_EQ(data.lookupDelta("foo"), 0);
	EXPECT_THAT(data.lookupAll(), IsEmpty());
	EXPECT_THAT(data.listVariables(), IsEmpty());
}

TEST(GameData, Manual1) {
	GameData data;
	uint8_t ram[] = { 1 };
	data.addressSpace().addBlock(0, sizeof(ram), ram);
	data.updateRam();
	data.setVariable("foo", {"|u1", 0});
	EXPECT_EQ(data.getVariable("foo"), Variable("|u1", 0));
	EXPECT_EQI(data.lookupValue("foo"), 1);
	EXPECT_EQI(const_cast<const GameData&>(data).lookupValue("foo"), 1);
	EXPECT_THAT(data.lookupAll(), ValuesAre(({make_pair("foo", 1)})));
	EXPECT_THAT(data.listVariables(), UnorderedElementsAre(make_pair("foo", Variable("|u1", 0))));
}

TEST(GameData, Manual2) {
	GameData data;
	uint8_t ram[] = { 1, 2 };
	data.addressSpace().addBlock(0, sizeof(ram), ram);
	data.updateRam();
	data.setVariable("foo", {">n2", 0});
	data.setVariable("bar", {">n1", 1});
	EXPECT_EQ(data.getVariable("foo"), Variable(">n2", 0));
	EXPECT_EQI(data.lookupValue("foo"), 12);
	EXPECT_EQI(const_cast<const GameData&>(data).lookupValue("foo"), 12);
	EXPECT_EQ(data.getVariable("bar"), Variable(">n1", 1));
	EXPECT_EQI(data.lookupValue("bar"), 2);
	EXPECT_EQI(const_cast<const GameData&>(data).lookupValue("bar"), 2);
	EXPECT_THAT(data.lookupAll(), ValuesAre(({make_pair("foo", 12), make_pair("bar", 2)})));
	EXPECT_THAT(data.listVariables(), UnorderedElementsAre(make_pair("foo", Variable(">n2", 0)), make_pair("bar", Variable(">n1", 1))));
}

TEST(GameData, Load) {
	GameData data;
	uint8_t ram[] = { 1 };
	data.addressSpace().addBlock(0, sizeof(ram), ram);
	data.updateRam();
	istringstream manifest(R"({
		"info": {
			"foo": {
				"type": "|u1",
				"address": 0
			}
		}
	})");
	EXPECT_TRUE(data.load(&manifest));
	EXPECT_EQ(data.getVariable("foo"), Variable("|u1", 0));
	EXPECT_EQI(data.lookupValue("foo"), 1);
	EXPECT_EQI(const_cast<const GameData&>(data).lookupValue("foo"), 1);
	EXPECT_THAT(data.lookupAll(), ValuesAre(({make_pair("foo", 1)})));
	EXPECT_THAT(data.listVariables(), UnorderedElementsAre(make_pair("foo", Variable("|u1", 0))));
}

TEST(GameData, FailLoad) {
	GameData data;
	uint8_t ram[] = { 1 };
	data.addressSpace().addBlock(0, sizeof(ram), ram);
	data.updateRam();
	istringstream manifest("{ don't parse }");
	EXPECT_FALSE(data.load(&manifest));
}

TEST(GameData, Update) {
	GameData data;
	uint8_t ram[] = { 1 };
	data.addressSpace().addBlock(0, sizeof(ram), ram);
	data.updateRam();
	data.setVariable("foo", {"|u1", 0});
	EXPECT_EQI(data.lookupValue("foo"), 1);
	ram[0] = 2;
	EXPECT_EQI(data.lookupValue("foo"), 2);

	data.updateRam();
	EXPECT_EQI(data.lookupValue("foo"), 2);
}

TEST(GameData, Delta) {
	GameData data;
	uint8_t ram[] = { 1 };
	data.addressSpace().addBlock(0, sizeof(ram), ram);
	data.updateRam();
	data.setVariable("foo", {"|u1", 0});
	EXPECT_EQI(data.lookupValue("foo"), 1);

	ram[0] = 2;
	data.updateRam();
	EXPECT_EQI(data.lookupValue("foo"), 2);
	EXPECT_EQI(data.lookupDelta("foo"), 1);
}

TEST(Scenario, Measurement) {
	EXPECT_EQ(Scenario::measurement("", M::ABSOLUTE), M::ABSOLUTE);
	EXPECT_EQ(Scenario::measurement("", M::DELTA), M::DELTA);
	EXPECT_EQ(Scenario::measurement("absolute", M::ABSOLUTE), M::ABSOLUTE);
	EXPECT_EQ(Scenario::measurement("absolute", M::DELTA), M::ABSOLUTE);
	EXPECT_EQ(Scenario::measurement("delta", M::ABSOLUTE), M::DELTA);
	EXPECT_EQ(Scenario::measurement("delta", M::DELTA), M::DELTA);
}

TEST(Scenario, Operation) {
	EXPECT_EQ(Scenario::op(""), O::NOOP);
	EXPECT_EQ(Scenario::op("equal"), O::EQUAL);
	EXPECT_EQ(Scenario::op("negative-equal"), O::NEGATIVE_EQUAL);
	EXPECT_EQ(Scenario::op("not-equal"), O::NOT_EQUAL);
	EXPECT_EQ(Scenario::op("less-than"), O::LESS_THAN);
	EXPECT_EQ(Scenario::op("greater-than"), O::GREATER_THAN);
	EXPECT_EQ(Scenario::op("less-or-equal"), O::LESS_OR_EQUAL);
	EXPECT_EQ(Scenario::op("greater-or-equal"), O::GREATER_OR_EQUAL);
	EXPECT_EQ(Scenario::op("nonzero"), O::NONZERO);
	EXPECT_EQ(Scenario::op("zero"), O::ZERO);
	EXPECT_EQ(Scenario::op("positive"), O::POSITIVE);
	EXPECT_EQ(Scenario::op("negative"), O::NEGATIVE);
	EXPECT_EQ(Scenario::op("sign"), O::SIGN);
}

TEST(Scenario, Calculate) {
	EXPECT_EQ(Scenario::calculate(M::ABSOLUTE, O::NOOP, 0, 1, 0), 1);
	EXPECT_EQ(Scenario::calculate(M::DELTA, O::NOOP, 0, 0, 1), 1);

	EXPECT_TRUE(Scenario::calculate(M::ABSOLUTE, O::EQUAL, 1, 1, 0));
	EXPECT_FALSE(Scenario::calculate(M::ABSOLUTE, O::EQUAL, 1, 0, 0));

	EXPECT_TRUE(Scenario::calculate(M::ABSOLUTE, O::NEGATIVE_EQUAL, 1, -1, 0));
	EXPECT_FALSE(Scenario::calculate(M::ABSOLUTE, O::NEGATIVE_EQUAL, 1, 1, 0));

	EXPECT_TRUE(Scenario::calculate(M::ABSOLUTE, O::NOT_EQUAL, 1, 0, 0));
	EXPECT_FALSE(Scenario::calculate(M::ABSOLUTE, O::NOT_EQUAL, 1, 1, 0));

	EXPECT_TRUE(Scenario::calculate(M::ABSOLUTE, O::LESS_THAN, 1, 0, 0));
	EXPECT_FALSE(Scenario::calculate(M::ABSOLUTE, O::LESS_THAN, 1, 1, 0));

	EXPECT_TRUE(Scenario::calculate(M::ABSOLUTE, O::GREATER_THAN, 1, 2, 0));
	EXPECT_FALSE(Scenario::calculate(M::ABSOLUTE, O::GREATER_THAN, 1, 1, 0));

	EXPECT_TRUE(Scenario::calculate(M::ABSOLUTE, O::LESS_OR_EQUAL, 1, 0, 0));
	EXPECT_TRUE(Scenario::calculate(M::ABSOLUTE, O::LESS_OR_EQUAL, 1, 1, 0));
	EXPECT_FALSE(Scenario::calculate(M::ABSOLUTE, O::LESS_OR_EQUAL, 1, 2, 0));

	EXPECT_TRUE(Scenario::calculate(M::ABSOLUTE, O::GREATER_OR_EQUAL, 1, 2, 0));
	EXPECT_TRUE(Scenario::calculate(M::ABSOLUTE, O::GREATER_OR_EQUAL, 1, 1, 0));
	EXPECT_FALSE(Scenario::calculate(M::ABSOLUTE, O::GREATER_OR_EQUAL, 1, 0, 0));

	EXPECT_TRUE(Scenario::calculate(M::ABSOLUTE, O::NONZERO, 0, 1, 0));
	EXPECT_FALSE(Scenario::calculate(M::ABSOLUTE, O::NONZERO, 0, 0, 0));

	EXPECT_TRUE(Scenario::calculate(M::ABSOLUTE, O::ZERO, 0, 0, 0));
	EXPECT_FALSE(Scenario::calculate(M::ABSOLUTE, O::ZERO, 0, 1, 0));

	EXPECT_TRUE(Scenario::calculate(M::ABSOLUTE, O::POSITIVE, 0, 1, 0));
	EXPECT_FALSE(Scenario::calculate(M::ABSOLUTE, O::POSITIVE, 0, 0, 0));

	EXPECT_TRUE(Scenario::calculate(M::ABSOLUTE, O::NEGATIVE, 0, -1, 0));
	EXPECT_FALSE(Scenario::calculate(M::ABSOLUTE, O::NEGATIVE, 0, 0, 0));

	EXPECT_EQ(Scenario::calculate(M::ABSOLUTE, O::SIGN, 0, 2, 0), 1);
	EXPECT_EQ(Scenario::calculate(M::ABSOLUTE, O::SIGN, 0, 1, 0), 1);
	EXPECT_EQ(Scenario::calculate(M::ABSOLUTE, O::SIGN, 0, 0, 0), 0);
	EXPECT_EQ(Scenario::calculate(M::ABSOLUTE, O::SIGN, 0, -1, 0), -1);
	EXPECT_EQ(Scenario::calculate(M::ABSOLUTE, O::SIGN, 0, -2, 0), -1);
}

TEST(RewardSpec, Zero) {
	Scenario::RewardSpec spec{ M::ABSOLUTE, O::NOOP, 0, 0, 0 };

	EXPECT_FLOAT_EQ(spec.calculate(0, 0), 0);
	EXPECT_FLOAT_EQ(spec.calculate(1, 0), 0);
	EXPECT_FLOAT_EQ(spec.calculate(-1, 0), 0);
}

TEST(RewardSpec, RewardOnly) {
	Scenario::RewardSpec spec{ M::ABSOLUTE, O::NOOP, 0, 1, 0 };

	EXPECT_FLOAT_EQ(spec.calculate(0, 0), 0);
	EXPECT_FLOAT_EQ(spec.calculate(1, 0), 1);
	EXPECT_FLOAT_EQ(spec.calculate(2, 0), 2);
	EXPECT_FLOAT_EQ(spec.calculate(-1, 0), 0);
}

TEST(RewardSpec, PenaltyOnly) {
	Scenario::RewardSpec spec{ M::ABSOLUTE, O::NOOP, 0, 0, 1 };

	EXPECT_FLOAT_EQ(spec.calculate(0, 0), 0);
	EXPECT_FLOAT_EQ(spec.calculate(-1, 0), -1);
	EXPECT_FLOAT_EQ(spec.calculate(-2, 0), -2);
	EXPECT_FLOAT_EQ(spec.calculate(1, 0), 0);
}

TEST(RewardSpec, Both) {
	Scenario::RewardSpec spec{ M::ABSOLUTE, O::NOOP, 0, 1, 1 };

	EXPECT_FLOAT_EQ(spec.calculate(0, 0), 0);
	EXPECT_FLOAT_EQ(spec.calculate(1, 0), 1);
	EXPECT_FLOAT_EQ(spec.calculate(2, 0), 2);
	EXPECT_FLOAT_EQ(spec.calculate(-1, 0), -1);
	EXPECT_FLOAT_EQ(spec.calculate(-2, 0), -2);
}

TEST(RewardSpec, NegativeReward) {
	Scenario::RewardSpec spec{ M::ABSOLUTE, O::NOOP, 0, -1, 0 };

	EXPECT_FLOAT_EQ(spec.calculate(0, 0), 0);
	EXPECT_FLOAT_EQ(spec.calculate(1, 0), -1);
	EXPECT_FLOAT_EQ(spec.calculate(2, 0), -2);
	EXPECT_FLOAT_EQ(spec.calculate(-1, 0), 0);
}

TEST(RewardSpec, NegativePenalty) {
	Scenario::RewardSpec spec{ M::ABSOLUTE, O::NOOP, 0, 0, -1 };

	EXPECT_FLOAT_EQ(spec.calculate(0, 0), 0);
	EXPECT_FLOAT_EQ(spec.calculate(-1, 0), 1);
	EXPECT_FLOAT_EQ(spec.calculate(-2, 0), 2);
	EXPECT_FLOAT_EQ(spec.calculate(1, 0), 0);
}

TEST(RewardSpec, NegativeBoth) {
	Scenario::RewardSpec spec{ M::ABSOLUTE, O::NOOP, 0, -1, -1 };

	EXPECT_FLOAT_EQ(spec.calculate(0, 0), 0);
	EXPECT_FLOAT_EQ(spec.calculate(1, 0), -1);
	EXPECT_FLOAT_EQ(spec.calculate(2, 0), -2);
	EXPECT_FLOAT_EQ(spec.calculate(-1, 0), 1);
	EXPECT_FLOAT_EQ(spec.calculate(-2, 0), 2);
}

TEST(RewardSpec, OpposingSigns) {
	Scenario::RewardSpec spec{ M::ABSOLUTE, O::NOOP, 0, -1, 1 };

	EXPECT_FLOAT_EQ(spec.calculate(0, 0), 0);
	EXPECT_FLOAT_EQ(spec.calculate(1, 0), -1);
	EXPECT_FLOAT_EQ(spec.calculate(2, 0), -2);
	EXPECT_FLOAT_EQ(spec.calculate(-1, 0), -1);
	EXPECT_FLOAT_EQ(spec.calculate(-2, 0), -2);
}

TEST(RewardSpec, RewardCoefficient) {
	Scenario::RewardSpec spec{ M::ABSOLUTE, O::NOOP, 0, 10, 0 };

	EXPECT_FLOAT_EQ(spec.calculate(0, 0), 0);
	EXPECT_FLOAT_EQ(spec.calculate(1, 0), 10);
	EXPECT_FLOAT_EQ(spec.calculate(2, 0), 20);
	EXPECT_FLOAT_EQ(spec.calculate(-1, 0), 0);
}

TEST(RewardSpec, PenaltyCoefficient) {
	Scenario::RewardSpec spec{ M::ABSOLUTE, O::NOOP, 0, 0, 10 };

	EXPECT_FLOAT_EQ(spec.calculate(0, 0), 0);
	EXPECT_FLOAT_EQ(spec.calculate(-1, 0), -10);
	EXPECT_FLOAT_EQ(spec.calculate(-2, 0), -20);
	EXPECT_FLOAT_EQ(spec.calculate(1, 0), 0);
}

TEST(RewardSpec, DeltaReward) {
	Scenario::RewardSpec spec{ M::DELTA, O::NOOP, 0, 1, 0 };

	EXPECT_FLOAT_EQ(spec.calculate(0, 0), 0);
	EXPECT_FLOAT_EQ(spec.calculate(0, 1), 1);
	EXPECT_FLOAT_EQ(spec.calculate(0, 2), 2);
	EXPECT_FLOAT_EQ(spec.calculate(0, -1), 0);
}

TEST(RewardSpec, DeltaPenalty) {
	Scenario::RewardSpec spec{ M::DELTA, O::NOOP, 0, 0, 1 };

	EXPECT_FLOAT_EQ(spec.calculate(0, 0), 0);
	EXPECT_FLOAT_EQ(spec.calculate(0, -1), -1);
	EXPECT_FLOAT_EQ(spec.calculate(0, -2), -2);
	EXPECT_FLOAT_EQ(spec.calculate(0, 1), 0);
}

TEST(Scenario, Empty) {
	GameData data;
	Scenario scen(data);
	EXPECT_FALSE(scen.isDone());
	EXPECT_FLOAT_EQ(scen.currentReward(), 0);
}

TEST(Scenario, ManualReward) {
	GameData data;
	Scenario scen(data);

	uint8_t ram[] = { 1 };
	data.addressSpace().addBlock(0, sizeof(ram), ram);
	data.setVariable("foo", {"|u1", 0});

	scen.setRewardVariable("foo", { M::ABSOLUTE, O::NOOP, 0, 1, 0 });

	data.updateRam();
	scen.update();
	EXPECT_FLOAT_EQ(scen.currentReward(), 1);

	ram[0] = 2;
	data.updateRam();
	scen.update();
	EXPECT_FLOAT_EQ(scen.currentReward(), 2);
}

TEST(Scenario, ManualDeltaReward) {
	GameData data;
	Scenario scen(data);

	uint8_t ram[] = { 1 };
	data.addressSpace().addBlock(0, sizeof(ram), ram);
	data.setVariable("foo", {"|u1", 0});

	scen.setRewardVariable("foo", { M::DELTA, O::NOOP, 0, 1, 0 });

	data.updateRam();
	scen.update();
	EXPECT_FLOAT_EQ(scen.currentReward(), 0);

	ram[0] = 2;
	data.updateRam();
	scen.update();
	EXPECT_FLOAT_EQ(scen.currentReward(), 1);
}

TEST(Scenario, ManualDone) {
	GameData data;
	Scenario scen(data);

	uint8_t ram[] = { 1 };
	data.addressSpace().addBlock(0, sizeof(ram), ram);
	data.setVariable("foo", {"|u1", 0});

	scen.setDoneVariable("foo", { M::ABSOLUTE, O::ZERO, 0 });

	data.updateRam();
	scen.update();
	EXPECT_FALSE(scen.isDone());

	ram[0] = 0;
	data.updateRam();
	scen.update();
	EXPECT_TRUE(scen.isDone());
}

TEST(Scenario, MultipleReward) {
	GameData data;
	Scenario scen(data);

	uint8_t ram[] = { 1, 2 };
	data.addressSpace().addBlock(0, sizeof(ram), ram);
	data.setVariable("foo", {"|u1", 0});
	data.setVariable("bar", {"|u1", 1});

	scen.setRewardVariable("foo", { M::ABSOLUTE, O::NOOP, 0, 2, 0 });
	scen.setRewardVariable("bar", { M::ABSOLUTE, O::NOOP, 0, 1, 0 });

	data.updateRam();
	scen.update();
	EXPECT_FLOAT_EQ(scen.currentReward(), 4);

	ram[0] = 2;
	ram[1] = 0;
	data.updateRam();
	scen.update();
	EXPECT_FLOAT_EQ(scen.currentReward(), 4);
}

TEST(Scenario, LoadReward) {
	GameData data;
	Scenario scen(data);

	istringstream manifest(R"({
		"reward": {
			"variables": {
				"foo": {
					"reward": 1
				}
			}
		}
	})");
	EXPECT_TRUE(scen.load(&manifest));

	uint8_t ram[] = { 1 };
	data.addressSpace().addBlock(0, sizeof(ram), ram);
	data.setVariable("foo", {"|u1", 0});

	data.updateRam();
	scen.update();
	EXPECT_FLOAT_EQ(scen.currentReward(), 0);

	ram[0] = 2;
	data.updateRam();
	scen.update();
	EXPECT_FLOAT_EQ(scen.currentReward(), 1);
}

TEST(Scenario, LoadPenalty) {
	GameData data;
	Scenario scen(data);

	istringstream manifest(R"({
		"reward": {
			"variables": {
				"foo": {
					"penalty": 1
				}
			}
		}
	})");
	EXPECT_TRUE(scen.load(&manifest));

	uint8_t ram[] = { 1 };
	data.addressSpace().addBlock(0, sizeof(ram), ram);
	data.setVariable("foo", {"|i1", 0});

	data.updateRam();
	scen.update();
	EXPECT_FLOAT_EQ(scen.currentReward(), 0);

	ram[0] = 0;
	data.updateRam();
	scen.update();
	EXPECT_FLOAT_EQ(scen.currentReward(), -1);
}

TEST(Scenario, LoadRewardMultiple) {
	GameData data;
	Scenario scen(data);

	istringstream manifest(R"({
		"reward": {
			"variables": {
				"foo": {
					"reward": 1
				},
				"bar": {
					"penalty": 1
				}
			}
		}
	})");
	EXPECT_TRUE(scen.load(&manifest));

	uint8_t ram[] = { 0, 0 };
	data.addressSpace().addBlock(0, sizeof(ram), ram);
	data.setVariable("foo", {"|u1", 0});
	data.setVariable("bar", {"|i1", 1});

	data.updateRam();
	scen.update();
	EXPECT_FLOAT_EQ(scen.currentReward(), 0);

	ram[0] = 1;
	ram[1] = 1;
	data.updateRam();
	scen.update();
	EXPECT_FLOAT_EQ(scen.currentReward(), 1);

	ram[0] = 2;
	ram[1] = 0;
	data.updateRam();
	scen.update();
	EXPECT_FLOAT_EQ(scen.currentReward(), 0);
}

TEST(Scenario, LoadRewardAbsolute) {
	GameData data;
	Scenario scen(data);

	istringstream manifest(R"({
		"reward": {
			"variables": {
				"foo": {
					"reward": 1,
					"measurement": "absolute"
				}
			}
		}
	})");
	EXPECT_TRUE(scen.load(&manifest));

	uint8_t ram[] = { 1 };
	data.addressSpace().addBlock(0, sizeof(ram), ram);
	data.setVariable("foo", {"|u1", 0});

	data.updateRam();
	scen.update();
	EXPECT_FLOAT_EQ(scen.currentReward(), 1);

	ram[0] = 2;
	data.updateRam();
	scen.update();
	EXPECT_FLOAT_EQ(scen.currentReward(), 2);
}

TEST(Scenario, LoadRewardOp) {
	GameData data;
	Scenario scen(data);

	istringstream manifest(R"({
		"reward": {
			"variables": {
				"foo": {
					"reward": 1,
					"op": "equal",
					"reference": 2,
					"measurement": "absolute"
				}
			}
		}
	})");
	EXPECT_TRUE(scen.load(&manifest));

	uint8_t ram[] = { 1 };
	data.addressSpace().addBlock(0, sizeof(ram), ram);
	data.setVariable("foo", {"|u1", 0});

	data.updateRam();
	scen.update();
	EXPECT_FLOAT_EQ(scen.currentReward(), 0);

	ram[0] = 2;
	data.updateRam();
	scen.update();
	EXPECT_FLOAT_EQ(scen.currentReward(), 1);
}

TEST(Scenario, LoadTimeReward) {
	GameData data;
	Scenario scen(data);

	istringstream manifest(R"({
		"reward": {
			"time": {
				"reward": 1
			}
		}
	})");
	EXPECT_TRUE(scen.load(&manifest));

	uint8_t ram[] = { 1 };
	data.addressSpace().addBlock(0, sizeof(ram), ram);

	data.updateRam();
	scen.update();
	EXPECT_FLOAT_EQ(scen.currentReward(), 1);

	ram[0] = 2;
	data.updateRam();
	scen.update();
	EXPECT_FLOAT_EQ(scen.currentReward(), 1);
}

TEST(Scenario, LoadDone) {
	GameData data;
	Scenario scen(data);

	istringstream manifest(R"({
		"done": {
			"variables": {
				"foo": {
					"op": "nonzero"
				}
			}
		}
	})");
	EXPECT_TRUE(scen.load(&manifest));

	uint8_t ram[] = { 0 };
	data.addressSpace().addBlock(0, sizeof(ram), ram);
	data.setVariable("foo", {"|u1", 0});

	data.updateRam();
	scen.update();
	EXPECT_FALSE(scen.isDone());

	ram[0] = 1;
	data.updateRam();
	scen.update();
	EXPECT_TRUE(scen.isDone());
}

TEST(Scenario, LoadDoneAny) {
	GameData data;
	Scenario scen(data);

	istringstream manifest(R"({
		"done": {
			"variables": {
				"foo": {
					"op": "nonzero"
				},
				"bar": {
					"op": "nonzero"
				}
			},
			"condition": "any"
		}
	})");
	EXPECT_TRUE(scen.load(&manifest));

	uint8_t ram[] = { 0, 0 };
	data.addressSpace().addBlock(0, sizeof(ram), ram);
	data.setVariable("foo", {"|u1", 0});
	data.setVariable("bar", {"|u1", 1});

	data.updateRam();
	scen.update();
	EXPECT_FALSE(scen.isDone());

	ram[0] = 1;
	data.updateRam();
	scen.update();
	EXPECT_TRUE(scen.isDone());

	ram[1] = 1;
	data.updateRam();
	scen.update();
	EXPECT_TRUE(scen.isDone());

	ram[0] = 0;
	data.updateRam();
	scen.update();
	EXPECT_TRUE(scen.isDone());
}

TEST(Scenario, LoadDoneAll) {
	GameData data;
	Scenario scen(data);

	istringstream manifest(R"({
		"done": {
			"variables": {
				"foo": {
					"op": "nonzero"
				},
				"bar": {
					"op": "nonzero"
				}
			},
			"condition": "all"
		}
	})");
	EXPECT_TRUE(scen.load(&manifest));

	uint8_t ram[] = { 0, 0 };
	data.addressSpace().addBlock(0, sizeof(ram), ram);
	data.setVariable("foo", {"|u1", 0});
	data.setVariable("bar", {"|u1", 1});

	data.updateRam();
	scen.update();
	EXPECT_FALSE(scen.isDone());

	ram[0] = 1;
	data.updateRam();
	scen.update();
	EXPECT_FALSE(scen.isDone());

	ram[1] = 1;
	data.updateRam();
	scen.update();
	EXPECT_TRUE(scen.isDone());

	ram[0] = 0;
	data.updateRam();
	scen.update();
	EXPECT_FALSE(scen.isDone());
}

TEST(Scenario, LoadEverything) {
	GameData data;
	Scenario scen(data);

	istringstream dataManifest(R"({
		"info": {
			"foo": {
				"type": "|u1",
				"address": 0
			}
		}
	})");

	istringstream scenManifest(R"({
		"reward": {
			"variables": {
				"foo": {
					"reward": 1
				}
			}
		},
		"done": {
			"variables": {
				"foo": {
					"op": "zero"
				}
			}
		}
	})");
	EXPECT_TRUE(data.load(&dataManifest));
	EXPECT_TRUE(scen.load(&scenManifest));

	uint8_t ram[] = { 1 };
	data.addressSpace().addBlock(0, sizeof(ram), ram);

	data.updateRam();
	scen.update();
	EXPECT_FLOAT_EQ(scen.currentReward(), 0);
	EXPECT_FALSE(scen.isDone());

	ram[0] = 2;
	data.updateRam();
	scen.update();
	EXPECT_FLOAT_EQ(scen.currentReward(), 1);
	EXPECT_FALSE(scen.isDone());

	ram[0] = 0;
	data.updateRam();
	scen.update();
	EXPECT_FLOAT_EQ(scen.currentReward(), 0);
	EXPECT_TRUE(scen.isDone());
}

TEST(Scenario, ManualReward2P) {
	GameData data;
	Scenario scen(data);

	uint8_t ram[] = { 1, 2 };
	data.addressSpace().addBlock(0, sizeof(ram), ram);
	data.setVariable("foo", {"|u1", 0});
	data.setVariable("bar", {"|u1", 1});

	scen.setRewardVariable("foo", { M::ABSOLUTE, O::NOOP, 0, 1, 0 }, 0);
	scen.setRewardVariable("bar", { M::ABSOLUTE, O::NOOP, 0, 1, 0 }, 1);

	data.updateRam();
	scen.update();
	EXPECT_FLOAT_EQ(scen.currentReward(0), 1);
	EXPECT_FLOAT_EQ(scen.currentReward(1), 2);

	ram[0] = 2;
	ram[1] = 1;
	data.updateRam();
	scen.update();
	EXPECT_FLOAT_EQ(scen.currentReward(0), 2);
	EXPECT_FLOAT_EQ(scen.currentReward(1), 1);
}

TEST(Scenario, LoadReward2P) {
	GameData data;
	Scenario scen(data);

	istringstream manifest(R"({
		"rewards": [
			{
				"variables": {
					"foo": {
						"reward": 1
					}
				}
			},
			{
				"variables": {
					"bar": {
						"reward": 1
					}
				}
			}
		]
	})");
	EXPECT_TRUE(scen.load(&manifest));

	uint8_t ram[] = { 1, 1 };
	data.addressSpace().addBlock(0, sizeof(ram), ram);
	data.setVariable("foo", {"|u1", 0});
	data.setVariable("bar", {"|u1", 1});

	data.updateRam();
	scen.update();
	EXPECT_FLOAT_EQ(scen.currentReward(0), 0);
	EXPECT_FLOAT_EQ(scen.currentReward(1), 0);

	ram[0] = 2;
	data.updateRam();
	scen.update();
	EXPECT_FLOAT_EQ(scen.currentReward(0), 1);
	EXPECT_FLOAT_EQ(scen.currentReward(1), 0);

	ram[1] = 2;
	data.updateRam();
	scen.update();
	EXPECT_FLOAT_EQ(scen.currentReward(0), 0);
	EXPECT_FLOAT_EQ(scen.currentReward(1), 1);
}

}

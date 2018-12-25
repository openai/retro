#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "data.h"
#include "script.h"
#include "script-lua.h"

#include <sstream>

using namespace Retro;
using namespace std;
using namespace ::testing;

TEST(ScriptLua, Create) {
	auto context = ScriptLua::create();
	ASSERT_TRUE(context->init());
}

TEST(ScriptLua, LoadString) {
	auto context = ScriptLua::create();
	ASSERT_TRUE(context->init());
	ASSERT_TRUE(context->loadString("function foo() end"));
}

TEST(ScriptLua, Functions) {
	auto context = ScriptLua::create();
	ASSERT_TRUE(context->init());
	ASSERT_TRUE(context->loadString(
		"function rBool()\n"
		"	return true\n"
		"end\n"
		"function rNum()\n"
		"	return 2\n"
		"end\n"
		"function rNil()\n"
		"	return\n"
		"end\n"));

	EXPECT_THAT(context->listFunctions(), UnorderedElementsAre("rBool", "rNum", "rNil"));
	EXPECT_EQ(static_cast<bool>(context->callFunction("rBool")), true);
	EXPECT_EQ((context->callFunction("rBool")).type(), Variant::Type::BOOL);
	EXPECT_EQ(static_cast<double>(context->callFunction("rNum")), 2);
	EXPECT_EQ((context->callFunction("rNum")).type(), Variant::Type::FLOAT);
	EXPECT_EQ((context->callFunction("rNil")).type(), Variant::Type::VOID);
}

TEST(ScriptLua, GetData) {
	GameData data;
	uint8_t ram[] = { 1 };
	data.addressSpace().addBlock(0, sizeof(ram), ram);
	data.updateRam();
	data.setVariable("foo", {"|u1", 0});

	auto context = ScriptLua::create();
	ASSERT_TRUE(context->init());
	ASSERT_TRUE(context->loadString(
		"function test()\n"
		"	return data.foo\n"
		"end\n"));
	context->setData(&data);

	EXPECT_EQ(static_cast<int64_t>(context->callFunction("test")), data.lookupValue("foo"));
	EXPECT_EQ((context->callFunction("test")).type(), Variant::Type::FLOAT);

	for (int i = 0; i < 64; ++i) {
		ram[0] = i;
		data.updateRam();
		EXPECT_EQ(static_cast<int64_t>(context->callFunction("test")), data.lookupValue("foo"));
	}
}

TEST(ScriptLua, SetData) {
	GameData data;
	uint8_t ram[] = { 1 };
	data.addressSpace().addBlock(0, sizeof(ram), ram);
	data.updateRam();
	data.setVariable("foo", {"|u1", 0});

	auto context = ScriptLua::create();
	ASSERT_TRUE(context->init());
	ASSERT_TRUE(context->loadString(
		"function test()\n"
		"	data.foo = 2\n"
		"end\n"));
	context->setData(&data);

	EXPECT_EQ(data.lookupValue("foo"), 1);
	context->callFunction("test");
	EXPECT_EQ(data.lookupValue("foo"), 2);
}

TEST(ScriptLua, SetCustomData) {
	GameData data;

	auto context = ScriptLua::create();
	ASSERT_TRUE(context->init());
	ASSERT_TRUE(context->loadString(
		"function test()\n"
		"	data.foo = 1\n"
		"end\n"));
	context->setData(&data);

	EXPECT_THROW(data.lookupValue("foo"), invalid_argument);
	context->callFunction("test");
	EXPECT_EQ(data.lookupValue("foo"), 1);
}

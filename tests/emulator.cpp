#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "coreinfo.h"
#include "emulator.h"

#include <sstream>
#include <fstream>

using namespace std;
using namespace ::testing;

struct EmulatorTestParam {
	string system;
	string rom;
};

class EmulatorTest : public TestWithParam<EmulatorTestParam> {
public:
	virtual void SetUp() override;
};

struct EmulatorTestParamName {
    string operator()(const TestParamInfo<EmulatorTestParam>& info) const {
        return info.param.system;
   }
};

void EmulatorTest::SetUp() {
	for (const string& core : { "fceumm", "gambatte", "genesis_plus_gx", "mednafen_pce_fast", "mgba", "snes9x", "stella" }) {
		ifstream in("../retro/cores/" + core + ".json");
		ostringstream out;
		Retro::corePath("../retro/cores");
		while (!in.eof()) {
			string line;
			in >> line;
			out << line;
		}
		Retro::loadCoreInfo(out.str().c_str());
	}
}

namespace Retro {

TEST_F(EmulatorTest, Create) {
	{
		Emulator e;
		EXPECT_FALSE(e.loadRom(""));
	}
	{
		Emulator f;
		EXPECT_FALSE(f.loadRom(""));
	}
}

TEST_P(EmulatorTest, Load) {
	const auto& param = GetParam();
	Emulator e;
	ASSERT_TRUE(e.loadRom("roms/" + param.rom));
	EXPECT_EQ(e.core(), param.system);
	e.run();
}

TEST_P(EmulatorTest, AutoUnload) {
	const auto& param = GetParam();
	{
		Emulator e;
		ASSERT_TRUE(e.loadRom("roms/" + param.rom));
		e.run();
	}
	{
		Emulator e;
		ASSERT_TRUE(e.loadRom("roms/" + param.rom));
	}
}


TEST_P(EmulatorTest, Unload) {
	const auto& param = GetParam();
	Emulator e;
	ASSERT_TRUE(e.loadRom("roms/" + param.rom));
	e.run();
	e.unloadRom();
	e.unloadCore();
}

TEST_P(EmulatorTest, UnloadOrder) {
	const auto& param = GetParam();
	Emulator e;
	ASSERT_TRUE(e.loadRom("roms/" + param.rom));
	e.run();
	e.unloadCore();
	e.unloadRom();
}

TEST_P(EmulatorTest, Output) {
	const auto& param = GetParam();
	Emulator e;
	ASSERT_TRUE(e.loadRom("roms/" + param.rom));
	e.run();

	EXPECT_GT(e.getImageHeight(), 0);
	EXPECT_GT(e.getImageWidth(), 0);
	EXPECT_GT(e.getImagePitch(), 0);
	EXPECT_GT(e.getImageDepth(), 0);
	EXPECT_THAT(e.getImageData(), NotNull());

	e.run();
	e.run();
	EXPECT_GT(e.getAudioSamples(), 0);
	EXPECT_THAT(e.getAudioData(), NotNull());
}

TEST_P(EmulatorTest, States) {
	const auto& param = GetParam();
	Emulator e;
	ASSERT_TRUE(e.loadRom("roms/" + param.rom));
	EXPECT_GT(e.serializeSize(), 0);

	ASSERT_NO_THROW(e.unserialize(static_cast<const void*>(""), 0));
	ASSERT_NO_THROW(e.unserialize(NULL, 0));
	EXPECT_FALSE(e.unserialize(static_cast<const void*>(""), 0));

	vector<uint8_t> v;
	e.run();
	v.resize(e.serializeSize());
	EXPECT_TRUE(e.serialize(v.data(), v.size()));
	EXPECT_TRUE(e.unserialize(v.data(), v.size()));
	e.run();
}

vector<EmulatorTestParam> s_systems{
	{ "Nes", "Dr88-FamiconIntro.nes" },
	{ "Snes", "Anthrox-SineDotDemo.sfc" },
	{ "Genesis", "Dekadence-Dekadrive.md" },
	{ "Atari2600", "automaton.a26" },
	{ "GameBoy", "dox-fire.gb" },
	{ "GbAdvance", "Vantage-LostMarbles.gba" },
	{ "PCEngine", "chrisc-512_Colours.pce" },
	{ "GameGear", "benryves-SegaTween.gg" },
	{ "Sms", "blind-happy10.sms" },
};

INSTANTIATE_TEST_CASE_P(EmulatorCore, EmulatorTest, ValuesIn(s_systems), EmulatorTestParamName());

TEST_F(EmulatorTest, HotSwap) {
	Emulator e;
	for (const auto& system : s_systems) {
		ASSERT_TRUE(e.loadRom("roms/" + system.rom));
		ASSERT_EQ(e.core(), system.system);
		e.run();
		e.unloadCore();
	}
}

}

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "memory.h"

#include <vector>

using namespace std;
using namespace Retro;
using namespace ::testing;

struct MemoryOverlayTestParam {
	MemoryOverlay overlay;
	vector<uint8_t> in;
	vector<uint8_t> out;

	size_t size;
	size_t offset = 0;
};

class MemoryOverlayTest : public TestWithParam<MemoryOverlayTestParam> {
};

TEST_P(MemoryOverlayTest, Parse) {
	const auto& param = GetParam();

	ASSERT_EQ(param.in.size(), param.out.size());

	size_t offset = param.offset & ~(param.overlay.width - 1);
	size_t width = min(param.size, param.in.size() - param.offset);

	vector<uint8_t> parsed(param.out);
	const void* parseLoc = param.overlay.parse(reinterpret_cast<const void*>(&param.in[0]), param.offset, reinterpret_cast<void*>(&parsed[offset]), width);
	EXPECT_EQ(parseLoc, &parsed[param.offset]);
	EXPECT_THAT(parsed, ElementsAreArray(param.out));
}

TEST_P(MemoryOverlayTest, Unparse) {
	const auto& param = GetParam();

	ASSERT_EQ(param.in.size(), param.out.size());

	size_t offset = param.offset & ~(param.overlay.width - 1);
	size_t width = min(param.size, param.out.size() - param.offset);

	vector<uint8_t> unparsed(param.in);
	param.overlay.unparse(reinterpret_cast<void*>(&unparsed[0]), param.offset, reinterpret_cast<const void*>(&param.out[offset]), width);
	EXPECT_THAT(unparsed, ElementsAreArray(param.in));
}


#define INSTANTIATE_OVERLAY_TEST_CASE(NAME, ...) \
	INSTANTIATE_TEST_CASE_P(NAME, MemoryOverlayTest, Values(MemoryOverlayTestParam{ __VA_ARGS__ }));

INSTANTIATE_OVERLAY_TEST_CASE(NativeNative1,
	{ Endian::NATIVE, Endian::NATIVE, 1 },
	{ 0, 1, 2, 3 },
	{ 0, 1, 2, 3 },
	4
)

INSTANTIATE_OVERLAY_TEST_CASE(NativeNative2,
	{ Endian::NATIVE, Endian::NATIVE, 2 },
	{ 0, 1, 2, 3 },
	{ 0, 1, 2, 3 },
	4
)

INSTANTIATE_OVERLAY_TEST_CASE(NativeNative4,
	{ Endian::NATIVE, Endian::NATIVE, 4 },
	{ 0, 1, 2, 3 },
	{ 0, 1, 2, 3 },
	4
)

INSTANTIATE_OVERLAY_TEST_CASE(LittleLittle1,
	{ Endian::LITTLE, Endian::LITTLE, 1 },
	{ 0, 1, 2, 3 },
	{ 0, 1, 2, 3 },
	4
)

INSTANTIATE_OVERLAY_TEST_CASE(LittleLittle2,
	{ Endian::LITTLE, Endian::LITTLE, 2 },
	{ 0, 1, 2, 3 },
	{ 0, 1, 2, 3 },
	4
)

INSTANTIATE_OVERLAY_TEST_CASE(LittleLittle4,
	{ Endian::LITTLE, Endian::LITTLE, 4 },
	{ 0, 1, 2, 3 },
	{ 0, 1, 2, 3 },
	4
)

INSTANTIATE_OVERLAY_TEST_CASE(BigBig1,
	{ Endian::BIG, Endian::BIG, 1 },
	{ 0, 1, 2, 3 },
	{ 0, 1, 2, 3 },
	4
)

INSTANTIATE_OVERLAY_TEST_CASE(BigBig2,
	{ Endian::BIG, Endian::BIG, 2 },
	{ 0, 1, 2, 3 },
	{ 0, 1, 2, 3 },
	4
)

INSTANTIATE_OVERLAY_TEST_CASE(BigBig4,
	{ Endian::BIG, Endian::BIG, 2 },
	{ 0, 1, 2, 3 },
	{ 0, 1, 2, 3 },
	4
)

INSTANTIATE_OVERLAY_TEST_CASE(BigLittle1,
	{ Endian::BIG, Endian::LITTLE, 1 },
	{ 0, 1, 2, 3 },
	{ 0, 1, 2, 3 },
	4
)

INSTANTIATE_OVERLAY_TEST_CASE(BigLittle2,
	{ Endian::BIG, Endian::LITTLE, 2 },
	{ 0, 1, 2, 3 },
	{ 1, 0, 3, 2 },
	4
)

INSTANTIATE_OVERLAY_TEST_CASE(BigLittle4,
	{ Endian::BIG, Endian::LITTLE, 4 },
	{ 0, 1, 2, 3 },
	{ 3, 2, 1, 0 },
	4
)

INSTANTIATE_OVERLAY_TEST_CASE(LittleBig1,
	{ Endian::LITTLE, Endian::BIG, 1 },
	{ 0, 1, 2, 3 },
	{ 0, 1, 2, 3 },
	4
)

INSTANTIATE_OVERLAY_TEST_CASE(LittleBig2,
	{ Endian::LITTLE, Endian::BIG, 2 },
	{ 0, 1, 2, 3 },
	{ 1, 0, 3, 2 },
	4
)

INSTANTIATE_OVERLAY_TEST_CASE(LittleBig4,
	{ Endian::LITTLE, Endian::BIG, 4 },
	{ 0, 1, 2, 3 },
	{ 3, 2, 1, 0 },
	4
)

INSTANTIATE_OVERLAY_TEST_CASE(LittleLittle2Offset1,
	{ Endian::LITTLE, Endian::LITTLE, 2 },
	{ 0, 1, 2, 3 },
	{ 0, 1, 2, 3 },
	4,
	1
)

INSTANTIATE_OVERLAY_TEST_CASE(LittleLittle2Offset2,
	{ Endian::LITTLE, Endian::LITTLE, 2 },
	{ 0, 1, 2, 3 },
	{ 0, 1, 2, 3 },
	4,
	2
)

INSTANTIATE_OVERLAY_TEST_CASE(BigLittle2Offset1,
	{ Endian::BIG, Endian::LITTLE, 2 },
	{ 0, 1, 2, 3 },
	{ 1, 0, 3, 2 },
	4,
	1
)

INSTANTIATE_OVERLAY_TEST_CASE(BigLittle2Offset2,
	{ Endian::BIG, Endian::LITTLE, 2 },
	{ 0, 1, 2, 3 },
	{ 1, 0, 3, 2 },
	4,
	2
)

INSTANTIATE_OVERLAY_TEST_CASE(BigLittle2Size1,
	{ Endian::BIG, Endian::LITTLE, 2 },
	{ 0, 1, 2, 3 },
	{ 1, 0, 2, 3 },
	1
)

INSTANTIATE_OVERLAY_TEST_CASE(BigLittle2Size1Offset1,
	{ Endian::BIG, Endian::LITTLE, 2 },
	{ 0, 1, 2, 3 },
	{ 1, 0, 2, 3 },
	1,
	1
)

INSTANTIATE_OVERLAY_TEST_CASE(BigLittle2Size1Offset2,
	{ Endian::BIG, Endian::LITTLE, 2 },
	{ 0, 1, 2, 3 },
	{ 0, 1, 3, 2 },
	1,
	2
)

INSTANTIATE_OVERLAY_TEST_CASE(BigLittle2Size1Offset3,
	{ Endian::BIG, Endian::LITTLE, 2 },
	{ 0, 1, 2, 3 },
	{ 0, 1, 3, 2 },
	1,
	3
)

INSTANTIATE_OVERLAY_TEST_CASE(BigLittle2Size2,
	{ Endian::BIG, Endian::LITTLE, 2 },
	{ 0, 1, 2, 3 },
	{ 1, 0, 2, 3 },
	2
)

INSTANTIATE_OVERLAY_TEST_CASE(BigLittle2Size2Offset1,
	{ Endian::BIG, Endian::LITTLE, 2 },
	{ 0, 1, 2, 3 },
	{ 1, 0, 3, 2 },
	2,
	1
)

INSTANTIATE_OVERLAY_TEST_CASE(BigLittle2Size2Offset2,
	{ Endian::BIG, Endian::LITTLE, 2 },
	{ 0, 1, 2, 3 },
	{ 0, 1, 3, 2 },
	2,
	2
)

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "memory.h"

#include <vector>

using namespace std;
using namespace ::testing;

namespace Retro {

TEST(Endian, Reduce) {
	EXPECT_EQ(reduce(Endian::BIG), Endian::BIG);
	EXPECT_EQ(reduce(Endian::LITTLE), Endian::LITTLE);
	EXPECT_EQ(reduce(Endian::MIXED_BL), Endian::MIXED_BL);
	EXPECT_EQ(reduce(Endian::MIXED_LB), Endian::MIXED_LB);
#ifdef __BIG_ENDIAN__
	EXPECT_EQ(reduce(Endian::NATIVE), Endian::BIG);
	EXPECT_EQ(reduce(Endian::MIXED_LN), Endian::MIXED_LB);
	EXPECT_EQ(reduce(Endian::MIXED_BN), Endian::BIG);
#elif defined(__LITTLE_ENDIAN__)
	EXPECT_EQ(reduce(Endian::NATIVE), Endian::LITTLE);
	EXPECT_EQ(reduce(Endian::MIXED_LN), Endian::LITTLE);
	EXPECT_EQ(reduce(Endian::MIXED_BN), Endian::MIXED_BL);
#endif
}

TEST(DataType, Width) {
	EXPECT_EQ(DataType("|u1").width, 1);
	EXPECT_EQ(DataType("|u2").width, 2);
	EXPECT_EQ(DataType("|u3").width, 3);
	EXPECT_EQ(DataType("|u8").width, 8);
}

TEST(DataTypeEncode, ndian) {
	EXPECT_EQ(DataType("|u4").endian, Endian::UNDEF);
	EXPECT_EQ(DataType("<u4").endian, Endian::LITTLE);
	EXPECT_EQ(DataType(">u4").endian, Endian::BIG);
	EXPECT_EQ(DataType("=u4").endian, Endian::NATIVE);
	EXPECT_EQ(DataType("><u4").endian, Endian::MIXED_BL);
	EXPECT_EQ(DataType("<>u4").endian, Endian::MIXED_LB);
	EXPECT_EQ(DataType(">=u4").endian, Endian::MIXED_BN);
	EXPECT_EQ(DataType("<=u4").endian, Endian::MIXED_LN);
	EXPECT_EQ(DataType("?u4").endian, Endian::UNDEF);
}

TEST(DataType, Repr) {
	EXPECT_EQ(DataType("|i1").repr, Repr::SIGNED);
	EXPECT_EQ(DataType("|u1").repr, Repr::UNSIGNED);
	EXPECT_EQ(DataType("|d1").repr, Repr::BCD);
	EXPECT_EQ(DataType("|n1").repr, Repr::LN_BCD);
}

TEST(DataTypeShift, 1) {
	EXPECT_THAT(DataType("|i1").shift, ElementsAre(1, 0, 0, 0, 0, 0, 0, 0));
	EXPECT_THAT(DataType(">i1").shift, ElementsAre(1, 0, 0, 0, 0, 0, 0, 0));
	EXPECT_THAT(DataType("<i1").shift, ElementsAre(1, 0, 0, 0, 0, 0, 0, 0));
	EXPECT_THAT(DataType("=i1").shift, ElementsAre(1, 0, 0, 0, 0, 0, 0, 0));
	EXPECT_THAT(DataType("|d1").shift, ElementsAre(1, 0, 0, 0, 0, 0, 0, 0));
	EXPECT_THAT(DataType("|n1").shift, ElementsAre(1, 0, 0, 0, 0, 0, 0, 0));
}

TEST(DataTypeShift, 2) {
	EXPECT_THAT(DataType(">i2").shift, ElementsAre(0x100, 1, 0, 0, 0, 0, 0, 0));
	EXPECT_THAT(DataType(">d2").shift, ElementsAre(100, 1, 0, 0, 0, 0, 0, 0));
	EXPECT_THAT(DataType(">n2").shift, ElementsAre(10, 1, 0, 0, 0, 0, 0, 0));
	EXPECT_THAT(DataType("<i2").shift, ElementsAre(1, 0x100, 0, 0, 0, 0, 0, 0));
	EXPECT_THAT(DataType("<d2").shift, ElementsAre(1, 100, 0, 0, 0, 0, 0, 0));
	EXPECT_THAT(DataType("<n2").shift, ElementsAre(1, 10, 0, 0, 0, 0, 0, 0));
#ifdef __BIG_ENDIAN__
	EXPECT_THAT(DataType("=i2").shift, ElementsAre(0x100, 1, 0, 0, 0, 0, 0, 0));
	EXPECT_THAT(DataType("=d2").shift, ElementsAre(100, 1, 0, 0, 0, 0, 0, 0));
	EXPECT_THAT(DataType("=n2").shift, ElementsAre(10, 1, 0, 0, 0, 0, 0, 0));
#elif defined(__LITTLE_ENDIAN__)
	EXPECT_THAT(DataType("=i2").shift, ElementsAre(1, 0x100, 0, 0, 0, 0, 0, 0));
	EXPECT_THAT(DataType("=d2").shift, ElementsAre(1, 100, 0, 0, 0, 0, 0, 0));
	EXPECT_THAT(DataType("=n2").shift, ElementsAre(1, 10, 0, 0, 0, 0, 0, 0));
#endif
}

TEST(DataTypeShift, 3) {
	EXPECT_THAT(DataType(">i3").shift, ElementsAre(0x10000, 0x100, 1, 0, 0, 0, 0, 0));
	EXPECT_THAT(DataType(">d3").shift, ElementsAre(10000, 100, 1, 0, 0, 0, 0, 0));
	EXPECT_THAT(DataType(">n3").shift, ElementsAre(100, 10, 1, 0, 0, 0, 0, 0));
	EXPECT_THAT(DataType("<i3").shift, ElementsAre(1, 0x100, 0x10000, 0, 0, 0, 0, 0));
	EXPECT_THAT(DataType("<d3").shift, ElementsAre(1, 100, 10000, 0, 0, 0, 0, 0));
	EXPECT_THAT(DataType("<n3").shift, ElementsAre(1, 10, 100, 0, 0, 0, 0, 0));
#ifdef __BIG_ENDIAN__
	EXPECT_THAT(DataType("=i3").shift, ElementsAre(0x10000, 0x100, 1, 0, 0, 0, 0, 0));
	EXPECT_THAT(DataType("=d3").shift, ElementsAre(10000, 100, 1, 0, 0, 0, 0, 0));
	EXPECT_THAT(DataType("=n3").shift, ElementsAre(100, 10, 1, 0, 0, 0, 0, 0));
#elif defined(__LITTLE_ENDIAN__)
	EXPECT_THAT(DataType("=i3").shift, ElementsAre(1, 0x100, 0x10000, 0, 0, 0, 0, 0));
	EXPECT_THAT(DataType("=d3").shift, ElementsAre(1, 100, 10000, 0, 0, 0, 0, 0));
	EXPECT_THAT(DataType("=n3").shift, ElementsAre(1, 10, 100, 0, 0, 0, 0, 0));
#endif
}

TEST(DataTypeShift, 4) {
	EXPECT_THAT(DataType(">i4").shift, ElementsAre(0x1000000, 0x10000, 0x100, 1, 0, 0, 0, 0));
	EXPECT_THAT(DataType(">d4").shift, ElementsAre(1000000, 10000, 100, 1, 0, 0, 0, 0));
	EXPECT_THAT(DataType(">n4").shift, ElementsAre(1000, 100, 10, 1, 0, 0, 0, 0));
	EXPECT_THAT(DataType("<i4").shift, ElementsAre(1, 0x100, 0x10000, 0x1000000, 0, 0, 0, 0));
	EXPECT_THAT(DataType("<d4").shift, ElementsAre(1, 100, 10000, 1000000, 0, 0, 0, 0));
	EXPECT_THAT(DataType("<n4").shift, ElementsAre(1, 10, 100, 1000, 0, 0, 0, 0));
	EXPECT_THAT(DataType("><i4").shift, ElementsAre(0x10000, 0x1000000, 0x1, 0x100, 0, 0, 0, 0));
	EXPECT_THAT(DataType("><d4").shift, ElementsAre(10000, 1000000, 1, 100, 0, 0, 0, 0));
	EXPECT_THAT(DataType("><n4").shift, ElementsAre(100, 1000, 1, 10, 0, 0, 0, 0));
	EXPECT_THAT(DataType("<>i4").shift, ElementsAre(0x100, 0x1, 0x1000000, 0x10000, 0, 0, 0, 0));
	EXPECT_THAT(DataType("<>d4").shift, ElementsAre(100, 1, 1000000, 10000, 0, 0, 0, 0));
	EXPECT_THAT(DataType("<>n4").shift, ElementsAre(10, 1, 1000, 100, 0, 0, 0, 0));
#ifdef __BIG_ENDIAN__
	EXPECT_THAT(DataType("=i4").shift, ElementsAre(0x1000000, 0x10000, 0x100, 1, 0, 0, 0, 0));
	EXPECT_THAT(DataType("=d4").shift, ElementsAre(1000000, 10000, 100, 1, 0, 0, 0, 0));
	EXPECT_THAT(DataType("=n4").shift, ElementsAre(1000, 100, 10, 1, 0, 0, 0, 0));
	EXPECT_THAT(DataType(">=i4").shift, ElementsAre(0x1000000, 0x10000, 0x100, 1, 0, 0, 0, 0));
	EXPECT_THAT(DataType(">=d4").shift, ElementsAre(1000000, 10000, 100, 1, 0, 0, 0, 0));
	EXPECT_THAT(DataType(">=n4").shift, ElementsAre(1000, 100, 10, 1, 0, 0, 0, 0));
	EXPECT_THAT(DataType("<=i4").shift, ElementsAre(0x100, 0x1, 0x1000000, 0x10000, 0, 0, 0, 0));
	EXPECT_THAT(DataType("<=d4").shift, ElementsAre(100, 1, 1000000, 10000, 0, 0, 0, 0));
	EXPECT_THAT(DataType("<=n4").shift, ElementsAre(10, 1, 1000, 100, 0, 0, 0, 0));
#elif defined(__LITTLE_ENDIAN__)
	EXPECT_THAT(DataType("=i4").shift, ElementsAre(1, 0x100, 0x10000, 0x1000000, 0, 0, 0, 0));
	EXPECT_THAT(DataType("=d4").shift, ElementsAre(1, 100, 10000, 1000000, 0, 0, 0, 0));
	EXPECT_THAT(DataType("=n4").shift, ElementsAre(1, 10, 100, 1000, 0, 0, 0, 0));
	EXPECT_THAT(DataType(">=i4").shift, ElementsAre(0x10000, 0x1000000, 0x1, 0x100, 0, 0, 0, 0));
	EXPECT_THAT(DataType(">=d4").shift, ElementsAre(10000, 1000000, 1, 100, 0, 0, 0, 0));
	EXPECT_THAT(DataType(">=n4").shift, ElementsAre(100, 1000, 1, 10, 0, 0, 0, 0));
	EXPECT_THAT(DataType("<=i4").shift, ElementsAre(1, 0x100, 0x10000, 0x1000000, 0, 0, 0, 0));
	EXPECT_THAT(DataType("<=d4").shift, ElementsAre(1, 100, 10000, 1000000, 0, 0, 0, 0));
	EXPECT_THAT(DataType("<=n4").shift, ElementsAre(1, 10, 100, 1000, 0, 0, 0, 0));
#endif
}

TEST(DataTypeShift, 5) {
	EXPECT_THAT(DataType(">i5").shift, ElementsAre(0x100000000, 0x1000000, 0x10000, 0x100, 1, 0, 0, 0));
	EXPECT_THAT(DataType(">d5").shift, ElementsAre(100000000, 1000000, 10000, 100, 1, 0, 0, 0));
	EXPECT_THAT(DataType(">n5").shift, ElementsAre(10000, 1000, 100, 10, 1, 0, 0, 0));
	EXPECT_THAT(DataType("<i5").shift, ElementsAre(1, 0x100, 0x10000, 0x1000000, 0x100000000, 0, 0, 0));
	EXPECT_THAT(DataType("<d5").shift, ElementsAre(1, 100, 10000, 1000000, 100000000, 0, 0, 0));
	EXPECT_THAT(DataType("<n5").shift, ElementsAre(1, 10, 100, 1000, 10000, 0, 0, 0));
#ifdef __BIG_ENDIAN__
	EXPECT_THAT(DataType("=i5").shift, ElementsAre(0x100000000, 0x1000000, 0x10000, 0x100, 1, 0, 0, 0));
	EXPECT_THAT(DataType("=d5").shift, ElementsAre(100000000, 1000000, 10000, 100, 1, 0, 0, 0));
	EXPECT_THAT(DataType("=n5").shift, ElementsAre(10000, 1000, 100, 10, 1, 0, 0, 0));
#elif defined(__LITTLE_ENDIAN__)
	EXPECT_THAT(DataType("=i5").shift, ElementsAre(1, 0x100, 0x10000, 0x1000000, 0x100000000, 0, 0, 0));
	EXPECT_THAT(DataType("=d5").shift, ElementsAre(1, 100, 10000, 1000000, 100000000, 0, 0, 0));
	EXPECT_THAT(DataType("=n5").shift, ElementsAre(1, 10, 100, 1000, 10000, 0, 0, 0));
#endif
}

TEST(DataTypeShift, 6) {
	EXPECT_THAT(DataType(">i6").shift, ElementsAre(0x10000000000, 0x100000000, 0x1000000, 0x10000, 0x100, 1, 0, 0));
	EXPECT_THAT(DataType(">d6").shift, ElementsAre(10000000000, 100000000, 1000000, 10000, 100, 1, 0, 0));
	EXPECT_THAT(DataType(">n6").shift, ElementsAre(100000, 10000, 1000, 100, 10, 1, 0, 0));
	EXPECT_THAT(DataType("<i6").shift, ElementsAre(1, 0x100, 0x10000, 0x1000000, 0x100000000, 0x10000000000, 0, 0));
	EXPECT_THAT(DataType("<d6").shift, ElementsAre(1, 100, 10000, 1000000, 100000000, 10000000000, 0, 0));
	EXPECT_THAT(DataType("<n6").shift, ElementsAre(1, 10, 100, 1000, 10000, 100000, 0, 0));
#ifdef __BIG_ENDIAN__
	EXPECT_THAT(DataType("=i6").shift, ElementsAre(0x10000000000, 0x100000000, 0x1000000, 0x10000, 0x100, 1, 0, 0));
	EXPECT_THAT(DataType("=d6").shift, ElementsAre(10000000000, 100000000, 1000000, 10000, 100, 1, 0, 0));
	EXPECT_THAT(DataType("=n6").shift, ElementsAre(100000, 10000, 1000, 100, 10, 1, 0, 0));
#elif defined(__LITTLE_ENDIAN__)
	EXPECT_THAT(DataType("=i6").shift, ElementsAre(1, 0x100, 0x10000, 0x1000000, 0x100000000, 0x10000000000, 0, 0));
	EXPECT_THAT(DataType("=d6").shift, ElementsAre(1, 100, 10000, 1000000, 100000000, 10000000000, 0, 0));
	EXPECT_THAT(DataType("=n6").shift, ElementsAre(1, 10, 100, 1000, 10000, 100000, 0, 0));
#endif
}

TEST(DataTypeShift, 7) {
	EXPECT_THAT(DataType(">i7").shift, ElementsAre(0x1000000000000, 0x10000000000, 0x100000000, 0x1000000, 0x10000, 0x100, 1, 0));
	EXPECT_THAT(DataType(">d7").shift, ElementsAre(1000000000000, 10000000000, 100000000, 1000000, 10000, 100, 1, 0));
	EXPECT_THAT(DataType(">n7").shift, ElementsAre(1000000, 100000, 10000, 1000, 100, 10, 1, 0));
	EXPECT_THAT(DataType("<i7").shift, ElementsAre(1, 0x100, 0x10000, 0x1000000, 0x100000000, 0x10000000000, 0x1000000000000, 0));
	EXPECT_THAT(DataType("<d7").shift, ElementsAre(1, 100, 10000, 1000000, 100000000, 10000000000, 1000000000000, 0));
	EXPECT_THAT(DataType("<n7").shift, ElementsAre(1, 10, 100, 1000, 10000, 100000, 1000000, 0));
#ifdef __BIG_ENDIAN__
	EXPECT_THAT(DataType("=i7").shift, ElementsAre(0x1000000000000, 0x10000000000, 0x100000000, 0x1000000, 0x10000, 0x100, 1, 0));
	EXPECT_THAT(DataType("=d7").shift, ElementsAre(1000000000000, 10000000000, 100000000, 1000000, 10000, 100, 1, 0));
	EXPECT_THAT(DataType("=n7").shift, ElementsAre(1000000, 100000, 10000, 1000, 100, 10, 1, 0));
#elif defined(__LITTLE_ENDIAN__)
	EXPECT_THAT(DataType("=i7").shift, ElementsAre(1, 0x100, 0x10000, 0x1000000, 0x100000000, 0x10000000000, 0x1000000000000, 0));
	EXPECT_THAT(DataType("=d7").shift, ElementsAre(1, 100, 10000, 1000000, 100000000, 10000000000, 1000000000000, 0));
	EXPECT_THAT(DataType("=n7").shift, ElementsAre(1, 10, 100, 1000, 10000, 100000, 1000000, 0));
#endif
}

TEST(DataTypeShift, 8) {
	EXPECT_THAT(DataType(">i8").shift, ElementsAre(0x100000000000000, 0x1000000000000, 0x10000000000, 0x100000000, 0x1000000, 0x10000, 0x100, 1));
	EXPECT_THAT(DataType(">d8").shift, ElementsAre(100000000000000, 1000000000000, 10000000000, 100000000, 1000000, 10000, 100, 1));
	EXPECT_THAT(DataType(">n8").shift, ElementsAre(10000000, 1000000, 100000, 10000, 1000, 100, 10, 1));
	EXPECT_THAT(DataType("<i8").shift, ElementsAre(1, 0x100, 0x10000, 0x1000000, 0x100000000, 0x10000000000, 0x1000000000000, 0x100000000000000));
	EXPECT_THAT(DataType("<d8").shift, ElementsAre(1, 100, 10000, 1000000, 100000000, 10000000000, 1000000000000, 100000000000000));
	EXPECT_THAT(DataType("<n8").shift, ElementsAre(1, 10, 100, 1000, 10000, 100000, 1000000, 10000000));
#ifdef __BIG_ENDIAN__
	EXPECT_THAT(DataType("=i8").shift, ElementsAre(0x100000000000000, 0x1000000000000, 0x10000000000, 0x100000000, 0x1000000, 0x10000, 0x100, 1));
	EXPECT_THAT(DataType("=d8").shift, ElementsAre(100000000000000, 1000000000000, 10000000000, 100000000, 1000000, 10000, 100, 1));
	EXPECT_THAT(DataType("=n8").shift, ElementsAre(10000000, 1000000, 100000, 10000, 1000, 100, 10, 1));
#elif defined(__LITTLE_ENDIAN__)
	EXPECT_THAT(DataType("=i8").shift, ElementsAre(1, 0x100, 0x10000, 0x1000000, 0x100000000, 0x10000000000, 0x1000000000000, 0x100000000000000));
	EXPECT_THAT(DataType("=d8").shift, ElementsAre(1, 100, 10000, 1000000, 100000000, 10000000000, 1000000000000, 100000000000000));
	EXPECT_THAT(DataType("=n8").shift, ElementsAre(1, 10, 100, 1000, 10000, 100000, 1000000, 10000000));
#endif
}

TEST(DataTypeDecode, x00) {
	uint8_t mem[] { 0x00 };
	EXPECT_EQ(DataType("|i1").decode(mem), 0);
	EXPECT_EQ(DataType("|u1").decode(mem), 0);
	EXPECT_EQ(DataType("|d1").decode(mem), 0);
	EXPECT_EQ(DataType("|n1").decode(mem), 0);
}

TEST(DataTypeDecode, x01) {
	uint8_t mem[] { 0x01 };
	EXPECT_EQ(DataType("|i1").decode(mem), 1);
	EXPECT_EQ(DataType("|u1").decode(mem), 1);
	EXPECT_EQ(DataType("|d1").decode(mem), 1);
	EXPECT_EQ(DataType("|n1").decode(mem), 1);
}

TEST(DataTypeDecode, x10) {
	uint8_t mem[] { 0x10 };
	EXPECT_EQ(DataType("|i1").decode(mem), 16);
	EXPECT_EQ(DataType("|u1").decode(mem), 16);
	EXPECT_EQ(DataType("|d1").decode(mem), 10);
	EXPECT_EQ(DataType("|n1").decode(mem), 0);
}

TEST(DataTypeDecode, x11) {
	uint8_t mem[] { 0x11 };
	EXPECT_EQ(DataType("|i1").decode(mem), 17);
	EXPECT_EQ(DataType("|u1").decode(mem), 17);
	EXPECT_EQ(DataType("|d1").decode(mem), 11);
	EXPECT_EQ(DataType("|n1").decode(mem), 1);
}

TEST(DataTypeDecode, x80) {
	uint8_t mem[] { 0x80 };
	EXPECT_EQ(DataType("|i1").decode(mem), -128);
	EXPECT_EQ(DataType("|u1").decode(mem), 128);
	EXPECT_EQ(DataType("|d1").decode(mem), 80);
	EXPECT_EQ(DataType("|n1").decode(mem), 0);
}

TEST(DataTypeDecode, xFF) {
	uint8_t mem[] { 0xFF };
	EXPECT_EQ(DataType("|i1").decode(mem), -1);
	EXPECT_EQ(DataType("|u1").decode(mem), 255);
	EXPECT_EQ(DataType("|d1").decode(mem), 55);
	EXPECT_EQ(DataType("|n1").decode(mem), 5);
}

TEST(DataTypeDecode, x0000) {
	uint8_t mem[] { 0x00, 0x00 };
	EXPECT_EQ(DataType(">i2").decode(mem), 0);
	EXPECT_EQ(DataType(">u2").decode(mem), 0);
	EXPECT_EQ(DataType(">d2").decode(mem), 0);
	EXPECT_EQ(DataType(">n2").decode(mem), 0);
	EXPECT_EQ(DataType("<i2").decode(mem), 0);
	EXPECT_EQ(DataType("<u2").decode(mem), 0);
	EXPECT_EQ(DataType("<d2").decode(mem), 0);
	EXPECT_EQ(DataType("<n2").decode(mem), 0);
}

TEST(DataTypeDecode, x0001) {
	uint8_t mem[] { 0x00, 0x01 };
	EXPECT_EQ(DataType(">i2").decode(mem), 1);
	EXPECT_EQ(DataType(">u2").decode(mem), 1);
	EXPECT_EQ(DataType(">d2").decode(mem), 1);
	EXPECT_EQ(DataType(">n2").decode(mem), 1);
	EXPECT_EQ(DataType("<i2").decode(mem), 0x100);
	EXPECT_EQ(DataType("<u2").decode(mem), 0x100);
	EXPECT_EQ(DataType("<d2").decode(mem), 100);
	EXPECT_EQ(DataType("<n2").decode(mem), 10);
}

TEST(DataTypeDecode, x0010) {
	uint8_t mem[] { 0x00, 0x10 };
	EXPECT_EQ(DataType(">i2").decode(mem), 0x10);
	EXPECT_EQ(DataType(">u2").decode(mem), 0x10);
	EXPECT_EQ(DataType(">d2").decode(mem), 10);
	EXPECT_EQ(DataType(">n2").decode(mem), 0);
	EXPECT_EQ(DataType("<i2").decode(mem), 0x1000);
	EXPECT_EQ(DataType("<u2").decode(mem), 0x1000);
	EXPECT_EQ(DataType("<d2").decode(mem), 1000);
	EXPECT_EQ(DataType("<n2").decode(mem), 0);
}

TEST(DataTypeDecode, x0080) {
	uint8_t mem[] { 0x00, 0x80 };
	EXPECT_EQ(DataType(">i2").decode(mem), 0x80);
	EXPECT_EQ(DataType(">u2").decode(mem), 0x80);
	EXPECT_EQ(DataType(">d2").decode(mem), 80);
	EXPECT_EQ(DataType(">n2").decode(mem), 0);
	EXPECT_EQ(DataType("<i2").decode(mem), (int16_t) 0x8000);
	EXPECT_EQ(DataType("<u2").decode(mem), 0x8000);
	EXPECT_EQ(DataType("<d2").decode(mem), 8000);
	EXPECT_EQ(DataType("<n2").decode(mem), 0);
}

TEST(DataTypeDecode, x0100) {
	uint8_t mem[] { 0x01, 0x00 };
	EXPECT_EQ(DataType(">i2").decode(mem), 0x100);
	EXPECT_EQ(DataType(">u2").decode(mem), 0x100);
	EXPECT_EQ(DataType(">d2").decode(mem), 100);
	EXPECT_EQ(DataType(">n2").decode(mem), 10);
	EXPECT_EQ(DataType("<i2").decode(mem), 1);
	EXPECT_EQ(DataType("<u2").decode(mem), 1);
	EXPECT_EQ(DataType("<d2").decode(mem), 1);
	EXPECT_EQ(DataType("<n2").decode(mem), 1);
}

TEST(DataTypeDecode, x1000) {
	uint8_t mem[] { 0x10, 0x00 };
	EXPECT_EQ(DataType(">i2").decode(mem), 0x1000);
	EXPECT_EQ(DataType(">u2").decode(mem), 0x1000);
	EXPECT_EQ(DataType(">d2").decode(mem), 1000);
	EXPECT_EQ(DataType(">n2").decode(mem), 0);
	EXPECT_EQ(DataType("<i2").decode(mem), 0x10);
	EXPECT_EQ(DataType("<u2").decode(mem), 0x10);
	EXPECT_EQ(DataType("<d2").decode(mem), 10);
	EXPECT_EQ(DataType("<n2").decode(mem), 0);
}

TEST(DataTypeDecode, x8000) {
	uint8_t mem[] { 0x80, 0x00 };
	EXPECT_EQ(DataType(">i2").decode(mem), (int16_t) 0x8000);
	EXPECT_EQ(DataType(">u2").decode(mem), 0x8000);
	EXPECT_EQ(DataType(">d2").decode(mem), 8000);
	EXPECT_EQ(DataType(">n2").decode(mem), 0);
	EXPECT_EQ(DataType("<i2").decode(mem), 0x80);
	EXPECT_EQ(DataType("<u2").decode(mem), 0x80);
	EXPECT_EQ(DataType("<d2").decode(mem), 80);
	EXPECT_EQ(DataType("<n2").decode(mem), 0);
}

TEST(DataTypeDecode, x0101) {
	uint8_t mem[] { 0x01, 0x01 };
	EXPECT_EQ(DataType(">i2").decode(mem), 0x101);
	EXPECT_EQ(DataType(">u2").decode(mem), 0x101);
	EXPECT_EQ(DataType(">d2").decode(mem), 101);
	EXPECT_EQ(DataType(">n2").decode(mem), 11);
	EXPECT_EQ(DataType("<i2").decode(mem), 0x101);
	EXPECT_EQ(DataType("<u2").decode(mem), 0x101);
	EXPECT_EQ(DataType("<d2").decode(mem), 101);
	EXPECT_EQ(DataType("<n2").decode(mem), 11);
}

TEST(DataTypeDecode, x0110) {
	uint8_t mem[] { 0x01, 0x10 };
	EXPECT_EQ(DataType(">i2").decode(mem), 0x110);
	EXPECT_EQ(DataType(">u2").decode(mem), 0x110);
	EXPECT_EQ(DataType(">d2").decode(mem), 110);
	EXPECT_EQ(DataType(">n2").decode(mem), 10);
	EXPECT_EQ(DataType("<i2").decode(mem), 0x1001);
	EXPECT_EQ(DataType("<u2").decode(mem), 0x1001);
	EXPECT_EQ(DataType("<d2").decode(mem), 1001);
	EXPECT_EQ(DataType("<n2").decode(mem), 1);
}

TEST(DataTypeDecode, x0180) {
	uint8_t mem[] { 0x01, 0x80 };
	EXPECT_EQ(DataType(">i2").decode(mem), 0x180);
	EXPECT_EQ(DataType(">u2").decode(mem), 0x180);
	EXPECT_EQ(DataType(">d2").decode(mem), 180);
	EXPECT_EQ(DataType(">n2").decode(mem), 10);
	EXPECT_EQ(DataType("<i2").decode(mem), (int16_t) 0x8001);
	EXPECT_EQ(DataType("<u2").decode(mem), 0x8001);
	EXPECT_EQ(DataType("<d2").decode(mem), 8001);
	EXPECT_EQ(DataType("<n2").decode(mem), 1);
}

TEST(DataTypeDecode, x0888) {
	uint8_t mem[] { 0x08, 0x88 };
	EXPECT_EQ(DataType(">i2").decode(mem), 0x888);
	EXPECT_EQ(DataType(">u2").decode(mem), 0x888);
	EXPECT_EQ(DataType(">d2").decode(mem), 888);
	EXPECT_EQ(DataType(">n2").decode(mem), 88);
	EXPECT_EQ(DataType("<i2").decode(mem), (int16_t) 0x8808);
	EXPECT_EQ(DataType("<u2").decode(mem), 0x8808);
	EXPECT_EQ(DataType("<d2").decode(mem), 8808);
	EXPECT_EQ(DataType("<n2").decode(mem), 88);
}

TEST(DataTypeDecode, x8001) {
	uint8_t mem[] { 0x80, 0x01 };
	EXPECT_EQ(DataType(">i2").decode(mem), (int16_t) 0x8001);
	EXPECT_EQ(DataType(">u2").decode(mem), 0x8001);
	EXPECT_EQ(DataType(">d2").decode(mem), 8001);
	EXPECT_EQ(DataType(">n2").decode(mem), 1);
	EXPECT_EQ(DataType("<i2").decode(mem), 0x180);
	EXPECT_EQ(DataType("<u2").decode(mem), 0x180);
	EXPECT_EQ(DataType("<d2").decode(mem), 180);
	EXPECT_EQ(DataType("<n2").decode(mem), 10);
}

TEST(DataTypeDecode, x000000) {
	uint8_t mem[] { 0x00, 0x00, 0x00 };
	EXPECT_EQ(DataType(">i3").decode(mem), 0);
	EXPECT_EQ(DataType(">u3").decode(mem), 0);
	EXPECT_EQ(DataType(">d3").decode(mem), 0);
	EXPECT_EQ(DataType(">n3").decode(mem), 0);
	EXPECT_EQ(DataType("<i3").decode(mem), 0);
	EXPECT_EQ(DataType("<u3").decode(mem), 0);
	EXPECT_EQ(DataType("<d3").decode(mem), 0);
	EXPECT_EQ(DataType("<n3").decode(mem), 0);
}

TEST(DataTypeDecode, x000001) {
	uint8_t mem[] { 0x00, 0x00, 0x01 };
	EXPECT_EQ(DataType(">i3").decode(mem), 1);
	EXPECT_EQ(DataType(">u3").decode(mem), 1);
	EXPECT_EQ(DataType(">d3").decode(mem), 1);
	EXPECT_EQ(DataType(">n3").decode(mem), 1);
	EXPECT_EQ(DataType("<i3").decode(mem), 0x10000);
	EXPECT_EQ(DataType("<u3").decode(mem), 0x10000);
	EXPECT_EQ(DataType("<d3").decode(mem), 10000);
	EXPECT_EQ(DataType("<n3").decode(mem), 100);
}

TEST(DataTypeDecode, x000080) {
	uint8_t mem[] { 0x00, 0x00, 0x80 };
	EXPECT_EQ(DataType(">i3").decode(mem), 0x80);
	EXPECT_EQ(DataType(">u3").decode(mem), 0x80);
	EXPECT_EQ(DataType(">d3").decode(mem), 80);
	EXPECT_EQ(DataType(">n3").decode(mem), 0);
	EXPECT_EQ(DataType("<i3").decode(mem), -0x800000);
	EXPECT_EQ(DataType("<u3").decode(mem), 0x800000);
	EXPECT_EQ(DataType("<d3").decode(mem), 800000);
	EXPECT_EQ(DataType("<n3").decode(mem), 0);
}

TEST(DataTypeDecode, x000100) {
	uint8_t mem[] { 0x00, 0x01, 0x00 };
	EXPECT_EQ(DataType(">i3").decode(mem), 0x100);
	EXPECT_EQ(DataType(">u3").decode(mem), 0x100);
	EXPECT_EQ(DataType(">d3").decode(mem), 100);
	EXPECT_EQ(DataType(">n3").decode(mem), 10);
	EXPECT_EQ(DataType("<i3").decode(mem), 0x100);
	EXPECT_EQ(DataType("<u3").decode(mem), 0x100);
	EXPECT_EQ(DataType("<d3").decode(mem), 100);
	EXPECT_EQ(DataType("<n3").decode(mem), 10);
}

TEST(DataTypeDecode, x008000) {
	uint8_t mem[] { 0x00, 0x80, 0x00 };
	EXPECT_EQ(DataType(">i3").decode(mem), 0x8000);
	EXPECT_EQ(DataType(">u3").decode(mem), 0x8000);
	EXPECT_EQ(DataType(">d3").decode(mem), 8000);
	EXPECT_EQ(DataType(">n3").decode(mem), 0);
	EXPECT_EQ(DataType("<i3").decode(mem), 0x8000);
	EXPECT_EQ(DataType("<u3").decode(mem), 0x8000);
	EXPECT_EQ(DataType("<d3").decode(mem), 8000);
	EXPECT_EQ(DataType("<n3").decode(mem), 0);
}

TEST(DataTypeDecode, x010000) {
	uint8_t mem[] { 0x01, 0x00, 0x00 };
	EXPECT_EQ(DataType(">i3").decode(mem), 0x10000);
	EXPECT_EQ(DataType(">u3").decode(mem), 0x10000);
	EXPECT_EQ(DataType(">d3").decode(mem), 10000);
	EXPECT_EQ(DataType(">n3").decode(mem), 100);
	EXPECT_EQ(DataType("<i3").decode(mem), 1);
	EXPECT_EQ(DataType("<u3").decode(mem), 1);
	EXPECT_EQ(DataType("<d3").decode(mem), 1);
	EXPECT_EQ(DataType("<n3").decode(mem), 1);
}

TEST(DataTypeDecode, x800000) {
	uint8_t mem[] { 0x80, 0x00, 0x00 };
	EXPECT_EQ(DataType(">i3").decode(mem), -0x800000);
	EXPECT_EQ(DataType(">u3").decode(mem), 0x800000);
	EXPECT_EQ(DataType(">d3").decode(mem), 800000);
	EXPECT_EQ(DataType(">n3").decode(mem), 0);
	EXPECT_EQ(DataType("<i3").decode(mem), 0x80);
	EXPECT_EQ(DataType("<u3").decode(mem), 0x80);
	EXPECT_EQ(DataType("<d3").decode(mem), 80);
	EXPECT_EQ(DataType("<n3").decode(mem), 0);
}

TEST(DataTypeDecode, x00000000) {
	uint8_t mem[] { 0x00, 0x00, 0x00, 0x00 };
	EXPECT_EQ(DataType(">i4").decode(mem), 0);
	EXPECT_EQ(DataType(">u4").decode(mem), 0);
	EXPECT_EQ(DataType(">d4").decode(mem), 0);
	EXPECT_EQ(DataType(">n4").decode(mem), 0);
	EXPECT_EQ(DataType("<i4").decode(mem), 0);
	EXPECT_EQ(DataType("<u4").decode(mem), 0);
	EXPECT_EQ(DataType("<d4").decode(mem), 0);
	EXPECT_EQ(DataType("<n4").decode(mem), 0);
	EXPECT_EQ(DataType("<>i4").decode(mem), 0);
	EXPECT_EQ(DataType("<>u4").decode(mem), 0);
	EXPECT_EQ(DataType("<>d4").decode(mem), 0);
	EXPECT_EQ(DataType("<>n4").decode(mem), 0);
	EXPECT_EQ(DataType("><i4").decode(mem), 0);
	EXPECT_EQ(DataType("><u4").decode(mem), 0);
	EXPECT_EQ(DataType("><d4").decode(mem), 0);
	EXPECT_EQ(DataType("><n4").decode(mem), 0);
}

TEST(DataTypeDecode, x00000001) {
	uint8_t mem[] { 0x00, 0x00, 0x00, 0x01 };
	EXPECT_EQ(DataType(">i4").decode(mem), 1);
	EXPECT_EQ(DataType(">u4").decode(mem), 1);
	EXPECT_EQ(DataType(">d4").decode(mem), 1);
	EXPECT_EQ(DataType(">n4").decode(mem), 1);
	EXPECT_EQ(DataType("<i4").decode(mem), 0x1000000);
	EXPECT_EQ(DataType("<u4").decode(mem), 0x1000000);
	EXPECT_EQ(DataType("<d4").decode(mem), 1000000);
	EXPECT_EQ(DataType("<n4").decode(mem), 1000);
	EXPECT_EQ(DataType("><i4").decode(mem), 0x100);
	EXPECT_EQ(DataType("><u4").decode(mem), 0x100);
	EXPECT_EQ(DataType("><d4").decode(mem), 100);
	EXPECT_EQ(DataType("><n4").decode(mem), 10);
	EXPECT_EQ(DataType("<>i4").decode(mem), 0x10000);
	EXPECT_EQ(DataType("<>u4").decode(mem), 0x10000);
	EXPECT_EQ(DataType("<>d4").decode(mem), 10000);
	EXPECT_EQ(DataType("<>n4").decode(mem), 100);
}

TEST(DataTypeDecode, x00000080) {
	uint8_t mem[] { 0x00, 0x00, 0x00, 0x80 };
	EXPECT_EQ(DataType(">i4").decode(mem), 0x80);
	EXPECT_EQ(DataType(">u4").decode(mem), 0x80);
	EXPECT_EQ(DataType(">d4").decode(mem), 80);
	EXPECT_EQ(DataType("<i4").decode(mem), (int32_t) 0x80000000);
	EXPECT_EQ(DataType("<u4").decode(mem), 0x80000000);
	EXPECT_EQ(DataType("<d4").decode(mem), 80000000);
	EXPECT_EQ(DataType("><i4").decode(mem), 0x8000);
	EXPECT_EQ(DataType("><u4").decode(mem), 0x8000);
	EXPECT_EQ(DataType("><d4").decode(mem), 8000);
	EXPECT_EQ(DataType("<>i4").decode(mem), 0x800000);
	EXPECT_EQ(DataType("<>u4").decode(mem), 0x800000);
	EXPECT_EQ(DataType("<>d4").decode(mem), 800000);
}

TEST(DataTypeDecode, x00000100) {
	uint8_t mem[] { 0x00, 0x00, 0x01, 0x00 };
	EXPECT_EQ(DataType(">i4").decode(mem), 0x100);
	EXPECT_EQ(DataType(">u4").decode(mem), 0x100);
	EXPECT_EQ(DataType(">d4").decode(mem), 100);
	EXPECT_EQ(DataType(">n4").decode(mem), 10);
	EXPECT_EQ(DataType("<i4").decode(mem), 0x10000);
	EXPECT_EQ(DataType("<u4").decode(mem), 0x10000);
	EXPECT_EQ(DataType("<d4").decode(mem), 10000);
	EXPECT_EQ(DataType("<n4").decode(mem), 100);
	EXPECT_EQ(DataType("><i4").decode(mem), 1);
	EXPECT_EQ(DataType("><u4").decode(mem), 1);
	EXPECT_EQ(DataType("><d4").decode(mem), 1);
	EXPECT_EQ(DataType("><n4").decode(mem), 1);
	EXPECT_EQ(DataType("<>i4").decode(mem), 0x1000000);
	EXPECT_EQ(DataType("<>u4").decode(mem), 0x1000000);
	EXPECT_EQ(DataType("<>d4").decode(mem), 1000000);
	EXPECT_EQ(DataType("<>n4").decode(mem), 1000);
}

TEST(DataTypeDecode, x00000180) {
	uint8_t mem[] { 0x00, 0x00, 0x01, 0x80 };
	EXPECT_EQ(DataType(">i4").decode(mem), 0x180);
	EXPECT_EQ(DataType(">u4").decode(mem), 0x180);
	EXPECT_EQ(DataType(">d4").decode(mem), 180);
	EXPECT_EQ(DataType(">n4").decode(mem), 10);
	EXPECT_EQ(DataType("<i4").decode(mem), (int32_t) 0x80010000);
	EXPECT_EQ(DataType("<u4").decode(mem), 0x80010000);
	EXPECT_EQ(DataType("<d4").decode(mem), 80010000);
	EXPECT_EQ(DataType("<n4").decode(mem), 100);
	EXPECT_EQ(DataType("><i4").decode(mem), 0x8001);
	EXPECT_EQ(DataType("><u4").decode(mem), 0x8001);
	EXPECT_EQ(DataType("><d4").decode(mem), 8001);
	EXPECT_EQ(DataType("><n4").decode(mem), 1);
	EXPECT_EQ(DataType("<>i4").decode(mem), 0x1800000);
	EXPECT_EQ(DataType("<>u4").decode(mem), 0x1800000);
	EXPECT_EQ(DataType("<>d4").decode(mem), 1800000);
	EXPECT_EQ(DataType("<>n4").decode(mem), 1000);
}

TEST(DataTypeDecode, x00008000) {
	uint8_t mem[] { 0x00, 0x00, 0x80, 0x00 };
	EXPECT_EQ(DataType(">i4").decode(mem), 0x8000);
	EXPECT_EQ(DataType(">u4").decode(mem), 0x8000);
	EXPECT_EQ(DataType(">d4").decode(mem), 8000);
	EXPECT_EQ(DataType("<i4").decode(mem), 0x800000);
	EXPECT_EQ(DataType("<u4").decode(mem), 0x800000);
	EXPECT_EQ(DataType("<d4").decode(mem), 800000);
	EXPECT_EQ(DataType("><i4").decode(mem), 0x80);
	EXPECT_EQ(DataType("><u4").decode(mem), 0x80);
	EXPECT_EQ(DataType("><d4").decode(mem), 80);
	EXPECT_EQ(DataType("<>i4").decode(mem), (int32_t) 0x80000000);
	EXPECT_EQ(DataType("<>u4").decode(mem), 0x80000000);
	EXPECT_EQ(DataType("<>d4").decode(mem), 80000000);
}

TEST(DataTypeDecode, x00008001) {
	uint8_t mem[] { 0x00, 0x00, 0x80, 0x01 };
	EXPECT_EQ(DataType(">i4").decode(mem), 0x8001);
	EXPECT_EQ(DataType(">u4").decode(mem), 0x8001);
	EXPECT_EQ(DataType(">d4").decode(mem), 8001);
	EXPECT_EQ(DataType("<i4").decode(mem), 0x1800000);
	EXPECT_EQ(DataType("<u4").decode(mem), 0x1800000);
	EXPECT_EQ(DataType("<d4").decode(mem), 1800000);
	EXPECT_EQ(DataType("><i4").decode(mem), 0x180);
	EXPECT_EQ(DataType("><u4").decode(mem), 0x180);
	EXPECT_EQ(DataType("><d4").decode(mem), 180);
	EXPECT_EQ(DataType("<>i4").decode(mem), (int32_t) 0x80010000);
	EXPECT_EQ(DataType("<>u4").decode(mem), 0x80010000);
	EXPECT_EQ(DataType("<>d4").decode(mem), 80010000);
}

TEST(DataTypeDecode, x00010000) {
	uint8_t mem[] { 0x00, 0x01, 0x00, 0x00 };
	EXPECT_EQ(DataType(">i4").decode(mem), 0x10000);
	EXPECT_EQ(DataType(">u4").decode(mem), 0x10000);
	EXPECT_EQ(DataType(">d4").decode(mem), 10000);
	EXPECT_EQ(DataType("<i4").decode(mem), 0x100);
	EXPECT_EQ(DataType("<u4").decode(mem), 0x100);
	EXPECT_EQ(DataType("<d4").decode(mem), 100);
	EXPECT_EQ(DataType("><i4").decode(mem), 0x1000000);
	EXPECT_EQ(DataType("><u4").decode(mem), 0x1000000);
	EXPECT_EQ(DataType("><d4").decode(mem), 1000000);
	EXPECT_EQ(DataType("<>i4").decode(mem), 1);
	EXPECT_EQ(DataType("<>u4").decode(mem), 1);
	EXPECT_EQ(DataType("<>d4").decode(mem), 1);
}

TEST(DataTypeDecode, x00010080) {
	uint8_t mem[] { 0x00, 0x01, 0x00, 0x80 };
	EXPECT_EQ(DataType(">i4").decode(mem), 0x10080);
	EXPECT_EQ(DataType(">u4").decode(mem), 0x10080);
	EXPECT_EQ(DataType(">d4").decode(mem), 10080);
	EXPECT_EQ(DataType("<i4").decode(mem), (int32_t) 0x80000100);
	EXPECT_EQ(DataType("<u4").decode(mem), 0x80000100);
	EXPECT_EQ(DataType("<d4").decode(mem), 80000100);
	EXPECT_EQ(DataType("><i4").decode(mem), 0x1008000);
	EXPECT_EQ(DataType("><u4").decode(mem), 0x1008000);
	EXPECT_EQ(DataType("><d4").decode(mem), 1008000);
	EXPECT_EQ(DataType("<>i4").decode(mem), 0x800001);
	EXPECT_EQ(DataType("<>u4").decode(mem), 0x800001);
	EXPECT_EQ(DataType("<>d4").decode(mem), 800001);
}

TEST(DataTypeDecode, x00018000) {
	uint8_t mem[] { 0x00, 0x01, 0x80, 0x00 };
	EXPECT_EQ(DataType(">i4").decode(mem), 0x18000);
	EXPECT_EQ(DataType(">u4").decode(mem), 0x18000);
	EXPECT_EQ(DataType(">d4").decode(mem), 18000);
	EXPECT_EQ(DataType("<i4").decode(mem), 0x800100);
	EXPECT_EQ(DataType("<u4").decode(mem), 0x800100);
	EXPECT_EQ(DataType("<d4").decode(mem), 800100);
	EXPECT_EQ(DataType("><i4").decode(mem), 0x1000080);
	EXPECT_EQ(DataType("><u4").decode(mem), 0x1000080);
	EXPECT_EQ(DataType("><d4").decode(mem), 1000080);
	EXPECT_EQ(DataType("<>i4").decode(mem), (int32_t) 0x80000001);
	EXPECT_EQ(DataType("<>u4").decode(mem), 0x80000001);
	EXPECT_EQ(DataType("<>d4").decode(mem), 80000001);
}

TEST(DataTypeDecode, x00018001) {
	uint8_t mem[] { 0x00, 0x01, 0x80, 0x01 };
	EXPECT_EQ(DataType(">i4").decode(mem), 0x18001);
	EXPECT_EQ(DataType(">u4").decode(mem), 0x18001);
	EXPECT_EQ(DataType(">d4").decode(mem), 18001);
	EXPECT_EQ(DataType("<i4").decode(mem), 0x1800100);
	EXPECT_EQ(DataType("<u4").decode(mem), 0x1800100);
	EXPECT_EQ(DataType("<d4").decode(mem), 1800100);
	EXPECT_EQ(DataType("><i4").decode(mem), 0x1000180);
	EXPECT_EQ(DataType("><u4").decode(mem), 0x1000180);
	EXPECT_EQ(DataType("><d4").decode(mem), 1000180);
	EXPECT_EQ(DataType("<>i4").decode(mem), (int32_t) 0x80010001);
	EXPECT_EQ(DataType("<>u4").decode(mem), 0x80010001);
	EXPECT_EQ(DataType("<>d4").decode(mem), 80010001);
}

TEST(DataTypeDecode, x00800000) {
	uint8_t mem[] { 0x00, 0x80, 0x00, 0x00 };
	EXPECT_EQ(DataType(">i4").decode(mem), 0x800000);
	EXPECT_EQ(DataType(">u4").decode(mem), 0x800000);
	EXPECT_EQ(DataType(">d4").decode(mem), 800000);
	EXPECT_EQ(DataType("<i4").decode(mem), 0x8000);
	EXPECT_EQ(DataType("<u4").decode(mem), 0x8000);
	EXPECT_EQ(DataType("<d4").decode(mem), 8000);
	EXPECT_EQ(DataType("><i4").decode(mem), (int32_t) 0x80000000);
	EXPECT_EQ(DataType("><u4").decode(mem), 0x80000000);
	EXPECT_EQ(DataType("><d4").decode(mem), 80000000);
	EXPECT_EQ(DataType("<>i4").decode(mem), 0x80);
	EXPECT_EQ(DataType("<>u4").decode(mem), 0x80);
	EXPECT_EQ(DataType("<>d4").decode(mem), 80);
}

TEST(DataTypeDecode, x80000000) {
	uint8_t mem[] { 0x80, 0x00, 0x00, 0x00 };
	EXPECT_EQ(DataType(">i4").decode(mem), (int32_t) 0x80000000);
	EXPECT_EQ(DataType(">u4").decode(mem), 0x80000000);
	EXPECT_EQ(DataType(">d4").decode(mem), 80000000);
	EXPECT_EQ(DataType("<i4").decode(mem), 0x80);
	EXPECT_EQ(DataType("<u4").decode(mem), 0x80);
	EXPECT_EQ(DataType("<d4").decode(mem), 80);
	EXPECT_EQ(DataType("><i4").decode(mem), 0x800000);
	EXPECT_EQ(DataType("><u4").decode(mem), 0x800000);
	EXPECT_EQ(DataType("><d4").decode(mem), 800000);
	EXPECT_EQ(DataType("<>i4").decode(mem), 0x8000);
	EXPECT_EQ(DataType("<>u4").decode(mem), 0x8000);
	EXPECT_EQ(DataType("<>d4").decode(mem), 8000);
}

TEST(DataTypeDecode, x0000000000) {
	uint8_t mem[] { 0x00, 0x00, 0x00, 0x00, 0x00 };
	EXPECT_EQ(DataType(">i5").decode(mem), 0);
	EXPECT_EQ(DataType(">u5").decode(mem), 0);
	EXPECT_EQ(DataType(">d5").decode(mem), 0);
	EXPECT_EQ(DataType(">n5").decode(mem), 0);
	EXPECT_EQ(DataType("<i5").decode(mem), 0);
	EXPECT_EQ(DataType("<u5").decode(mem), 0);
	EXPECT_EQ(DataType("<d5").decode(mem), 0);
	EXPECT_EQ(DataType("<n5").decode(mem), 0);
}

TEST(DataTypeDecode, x0000000001) {
	uint8_t mem[] { 0x00, 0x00, 0x00, 0x00, 0x01 };
	EXPECT_EQ(DataType(">i5").decode(mem), 1);
	EXPECT_EQ(DataType(">u5").decode(mem), 1);
	EXPECT_EQ(DataType(">d5").decode(mem), 1);
	EXPECT_EQ(DataType(">n5").decode(mem), 1);
	EXPECT_EQ(DataType("<i5").decode(mem), 0x100000000);
	EXPECT_EQ(DataType("<u5").decode(mem), 0x100000000);
	EXPECT_EQ(DataType("<d5").decode(mem), 100000000);
	EXPECT_EQ(DataType("<n5").decode(mem), 10000);
}

TEST(DataTypeDecode, x0000000080) {
	uint8_t mem[] { 0x00, 0x00, 0x00, 0x00, 0x80 };
	EXPECT_EQ(DataType(">i5").decode(mem), 0x80);
	EXPECT_EQ(DataType(">u5").decode(mem), 0x80);
	EXPECT_EQ(DataType(">d5").decode(mem), 80);
	EXPECT_EQ(DataType("<i5").decode(mem), -0x8000000000);
	EXPECT_EQ(DataType("<u5").decode(mem), 0x8000000000);
	EXPECT_EQ(DataType("<d5").decode(mem), 8000000000);
}

TEST(DataTypeDecode, x0100000000) {
	uint8_t mem[] { 0x01, 0x00, 0x00, 0x00, 0x00 };
	EXPECT_EQ(DataType(">i5").decode(mem), 0x100000000);
	EXPECT_EQ(DataType(">u5").decode(mem), 0x100000000);
	EXPECT_EQ(DataType(">d5").decode(mem), 100000000);
	EXPECT_EQ(DataType(">n5").decode(mem), 10000);
	EXPECT_EQ(DataType("<i5").decode(mem), 1);
	EXPECT_EQ(DataType("<u5").decode(mem), 1);
	EXPECT_EQ(DataType("<d5").decode(mem), 1);
	EXPECT_EQ(DataType("<n5").decode(mem), 1);
}

TEST(DataTypeDecode, x8000000000) {
	uint8_t mem[] { 0x80, 0x00, 0x00, 0x00, 0x00 };
	EXPECT_EQ(DataType(">i5").decode(mem), -0x8000000000);
	EXPECT_EQ(DataType(">u5").decode(mem), 0x8000000000);
	EXPECT_EQ(DataType(">d5").decode(mem), 8000000000);
	EXPECT_EQ(DataType("<i5").decode(mem), 0x80);
	EXPECT_EQ(DataType("<u5").decode(mem), 0x80);
	EXPECT_EQ(DataType("<d5").decode(mem), 80);
}

TEST(DataTypeEncode, x00) {
	uint8_t mem[1] {};
	DataType("|i1").encode(mem, 0);
	EXPECT_THAT(mem, ElementsAre(0));
	DataType("|u1").encode(mem, 0);
	EXPECT_THAT(mem, ElementsAre(0));
	DataType("|d1").encode(mem, 0);
	EXPECT_THAT(mem, ElementsAre(0));
	DataType("|n1").encode(mem, 0);
	EXPECT_THAT(mem, ElementsAre(0));
}

TEST(DataTypeEncode, x01) {
	uint8_t mem[1] {};
	DataType("|i1").encode(mem, 1);
	EXPECT_THAT(mem, ElementsAre(0x01));
	DataType("|u1").encode(mem, 1);
	EXPECT_THAT(mem, ElementsAre(0x01));
	DataType("|d1").encode(mem, 1);
	EXPECT_THAT(mem, ElementsAre(0x01));
	DataType("|n1").encode(mem, 1);
	EXPECT_THAT(mem, ElementsAre(0x01));
}

TEST(DataTypeEncode, x10) {
	uint8_t mem[1] {};
	DataType("|i1").encode(mem, 0x10);
	EXPECT_THAT(mem, ElementsAre(0x10));
	DataType("|u1").encode(mem, 0x10);
	EXPECT_THAT(mem, ElementsAre(0x10));
	DataType("|d1").encode(mem, 0x10);
	EXPECT_THAT(mem, ElementsAre(0x16));
	DataType("|n1").encode(mem, 0x10);
	EXPECT_THAT(mem, ElementsAre(6));
}

TEST(DataTypeEncode, x80) {
	uint8_t mem[1] {};
	DataType("|i1").encode(mem, 0x80);
	EXPECT_THAT(mem, ElementsAre(0x80));
	DataType("|u1").encode(mem, 0x80);
	EXPECT_THAT(mem, ElementsAre(0x80));
	DataType("|d1").encode(mem, 0x80);
	EXPECT_THAT(mem, ElementsAre(0x28));
	DataType("|n1").encode(mem, 0x80);
	EXPECT_THAT(mem, ElementsAre(8));
}

TEST(DataTypeEncode, d10) {
	uint8_t mem[1] {};
	DataType("|i1").encode(mem, 10);
	EXPECT_THAT(mem, ElementsAre(0x0A));
	DataType("|u1").encode(mem, 10);
	EXPECT_THAT(mem, ElementsAre(0x0A));
	DataType("|d1").encode(mem, 10);
	EXPECT_THAT(mem, ElementsAre(0x10));
	DataType("|n1").encode(mem, 10);
	EXPECT_THAT(mem, ElementsAre(0));
}

TEST(DataTypeEncode, d80) {
	uint8_t mem[1] {};
	DataType("|i1").encode(mem, 80);
	EXPECT_THAT(mem, ElementsAre(0x50));
	DataType("|u1").encode(mem, 80);
	EXPECT_THAT(mem, ElementsAre(0x50));
	DataType("|d1").encode(mem, 80);
	EXPECT_THAT(mem, ElementsAre(0x80));
	DataType("|n1").encode(mem, 80);
	EXPECT_THAT(mem, ElementsAre(0));
}

TEST(DataTypeEncode, dN01) {
	uint8_t mem[1] {};
	DataType("|i1").encode(mem, -1);
	EXPECT_THAT(mem, ElementsAre(0xFF));
}

TEST(DataTypeEncode, x0000) {
	uint8_t mem[2] {};
	DataType(">i2").encode(mem, 0);
	EXPECT_THAT(mem, ElementsAre(0, 0));
	DataType(">u2").encode(mem, 0);
	EXPECT_THAT(mem, ElementsAre(0, 0));
	DataType(">d2").encode(mem, 0);
	EXPECT_THAT(mem, ElementsAre(0, 0));
	DataType(">n2").encode(mem, 0);
	EXPECT_THAT(mem, ElementsAre(0, 0));
	DataType("<i2").encode(mem, 0);
	EXPECT_THAT(mem, ElementsAre(0, 0));
	DataType("<u2").encode(mem, 0);
	EXPECT_THAT(mem, ElementsAre(0, 0));
	DataType("<d2").encode(mem, 0);
	EXPECT_THAT(mem, ElementsAre(0, 0));
	DataType("<n2").encode(mem, 0);
	EXPECT_THAT(mem, ElementsAre(0, 0));
}

TEST(DataTypeEncode, x0001) {
	uint8_t mem[2] {};
	DataType(">i2").encode(mem, 1);
	EXPECT_THAT(mem, ElementsAre(0, 1));
	DataType(">u2").encode(mem, 1);
	EXPECT_THAT(mem, ElementsAre(0, 1));
	DataType(">d2").encode(mem, 1);
	EXPECT_THAT(mem, ElementsAre(0, 1));
	DataType(">n2").encode(mem, 1);
	EXPECT_THAT(mem, ElementsAre(0, 1));
	DataType("<i2").encode(mem, 1);
	EXPECT_THAT(mem, ElementsAre(1, 0));
	DataType("<u2").encode(mem, 1);
	EXPECT_THAT(mem, ElementsAre(1, 0));
	DataType("<d2").encode(mem, 1);
	EXPECT_THAT(mem, ElementsAre(1, 0));
	DataType("<n2").encode(mem, 1);
	EXPECT_THAT(mem, ElementsAre(1, 0));
}

TEST(DataTypeEncode, x0010) {
	uint8_t mem[2] {};
	DataType(">i2").encode(mem, 0x10);
	EXPECT_THAT(mem, ElementsAre(0, 0x10));
	DataType(">u2").encode(mem, 0x10);
	EXPECT_THAT(mem, ElementsAre(0, 0x10));
	DataType(">d2").encode(mem, 0x10);
	EXPECT_THAT(mem, ElementsAre(0, 0x16));
	DataType(">n2").encode(mem, 0x10);
	EXPECT_THAT(mem, ElementsAre(1, 6));
	DataType("<i2").encode(mem, 0x10);
	EXPECT_THAT(mem, ElementsAre(0x10, 0));
	DataType("<u2").encode(mem, 0x10);
	EXPECT_THAT(mem, ElementsAre(0x10, 0));
	DataType("<d2").encode(mem, 0x10);
	EXPECT_THAT(mem, ElementsAre(0x16, 0));
	DataType("<n2").encode(mem, 0x10);
	EXPECT_THAT(mem, ElementsAre(6, 1));
}

TEST(DataTypeEncode, x0100) {
	uint8_t mem[2] {};
	DataType(">i2").encode(mem, 0x100);
	EXPECT_THAT(mem, ElementsAre(1, 0));
	DataType(">u2").encode(mem, 0x100);
	EXPECT_THAT(mem, ElementsAre(1, 0));
	DataType(">d2").encode(mem, 0x100);
	EXPECT_THAT(mem, ElementsAre(2, 0x56));
	DataType(">n2").encode(mem, 0x100);
	EXPECT_THAT(mem, ElementsAre(5, 6));
	DataType("<i2").encode(mem, 0x100);
	EXPECT_THAT(mem, ElementsAre(0, 1));
	DataType("<u2").encode(mem, 0x100);
	EXPECT_THAT(mem, ElementsAre(0, 1));
	DataType("<d2").encode(mem, 0x100);
	EXPECT_THAT(mem, ElementsAre(0x56, 2));
	DataType("<n2").encode(mem, 0x100);
	EXPECT_THAT(mem, ElementsAre(6, 5));
}

TEST(DataTypeEncode, x0110) {
	uint8_t mem[2] {};
	DataType(">i2").encode(mem, 0x110);
	EXPECT_THAT(mem, ElementsAre(1, 0x10));
	DataType(">u2").encode(mem, 0x110);
	EXPECT_THAT(mem, ElementsAre(1, 0x10));
	DataType(">d2").encode(mem, 0x110);
	EXPECT_THAT(mem, ElementsAre(2, 0x72));
	DataType(">n2").encode(mem, 0x110);
	EXPECT_THAT(mem, ElementsAre(7, 2));
	DataType("<i2").encode(mem, 0x110);
	EXPECT_THAT(mem, ElementsAre(0x10, 1));
	DataType("<u2").encode(mem, 0x110);
	EXPECT_THAT(mem, ElementsAre(0x10, 1));
	DataType("<d2").encode(mem, 0x110);
	EXPECT_THAT(mem, ElementsAre(0x72, 2));
	DataType("<n2").encode(mem, 0x110);
	EXPECT_THAT(mem, ElementsAre(2, 7));
}

TEST(DataTypeEncode, x8000) {
	uint8_t mem[2] {};
	DataType(">i2").encode(mem, 0x8000);
	EXPECT_THAT(mem, ElementsAre(0x80, 0));
	DataType(">u2").encode(mem, 0x8000);
	EXPECT_THAT(mem, ElementsAre(0x80, 0));
	DataType(">d2").encode(mem, 0x8000);
	EXPECT_THAT(mem, ElementsAre(0x27, 0x68));
	DataType(">n2").encode(mem, 0x8000);
	EXPECT_THAT(mem, ElementsAre(6, 8));
	DataType("<i2").encode(mem, 0x8000);
	EXPECT_THAT(mem, ElementsAre(0, 0x80));
	DataType("<u2").encode(mem, 0x8000);
	EXPECT_THAT(mem, ElementsAre(0, 0x80));
	DataType("<d2").encode(mem, 0x8000);
	EXPECT_THAT(mem, ElementsAre(0x68, 0x27));
	DataType("<n2").encode(mem, 0x8000);
	EXPECT_THAT(mem, ElementsAre(8, 6));
}

TEST(DataTypeEncode, x8001) {
	uint8_t mem[2] {};
	DataType(">i2").encode(mem, 0x8001);
	EXPECT_THAT(mem, ElementsAre(0x80, 1));
	DataType(">u2").encode(mem, 0x8001);
	EXPECT_THAT(mem, ElementsAre(0x80, 1));
	DataType(">d2").encode(mem, 0x8001);
	EXPECT_THAT(mem, ElementsAre(0x27, 0x69));
	DataType(">n2").encode(mem, 0x8001);
	EXPECT_THAT(mem, ElementsAre(6, 9));
	DataType("<i2").encode(mem, 0x8001);
	EXPECT_THAT(mem, ElementsAre(1, 0x80));
	DataType("<u2").encode(mem, 0x8001);
	EXPECT_THAT(mem, ElementsAre(1, 0x80));
	DataType("<d2").encode(mem, 0x8001);
	EXPECT_THAT(mem, ElementsAre(0x69, 0x27));
	DataType("<n2").encode(mem, 0x8001);
	EXPECT_THAT(mem, ElementsAre(9, 6));
}

TEST(DataTypeEncode, xFFFE) {
	uint8_t mem[2] {};
	DataType(">i2").encode(mem, 0xFFFE);
	EXPECT_THAT(mem, ElementsAre(0xFF, 0xFE));
	DataType(">u2").encode(mem, 0xFFFE);
	EXPECT_THAT(mem, ElementsAre(0xFF, 0xFE));
	DataType(">d2").encode(mem, 0xFFFE);
	EXPECT_THAT(mem, ElementsAre(0x55, 0x34));
	DataType(">n2").encode(mem, 0xFFFE);
	EXPECT_THAT(mem, ElementsAre(3, 4));
	DataType("<i2").encode(mem, 0xFFFE);
	EXPECT_THAT(mem, ElementsAre(0xFE, 0xFF));
	DataType("<u2").encode(mem, 0xFFFE);
	EXPECT_THAT(mem, ElementsAre(0xFE, 0xFF));
	DataType("<d2").encode(mem, 0xFFFE);
	EXPECT_THAT(mem, ElementsAre(0x34, 0x55));
	DataType("<n2").encode(mem, 0xFFFE);
	EXPECT_THAT(mem, ElementsAre(4, 3));
}

TEST(DataTypeEncode, d0010) {
	uint8_t mem[2] {};
	DataType(">i2").encode(mem, 10);
	EXPECT_THAT(mem, ElementsAre(0, 0x0A));
	DataType(">u2").encode(mem, 10);
	EXPECT_THAT(mem, ElementsAre(0, 0x0A));
	DataType(">d2").encode(mem, 10);
	EXPECT_THAT(mem, ElementsAre(0, 0x10));
	DataType(">n2").encode(mem, 10);
	EXPECT_THAT(mem, ElementsAre(1, 0));
	DataType("<i2").encode(mem, 10);
	EXPECT_THAT(mem, ElementsAre(0x0A, 0));
	DataType("<u2").encode(mem, 10);
	EXPECT_THAT(mem, ElementsAre(0x0A, 0));
	DataType("<d2").encode(mem, 10);
	EXPECT_THAT(mem, ElementsAre(0x10, 0));
	DataType("<n2").encode(mem, 10);
	EXPECT_THAT(mem, ElementsAre(0, 1));
}

TEST(DataTypeEncode, d0110) {
	uint8_t mem[2] {};
	DataType(">i2").encode(mem, 110);
	EXPECT_THAT(mem, ElementsAre(0, 0x6E));
	DataType(">u2").encode(mem, 110);
	EXPECT_THAT(mem, ElementsAre(0, 0x6E));
	DataType(">d2").encode(mem, 110);
	EXPECT_THAT(mem, ElementsAre(1, 0x10));
	DataType(">n2").encode(mem, 110);
	EXPECT_THAT(mem, ElementsAre(1, 0));
	DataType("<i2").encode(mem, 110);
	EXPECT_THAT(mem, ElementsAre(0x6E, 0));
	DataType("<u2").encode(mem, 110);
	EXPECT_THAT(mem, ElementsAre(0x6E, 0));
	DataType("<d2").encode(mem, 110);
	EXPECT_THAT(mem, ElementsAre(0x10, 1));
	DataType("<n2").encode(mem, 110);
	EXPECT_THAT(mem, ElementsAre(0, 1));
}

TEST(DataTypeEncode, d1001) {
	uint8_t mem[2] {};
	DataType(">i2").encode(mem, 1001);
	EXPECT_THAT(mem, ElementsAre(3, 0xE9));
	DataType(">u2").encode(mem, 1001);
	EXPECT_THAT(mem, ElementsAre(3, 0xE9));
	DataType(">d2").encode(mem, 1001);
	EXPECT_THAT(mem, ElementsAre(0x10, 1));
	DataType(">n2").encode(mem, 1001);
	EXPECT_THAT(mem, ElementsAre(0, 1));
	DataType("<i2").encode(mem, 1001);
	EXPECT_THAT(mem, ElementsAre(0xE9, 3));
	DataType("<u2").encode(mem, 1001);
	EXPECT_THAT(mem, ElementsAre(0xE9, 3));
	DataType("<d2").encode(mem, 1001);
	EXPECT_THAT(mem, ElementsAre(1, 0x10));
	DataType("<n2").encode(mem, 1001);
	EXPECT_THAT(mem, ElementsAre(1, 0));
}

TEST(DataTypeEncode, xN0001) {
	uint8_t mem[2] {};
	DataType(">i2").encode(mem, -1);
	EXPECT_THAT(mem, ElementsAre(0xFF, 0xFF));
	DataType("<i2").encode(mem, -1);
	EXPECT_THAT(mem, ElementsAre(0xFF, 0xFF));
}

TEST(DataTypeEncode, xN0100) {
	uint8_t mem[2] {};
	DataType(">i2").encode(mem, -256);
	EXPECT_THAT(mem, ElementsAre(0xFF, 0));
	DataType("<i2").encode(mem, -256);
	EXPECT_THAT(mem, ElementsAre(0, 0xFF));
}

TEST(DataTypeEncode, xN80FF) {
	uint8_t mem[2] {};
	DataType(">i2").encode(mem, -32513);
	EXPECT_THAT(mem, ElementsAre(0x80, 0xFF));
	DataType("<i2").encode(mem, -32513);
	EXPECT_THAT(mem, ElementsAre(0xFF, 0x80));
}

TEST(DataTypeEncode, x000000) {
	uint8_t mem[3] {};
	DataType(">i3").encode(mem, 0);
	EXPECT_THAT(mem, ElementsAre(0, 0, 0));
	DataType(">u3").encode(mem, 0);
	EXPECT_THAT(mem, ElementsAre(0, 0, 0));
	DataType(">d3").encode(mem, 0);
	EXPECT_THAT(mem, ElementsAre(0, 0, 0));
	DataType(">n3").encode(mem, 0);
	EXPECT_THAT(mem, ElementsAre(0, 0, 0));
	DataType("<i3").encode(mem, 0);
	EXPECT_THAT(mem, ElementsAre(0, 0, 0));
	DataType("<u3").encode(mem, 0);
	EXPECT_THAT(mem, ElementsAre(0, 0, 0));
	DataType("<d3").encode(mem, 0);
	EXPECT_THAT(mem, ElementsAre(0, 0, 0));
	DataType("<n3").encode(mem, 0);
	EXPECT_THAT(mem, ElementsAre(0, 0, 0));
}

TEST(DataTypeEncode, x000001) {
	uint8_t mem[3] {};
	DataType(">i3").encode(mem, 1);
	EXPECT_THAT(mem, ElementsAre(0, 0, 1));
	DataType(">u3").encode(mem, 1);
	EXPECT_THAT(mem, ElementsAre(0, 0, 1));
	DataType(">d3").encode(mem, 1);
	EXPECT_THAT(mem, ElementsAre(0, 0, 1));
	DataType(">n3").encode(mem, 1);
	EXPECT_THAT(mem, ElementsAre(0, 0, 1));
	DataType("<i3").encode(mem, 1);
	EXPECT_THAT(mem, ElementsAre(1, 0, 0));
	DataType("<u3").encode(mem, 1);
	EXPECT_THAT(mem, ElementsAre(1, 0, 0));
	DataType("<d3").encode(mem, 1);
	EXPECT_THAT(mem, ElementsAre(1, 0, 0));
	DataType("<n3").encode(mem, 1);
	EXPECT_THAT(mem, ElementsAre(1, 0, 0));
}

TEST(DataTypeEncode, x000010) {
	uint8_t mem[3] {};
	DataType(">i3").encode(mem, 0x10);
	EXPECT_THAT(mem, ElementsAre(0, 0, 0x10));
	DataType(">u3").encode(mem, 0x10);
	EXPECT_THAT(mem, ElementsAre(0, 0, 0x10));
	DataType(">d3").encode(mem, 0x10);
	EXPECT_THAT(mem, ElementsAre(0, 0, 0x16));
	DataType(">n3").encode(mem, 0x10);
	EXPECT_THAT(mem, ElementsAre(0, 1, 6));
	DataType("<i3").encode(mem, 0x10);
	EXPECT_THAT(mem, ElementsAre(0x10, 0, 0));
	DataType("<u3").encode(mem, 0x10);
	EXPECT_THAT(mem, ElementsAre(0x10, 0, 0));
	DataType("<d3").encode(mem, 0x10);
	EXPECT_THAT(mem, ElementsAre(0x16, 0, 0));
	DataType("<n3").encode(mem, 0x10);
	EXPECT_THAT(mem, ElementsAre(6, 1, 0));
}

TEST(DataTypeEncode, x000100) {
	uint8_t mem[3] {};
	DataType(">i3").encode(mem, 0x100);
	EXPECT_THAT(mem, ElementsAre(0, 1, 0));
	DataType(">u3").encode(mem, 0x100);
	EXPECT_THAT(mem, ElementsAre(0, 1, 0));
	DataType(">d3").encode(mem, 0x100);
	EXPECT_THAT(mem, ElementsAre(0, 2, 0x56));
	DataType(">n3").encode(mem, 0x100);
	EXPECT_THAT(mem, ElementsAre(2, 5, 6));
	DataType("<i3").encode(mem, 0x100);
	EXPECT_THAT(mem, ElementsAre(0, 1, 0));
	DataType("<u3").encode(mem, 0x100);
	EXPECT_THAT(mem, ElementsAre(0, 1, 0));
	DataType("<d3").encode(mem, 0x100);
	EXPECT_THAT(mem, ElementsAre(0x56, 2, 0));
	DataType("<n3").encode(mem, 0x100);
	EXPECT_THAT(mem, ElementsAre(6, 5, 2));
}

TEST(DataTypeEncode, x000110) {
	uint8_t mem[3] {};
	DataType(">i3").encode(mem, 0x110);
	EXPECT_THAT(mem, ElementsAre(0, 1, 0x10));
	DataType(">u3").encode(mem, 0x110);
	EXPECT_THAT(mem, ElementsAre(0, 1, 0x10));
	DataType("<i3").encode(mem, 0x110);
	EXPECT_THAT(mem, ElementsAre(0x10, 1, 0));
	DataType("<u3").encode(mem, 0x110);
	EXPECT_THAT(mem, ElementsAre(0x10, 1, 0));
}

TEST(DataTypeEncode, x010000) {
	uint8_t mem[3] {};
	DataType(">i3").encode(mem, 0x10000);
	EXPECT_THAT(mem, ElementsAre(1, 0, 0));
	DataType(">u3").encode(mem, 0x10000);
	EXPECT_THAT(mem, ElementsAre(1, 0, 0));
	DataType(">d3").encode(mem, 0x10000);
	EXPECT_THAT(mem, ElementsAre(6, 0x55, 0x36));
	DataType(">n3").encode(mem, 0x10000);
	EXPECT_THAT(mem, ElementsAre(5, 3, 6));
	DataType("<i3").encode(mem, 0x10000);
	EXPECT_THAT(mem, ElementsAre(0, 0, 1));
	DataType("<u3").encode(mem, 0x10000);
	EXPECT_THAT(mem, ElementsAre(0, 0, 1));
	DataType("<d3").encode(mem, 0x10000);
	EXPECT_THAT(mem, ElementsAre(0x36, 0x55, 6));
	DataType("<n3").encode(mem, 0x10000);
	EXPECT_THAT(mem, ElementsAre(6, 3, 5));
}

TEST(DataTypeEncode, x010010) {
	uint8_t mem[3] {};
	DataType(">i3").encode(mem, 0x10010);
	EXPECT_THAT(mem, ElementsAre(1, 0, 0x10));
	DataType(">u3").encode(mem, 0x10010);
	EXPECT_THAT(mem, ElementsAre(1, 0, 0x10));
	DataType("<i3").encode(mem, 0x10010);
	EXPECT_THAT(mem, ElementsAre(0x10, 0, 1));
	DataType("<u3").encode(mem, 0x10010);
	EXPECT_THAT(mem, ElementsAre(0x10, 0, 1));
}

TEST(DataTypeEncode, x010810) {
	uint8_t mem[3] {};
	DataType(">i3").encode(mem, 0x10810);
	EXPECT_THAT(mem, ElementsAre(1, 8, 0x10));
	DataType(">u3").encode(mem, 0x10810);
	EXPECT_THAT(mem, ElementsAre(1, 8, 0x10));
	DataType("<i3").encode(mem, 0x10810);
	EXPECT_THAT(mem, ElementsAre(0x10, 8, 1));
	DataType("<u3").encode(mem, 0x10810);
	EXPECT_THAT(mem, ElementsAre(0x10, 8, 1));
}

TEST(DataTypeEncode, x800000) {
	uint8_t mem[3] {};
	DataType(">i3").encode(mem, 0x800000);
	EXPECT_THAT(mem, ElementsAre(0x80, 0, 0));
	DataType(">u3").encode(mem, 0x800000);
	EXPECT_THAT(mem, ElementsAre(0x80, 0, 0));
	DataType("<i3").encode(mem, 0x800000);
	EXPECT_THAT(mem, ElementsAre(0, 0, 0x80));
	DataType("<u3").encode(mem, 0x800000);
	EXPECT_THAT(mem, ElementsAre(0, 0, 0x80));
}

TEST(DataTypeEncode, x0000000) {
	uint8_t mem[4] {};
	DataType(">i4").encode(mem, 0);
	EXPECT_THAT(mem, ElementsAre(0, 0, 0, 0));
	DataType(">u4").encode(mem, 0);
	EXPECT_THAT(mem, ElementsAre(0, 0, 0, 0));
	DataType(">d4").encode(mem, 0);
	EXPECT_THAT(mem, ElementsAre(0, 0, 0, 0));
	DataType(">n4").encode(mem, 0);
	EXPECT_THAT(mem, ElementsAre(0, 0, 0, 0));
	DataType("<i4").encode(mem, 0);
	EXPECT_THAT(mem, ElementsAre(0, 0, 0, 0));
	DataType("<u4").encode(mem, 0);
	EXPECT_THAT(mem, ElementsAre(0, 0, 0, 0));
	DataType("<d4").encode(mem, 0);
	EXPECT_THAT(mem, ElementsAre(0, 0, 0, 0));
	DataType("<n4").encode(mem, 0);
	EXPECT_THAT(mem, ElementsAre(0, 0, 0, 0));
	DataType("><i4").encode(mem, 0);
	EXPECT_THAT(mem, ElementsAre(0, 0, 0, 0));
	DataType("><u4").encode(mem, 0);
	EXPECT_THAT(mem, ElementsAre(0, 0, 0, 0));
	DataType("><d4").encode(mem, 0);
	EXPECT_THAT(mem, ElementsAre(0, 0, 0, 0));
	DataType("><n4").encode(mem, 0);
	EXPECT_THAT(mem, ElementsAre(0, 0, 0, 0));
	DataType("<>i4").encode(mem, 0);
	EXPECT_THAT(mem, ElementsAre(0, 0, 0, 0));
	DataType("<>u4").encode(mem, 0);
	EXPECT_THAT(mem, ElementsAre(0, 0, 0, 0));
	DataType("<>d4").encode(mem, 0);
	EXPECT_THAT(mem, ElementsAre(0, 0, 0, 0));
	DataType("<>n4").encode(mem, 0);
	EXPECT_THAT(mem, ElementsAre(0, 0, 0, 0));
}

TEST(DataTypeEncode, x00000001) {
	uint8_t mem[4] {};
	DataType(">i4").encode(mem, 1);
	EXPECT_THAT(mem, ElementsAre(0, 0, 0, 1));
	DataType(">u4").encode(mem, 1);
	EXPECT_THAT(mem, ElementsAre(0, 0, 0, 1));
	DataType(">d4").encode(mem, 1);
	EXPECT_THAT(mem, ElementsAre(0, 0, 0, 1));
	DataType(">n4").encode(mem, 1);
	EXPECT_THAT(mem, ElementsAre(0, 0, 0, 1));
	DataType("<i4").encode(mem, 1);
	EXPECT_THAT(mem, ElementsAre(1, 0, 0, 0));
	DataType("<u4").encode(mem, 1);
	EXPECT_THAT(mem, ElementsAre(1, 0, 0, 0));
	DataType("<d4").encode(mem, 1);
	EXPECT_THAT(mem, ElementsAre(1, 0, 0, 0));
	DataType("<n4").encode(mem, 1);
	EXPECT_THAT(mem, ElementsAre(1, 0, 0, 0));
	DataType("><i4").encode(mem, 1);
	EXPECT_THAT(mem, ElementsAre(0, 0, 1, 0));
	DataType("><u4").encode(mem, 1);
	EXPECT_THAT(mem, ElementsAre(0, 0, 1, 0));
	DataType("><d4").encode(mem, 1);
	EXPECT_THAT(mem, ElementsAre(0, 0, 1, 0));
	DataType("><n4").encode(mem, 1);
	EXPECT_THAT(mem, ElementsAre(0, 0, 1, 0));
	DataType("<>i4").encode(mem, 1);
	EXPECT_THAT(mem, ElementsAre(0, 1, 0, 0));
	DataType("<>u4").encode(mem, 1);
	EXPECT_THAT(mem, ElementsAre(0, 1, 0, 0));
	DataType("<>d4").encode(mem, 1);
	EXPECT_THAT(mem, ElementsAre(0, 1, 0, 0));
	DataType("<>n4").encode(mem, 1);
	EXPECT_THAT(mem, ElementsAre(0, 1, 0, 0));
}

TEST(DataTypeEncode, x00000100) {
	uint8_t mem[4] {};
	DataType(">i4").encode(mem, 0x100);
	EXPECT_THAT(mem, ElementsAre(0, 0, 1, 0));
	DataType(">u4").encode(mem, 0x100);
	EXPECT_THAT(mem, ElementsAre(0, 0, 1, 0));
	DataType(">d4").encode(mem, 0x100);
	EXPECT_THAT(mem, ElementsAre(0, 0, 2, 0x56));
	DataType(">n4").encode(mem, 0x100);
	EXPECT_THAT(mem, ElementsAre(0, 2, 5, 6));
	DataType("<i4").encode(mem, 0x100);
	EXPECT_THAT(mem, ElementsAre(0, 1, 0, 0));
	DataType("<u4").encode(mem, 0x100);
	EXPECT_THAT(mem, ElementsAre(0, 1, 0, 0));
	DataType("<d4").encode(mem, 0x100);
	EXPECT_THAT(mem, ElementsAre(0x56, 2, 0, 0));
	DataType("<n4").encode(mem, 0x100);
	EXPECT_THAT(mem, ElementsAre(6, 5, 2, 0));
	DataType("><i4").encode(mem, 0x100);
	EXPECT_THAT(mem, ElementsAre(0, 0, 0, 1));
	DataType("><u4").encode(mem, 0x100);
	EXPECT_THAT(mem, ElementsAre(0, 0, 0, 1));
	DataType("><d4").encode(mem, 0x100);
	EXPECT_THAT(mem, ElementsAre(0, 0, 0x56, 2));
	DataType("><n4").encode(mem, 0x100);
	EXPECT_THAT(mem, ElementsAre(2, 0, 6, 5));
	DataType("<>i4").encode(mem, 0x100);
	EXPECT_THAT(mem, ElementsAre(1, 0, 0, 0));
	DataType("<>u4").encode(mem, 0x100);
	EXPECT_THAT(mem, ElementsAre(1, 0, 0, 0));
	DataType("<>d4").encode(mem, 0x100);
	EXPECT_THAT(mem, ElementsAre(2, 0x56, 0, 0));
	DataType("<>n4").encode(mem, 0x100);
	EXPECT_THAT(mem, ElementsAre(5, 6, 0, 2));
}

TEST(DataTypeEncode, x00010000) {
	uint8_t mem[4] {};
	DataType(">i4").encode(mem, 0x10000);
	EXPECT_THAT(mem, ElementsAre(0, 1, 0, 0));
	DataType(">u4").encode(mem, 0x10000);
	EXPECT_THAT(mem, ElementsAre(0, 1, 0, 0));
	DataType(">d4").encode(mem, 0x10000);
	EXPECT_THAT(mem, ElementsAre(0, 6, 0x55, 0x36));
	DataType(">n4").encode(mem, 0x10000);
	EXPECT_THAT(mem, ElementsAre(5, 5, 3, 6));
	DataType("<i4").encode(mem, 0x10000);
	EXPECT_THAT(mem, ElementsAre(0, 0, 1, 0));
	DataType("<u4").encode(mem, 0x10000);
	EXPECT_THAT(mem, ElementsAre(0, 0, 1, 0));
	DataType("<d4").encode(mem, 0x10000);
	EXPECT_THAT(mem, ElementsAre(0x36, 0x55, 6, 0));
	DataType("<n4").encode(mem, 0x10000);
	EXPECT_THAT(mem, ElementsAre(6, 3, 5, 5));
	DataType("><i4").encode(mem, 0x10000);
	EXPECT_THAT(mem, ElementsAre(1, 0, 0, 0));
	DataType("><u4").encode(mem, 0x10000);
	EXPECT_THAT(mem, ElementsAre(1, 0, 0, 0));
	DataType("><d4").encode(mem, 0x10000);
	EXPECT_THAT(mem, ElementsAre(6, 0, 0x36, 0x55));
	DataType("><n4").encode(mem, 0x10000);
	EXPECT_THAT(mem, ElementsAre(5, 5, 6, 3));
	DataType("<>i4").encode(mem, 0x10000);
	EXPECT_THAT(mem, ElementsAre(0, 0, 0, 1));
	DataType("<>u4").encode(mem, 0x10000);
	EXPECT_THAT(mem, ElementsAre(0, 0, 0, 1));
	DataType("<>d4").encode(mem, 0x10000);
	EXPECT_THAT(mem, ElementsAre(0x55, 0x36, 0, 6));
	DataType("<>n4").encode(mem, 0x10000);
	EXPECT_THAT(mem, ElementsAre(3, 6, 5, 5));
}

TEST(DataTypeEncode, x01000000) {
	uint8_t mem[4] {};
	DataType(">i4").encode(mem, 0x1000000);
	EXPECT_THAT(mem, ElementsAre(1, 0, 0, 0));
	DataType(">u4").encode(mem, 0x1000000);
	EXPECT_THAT(mem, ElementsAre(1, 0, 0, 0));
	DataType(">d4").encode(mem, 0x1000000);
	EXPECT_THAT(mem, ElementsAre(0x16, 0x77, 0x72, 0x16));
	DataType(">n4").encode(mem, 0x1000000);
	EXPECT_THAT(mem, ElementsAre(7, 2, 1, 6));
	DataType("<i4").encode(mem, 0x1000000);
	EXPECT_THAT(mem, ElementsAre(0, 0, 0, 1));
	DataType("<u4").encode(mem, 0x1000000);
	EXPECT_THAT(mem, ElementsAre(0, 0, 0, 1));
	DataType("<d4").encode(mem, 0x1000000);
	EXPECT_THAT(mem, ElementsAre(0x16, 0x72, 0x77, 0x16));
	DataType("<n4").encode(mem, 0x1000000);
	EXPECT_THAT(mem, ElementsAre(6, 1, 2, 7));
	DataType("><i4").encode(mem, 0x1000000);
	EXPECT_THAT(mem, ElementsAre(0, 1, 0, 0));
	DataType("><u4").encode(mem, 0x1000000);
	EXPECT_THAT(mem, ElementsAre(0, 1, 0, 0));
	DataType("><d4").encode(mem, 0x1000000);
	EXPECT_THAT(mem, ElementsAre(0x77, 0x16, 0x16, 0x72));
	DataType("><n4").encode(mem, 0x1000000);
	EXPECT_THAT(mem, ElementsAre(2, 7, 6, 1));
	DataType("<>i4").encode(mem, 0x1000000);
	EXPECT_THAT(mem, ElementsAre(0, 0, 1, 0));
	DataType("<>u4").encode(mem, 0x1000000);
	EXPECT_THAT(mem, ElementsAre(0, 0, 1, 0));
	DataType("<>d4").encode(mem, 0x1000000);
	EXPECT_THAT(mem, ElementsAre(0x72, 0x16, 0x16, 0x77));
	DataType("<>n4").encode(mem, 0x1000000);
	EXPECT_THAT(mem, ElementsAre(1, 6, 7, 2));
}

TEST(DataTypeEncode, x01000004) {
	uint8_t mem[4] {};
	DataType(">i4").encode(mem, 0x1000004);
	EXPECT_THAT(mem, ElementsAre(1, 0, 0, 4));
	DataType(">u4").encode(mem, 0x1000004);
	EXPECT_THAT(mem, ElementsAre(1, 0, 0, 4));
	DataType(">d4").encode(mem, 0x1000004);
	EXPECT_THAT(mem, ElementsAre(0x16, 0x77, 0x72, 0x20));
	DataType(">n4").encode(mem, 0x1000004);
	EXPECT_THAT(mem, ElementsAre(7, 2, 2, 0));
	DataType("<i4").encode(mem, 0x1000004);
	EXPECT_THAT(mem, ElementsAre(4, 0, 0, 1));
	DataType("<u4").encode(mem, 0x1000004);
	EXPECT_THAT(mem, ElementsAre(4, 0, 0, 1));
	DataType("<d4").encode(mem, 0x1000004);
	EXPECT_THAT(mem, ElementsAre(0x20, 0x72, 0x77, 0x16));
	DataType("<n4").encode(mem, 0x1000004);
	EXPECT_THAT(mem, ElementsAre(0, 2, 2, 7));
	DataType("><i4").encode(mem, 0x1000004);
	EXPECT_THAT(mem, ElementsAre(0, 1, 4, 0));
	DataType("><u4").encode(mem, 0x1000004);
	EXPECT_THAT(mem, ElementsAre(0, 1, 4, 0));
	DataType("><d4").encode(mem, 0x1000004);
	EXPECT_THAT(mem, ElementsAre(0x77, 0x16, 0x20, 0x72));
	DataType("><n4").encode(mem, 0x1000004);
	EXPECT_THAT(mem, ElementsAre(2, 7, 0, 2));
	DataType("<>i4").encode(mem, 0x1000004);
	EXPECT_THAT(mem, ElementsAre(0, 4, 1, 0));
	DataType("<>u4").encode(mem, 0x1000004);
	EXPECT_THAT(mem, ElementsAre(0, 4, 1, 0));
	DataType("<>d4").encode(mem, 0x1000004);
	EXPECT_THAT(mem, ElementsAre(0x72, 0x20, 0x16, 0x77));
	DataType("<>n4").encode(mem, 0x1000004);
	EXPECT_THAT(mem, ElementsAre(2, 0, 7, 2));
}

TEST(DataTypeEncode, x01000304) {
	uint8_t mem[4] {};
	DataType(">i4").encode(mem, 0x1000304);
	EXPECT_THAT(mem, ElementsAre(1, 0, 3, 4));
	DataType(">u4").encode(mem, 0x1000304);
	EXPECT_THAT(mem, ElementsAre(1, 0, 3, 4));
	DataType("<i4").encode(mem, 0x1000304);
	EXPECT_THAT(mem, ElementsAre(4, 3, 0, 1));
	DataType("<u4").encode(mem, 0x1000304);
	EXPECT_THAT(mem, ElementsAre(4, 3, 0, 1));
	DataType("><i4").encode(mem, 0x1000304);
	EXPECT_THAT(mem, ElementsAre(0, 1, 4, 3));
	DataType("><u4").encode(mem, 0x1000304);
	EXPECT_THAT(mem, ElementsAre(0, 1, 4, 3));
	DataType("<>i4").encode(mem, 0x1000304);
	EXPECT_THAT(mem, ElementsAre(3, 4, 1, 0));
	DataType("<>u4").encode(mem, 0x1000304);
	EXPECT_THAT(mem, ElementsAre(3, 4, 1, 0));
}

TEST(DataTypeEncode, x01020304) {
	uint8_t mem[4] {};
	DataType(">i4").encode(mem, 0x1020304);
	EXPECT_THAT(mem, ElementsAre(1, 2, 3, 4));
	DataType(">u4").encode(mem, 0x1020304);
	EXPECT_THAT(mem, ElementsAre(1, 2, 3, 4));
	DataType("<i4").encode(mem, 0x1020304);
	EXPECT_THAT(mem, ElementsAre(4, 3, 2, 1));
	DataType("<u4").encode(mem, 0x1020304);
	EXPECT_THAT(mem, ElementsAre(4, 3, 2, 1));
	DataType("><i4").encode(mem, 0x1020304);
	EXPECT_THAT(mem, ElementsAre(2, 1, 4, 3));
	DataType("><u4").encode(mem, 0x1020304);
	EXPECT_THAT(mem, ElementsAre(2, 1, 4, 3));
	DataType("<>i4").encode(mem, 0x1020304);
	EXPECT_THAT(mem, ElementsAre(3, 4, 1, 2));
	DataType("<>u4").encode(mem, 0x1020304);
	EXPECT_THAT(mem, ElementsAre(3, 4, 1, 2));
}

}

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "search.h"

#include <map>
#include <memory>
#include <unordered_set>
#include <vector>

using namespace std;
using namespace ::testing;

namespace Retro {

std::ostream& operator<<(std::ostream& o, const SearchResult& result) {
	return o << "(" << result.address << ", " << result.mult << ", " << result.div << ", " << result.bias << ")";
}

std::ostream& operator<<(std::ostream& o, const TypedSearchResult& result) {
	return o << "(" << static_cast<const SearchResult&>(result) << ", " << result.type.type << ")";
}

struct SearchTestParam {
	vector<pair<int64_t, vector<uint8_t>>> searches;
	map<SearchResult, unordered_set<DataType>> results;
	shared_ptr<TypedSearchResult> uniqueResult{};
	vector<DataType> types{
		"<u4", ">u4", "><u4", "<>u4",
		"<i4", ">i4", "><i4", "<>i4",
		"<d4", ">d4", "><d4", "<>d4",
		"<n4", ">n4", "><n4", "<>n4",
		"<u3", ">u3",
		"<i3", ">i3",
		"<d3", ">d3",
		"<n3", ">n3",
		"<u2", ">u2",
		"<i2", ">i2",
		"<d2", ">d2",
		"|u1", "|i1", "|d1"
	};
};

class SearchTest : public TestWithParam<SearchTestParam> {
public:
	virtual void SetUp() override;

	Search search;

	vector<SearchResult> untypedResults;
	vector<TypedSearchResult> typedResults;
};

void SearchTest::SetUp() {
	const auto& param = GetParam();
	search = Search(param.types);
	AddressSpace mem;
	for (const auto& s : param.searches) {
		vector<uint8_t> mem2 = s.second;
		mem.reset();
		mem.addBlock(0, mem2.size(), static_cast<void*>(&mem2.front()));
		ASSERT_TRUE(mem.ok());
		search.search(mem, s.first);
	}

	for (const auto& result : param.results) {
		untypedResults.emplace_back(result.first);
	}

	auto last = unique(untypedResults.begin(), untypedResults.end());
	untypedResults.erase(last, untypedResults.end());

	for (const auto& result : param.results) {
		for (const auto& type : result.second) {
			typedResults.emplace_back(result.first, type);
		}
	}
}

TEST_P(SearchTest, Value) {
	EXPECT_THAT(search.results(), UnorderedElementsAreArray(untypedResults));
	EXPECT_THAT(search.typedResults(), UnorderedElementsAreArray(typedResults));
}

TEST_P(SearchTest, UniqueResult) {
	const auto& param = GetParam();
	if (param.uniqueResult) {
		EXPECT_TRUE(search.hasUniqueResult());
		EXPECT_EQ(search.uniqueResult(), *param.uniqueResult);
	} else {
		EXPECT_FALSE(search.hasUniqueResult());
	}
}

#define INSTANTIATE_SEARCH_TEST_CASE(NAME, ...) \
	INSTANTIATE_TEST_CASE_P(NAME, SearchTest, Values(SearchTestParam{ __VA_ARGS__ }));


INSTANTIATE_SEARCH_TEST_CASE(NoMatch,
	{
		{ 10, { 0, 0, 0, 0 } }
	},
	{}
)

INSTANTIATE_SEARCH_TEST_CASE(ExactMatch,
	{
		{ 1, { 1, 99, 99, 99 } }
	},
	{
		{ { 0, 1, 1, 0 }, {"|d1", "|u1", "|i1"} }
	},
	make_shared<TypedSearchResult>(SearchResult{ 0, 1, 1, 0 }, DataType{"|u1"})
)

INSTANTIATE_SEARCH_TEST_CASE(ExactMatchBcd,
	{
		{ 10, { 16, 99, 99, 99 } }
	},
	{
		{ { 0, 1, 1, 0 }, {"|d1"} }
	},
	make_shared<TypedSearchResult>(SearchResult{ 0, 1, 1, 0 }, DataType{"|d1"})
)

INSTANTIATE_SEARCH_TEST_CASE(ExactMatchN,
	{
		{ -1, { 255, 99, 99, 99 } }
	},
	{
		{ { 0, 1, 1, 0 }, {"|i1"} }
	},
	make_shared<TypedSearchResult>(SearchResult{ 0, 1, 1, 0 }, DataType{"|i1"})
)

INSTANTIATE_SEARCH_TEST_CASE(BiasMatch1,
	{
		{ 0, { 1, 99, 99, 99 } }
	},
	{
		{ { 0, 1, 1, 1 }, {"|d1", "|u1", "|i1"} }
	},
	make_shared<TypedSearchResult>(SearchResult{ 0, 1, 1, 1 }, DataType{"|u1"})
)

INSTANTIATE_SEARCH_TEST_CASE(BiasMatchN1,
	{
		{ 2, { 1, 99, 99, 99 } }
	},
	{
		{ { 0, 1, 1, -1}, {"|d1", "|u1", "|i1"} }
	},
	make_shared<TypedSearchResult>(SearchResult{ 0, 1, 1, -1 }, DataType{"|u1"})
)

INSTANTIATE_SEARCH_TEST_CASE(MultMatch2,
	{
		{ 2, { 4, 99, 99, 99 } }
	},
	{
		{ { 0, 2, 1, 0 }, {"|d1", "|u1", "|i1"} }
	},
	make_shared<TypedSearchResult>(SearchResult{ 0, 2, 1, 0 }, DataType{"|u1"})
)

INSTANTIATE_SEARCH_TEST_CASE(MultMatch4,
	{
		{ 2, { 8, 99, 99, 99 } }
	},
	{
		{ { 0, 4, 1, 0 }, {"|d1", "|u1", "|i1"} }
	},
	make_shared<TypedSearchResult>(SearchResult{ 0, 4, 1, 0 }, DataType{"|u1"})
)

INSTANTIATE_SEARCH_TEST_CASE(DivMatch10,
	{
		{ 10, { 1, 99, 99, 99 } }
	},
	{
		{ { 0, 1, 10, 0 }, {"|u1", "|i1"} },
		{ { 0, 1, 16, 0 }, {"|d1"} }
	},
	make_shared<TypedSearchResult>(SearchResult{ 0, 1, 10, 0 }, DataType{"|u1"})
)

INSTANTIATE_SEARCH_TEST_CASE(DivMatch16,
	{
		{ 16, { 1, 99, 99, 99 } }
	},
	{
		{ { 0, 1, 16, 0 }, {"|u1", "|i1"} }
	},
	make_shared<TypedSearchResult>(SearchResult{ 0, 1, 16, 0 }, DataType{"|u1"})
)

INSTANTIATE_SEARCH_TEST_CASE(DivMatch100,
	{
		{ 100, { 1, 199, 199, 199 } }
	},
	{
		{ { 0, 1, 100, 0 }, {"|u1", "|i1"} },
		{ { 0, 1, 256, 0 }, {"|d1"} }
	},
	make_shared<TypedSearchResult>(SearchResult{ 0, 1, 100, 0 }, DataType{"|u1"})
)

INSTANTIATE_SEARCH_TEST_CASE(DivMatch100Bcd,
	{
		{ 100, { 16, 199, 199, 199 } }
	},
	{
		{ { 0, 1, 16, 0 }, {"|d1"} }
	},
	make_shared<TypedSearchResult>(SearchResult{ 0, 1, 16, 0 }, DataType{"|d1"})
)

INSTANTIATE_SEARCH_TEST_CASE(ExactMatch16Le,
	{
		{ 513, { 1, 2, 99, 99 } }
	},
	{
		{ { 0, 1, 1, 0 }, {"<u2", "<i2"} }
	},
	make_shared<TypedSearchResult>(SearchResult{ 0, 1, 1, 0 }, DataType{"<u2"})
)

INSTANTIATE_SEARCH_TEST_CASE(ExactMatch16Be,
	{
		{ 513, { 2, 1, 99, 99 } }
	},
	{
		{ { 0, 1, 1, 0 }, {">u2", ">i2"} }
	},
	make_shared<TypedSearchResult>(SearchResult{ 0, 1, 1, 0 }, DataType{">u2"})
)

INSTANTIATE_SEARCH_TEST_CASE(ExactMatchBcd16Le,
	{
		{ 102, { 2, 1, 99, 99 } }
	},
	{
		{ { 0, 1, 1, 0 }, {"<d2"} }
	},
	make_shared<TypedSearchResult>(SearchResult{ 0, 1, 1, 0 }, DataType{"<d2"})
)

INSTANTIATE_SEARCH_TEST_CASE(ExactMatchBcd16Be,
	{
		{ 201, { 2, 1, 99, 99 } }
	},
	{
		{ { 0, 1, 1, 0 }, {">d2"} }
	},
	make_shared<TypedSearchResult>(SearchResult{ 0, 1, 1, 0 }, DataType{">d2"})
)

INSTANTIATE_SEARCH_TEST_CASE(BiasMatch116,
	{
		{ 511, { 0, 2, 99, 99 } }
	},
	{
		{ { 0, 1, 1, 1 }, {"<u2", "<i2"} }
	},
	make_shared<TypedSearchResult>(SearchResult{ 0, 1, 1, 1 }, DataType{"<u2"})
)

INSTANTIATE_SEARCH_TEST_CASE(BiasMatchN116,
	{
		{ 511, { 254, 1, 99, 99 } }
	},
	{
		{ { 0, 1, 1, -1 }, {"<u2", "<i2"} }
	},
	make_shared<TypedSearchResult>(SearchResult{ 0, 1, 1, -1 }, DataType{"<u2"})
)

INSTANTIATE_SEARCH_TEST_CASE(MultMatch216,
	{
		{ 128, { 0, 1, 99, 99 } }
	},
	{
		{ { 0, 2, 1, 0 }, {"<u2", "<i2"} }
	},
	make_shared<TypedSearchResult>(SearchResult{ 0, 2, 1, 0 }, DataType{"<u2"})
)

INSTANTIATE_SEARCH_TEST_CASE(MultMatch416,
	{
		{ 64, { 0, 1, 99, 99 } }
	},
	{
		{ { 0, 4, 1, 0 }, {"<u2", "<i2"} }
	},
	make_shared<TypedSearchResult>(SearchResult{ 0, 4, 1, 0 }, DataType{"<u2"})
)

INSTANTIATE_SEARCH_TEST_CASE(2ExactMatch1,
	{
		{ 1, { 1, 99, 99, 99 } },
		{ 1, { 1, 99, 99, 99 } }
	},
	{
		{ { 0, 1, 1, 0 }, {"|d1", "|u1", "|i1"} }
	},
	make_shared<TypedSearchResult>(SearchResult{ 0, 1, 1, 0 }, DataType{"|u1"})
)

INSTANTIATE_SEARCH_TEST_CASE(2ExactMatch12,
	{
		{ 1, { 1, 99, 99, 99 } },
		{ 2, { 2, 99, 99, 99 } }
	},
	{
		{ { 0, 1, 1, 0 }, {"|d1", "|u1", "|i1"} }
	},
	make_shared<TypedSearchResult>(SearchResult{ 0, 1, 1, 0 }, DataType{"|u1"})
)

INSTANTIATE_SEARCH_TEST_CASE(2NoOverlap,
	{
		{ 1, { 1, 99, 99, 99 } },
		{ 2, { 99, 2, 99, 99 } }
	},
	{}
)

INSTANTIATE_SEARCH_TEST_CASE(OddSize24Le,
	{
		{ 0x010203, { 3, 2, 1, 99 } }
	},
	{
		{ { 0, 1, 1, 0 }, {"<u3", "<i3"} }
	},
	make_shared<TypedSearchResult>(SearchResult{ 0, 1, 1, 0 }, DataType{"<u3"})
)

INSTANTIATE_SEARCH_TEST_CASE(OddSize24Be,
	{
		{ 0x010203, { 1, 2, 3, 99 } }
	},
	{
		{ { 0, 1, 1, 0 }, {">u3", ">i3"} }
	},
	make_shared<TypedSearchResult>(SearchResult{ 0, 1, 1, 0 }, DataType{">u3"})
)

INSTANTIATE_SEARCH_TEST_CASE(OddSizeBcd24Le,
	{
		{ 10203, { 3, 2, 1, 99 } }
	},
	{
		{ { 0, 1, 1, 0 }, {"<d3"} }
	},
	make_shared<TypedSearchResult>(SearchResult{ 0, 1, 1, 0 }, DataType{"<d3"})
)

INSTANTIATE_SEARCH_TEST_CASE(OddSizeBcd24Be,
	{
		{ 10203, { 1, 2, 3, 99 } }
	},
	{
		{ { 0, 1, 1, 0 }, {">d3"} }
	},
	make_shared<TypedSearchResult>(SearchResult{ 0, 1, 1, 0 }, DataType{">d3"})
)

INSTANTIATE_SEARCH_TEST_CASE(OddSizeLnbcd24Le,
	{
		{ 123, { 3, 2, 1, 99 } }
	},
	{
		{ { 0, 1, 1, 0 }, {"<n3"} }
	},
	make_shared<TypedSearchResult>(SearchResult{ 0, 1, 1, 0 }, DataType{"<n3"})
)

INSTANTIATE_SEARCH_TEST_CASE(OddSizeLnbcd24Be,
	{
		{ 123, { 1, 2, 3, 99 } }
	},
	{
		{ { 0, 1, 1, 0 }, {">n3"} }
	},
	make_shared<TypedSearchResult>(SearchResult{ 0, 1, 1, 0 }, DataType{">n3"})
)

INSTANTIATE_SEARCH_TEST_CASE(ExactMatch32Le,
	{
		{ 0x01020304, { 4, 3, 2, 1 } }
	},
	{
		{ { 0, 1, 1, 0 }, {"<u4", "<i4"} }
	},
	make_shared<TypedSearchResult>(SearchResult{ 0, 1, 1, 0 }, DataType{"<u4"})
)

INSTANTIATE_SEARCH_TEST_CASE(ExactMatch32Be,
	{
		{ 0x01020304, { 1, 2, 3, 4 } }
	},
	{
		{ { 0, 1, 1, 0 }, {">u4", ">i4"} }
	},
	make_shared<TypedSearchResult>(SearchResult{ 0, 1, 1, 0 }, DataType{">u4"})
)

INSTANTIATE_SEARCH_TEST_CASE(NoTypes,
	{
		{ 0, { 0, 0, 0, 0 } }
	},
	{},
	{},
	{}
)

INSTANTIATE_SEARCH_TEST_CASE(DivMatchLnbcd,
	{
		{ 10, { 255, 0, 1, 0 } }
	},
	{
		{ { 1, 1, 1, 0 }, {"<n2"} },
		{ { 1, 1, 10, 0 }, {">n2"} },
		{ { 2, 1, 10, 0 }, {"<n2"} },
		{ { 2, 1, 1, 0 }, {">n2"} }
	},
	{},
	{">n2", "<n2"}

)
INSTANTIATE_SEARCH_TEST_CASE(ExactMatch1End,
	{
		{ 1, { 99, 99, 99, 1 } }
	},
	{
		{ { 3, 1, 1, 0 }, {"|d1", "|u1", "|i1"} }
	},
	make_shared<TypedSearchResult>(SearchResult{ 3, 1, 1, 0 }, DataType{"|u1"})
)

INSTANTIATE_SEARCH_TEST_CASE(ExactMatchBcdEnd,
	{
		{ 10, { 99, 99, 99, 16 } }
	},
	{
		{ { 3, 1, 1, 0 }, {"|d1"} }
	},
	make_shared<TypedSearchResult>(SearchResult{ 3, 1, 1, 0 }, DataType{"|d1"})
)

INSTANTIATE_SEARCH_TEST_CASE(ExactMatch16LeEnd,
	{
		{ 513, { 99, 99, 1, 2 } }
	},
	{
		{ { 2, 1, 1, 0 }, {"<u2", "<i2"} }
	},
	make_shared<TypedSearchResult>(SearchResult{ 2, 1, 1, 0 }, DataType{"<u2"})
)

INSTANTIATE_SEARCH_TEST_CASE(ExactMatch16BeEnd,
	{
		{ 513, { 99, 99, 2, 1 } }
	},
	{
		{ { 2, 1, 1, 0 }, {">u2", ">i2"} }
	},
	make_shared<TypedSearchResult>(SearchResult{ 2, 1, 1, 0 }, DataType{">u2"})
)

INSTANTIATE_SEARCH_TEST_CASE(ExactMatchBcd16LeEnd,
	{
		{ 102, { 99, 99, 2, 1 } }
	},
	{
		{ { 2, 1, 1, 0 }, {"<d2"} }
	},
	make_shared<TypedSearchResult>(SearchResult{ 2, 1, 1, 0 }, DataType{"<d2"})
)

INSTANTIATE_SEARCH_TEST_CASE(ExactMatchBcd16BeEnd,
	{
		{ 201, { 99, 99, 2, 1 } }
	},
	{
		{ { 2, 1, 1, 0 }, {">d2"} }
	},
	make_shared<TypedSearchResult>(SearchResult{ 2, 1, 1, 0 }, DataType{">d2"})
)

INSTANTIATE_SEARCH_TEST_CASE(OddSize24LeEnd,
	{
		{ 0x010203, { 99, 3, 2, 1 } }
	},
	{
		{ { 1, 1, 1, 0 }, {"<u3", "<i3"} }
	},
	make_shared<TypedSearchResult>(SearchResult{ 1, 1, 1, 0 }, DataType{"<u3"})
)

INSTANTIATE_SEARCH_TEST_CASE(OddSize24BeEnd,
	{
		{ 0x010203, { 99, 1, 2, 3 } }
	},
	{
		{ { 1, 1, 1, 0 }, {">u3", ">i3"} }
	},
	make_shared<TypedSearchResult>(SearchResult{ 1, 1, 1, 0 }, DataType{">u3"})
)

INSTANTIATE_SEARCH_TEST_CASE(OddSizeBcd24LeEnd,
	{
		{ 10203, { 99, 3, 2, 1 } }
	},
	{
		{ { 1, 1, 1, 0 }, {"<d3"} }
	},
	make_shared<TypedSearchResult>(SearchResult{ 1, 1, 1, 0 }, DataType{"<d3"})
)

INSTANTIATE_SEARCH_TEST_CASE(OddSizeBcd24BeEnd,
	{
		{ 10203, { 99, 1, 2, 3 } }
	},
	{
		{ { 1, 1, 1, 0 }, {">d3"} }
	},
	make_shared<TypedSearchResult>(SearchResult{ 1, 1, 1, 0 }, DataType{">d3"})
)

INSTANTIATE_SEARCH_TEST_CASE(OddSizeLnbcd24LeEnd,
	{
		{ 123, { 99, 3, 2, 1 } }
	},
	{
		{ { 1, 1, 1, 0 }, {"<n3"} }
	},
	make_shared<TypedSearchResult>(SearchResult{ 1, 1, 1, 0 }, DataType{"<n3"})
)

INSTANTIATE_SEARCH_TEST_CASE(OddSizeLnbcd24BeEnd,
	{
		{ 123, { 99, 1, 2, 3 } }
	},
	{
		{ { 1, 1, 1, 0 }, {">n3"} }
	},
	make_shared<TypedSearchResult>(SearchResult{ 1, 1, 1, 0 }, DataType{">n3"})
)

INSTANTIATE_SEARCH_TEST_CASE(ExactMatch16LeMisaligned,
	{
		{ 513, { 99, 1, 2, 99 } }
	},
	{
		{ { 1, 1, 1, 0 }, {"<u2", "<i2"} }
	},
	make_shared<TypedSearchResult>(SearchResult{ 1, 1, 1, 0 }, DataType{"<u2"})
)

INSTANTIATE_SEARCH_TEST_CASE(ExactMatch16BeMisaligned,
	{
		{ 513, { 99, 2, 1, 99 } }
	},
	{
		{ { 1, 1, 1, 0 }, {">u2", ">i2"} }
	},
	make_shared<TypedSearchResult>(SearchResult{ 1, 1, 1, 0 }, DataType{">u2"})
)

INSTANTIATE_SEARCH_TEST_CASE(ExactMatch16LeWide,
	{
		{ 515, { 3, 2, 0, 0 } }
	},
	{
		{ { 0, 1, 1, 0 }, {"<u2", "<i2", "<u3", "<i3", "<u4", "<i4"} }
	},
	make_shared<TypedSearchResult>(SearchResult{ 0, 1, 1, 0 }, DataType{"<u4"})
)

INSTANTIATE_SEARCH_TEST_CASE(ExactMatch16BeWide,
	{
		{ 515, { 0, 0, 2, 3 } }
	},
	{
		{ { 0, 1, 1, 0 }, {">u4", ">i4"} },
		{ { 1, 1, 1, 0 }, {">u3", ">i3"} },
		{ { 2, 1, 1, 0 }, {">u2", ">i2"} }
	},
	make_shared<TypedSearchResult>(SearchResult{ 0, 1, 1, 0 }, DataType{">u4"})
)

INSTANTIATE_SEARCH_TEST_CASE(ExactMatch16BeHalfWide,
	{
		{ 515, { 99, 0, 2, 3 } }
	},
	{
		{ { 1, 1, 1, 0 }, {">u3", ">i3"} },
		{ { 2, 1, 1, 0 }, {">u2", ">i2"} }
	},
	make_shared<TypedSearchResult>(SearchResult{ 1, 1, 1, 0 }, DataType{">u3"})
)

INSTANTIATE_SEARCH_TEST_CASE(NoExtraMatch16BeWide,
	{
		{ 515, { 0, 99, 2, 3 } }
	},
	{
		{ { 2, 1, 1, 0 }, {">u2", ">i2"} }
	},
	make_shared<TypedSearchResult>(SearchResult{ 2, 1, 1, 0 }, DataType{">u2"})
)

INSTANTIATE_SEARCH_TEST_CASE(TypeNarrow,
	{
		{ 1, { 0, 1, 102, 103 } },
		{ 1, { 99, 1, 102, 103 } }
	},
	{
		{ { 1, 1, 1, 0 }, {"|u1", "|i1", "|d1"} }
	},
	make_shared<TypedSearchResult>(SearchResult{ 1, 1, 1, 0 }, DataType{"|u1"})
)

INSTANTIATE_SEARCH_TEST_CASE(UniqueOrderU,
	{
		{ 1, { 1, 101, 102, 103 } },
	},
	{
		{ { 0, 1, 1, 0 }, {"|u1", "|i1", "|d1"} }
	},
	make_shared<TypedSearchResult>(SearchResult{ 0, 1, 1, 0 }, DataType{"|u1"}),
	{ "|u1", "|i1", "|d1" }
)

INSTANTIATE_SEARCH_TEST_CASE(UniqueOrderI,
	{
		{ 1, { 1, 101, 102, 103 } },
	},
	{
		{ { 0, 1, 1, 0 }, {"|u1", "|i1", "|d1"} }
	},
	make_shared<TypedSearchResult>(SearchResult{ 0, 1, 1, 0 }, DataType{"|i1"}),
	{ "|i1", "|u1", "|d1" }
)

INSTANTIATE_SEARCH_TEST_CASE(UniqueOrderD,
	{
		{ 1, { 1, 101, 102, 103 } },
	},
	{
		{ { 0, 1, 1, 0 }, {"|u1", "|i1", "|d1"} }
	},
	make_shared<TypedSearchResult>(SearchResult{ 0, 1, 1, 0 }, DataType{"|d1"}),
	{ "|d1", "|u1", "|i1" }
)

INSTANTIATE_SEARCH_TEST_CASE(WidestUniqueBe,
	{
		{ 99, { 0, 0, 0, 99 } },
	},
	{
		{ { 0, 1, 1, 0 }, {">u4", ">i4"} },
		{ { 1, 1, 1, 0 }, {">u3", ">i3"} },
		{ { 2, 1, 1, 0 }, {">u2", ">i2"} },
		{ { 3, 1, 1, 0 }, {"|u1", "|i1"} }
	},
	make_shared<TypedSearchResult>(SearchResult{ 0, 1, 1, 0 }, DataType{">u4"})
)

INSTANTIATE_SEARCH_TEST_CASE(WidestUniqueLe,
	{
		{ 99, { 99, 0, 0, 0 } },
	},
	{
		{ { 0, 1, 1, 0 }, {"<u4", "<i4", "<u3", "<i3", "<u2", "<i2", "|u1", "|i1"} },
	},
	make_shared<TypedSearchResult>(SearchResult{ 0, 1, 1, 0 }, DataType{"<u4"})
)

struct DeltaTestParam {
	vector<pair<Operation, int64_t>> ops;
	vector<vector<uint8_t>> memory;
	vector<pair<SearchResult, unordered_set<DataType>>> results;
	vector<DataType> types{
		"|u1", "|i1",
		"<u2", "<i2",
		">u2", ">i2",
		"<u4", "<i4",
		">u4", ">i4"
	};
};

class DeltaTest : public TestWithParam<DeltaTestParam> {
};

TEST_P(DeltaTest, Delta) {
	const auto& param = GetParam();
	Search search(param.types);
	AddressSpace mem;
	AddressSpace memOld;
	ASSERT_EQ(param.ops.size() + 1, param.memory.size());
	ASSERT_GE(param.ops.size(), 1);

	vector<uint8_t> mem2;
	vector<uint8_t> memOld2 = param.memory[0];

	memOld.addBlock(0, memOld2.size(), static_cast<void*>(&memOld2.front()));

	size_t i;
	for (i = 0; i < param.ops.size(); ++i) {
		mem2 = param.memory[i + 1];
		mem.reset();
		mem.addBlock(0, mem2.size(), static_cast<void*>(&mem2.front()));
		ASSERT_TRUE(mem.ok());
		ASSERT_TRUE(memOld.ok());
		search.delta(mem, memOld, param.ops[i].first, param.ops[i].second);
		memOld2 = move(mem2);
		memOld.reset();
		memOld.addBlock(0, memOld2.size(), static_cast<void*>(&memOld2.front()));
	}

	vector<SearchResult> halfResults;
	for (const auto& result : param.results) {
		halfResults.emplace_back(result.first);
	}

	auto last = unique(halfResults.begin(), halfResults.end());
	halfResults.erase(last, halfResults.end());

	vector<TypedSearchResult> expandedResults;
	for (const auto& result : param.results) {
		for (const auto& type : result.second) {
			expandedResults.emplace_back(result.first, type);
		}
	}

	EXPECT_THAT(search.results(), UnorderedElementsAreArray(halfResults));
	EXPECT_THAT(search.typedResults(), UnorderedElementsAreArray(expandedResults));
}

#define INSTANTIATE_DELTA_TEST_CASE(NAME, ...) \
	INSTANTIATE_TEST_CASE_P(NAME, DeltaTest, Values(DeltaTestParam{ __VA_ARGS__ }));

using O = Operation;

INSTANTIATE_DELTA_TEST_CASE(Equal1,
	{ { O::ZERO, 0 } },
	{
		{ 100, 101, 102, 103 },
		{ 100, 100, 100, 100 }
	},
	{
		{ { 0, 1, 1, 0 }, {"|i1", "|u1"} }
	}
)

INSTANTIATE_DELTA_TEST_CASE(Equal2,
	{ { O::ZERO, 0 } },
	{
		{ 100, 101, 102, 103 },
		{ 100, 101, 100, 100 }
	},
	{
		{ { 0, 1, 1, 0 }, {"|i1", "|u1", "<i2", "<u2", ">i2", ">u2"} },
		{ { 1, 1, 1, 0 }, {"|i1", "|u1"} }
	}
)

INSTANTIATE_DELTA_TEST_CASE(Less,
	{ { O::NEGATIVE, 0 } },
	{
		{ 100, 101, 102, 103 },
		{ 99, 101, 102, 103 }
	},
	{
		{ { 0, 1, 1, 0 }, {"|i1", "|u1", "<i2", "<u2", ">i2", ">u2", "<i4", "<u4", ">i4", ">u4"} }
	}
)

INSTANTIATE_DELTA_TEST_CASE(Greater,
	{ { O::POSITIVE, 0 } },
	{
		{ 100, 101, 102, 103 },
		{ 101, 101, 102, 103 }
	},
	{
		{ { 0, 1, 1, 0 }, {"|i1", "|u1", "<i2", "<u2", ">i2", ">u2", "<i4", "<u4", ">i4", ">u4"} }
	}
)

INSTANTIATE_DELTA_TEST_CASE(LessAcross,
	{ { O::NEGATIVE, 0 } },
	{
		{ 100, 101, 102, 103 },
		{ 101, 100, 102, 103 }
	},
	{
		{ { 0, 1, 1, 0 }, {"<i2", "<u2", "<i4", "<u4"} },
		{ { 1, 1, 1, 0 }, {"|i1", "|u1", "<i2", "<u2", ">i2", ">u2"} }
	}
)

INSTANTIATE_DELTA_TEST_CASE(GreaterAcross,
	{ { O::POSITIVE, 0 } },
	{
		{ 100, 101, 102, 103 },
		{ 99, 102, 102, 103 }
	},
	{
		{ { 0, 1, 1, 0 }, {"<i2", "<u2", "<i4", "<u4"} },
		{ { 1, 1, 1, 0 }, {"|i1", "|u1", "<i2", "<u2", ">i2", ">u2"} }
	}
)

INSTANTIATE_DELTA_TEST_CASE(Less1,
	{ { O::EQUAL, -1 } },
	{
		{ 100, 101, 102, 103 },
		{ 99, 101, 102, 103 }
	},
	{
		{ { 0, 1, 1, 0 }, {"|i1", "|u1", "<i2", "<u2", "<i4", "<u4"} }
	}
)

INSTANTIATE_DELTA_TEST_CASE(Greater1,
	{ { O::EQUAL, 1 } },
	{
		{ 100, 101, 102, 103 },
		{ 101, 101, 102, 103 }
	},
	{
		{ { 0, 1, 1, 0 }, {"|i1", "|u1", "<i2", "<u2", "<i4", "<u4"} }
	}
)

INSTANTIATE_DELTA_TEST_CASE(GreaterGreaterMatch,
	{
		{ O::POSITIVE, 0 },
		{ O::POSITIVE, 0 }
	},
	{
		{ 100, 101, 102, 103 },
		{ 101, 101, 102, 103 },
		{ 102, 101, 102, 103 }
	},
	{
		{ { 0, 1, 1, 0 }, {"|i1", "|u1", "<i2", "<u2", ">i2", ">u2", "<i4", "<u4", ">i4", ">u4"} }
	}
)

INSTANTIATE_DELTA_TEST_CASE(GreaterLessMatch,
	{
		{ O::POSITIVE, 0 },
		{ O::NEGATIVE, 0 }
	},
	{
		{ 100, 101, 102, 103 },
		{ 101, 101, 102, 103 },
		{ 99, 101, 102, 103 }
	},
	{
		{ { 0, 1, 1, 0 }, {"|i1", "|u1", "<i2", "<u2", ">i2", ">u2", "<i4", "<u4", ">i4", ">u4"} }
	}
)

INSTANTIATE_DELTA_TEST_CASE(GreaterGreaterNoMatch,
	{
		{ O::POSITIVE, 0 },
		{ O::POSITIVE, 0 }
	},
	{
		{ 100, 101, 102, 103 },
		{ 101, 101, 102, 103 },
		{ 101, 101, 102, 103 }
	},
	{}
)

INSTANTIATE_DELTA_TEST_CASE(GreaterOffset,
	{ { O::POSITIVE, 0 } },
	{
		{ 100, 101, 102, 103 },
		{ 100, 101, 104, 103 }
	},
	{
		{ { 0, 1, 1, 0 }, {"<i4", "<u4", ">i4", ">u4"} },
		{ { 1, 1, 1, 0 }, {"<i2", "<u2", ">i2", ">u2"} },
		{ { 2, 1, 1, 0 }, {"|i1", "|u1", "<i2", "<u2", ">i2", ">u2"} }
	}
)

INSTANTIATE_DELTA_TEST_CASE(GreaterGreaterOffset,
	{
		{ O::POSITIVE, 0 },
		{ O::POSITIVE, 0 }
	},
	{
		{ 100, 101, 102, 103 },
		{ 101, 101, 104, 103 },
		{ 101, 101, 105, 103 }
	},
	{
		{ { 0, 1, 1, 0 }, {"<i4", "<u4", ">i4", ">u4"} },
		{ { 1, 1, 1, 0 }, {"<i2", "<u2", ">i2", ">u2"} },
		{ { 2, 1, 1, 0 }, {"|i1", "|u1", "<i2", "<u2", ">i2", ">u2"} }
	}
)

}

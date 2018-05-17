#pragma once

#include "memory.h"
#include "utils.h"

#include <vector>

namespace Retro {

struct Variable;
struct SearchResult {
	size_t address;
	uint64_t mult;
	uint64_t div;
	int64_t bias;

	bool operator<(const SearchResult&) const;
	bool operator==(const SearchResult&) const;
	bool operator!=(const SearchResult&) const;
};

struct TypedSearchResult : public SearchResult {
	TypedSearchResult(SearchResult&&, DataType&& type);
	TypedSearchResult(const SearchResult&, const DataType& type);
	TypedSearchResult(const TypedSearchResult&) = default;
	DataType type;

	bool operator==(const TypedSearchResult&) const;
	operator Variable() const;
};

class Search {
public:
	Search();
	Search(const std::vector<DataType>& types);
	void search(const AddressSpace& mem, int64_t value);
	void delta(const AddressSpace& mem, const AddressSpace& oldMem, Operation op, int64_t reference);

	std::vector<SearchResult> results() const;
	const std::vector<TypedSearchResult>& typedResults() const;
	std::vector<DataType> validTypes() const;

	void stuff(const std::vector<TypedSearchResult>&);
	void remove(const std::vector<TypedSearchResult>&);

	size_t numResults() const;

	bool hasUniqueResult() const;
	TypedSearchResult uniqueResult() const;

	Search& operator=(const Search&);

private:
	std::vector<SearchResult> makeResults(std::vector<size_t> addrs, uint64_t mult = 1, uint64_t div = 1, int64_t bias = 0);
	std::vector<size_t> searchValue(const AddressSpace& mem, int64_t value);
	std::vector<size_t> searchByte(const AddressSpace& mem, uint8_t value, const std::vector<size_t> addresses = {}, ssize_t offset = 0);
	std::vector<size_t> overlap(const std::vector<size_t> start, std::vector<size_t> end, size_t width);
	void reduceOnTypes(const AddressSpace& mem, const std::vector<SearchResult>&, int64_t value);

	void intersectCurrent(std::vector<TypedSearchResult>&&);
	void differenceCurrent(const std::vector<TypedSearchResult>&);

	std::vector<TypedSearchResult> m_current;
	std::vector<DataType> m_types;
	bool m_hasStarted = false;
};
}

namespace std {

template<>
struct hash<Retro::SearchResult> {
	size_t operator()(const Retro::SearchResult&) const;
};

template<>
struct hash<Retro::TypedSearchResult> {
	size_t operator()(const Retro::TypedSearchResult&) const;
};
}

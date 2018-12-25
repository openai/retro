#include "search.h"

#include "data.h"

#include <unordered_map>
#include <unordered_set>

using namespace Retro;
using namespace std;

vector<DataType> s_defaultTypes{
	">d8", ">n8",
	">d6", ">n6",
	"<u4", ">u4", "><u4", "<>u4", ">=u4", "<=u4", "=u4",
	"<i4", ">i4", "><i4", "<>i4", ">=i4", "<=i4", "=i4",
	"<d4", ">d4", "><d4", "<>d4", ">=d4", "<=d4", "=d4",
	"<n4", ">n4", "><n4", "<>n4", ">=n4", "<=n4", "=n4",
	"<u2", ">u2", "=u2",
	"<i2", ">i2", "=i2",
	"<d2", ">d2", "=d2",
	"|u1", "|i1", "|d1"
};

bool SearchResult::operator<(const SearchResult& other) const {
	if (address < other.address) {
		return true;
	}
	if (address > other.address) {
		return false;
	}
	if (mult < other.mult) {
		return true;
	}
	if (mult > other.mult) {
		return false;
	}
	if (div < other.div) {
		return true;
	}
	if (div > other.div) {
		return false;
	}
	if (bias < other.bias) {
		return true;
	}
	return false;
}

bool SearchResult::operator==(const SearchResult& other) const {
	if (address != other.address) {
		return false;
	}
	if (mult != other.mult) {
		return false;
	}
	if (div != other.div) {
		return false;
	}
	if (bias != other.bias) {
		return false;
	}
	return true;
}

bool SearchResult::operator!=(const SearchResult& other) const {
	return !(*this == other);
}

TypedSearchResult::TypedSearchResult(SearchResult&& result, DataType&& type)
	: SearchResult(result)
	, type(type) {
}

TypedSearchResult::TypedSearchResult(const SearchResult& result, const DataType& type)
	: SearchResult(result)
	, type(type) {
}

bool TypedSearchResult::operator==(const TypedSearchResult& other) const {
	return *static_cast<const SearchResult*>(this) == static_cast<const SearchResult&>(other) && type == other.type;
}

TypedSearchResult::operator Variable() const {
	return Variable{ type, address };
}

Search::Search()
	: m_types(s_defaultTypes) {
}

Search::Search(const vector<DataType>& types)
	: m_types(types) {
}

void Search::search(const AddressSpace& mem, int64_t value) {
	vector<SearchResult> results = makeResults(searchValue(mem, value));

	int64_t vscale = 1;
	int64_t v10 = value;
	while (v10 && !(v10 % 10)) {
		v10 /= 10;
		vscale *= 10;
		const auto& r = makeResults(searchValue(mem, v10), 1, vscale);
		results.insert(results.end(), r.begin(), r.end());
	}

	vscale = 1;
	int64_t v16 = value;
	while (v16 && !(v16 & 0xF)) {
		v16 >>= 4;
		vscale <<= 4;
		const auto& r = makeResults(searchValue(mem, v16), 1, vscale);
		results.insert(results.end(), r.begin(), r.end());
	}

	vscale = 1;
	int64_t v2 = value;
	while (v2 && v2 < 0x100000000 && vscale < 4) {
		v2 <<= 1;
		vscale <<= 1;
		const auto& r = makeResults(searchValue(mem, v2), vscale);
		results.insert(results.end(), r.begin(), r.end());
	}

	int64_t v1 = value + 1;
	{
		const auto& r = makeResults(searchValue(mem, v1), 1, 1, 1);
		results.insert(results.end(), r.begin(), r.end());
	}

	v1 = value - 1;
	{
		const auto& r = makeResults(searchValue(mem, v1), 1, 1, -1);
		results.insert(results.end(), r.begin(), r.end());
	}

	int64_t vBcd = toBcd(value);
	if (vBcd != value) {
		{
			const auto& r = makeResults(searchValue(mem, vBcd));
			results.insert(results.end(), r.begin(), r.end());
		}
		vscale = 1;
		while (vBcd && !(vBcd & 0xF)) {
			vBcd >>= 4;
			vscale <<= 4;
			const auto& r = makeResults(searchValue(mem, vBcd), 1, vscale);
			results.insert(results.end(), r.begin(), r.end());
		}
	}

	int64_t vNBcd = toLNBcd(value);
	if (vNBcd != value) {
		{
			const auto& r = makeResults(searchValue(mem, vNBcd));
			results.insert(results.end(), r.begin(), r.end());
		}
		vscale = 1;
		while (vNBcd && !(vNBcd & 0xF)) {
			vNBcd >>= 8;
			vscale <<= 8;
			const auto& r = makeResults(searchValue(mem, vNBcd), 1, vscale);
			results.insert(results.end(), r.begin(), r.end());
		}
	}

	sort(results.begin(), results.end());
	auto last = unique(results.begin(), results.end());
	results.erase(last, results.end());

	reduceOnTypes(mem, results, value);
}

void Search::delta(const AddressSpace& mem, const AddressSpace& oldMem, Operation op, int64_t reference) {
	vector<DataType> newTypes;
	multimap<size_t, DataType> results;
	for (const auto& type : m_types) {
		for (const auto& block : mem.blocks()) {
			const MemoryView<>& oldBlock = oldMem.block(block.first);
			const DynamicMemoryView dynmem(const_cast<void*>(block.second.offset(0)), block.second.size(), type, mem.overlay());
			const DynamicMemoryView dynmemOld(const_cast<void*>(oldBlock.offset(0)), oldBlock.size(), type, mem.overlay());
			vector<size_t> addresses;
			if (m_hasStarted) {
				for (const auto& result : m_current) {
					if (result.type == type && result.address >= block.first && result.address + type.width - block.first <= block.second.size()) {
						addresses.push_back(result.address);
					}
				}
				if (!addresses.size()) {
					continue;
				}
			}

			if (addresses.size()) {
				for (const auto& i : addresses) {
					int64_t delta = dynmem[i - block.first] - dynmemOld[i - block.first];
					if (calculate(op, reference, delta)) {
						if (!newTypes.size() || newTypes.back() != type) {
							newTypes.emplace_back(type);
						}
						results.emplace(i, type);
					}
				}
			} else {
				for (size_t i = 0; i + type.width <= block.second.size(); ++i) {
					int64_t delta = dynmem[i] - dynmemOld[i];
					if (calculate(op, reference, delta)) {
						if (!newTypes.size() || newTypes.back() != type) {
							newTypes.emplace_back(type);
						}
						results.emplace(i + block.first, type);
					}
				}
			}
		}
	}
	m_types = move(newTypes);

	vector<TypedSearchResult> results2;
	for (const auto& result : results) {
		results2.emplace_back(SearchResult{ result.first, 1, 1, 0 }, result.second);
	}

	intersectCurrent(move(results2));
}

vector<SearchResult> Search::results() const {
	vector<SearchResult> results;
	for (const auto& iter : m_current) {
		if (results.size() && results.back() == iter) {
			continue;
		}
		results.emplace_back(iter);
	}

	return results;
}

const vector<TypedSearchResult>& Search::typedResults() const {
	return m_current;
}

vector<DataType> Search::validTypes() const {
	return m_types;
}

void Search::stuff(const vector<TypedSearchResult>& fakeResults) {
	m_current = vector<TypedSearchResult>(fakeResults.begin(), fakeResults.end());
	m_hasStarted = true;
}

void Search::remove(const vector<TypedSearchResult>& removedResults) {
	vector<TypedSearchResult> out;
	unordered_set<TypedSearchResult, hash<TypedSearchResult>> results{ removedResults.begin(), removedResults.end() };
	for (const auto& result : m_current) {
		if (results.find(result) == results.end()) {
			out.emplace_back(result);
		}
	}
	m_current = move(out);
}

size_t Search::numResults() const {
	return m_current.size();
}

bool Search::hasUniqueResult() const {
	if (!m_current.size()) {
		return false;
	}
	const TypedSearchResult& result = m_current.front();
	for (const auto& iter : m_current) {
		if (result == iter || static_cast<const SearchResult&>(result) == iter) {
			continue;
		}
		if (result.address + result.type.width - 1 != iter.address + iter.type.width - 1) {
			return false;
		}
	}
	return true;
}

TypedSearchResult Search::uniqueResult() const {
	return m_current.front();
}

Search& Search::operator=(const Search& other) {
	m_current.clear();
	for (const auto& iter : other.m_current) {
		m_current.emplace_back(iter);
	}
	m_types.clear();
	for (const auto& iter : other.m_types) {
		m_types.emplace_back(iter);
	}
	m_hasStarted = other.m_hasStarted;
	return *this;
}

vector<SearchResult> Search::makeResults(vector<size_t> addrs, uint64_t mult, uint64_t div, int64_t bias) {
	vector<SearchResult> out;
	for (const auto& addr : addrs) {
		out.emplace_back(SearchResult{ addr, mult, div, bias });
	}
	return out;
}

vector<size_t> Search::searchValue(const AddressSpace& mem, int64_t value) {
	uint64_t unsignedValue = value;
	if (value < 0) {
		if (value > -0x100) {
			unsignedValue = value & 0xFF;
		} else if (value > -0x10000) {
			unsignedValue = value & 0xFFFF;
		} else if (value > -0x100000000) {
			unsignedValue = value & 0xFFFFFFFF;
		}
	}

	vector<size_t> start;
	vector<size_t> end;
	int nbytes = 0;
	while (unsignedValue || !nbytes) {
		++nbytes;
		uint8_t b = unsignedValue;
		if (start.size() && end.size()) {
			const auto& s = searchByte(mem, b, start, -1);
			const auto& e = searchByte(mem, b, end, 1);
			start.insert(start.end(), s.begin(), s.end());
			end.insert(end.end(), e.begin(), e.end());
		} else {
			start = searchByte(mem, b);
			end = start;
		}
		unsignedValue >>= 8;
	}
	sort(start.begin(), start.end());
	sort(end.begin(), end.end());
	vector<size_t> results = overlap(start, end, nbytes);

	vector<size_t> startWide;
	int remainingBytes = 4 - nbytes;
	int xBytes = 0;
	while (xBytes < remainingBytes) {
		if (startWide.size()) {
			const auto& s = searchByte(mem, 0, startWide, -1);
			startWide.insert(startWide.end(), s.begin(), s.end());
		} else if (!xBytes && start.size()) {
			startWide = searchByte(mem, 0, start, -1);
		} else {
			break;
		}
		++xBytes;
	}
	if (startWide.size()) {
		results.insert(results.end(), startWide.begin(), startWide.end());
	}
	return results;
}

vector<size_t> Search::searchByte(const AddressSpace& mem, uint8_t value, const vector<size_t> addresses, ssize_t offset) {
	vector<size_t> results;
	if (addresses.size()) {
		for (const auto& i : addresses) {
			if (offset < 0 && i < -offset) {
				continue;
			} else if (!mem.hasBlock(i) || !mem.hasBlock(i + offset)) {
				continue;
			}
			if (mem[i + offset] == value) {
				results.push_back(i + offset);
			}
		}
	} else {
		for (const auto& block : mem.blocks()) {
			if (offset < 0) {
				offset = 0;
			}
			for (ssize_t i = offset; i < block.second.size(); ++i) {
				if (block.second[i] == value) {
					results.push_back(i + block.first);
				}
			}
		}
	}
	return results;
}

vector<size_t> Search::overlap(const vector<size_t> start, vector<size_t> end, size_t width) {
	vector<size_t> results;
	auto siter = start.cbegin();
	auto eiter = end.cbegin();
	while (siter != start.cend() && eiter != end.cend()) {
		size_t e = *siter + width - 1;
		if (e < *eiter) {
			++siter;
		} else if (e > *eiter) {
			++eiter;
		} else {
			results.push_back(*siter);
			++siter;
			++eiter;
		}
	}
	return results;
}

void Search::reduceOnTypes(const AddressSpace& mem, const vector<SearchResult>& in, int64_t value) {
	DataType bcd("=d8");
	multimap<SearchResult, DataType> results;
	for (const auto& type : m_types) {
		auto result = in.cbegin();
		for (const auto& block : mem.blocks()) {
			const DynamicMemoryView dynmem(const_cast<void*>(block.second.offset(0)), block.second.size(), type, mem.overlay());
			for (; result != in.cend(); ++result) {
				if (type.width + result->address - block.first > block.second.size()) {
					break;
				}
				if (result->address < block.first) {
					continue;
				}
				int64_t inmem = dynmem[result->address - block.first];
				if (type.repr == Repr::BCD) {
					if (!isBcd(result->mult) || !isBcd(result->div)) {
						continue;
					}
					inmem /= bcd.decode(&result->mult);
					inmem *= bcd.decode(&result->div);
				} else {
					inmem /= result->mult;
					inmem *= result->div;
				}
				inmem -= result->bias;
				if (value == inmem) {
					results.emplace(*result, type);
				}
			}
		}
	}

	vector<TypedSearchResult> results2;
	for (const auto& result : results) {
		results2.emplace_back(result.first, result.second);
	}

	intersectCurrent(move(results2));
}

void Search::intersectCurrent(vector<TypedSearchResult>&& results) {
	if (m_hasStarted) {
		vector<TypedSearchResult> out;
		auto oldResults = m_current.begin();
		for (const auto& result : results) {
			if (oldResults == m_current.end()) {
				break;
			}
			// Invariant: always stored ordered by address
			while (oldResults != m_current.end() && static_cast<const SearchResult&>(*oldResults) < result) {
				++oldResults;
			}
			auto thisAddress = oldResults;
			while (thisAddress != m_current.end() && thisAddress->address == result.address) {
				if (*thisAddress == result) {
					out.emplace_back(result);
					break;
				}
				++thisAddress;
			}
		}
		m_current = move(out);
	} else {
		m_current = move(results);
	}

	m_hasStarted = true;
}

// From CityHash
#if __SIZEOF_SIZE_T__ == 8
template<class T>
inline void hash_combine(std::size_t& seed, const T& v) {
	std::hash<T> hasher;
	const std::size_t kMul = 0x9ddfea08eb382d69ULL;
	std::size_t a = (hasher(v) ^ seed) * kMul;
	a ^= (a >> 47);
	std::size_t b = (seed ^ a) * kMul;
	b ^= (b >> 47);
	seed = b * kMul;
}
#elif __SIZEOF_SIZE_T__ == 4
inline void hash_combine(std::size_t& seed, const T& v) {
	std::hash<T> hasher;
	const std::size_t kMul = 0x9e3779b9;
	std::size_t a = (hasher(v) ^ seed) * kMul;
	a ^= (a >> 47);
	std::size_t b = (seed ^ a) * kMul;
	b ^= (b >> 47);
	seed = b * kMul;
}
#else
#error Unsupported __SIZEOF_SIZE_T__
#endif

size_t hash<SearchResult>::operator()(const SearchResult& result) const {
	size_t seed = 0;
	hash_combine(seed, hash<size_t>()(result.address));
	hash_combine(seed, hash<uint64_t>()(result.mult));
	hash_combine(seed, hash<uint64_t>()(result.div));
	hash_combine(seed, hash<int64_t>()(result.bias));
	return seed;
}

size_t hash<TypedSearchResult>::operator()(const TypedSearchResult& result) const {
	size_t seed = 0;
	hash_combine(seed, hash<SearchResult>()(result));
	hash_combine(seed, hash<DataType>()(result.type));
	return seed;
}

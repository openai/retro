#include "data.h"

#ifdef ERROR
#undef ERROR
#endif

#ifdef VOID
#undef VOID
#endif

#include "serialize.capnp.h"
#include <capnp/message.h>
#include <capnp/serialize-packed.h>
#include <kj/exception.h>

using namespace ::capnp;
using namespace Retro;
using namespace std;

bool GameData::loadSearches(const std::string& filename) {
	int fd = open(filename.c_str(), O_RDONLY);
	if (fd < 0) {
		return false;
	}

	m_searches.clear();
	m_searchOldMem.clear();

	try {
		PackedFdMessageReader message(fd);

		List<Serialize::SearchTuple>::Reader searches = message.getRoot<List<Serialize::SearchTuple>>();

		for (const auto& ssearch : searches) {
			vector<DataType> types;
			for (const auto& type : ssearch.getSearch().getValidTypes()) {
				types.emplace_back(type);
			}

			vector<TypedSearchResult> results;
			for (const auto& result : ssearch.getSearch().getCurrentResults()) {
				results.emplace_back(SearchResult{ result.getAddress(), result.getMult(), result.getDiv(), result.getBias() }, DataType(result.getType()));
			}
			m_searches.emplace(ssearch.getName(), types);
			m_searches[ssearch.getName()].stuff(results);

			AddressSpace& mem = m_searchOldMem[ssearch.getName()];
			mem.reset();
			for (const auto& block : ssearch.getBlocks()) {
				mem.addBlock(block.getOffset(), block.getMem().size());
				MemoryView<>& memblock = mem.block(block.getOffset());
				size_t i = 0;
				for (const auto& ch : block.getMem()) {
					memblock[i] = ch;
					++i;
				}
			}
		}
	} catch (...) {
		return false;
	}

	return true;
}

bool GameData::saveSearches(const std::string& filename) const {
	int fd = open(filename.c_str(), O_WRONLY | O_TRUNC | O_CREAT, 0600);
	if (fd < 0) {
		return false;
	}

	MallocMessageBuilder message;

	try {
		List<Serialize::SearchTuple>::Builder searches = message.initRoot<List<Serialize::SearchTuple>>(m_searches.size());

		size_t i = 0;
		for (const auto& search : m_searches) {
			Serialize::SearchTuple::Builder searchMemBuilder = searches[i];
			++i;

			searchMemBuilder.setName(search.first);

			Serialize::Search::Builder searchBuilder = searchMemBuilder.initSearch();
			List<Serialize::TypedSearchResult>::Builder typedResults = searchBuilder.initCurrentResults(search.second.numResults());
			size_t j = 0;
			for (const auto& result : search.second.typedResults()) {
				Serialize::TypedSearchResult::Builder typedResult = typedResults[j];
				++j;
				typedResult.setAddress(result.address);
				typedResult.setMult(result.mult);
				typedResult.setDiv(result.div);
				typedResult.setBias(result.bias);
				typedResult.setType(kj::StringPtr(result.type.type, 4));
			}

			const auto types = search.second.validTypes();
			List<Text>::Builder validTypes = searchBuilder.initValidTypes(types.size());
			j = 0;
			for (const auto& type : types) {
				validTypes.set(j, kj::StringPtr(type.type, 4));
				++j;
			}

			const AddressSpace& mem = m_searchOldMem.find(search.first)->second;
			const auto& blocks = mem.blocks();
			List<Serialize::Block>::Builder blocksBuilder = searchMemBuilder.initBlocks(blocks.size());
			j = 0;
			for (const auto& block : blocks) {
				Serialize::Block::Builder blockBuilder = blocksBuilder[j];
				blockBuilder.setMem(kj::arrayPtr(static_cast<const uint8_t*>(block.second.offset(0)), block.second.size()));
				blockBuilder.setOffset(block.first);
				++j;
			}
		}

		writePackedMessageToFd(fd, message);
		return true;
	} catch (...) {
		return false;
	}
}

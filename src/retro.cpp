#include <cmath>
#ifdef _WIN32
// pyconfig.h doesn't seem to like hypot, so we need to work around it.
namespace std {
template<typename T>
static inline T _hypot(T x, T y) {
	return hypot(x, y);
}
}
#endif
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

#include "coreinfo.h"
#include "data.h"
#include "emulator.h"
#include "imageops.h"
#include "memory.h"
#include "search.h"
#include "script.h"
#include "movie.h"
#include "movie-bk2.h"

#include <map>
#include <unordered_map>
#include <unordered_set>

namespace py = pybind11;

using std::string;
using namespace Retro;

struct PyGameData;
struct PyRetroEmulator {
	Retro::Emulator m_re;
	int m_cheats = 0;
	PyRetroEmulator(const string& rom_path) {
		if (Emulator::isLoaded()) {
			throw std::runtime_error("Cannot create multiple emulator instances per process");
		}
		if (!m_re.loadRom(rom_path.c_str())) {
			throw std::runtime_error("Could not load ROM");
		}
		m_re.run(); // otherwise you get a segfault when you try to get screen for the first time
	}

	void step() {
		m_re.run();
	}

	py::bytes getState() {
		size_t size = m_re.serializeSize();
		py::bytes bytes(NULL, size);
		m_re.serialize(PyBytes_AsString(bytes.ptr()), size);
		return bytes;
	}

	bool setState(py::bytes o) {
		return m_re.unserialize(PyBytes_AsString(o.ptr()), PyBytes_Size(o.ptr()));
	}

	py::array_t<uint8_t> getScreen() {
		long w = m_re.getImageWidth();
		long h = m_re.getImageHeight();
		py::array_t<uint8_t> arr({ { h, w, 3 } });
		uint8_t* data = arr.mutable_data();
		Image out(Image::Format::RGB888, data, w, h, w);
		Image in;
		if (m_re.getImageDepth() == 16) {
			in = Image(Image::Format::RGB565, m_re.getImageData(), w, h, m_re.getImagePitch());
		} else if (m_re.getImageDepth() == 32) {
			in = Image(Image::Format::RGBX888, m_re.getImageData(), w, h, m_re.getImagePitch());
		}
		in.copyTo(&out);
		return arr;
	}

	double getScreenRate() {
		return m_re.getFrameRate();
	}

	py::array_t<int16_t> getAudio() {
		py::array_t<int16_t> arr(py::array::ShapeContainer{ m_re.getAudioSamples(), 2 });
		int16_t* data = arr.mutable_data();
		memcpy(data, m_re.getAudioData(), m_re.getAudioSamples() * 4);
		return arr;
	}

	double getAudioRate() {
		return m_re.getAudioRate();
	}

	py::tuple getResolution() {
		return py::make_tuple(m_re.getImageWidth(), m_re.getImageHeight());
	}

	void setButtonMask(py::array_t<uint8_t> mask, unsigned player) {
		if (mask.size() > N_BUTTONS) {
			throw std::runtime_error("mask.size() > N_BUTTONS");
		}
		if (player >= MAX_PLAYERS) {
			throw std::runtime_error("player >= MAX_PLAYERS");
		}
		for (int key = 0; key < mask.size(); ++key) {
			m_re.setKey(player, key, mask.data()[key]);
		}
	}

	void addCheat(const string& code) {
		m_re.setCheat(m_cheats, true, code.c_str());
		++m_cheats;
	}

	void clearCheats() {
		m_re.clearCheats();
		m_cheats = 0;
	}

	void configureData(PyGameData& data);
	static bool loadCoreInfo(const string& json) {
		return Retro::loadCoreInfo(json);
	}
};

struct PyMemoryView {
	Retro::AddressSpace& m_mem;
	PyMemoryView(Retro::AddressSpace& mem)
		: m_mem(mem) {
	}

	int64_t extract(size_t address, const string& type) {
		return m_mem[Variable{ type, address }];
	}

	void assign(size_t address, const string& type, int64_t value) {
		m_mem[Variable{ type, address }] = value;
	}

	void setitem(py::dict item, int64_t value) {
		return assign(py::int_(item["address"]), py::str(item["type"]), value);
	}

	int64_t getitem(py::dict item) {
		return extract(py::int_(item["address"]), py::str(item["type"]));
	}

	py::dict blocks() {
		py::dict obj;
		for (const auto& iter : m_mem.blocks()) {
			obj[py::int_(iter.first)] = py::bytes(static_cast<const char*>(iter.second.offset(0)), iter.second.size());
		}
		return obj;
	}
};

struct PySearch {
	Retro::Search* m_search;
	bool m_managed = true;
	PySearch(py::handle types) {
		if (!types.is_none()) {
			std::vector<Retro::DataType> dtypes;
			for (const auto& type : types) {
				dtypes.emplace_back(py::str(type));
			}
			m_search = new Retro::Search(dtypes);
		}
	}

	PySearch(Retro::Search* search) {
		m_search = search;
		m_managed = false;
	}

	~PySearch() {
		if (m_managed) {
			delete m_search;
		}
	}

	int numResults() const {
		return m_search->numResults();
	}

	bool hasUniqueResult() const {
		return m_search->hasUniqueResult();
	}

	py::dict uniqueResult() const {
		TypedSearchResult result = m_search->uniqueResult();
		py::dict obj;
		obj["address"] = result.address;
		obj["type"] = result.type.type;
		return obj;
	}

	py::list typedResults() const {
		std::map<SearchResult, std::unordered_set<DataType>> results;
		for (const auto& result : m_search->typedResults()) {
			results[static_cast<const SearchResult&>(result)].emplace(result.type);
		}
		py::list flattedResults;
		for (const auto& result : results) {
			py::list typeStrings;
			for (const auto& type : result.second) {
				typeStrings.append(py::str(type.type));
			}
			flattedResults.append(py::make_tuple(
				py::make_tuple(
					result.first.address,
					result.first.mult,
					result.first.div,
					result.first.bias),
				typeStrings));
		}
		return flattedResults;
	}
};

struct PyGameData {
	Retro::GameData m_data;
	Retro::Scenario m_scen{ m_data };

	bool load(py::handle data = py::none(), py::handle scen = py::none()) {
		ScriptContext::reset();

		bool success = true;
		if (!data.is_none()) {
			success = success && m_data.load(py::str(data));
		}
		if (!scen.is_none()) {
			success = success && m_scen.load(py::str(scen));
		}
		return success;
	}

	bool save(py::handle data = py::none(), py::handle scen = py::none()) {
		bool success = true;
		if (!data.is_none()) {
			success = success && m_data.save(py::str(data));
		}
		if (!scen.is_none()) {
			success = success && m_scen.save(py::str(scen));
		}
		return success;
	}

	void reset() {
		m_scen.restart();
		m_scen.reloadScripts();
	}

	uint16_t filterAction(uint16_t action) const {
		return m_scen.filterAction(action);
	}

	py::list validActions() const {
		py::list outer;
		for (const auto& action : m_scen.validActions()) {
			py::list inner;
			for (const auto& act : action.second) {
				inner.append(act);
			}
			outer.append(inner);
		}
		return outer;
	}

	void updateRam() {
		m_data.updateRam();
		m_scen.update();
	}

	py::object lookupValue(py::str name) const {
		try {
			Variant data = m_data.lookupValue(name);
			switch (data.type()) {
			case Variant::Type::BOOL:
				return static_cast<py::bool_>(data);
			case Variant::Type::INT:
				return static_cast<py::int_>(static_cast<int64_t>(data));
			case Variant::Type::FLOAT:
				return static_cast<py::float_>(static_cast<double>(data));
			case Variant::Type::VOID:
				return py::none();
			}
		} catch (std::invalid_argument e) {
			throw pybind11::key_error(e.what());
		}
	}

	py::object setValue(py::str name, py::object value) {
		if (py::isinstance<py::bool_>(value)) {
			m_data.setValue(name, Variant(static_cast<bool>(py::bool_(value))));
		}
		if (py::isinstance<py::int_>(value)) {
			m_data.setValue(name, Variant(static_cast<int64_t>(py::int_(value))));
		}
		if (py::isinstance<py::float_>(value)) {
			m_data.setValue(name, Variant(static_cast<double>(py::float_(value))));
		}
		if (value.is_none()) {
			m_data.setValue(name, Variant());
		}
		return value;
	}

	py::dict lookupAll() const {
		py::dict data;
		for (const auto& var : m_data.lookupAll()) {
			data[py::str(var.first)] = var.second;
		}
		return data;
	}

	py::dict getVariable(py::str name) const {
		py::dict obj;
		Retro::Variable var = m_data.getVariable(name);
		obj["address"] = var.address;
		obj["type"] = var.type.type;
		return obj;
	}

	void setVariable(py::str name, py::dict obj) {
		Retro::Variable var{ string(py::str(obj["type"])), py::int_(obj["address"]) };
		m_data.setVariable(name, var);
	}

	void removeVariable(py::str name) {
		m_data.removeVariable(name);
	}

	py::dict listVariables() {
		const auto& vars = m_data.listVariables();
		py::dict vdict;
		for (const auto& var : vars) {
			const auto& v = var.second;
			vdict[py::str(var.first)] = py::dict(py::arg("address") = v.address, py::arg("type") = v.type.type);
		}
		return vdict;
	}

	float currentReward(unsigned player = 0) const {
		return m_scen.currentReward(player);
	}

	float totalReward(unsigned player = 0) const {
		return m_scen.totalReward(player);
	}

	bool isDone() const {
		return m_scen.isDone();
	}

	py::tuple cropInfo(unsigned player = 0) {
		size_t x = 0;
		size_t y = 0;
		size_t width = 0;
		size_t height = 0;
		m_scen.getCrop(&x, &y, &width, &height, player);
		return py::make_tuple(x, y, width, height);
	}

	PyMemoryView memory() {
		return PyMemoryView(m_data.addressSpace());
	}

	void search(py::str name, int64_t value) {
		m_data.search(name, value);
	}

	void deltaSearch(py::str name, py::str op, int64_t ref) {
		m_data.deltaSearch(name, Retro::Scenario::op(op), ref);
	}

	PySearch getSearch(py::str name) {
		return m_data.getSearch(name);
	}

	void removeSearch(py::str name) {
		m_data.removeSearch(name);
	}

	py::dict listSearches() {
		const auto& names = m_data.listSearches();
		py::dict searches;
		for (const auto& name : names) {
			searches[py::str(name)] = PySearch(m_data.getSearch(name));
		}
		return searches;
	}
};

void PyRetroEmulator::configureData(PyGameData& data) {
	m_re.configureData(&data.m_data);
}

struct PyMovie {
	std::unique_ptr<Retro::Movie> m_movie;
	bool recording = false;
	PyMovie(py::str name, bool record, unsigned players) {
		recording = record;
		if (record) {
			m_movie = std::make_unique<MovieBK2>(name, true, players);
		} else {
			m_movie = Movie::load(name);
		}
		if (!m_movie) {
			throw std::runtime_error("Could not load movie");
		}
	}

	void configure(py::str name, const PyRetroEmulator& emu) {
		if (recording) {
			static_cast<MovieBK2*>(m_movie.get())->setGameName(name);
			static_cast<MovieBK2*>(m_movie.get())->loadKeymap(emu.m_re.core());
		}
	}

	py::str getGameName() const {
		return m_movie->getGameName();
	}

	bool step() {
		return m_movie->step();
	}

	void close() {
		m_movie->close();
	}

	unsigned players() {
		return m_movie->players();
	}

	bool getKey(int key, unsigned player = 0) {
		return m_movie->getKey(key, player);
	}

	void setKey(int key, bool set, unsigned player = 0) {
		return m_movie->setKey(key, set, player);
	}

	py::bytes getState() {
		std::vector<uint8_t> data;
		m_movie->getState(&data);
		return py::bytes(reinterpret_cast<const char*>(data.data()), data.size());
	}

	void setState(py::bytes data) {
		m_movie->setState(reinterpret_cast<uint8_t*>(PyBytes_AsString(data.ptr())), PyBytes_Size(data.ptr()));
	}
};

py::str corePath(py::handle hint = py::none()) {
	return Retro::corePath(py::str(hint));
}

py::str dataPath(py::handle hint = py::none()) {
	return Retro::GameData::dataPath(py::str(hint));
}

PYBIND11_MODULE(_retro, m) {
	m.doc() = "libretro bindings";

	py::class_<PyRetroEmulator>(m, "RetroEmulator")
		.def(py::init<const string&>())
		.def("step", &PyRetroEmulator::step)
		.def("set_button_mask", &PyRetroEmulator::setButtonMask, py::arg("mask"), py::arg("player") = 0)
		.def("get_state", &PyRetroEmulator::getState)
		.def("set_state", &PyRetroEmulator::setState)
		.def("get_screen", &PyRetroEmulator::getScreen)
		.def("get_screen_rate", &PyRetroEmulator::getScreenRate)
		.def("get_audio", &PyRetroEmulator::getAudio)
		.def("get_audio_rate", &PyRetroEmulator::getAudioRate)
		.def("get_resolution", &PyRetroEmulator::getResolution)
		.def("configure_data", &PyRetroEmulator::configureData)
		.def("add_cheat", &PyRetroEmulator::addCheat)
		.def("clear_cheats", &PyRetroEmulator::clearCheats)
		.def_static("load_core_info", &PyRetroEmulator::loadCoreInfo);

	py::class_<PyMemoryView>(m, "Memory")
		.def(py::init<Retro::AddressSpace&>())
		.def("extract", &PyMemoryView::extract, py::arg("address"), py::arg("type"))
		.def("assign", &PyMemoryView::assign, py::arg("address"), py::arg("type"), py::arg("value"))
		.def_property_readonly("blocks", &PyMemoryView::blocks)
		.def("__setitem__", &PyMemoryView::setitem, py::arg("item"), py::arg("value"))
		.def("__getitem__", &PyMemoryView::getitem, py::arg("item"));

	py::class_<PySearch>(m, "Search")
		.def(py::init<py::handle>(), py::arg("types") = py::none())
		.def("num_results", &PySearch::numResults)
		.def("has_unique_result", &PySearch::hasUniqueResult)
		.def("unique_result", &PySearch::uniqueResult)
		.def("typed_results", &PySearch::typedResults);

	py::class_<PyGameData>(m, "GameDataGlue")
		.def(py::init<>())
		.def("load", &PyGameData::load, py::arg("data") = py::none(), py::arg("scen") = py::none())
		.def("save", &PyGameData::save, py::arg("data") = py::none(), py::arg("scen") = py::none())
		.def("reset", &PyGameData::reset)
		.def("filter_action", &PyGameData::filterAction)
		.def("valid_actions", &PyGameData::validActions)
		.def("update_ram", &PyGameData::updateRam)
		.def("lookup_value", &PyGameData::lookupValue)
		.def("set_value", &PyGameData::setValue)
		.def("lookup_all", &PyGameData::lookupAll)
		.def("get_variable", &PyGameData::getVariable)
		.def("set_variable", &PyGameData::setVariable)
		.def("remove_variable", &PyGameData::removeVariable)
		.def("list_variables", &PyGameData::listVariables)
		.def("search", &PyGameData::search)
		.def("delta_search", &PyGameData::deltaSearch)
		.def("get_search", &PyGameData::getSearch)
		.def("remove_search", &PyGameData::removeSearch)
		.def("list_searches", &PyGameData::listSearches)
		.def("current_reward", &PyGameData::currentReward, py::arg("player") = 0)
		.def("total_reward", &PyGameData::totalReward, py::arg("player") = 0)
		.def("is_done", &PyGameData::isDone)
		.def("crop_info", &PyGameData::cropInfo, py::arg("player") = 0)
		.def_property_readonly("memory", &PyGameData::memory);

	py::class_<PyMovie>(m, "Movie")
		.def(py::init<py::str, bool, unsigned>(), py::arg("path"), py::arg("record") = false, py::arg("players") = 1)
		.def("configure", &PyMovie::configure)
		.def("get_game", &PyMovie::getGameName)
		.def("step", &PyMovie::step)
		.def("close", &PyMovie::close)
		.def_property_readonly("players", &PyMovie::players)
		.def("get_key", &PyMovie::getKey)
		.def("set_key", &PyMovie::setKey)
		.def("get_state", &PyMovie::getState)
		.def("set_state", &PyMovie::setState);

	m.def("core_path", &::corePath, py::arg("hint") = py::none());
	m.def("data_path", &::dataPath, py::arg("hint") = py::none());
}

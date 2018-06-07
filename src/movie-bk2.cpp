#include "movie-bk2.h"

#include <cstring>
#include <sstream>

#include "coreinfo.h"

using namespace std;
using namespace Retro;

static const unordered_map<string, char> s_keyNames{
	make_pair("A", 'A'),
	make_pair("B", 'B'),
	make_pair("C", 'C'),
	make_pair("X", 'X'),
	make_pair("Y", 'Y'),
	make_pair("Z", 'Z'),
	make_pair("START", 'S'),
	make_pair("SELECT", 's'),
	make_pair("MODE", 'M'),
	make_pair("UP", 'U'),
	make_pair("DOWN", 'D'),
	make_pair("LEFT", 'L'),
	make_pair("RIGHT", 'R'),
	make_pair("L", 'l'),
	make_pair("R", 'r'),
	make_pair("BUTTON", 'B'),
};

static const unordered_map<char, string> s_buttonNames{
	make_pair('A', "A"),
	make_pair('B', "B"),
	make_pair('C', "C"),
	make_pair('X', "X"),
	make_pair('Y', "Y"),
	make_pair('Z', "Z"),
	make_pair('S', "Start"),
	make_pair('s', "Select"),
	make_pair('M', "Mode"),
	make_pair('U', "Up"),
	make_pair('D', "Down"),
	make_pair('L', "Left"),
	make_pair('R', "Right"),
	make_pair('l', "L"),
	make_pair('r', "R"),
};

static const unordered_map<string, unordered_map<char, string>> s_platformButtonNames{
	make_pair<string, unordered_map<char, string>>("Atari2600", {
																	make_pair('B', "Button"),
																}),
};

unique_ptr<Movie> MovieBK2::load(const string& path) {
	unique_ptr<Zip> zip = make_unique<Zip>(path);
	if (!zip->open()) {
		return nullptr;
	}
	if (!zip->openFile("Input Log.txt")) {
		return nullptr;
	}
	return make_unique<MovieBK2>(move(zip));
}

MovieBK2::MovieBK2(unique_ptr<Zip> zip)
	: m_zip(move(zip))
	, m_log(m_zip->openFile("Input Log.txt")) {
	string platform;

	Zip::File* header = m_zip->openFile("Header.txt");
	while (true) {
		string headerLine = header->readline();
		if (headerLine.empty()) {
			break;
		}
		if (headerLine.compare(0, 8, "Platform") == 0) {
			platform = headerLine.substr(9);
			if (platform == "GEN") {
				platform = "Genesis";
			} else if (platform == "SNES") {
				platform = "Snes";
			} else if (platform == "NES") {
				platform = "Nes";
			} else if (platform == "A26") {
				platform = "Atari2600";
			}
			loadKeymap(platform);
		}
		if (headerLine.compare(0, 8, "GameName") == 0) {
			m_gameName = headerLine.substr(9);
		}
	}

	string tmp = m_log->readline();
	if (tmp == "[Input]") {
		tmp = m_log->readline();
		size_t pos = 0;
		string token;
		while ((pos = tmp.find('|')) != std::string::npos) {
			unsigned player = stoul(tmp.substr(1, 2));
			if (player > m_players) {
				m_players = player;
			}
			tmp.erase(0, pos + 1);
		}
	}

	loadState();
}

MovieBK2::MovieBK2(const std::string& path, bool write, unsigned players)
	: m_zip(make_unique<Zip>(path))
	, m_write(write) {
	m_players = players;
	m_zip->open(write);
	m_log = m_zip->openFile("Input Log.txt", write);
	if (write) {
		stringstream headerText;
		headerText << "[Input]" << endl;
		m_log->write(static_cast<const void*>(headerText.str().c_str()), headerText.str().size());
	} else {
		loadState();
	}
}

MovieBK2::~MovieBK2() {
	close();
}

string MovieBK2::getGameName() const {
	return m_gameName;
}

void MovieBK2::loadKeymap(const string& platform) {
	vector<string> buttons = Retro::buttons(platform);
	for (int i = 0; i < buttons.size(); ++i) {
		const auto& button = s_keyNames.find(buttons[i]);
		if (button != s_keyNames.end()) {
			m_keymap[button->second] = i;
			m_buttonmap[i] = button->second;
		}
	}
	if (m_write) {
		string realPlatform = platform;
		if (platform == "Genesis") {
			realPlatform = "GEN";
		} else if (platform == "Snes") {
			realPlatform = "SNES";
		} else if (platform == "Nes") {
			realPlatform = "NES";
		} else if (platform == "Atari2600") {
			realPlatform = "A26";
		}
		m_coreName = platform;
		m_platform = realPlatform;
	}
}

void MovieBK2::setGameName(const std::string& name) {
	m_gameName = name;
}

void MovieBK2::writeHeader() {
	if (m_headerWritten) {
		return;
	}
	Zip::File* header = m_zip->openFile("Header.txt", true);
	stringstream headerText;
	headerText << "MovieVersion Retro" << endl;
	headerText << "Author ?" << endl;
	headerText << "emuVersion ?" << endl;
	headerText << "Platform " << m_platform << endl;
	headerText << "GameName " << m_gameName << endl;
	headerText << "SHA1 ?" << endl;
	headerText << "Core ?" << endl;
	headerText << "rerecordCount 1" << endl;
	header->write(static_cast<const void*>(headerText.str().c_str()), headerText.str().size());

	headerText.str("LogKey:#Reset|Power|#");
	for (unsigned p = 1; p < m_players + 1; ++p) {
		for (const auto& key : m_buttonmap) {
			if (s_platformButtonNames.find(m_coreName) != s_platformButtonNames.end()) {
				const auto& platformButtons = s_platformButtonNames.at(m_coreName);
				if (platformButtons.find(key.second) != platformButtons.end()) {
					headerText << "P" << p << " " << platformButtons.at(key.second) << "|";
					continue;
				}
			}
			headerText << "P" << p << " " << s_buttonNames.at(key.second) << "|";
		}
	}
	headerText << endl;
	m_headerWritten = true;
	m_log->write(static_cast<const void*>(headerText.str().c_str()), headerText.str().size());
}

bool MovieBK2::step() {
	if (!m_log) {
		return false;
	}
	if (m_write) {
		if (!m_headerWritten) {
			writeHeader();
		}
		stringstream line;
		line << "|..|";
		for (unsigned i = 0; i < m_players; ++i) {
			for (const auto& key : m_buttonmap) {
				if (m_keys[i] & (1 << key.first)) {
					line << key.second;
				} else {
					line << '.';
				}
			}
			m_keys[i] = 0;
			line << '|';
		}
		line << endl;
		m_log->write(static_cast<const void*>(line.str().c_str()), line.str().size());
		return true;
	} else {
		string tmp = m_log->readline();
		while (tmp.size() && tmp[0] != '|') {
			tmp = m_log->readline();
		}
		if (!tmp.size()) {
			return false;
		}
		auto iter = tmp.begin();
		if (*iter == '|') {
			++iter;
			while (iter != tmp.end() && *iter != '|') {
				// Ignore commands
				++iter;
			}
			if (iter == tmp.end()) {
				return false;
			}
			for (unsigned i = 0; i < m_players; ++i) {
				m_keys[i] = 0;
				++iter;
				while (iter != tmp.end() && *iter != '|') {
					if (*iter == '.') {
						++iter;
						continue;
					}
					auto found = m_keymap.find(*iter);
					if (found != m_keymap.end()) {
						m_keys[i] |= 1 << found->second;
					}
					++iter;
				}
			}
			return true;
		}
	}
	return false;
}

void MovieBK2::close() {
	if (!m_zip) {
		return;
	}
	if (m_write) {
		const char* footerText = "[/Input]";
		m_log->write(static_cast<const void*>(footerText), strlen(footerText));
		if (!m_state.empty()) {
			auto state = m_zip->openFile("Core.bin", true);
			state->write(m_state.data(), m_state.size());
		}
	}
	m_zip->close();
	m_zip.reset();
}

bool MovieBK2::getState(vector<uint8_t>* state) const {
	if (m_state.empty()) {
		return false;
	}
	state->resize(m_state.size());
	memcpy(state->data(), m_state.data(), m_state.size());
	return true;
}

void MovieBK2::setState(const uint8_t* state, size_t size) {
	m_state.resize(size);
	memcpy(m_state.data(), state, size);
}

void MovieBK2::loadState() {
	Zip::File* state = m_zip->openFile("Core.bin");
	if (!state) {
		m_state.clear();
		return;
	}
	m_state.clear();
	m_state.resize(2048);
	ssize_t read;
	while ((read = state->read(&*(m_state.end() - 2048), 2048)) == 2048) {
		m_state.resize(m_state.size() + 2048);
	}
	m_state.resize(m_state.size() - 2048 + read);
}

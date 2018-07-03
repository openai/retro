#include "movie-fm2.h"

#include "coreinfo.h"

#include <fstream>
#include <unordered_map>

using namespace std;
using namespace Retro;

unordered_map<char, int> s_keymap;

std::unique_ptr<Movie> MovieFM2::load(const std::string& path) {
	unique_ptr<ifstream> stream = make_unique<ifstream>(path);
	string tmp;
	getline(*stream, tmp);
	if (tmp.back() == '\r') {
		tmp.pop_back();
	}
	if (tmp != "version 3") {
		return nullptr;
	}
	return make_unique<MovieFM2>(move(stream));
}

MovieFM2::MovieFM2(std::unique_ptr<std::istream> stream)
	: m_stream(move(stream)) {
	if (!s_keymap.size()) {
		vector<string> buttons = Retro::buttons("Nes");
		for (int i = 0; i < buttons.size(); ++i) {
			if (buttons[i] == "A") {
				s_keymap['A'] = i;
			} else if (buttons[i] == "B") {
				s_keymap['B'] = i;
			} else if (buttons[i] == "UP") {
				s_keymap['U'] = i;
			} else if (buttons[i] == "DOWN") {
				s_keymap['D'] = i;
			} else if (buttons[i] == "LEFT") {
				s_keymap['L'] = i;
			} else if (buttons[i] == "RIGHT") {
				s_keymap['R'] = i;
			} else if (buttons[i] == "START") {
				s_keymap['T'] = i;
			} else if (buttons[i] == "SELECT") {
				s_keymap['S'] = i;
			}
		}
	}
	string tmp;
	while (m_stream->good() && (tmp.size() < 6 || tmp.substr(0, 6) != "port2 ")) {
		getline(*m_stream, tmp);
		if (tmp.back() == '\r') {
			tmp.pop_back();
		}
		if (tmp == "port1 1") {
			m_players = 2;
		}
	}
}

bool MovieFM2::step() {
	string tmp = "?";
	while (m_stream->good() && tmp[0] != '|') {
		getline(*m_stream, tmp);
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
				auto found = s_keymap.find(*iter);
				if (found != s_keymap.end()) {
					m_keys[i] |= 1 << found->second;
				}
				++iter;
			}
		}
		return true;
	}
	return false;
}

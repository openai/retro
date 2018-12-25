#pragma once

#include "emulator.h"

#include <memory>
#include <string>
#include <vector>

namespace Retro {

class Movie {
public:
	static std::unique_ptr<Movie> load(const std::string& path);
	virtual ~Movie() {}

	virtual std::string getGameName() const { return {}; }

	virtual bool step() = 0;

	virtual void close() {}

	virtual bool getState(std::vector<uint8_t>*) const { return false; }
	virtual void setState(const uint8_t*, size_t) {}

	bool getKey(int, unsigned player = 0);
	void setKey(int key, bool, unsigned player = 0);

	unsigned players() const { return m_players; }

protected:
	uint16_t m_keys[MAX_PLAYERS] = { 0 };
	unsigned m_players = 1;
};
}

#pragma once

#include <unordered_map>
#include <vector>

#include "movie.h"
#include "zipfile.h"

namespace Retro {

class MovieBK2 final : public Movie {
public:
	MovieBK2(std::unique_ptr<Zip>);
	MovieBK2(const std::string& path, bool write = false, unsigned players = 1);
	~MovieBK2();

	virtual std::string getGameName() const override;

	void loadKeymap(const std::string& platform);
	void setGameName(const std::string& name);
	void writeHeader();

	static std::unique_ptr<Movie> load(const std::string& path);

	virtual bool step() override;

	virtual void close() override;

	virtual bool getState(std::vector<uint8_t>*) const override;
	virtual void setState(const uint8_t*, size_t) override;

private:
	void loadState();

	std::unique_ptr<Zip> m_zip;
	Zip::File* m_log;
	std::vector<uint8_t> m_state;

	std::unordered_map<char, int> m_keymap;
	std::unordered_map<int, char> m_buttonmap;
	bool m_write = false;

	bool m_headerWritten = false;
	std::string m_coreName;
	std::string m_platform;
	std::string m_gameName{ "?" };
};
}

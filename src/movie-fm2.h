#pragma once

#include <istream>

#include "movie.h"

namespace Retro {

class MovieFM2 final : public Movie {
public:
	MovieFM2(std::unique_ptr<std::istream>);

	static std::unique_ptr<Movie> load(const std::string& path);

	virtual bool step() override;

private:
	std::unique_ptr<std::istream> m_stream;
};
}

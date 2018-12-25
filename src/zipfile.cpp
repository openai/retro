#include "zipfile.h"

#include <algorithm>

using namespace Retro;
using namespace std;

Zip::Zip(const string& path)
	: m_path(path) {
}

Zip::~Zip() {
	close();
}

bool Zip::open(bool readwrite) {
	m_zip = zip_open(m_path.c_str(), readwrite ? ZIP_CREATE : ZIP_RDONLY, nullptr);
	return m_zip;
}

void Zip::close() {
	if (!m_zip) {
		return;
	}
	for (auto& file : m_files) {
		file->close();
	}
	zip_close(m_zip);
	m_zip = nullptr;
	m_files.clear();
}

Zip::File* Zip::openFile(const std::string& name, bool write) {
	if (!m_zip) {
		return nullptr;
	}
	Zip::File* zf;
	if (!write) {
		zip_file_t* file = zip_fopen(m_zip, name.c_str(), 0);
		if (!file) {
			return nullptr;
		}
		zf = new Zip::File(m_zip, name, file);
	} else {
		zf = new Zip::File(m_zip, name);
	}
	m_files.emplace_back(zf);
	return zf;
}

Zip::File::File(zip_t* zip, const std::string& name, zip_file_t* file)
	: m_zip(zip)
	, m_file(file)
	, m_name(name) {
}

string Zip::File::readline() {
	auto pos = m_buffer.end();
	pos = find(m_buffer.begin(), m_buffer.end(), '\n');
	while (pos == m_buffer.end()) {
		size_t size = m_buffer.size();
		m_buffer.resize(size + 256);
		size_t r = read(static_cast<void*>(&m_buffer[size]), 256);
		if (!r) {
			string s(m_buffer.begin(), m_buffer.end() - 256);
			m_buffer.clear();
			return s;
		}
		if (r < 256) {
			m_buffer.erase(m_buffer.end() - 256 + r, m_buffer.end());
		}
		pos = find(m_buffer.begin(), m_buffer.end(), '\n');
	}
	if (pos != m_buffer.begin() && *(pos - 1) == '\r') {
		// Strip out carriage returns
		--pos;
	}
	string s(m_buffer.begin(), pos);
	if (*pos == '\r') {
		++pos;
	}
	m_buffer.erase(m_buffer.begin(), pos + 1);
	return s;
}

ssize_t Zip::File::read(void* buffer, size_t size) {
	return zip_fread(m_file, buffer, size);
}

ssize_t Zip::File::write(const void* buffer, size_t size) {
	m_buffer.insert(m_buffer.end(), static_cast<const char*>(buffer), static_cast<const char*>(buffer) + size);
	return size;
}

void Zip::File::close() {
	if (m_file) {
		zip_fclose(m_file);
	} else if (m_buffer.size()) {
		zip_source_t* source = zip_source_buffer(m_zip, static_cast<void*>(&m_buffer.front()), m_buffer.size(), 0);
		if (!source) {
			return;
		}
		zip_int64_t i = zip_file_add(m_zip, m_name.c_str(), source, ZIP_FL_OVERWRITE);
		if (i < 0) {
			return;
		}
	}
}

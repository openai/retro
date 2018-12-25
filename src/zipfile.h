#pragma once

#include "zip.h"

#include <memory>
#include <string>
#include <vector>

namespace Retro {

class Zip {
public:
	class File {
	public:
		File(zip_t*, const std::string& name, zip_file_t* = nullptr);
		File(File&) = delete;

		std::string readline();
		ssize_t read(void* buffer, size_t size);
		ssize_t write(const void* buffer, size_t size);

	private:
		void close();
		friend class Zip;

		zip_t* m_zip;
		zip_file_t* m_file;
		std::vector<char> m_buffer;
		std::string m_name;
	};

	Zip(const std::string& path);
	~Zip();

	bool open(bool readwrite = false);
	void close();

	File* openFile(const std::string& name, bool write = false);

private:
	std::string m_path;

	zip_t* m_zip = nullptr;
	std::vector<std::unique_ptr<File>> m_files;
};
}

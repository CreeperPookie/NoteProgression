#pragma once

#include <fstream>
#include <cstdint>

typedef uint8_t byte;

class BinaryFile
{
	std::ofstream stream;
public:
	explicit BinaryFile(const std::string &file_path);
	void write_bool(bool boolean);
	void write_byte(byte $byte);
	void write_short(short $short);
	void write_int(int $int);
	void write_c_string(const char* string);
	void write_string(const std::string& string);
	void flush();
	void close();
};
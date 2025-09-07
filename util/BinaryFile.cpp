#include "BinaryFile.h"

#include <cstring>

BinaryFile::BinaryFile(const std::string &file_path)
{
	this->stream = std::ofstream(file_path.c_str(), std::ios::binary | std::ios::trunc);
}

void BinaryFile::write_bool(const bool boolean)
{
	write_byte(boolean ? static_cast<byte>(1) : static_cast<byte>(0));
}

void BinaryFile::write_byte(const byte $byte)
{
	stream << $byte;
}

void BinaryFile::write_short(const short $short)
{
	for (int i = 0; i < sizeof(short); i++) write_byte(($short >> (i * 8)) & 0xFF);
}

void BinaryFile::write_int(const int $int)
{
	for (int i = 0; i < sizeof(int); i++) write_byte(($int >> (i * 8)) & 0xFF);
}

void BinaryFile::write_c_string(const char* string)
{
	if (string == "" || string == "\0") write_int(0);
	else
	{
		write_int(static_cast<int>(strlen(string)));
		for (int i = 0; i < strlen(string); i++) stream << static_cast<byte>(string[i]);
	}
}

void BinaryFile::write_string(const std::string& string)
{
	for (char $char : string) write_byte(static_cast<byte>($char));
}

void BinaryFile::flush()
{
	stream.flush();
}

void BinaryFile::close()
{
	stream.close();
}

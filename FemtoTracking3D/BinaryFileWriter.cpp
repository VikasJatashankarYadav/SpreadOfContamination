#include "BinaryFileWriter.h"
#include <iostream>

namespace px {

static char NULL_CHAR = '\0';

BinaryFileWriter::BinaryFileWriter() {}

BinaryFileWriter::BinaryFileWriter(const BinaryFileWriter&) {}

BinaryFileWriter& BinaryFileWriter::operator = (const BinaryFileWriter&) { return *this; }

BinaryFileWriter::~BinaryFileWriter() {
    if ( this->file.is_open() == true ) this->file.close();
}

bool BinaryFileWriter::open(const std::string& filename, bool append) {
	if ( filename.length() == 0 ) return false;
	if ( this->file.is_open() == true ) this->file.close();
	if ( append ) this->file.open(filename.c_str(), std::ios::out | std::ios::binary | std::ios::app);
	else this->file.open(filename.c_str(), std::ios::out | std::ios::binary);
	if ( this->file.is_open() == true ) return true;
	else return false;
}

bool BinaryFileWriter::isOpen() const {
	if ( this->file.is_open() == true ) return true;
	return false;
}

bool BinaryFileWriter::isEmpty() {
	if ( this->file.is_open() == false ) {
		std::cout << "[BinaryFileWritter:isEmpty] Error: Binary file is not open." << std::endl;
		return false;
	}

	std::size_t g = this->file.tellp();
	this->file.seekp(0, std::ios::end);
	std::size_t size = this->file.tellp();
	bool empty;

	if ( size == 0 ) empty = true;
	else empty = false;
	this->file.seekp(g, std::ios::beg);

	return empty;
}

bool BinaryFileWriter::writeSizeT(std::size_t value) {
	if ( this->file.is_open() == false ) {
		std::cout << "[BinaryFileWritter:write] Error: Binary file is not open." << std::endl;
		return false;
	}

	this->file.write(reinterpret_cast<char*>(&value), sizeof(std::size_t));

	if ( this->file.bad() ) {
		std::cout << "good: " << this->file.good() << std::endl;
		std::cout << "eof: " << this->file.eof() << std::endl;
		std::cout << "fail: " << this->file.fail() << std::endl;
		std::cout << "bad: " << this->file.bad() << std::endl;
		std::cout << "rdstate: " << this->file.rdstate() << std::endl;
	}

	return true;
}

bool BinaryFileWriter::writeChar(char value) {
	if ( this->file.is_open() == false ) {
		std::cout << "[BinaryFileWritter:write] Error: Binary file is not open." << std::endl;
		return false;
	}

	this->file.write(&value, sizeof(char));
	return true;
}

bool BinaryFileWriter::writeCharBufferNull(char* buffer, std::size_t length, bool appendNull) {
	if ( this->file.is_open() == false ) {
		std::cout << "[BinaryFileWriter:write] Error: Binary file is not open." << std::endl;
		return false;
	}

	this->file.write(buffer, sizeof(char) * length);
	return true;
}

bool BinaryFileWriter::writeCharBuffer(const char* buffer, std::size_t length) {
	if ( this->file.is_open() == false ) {
		std::cout << "[BinaryFileWriter:write] Error: Binary file is not open." << std::endl;
		return false;
	}

	this->file.write(buffer, sizeof(char) * length);
	return true;
}

bool BinaryFileWriter::writeByte(int8_t value) {
	if ( this->file.is_open() == false ) {
		std::cout << "[BinaryFileWritter:write] Error: Binary file is not open." << std::endl;
		return false;
	}

	this->file.write(reinterpret_cast<char*>(&value), sizeof(int8_t));
	return true;
}

bool BinaryFileWriter::writeByteUnsigned(uint8_t value) {
	if ( this->file.is_open() == false ) {
		std::cout << "[BinaryFileWritter:write] Error: Binary file is not open." << std::endl;
		return false;
	}

	this->file.write(reinterpret_cast<char*>(&value), sizeof(uint8_t));
	return true;
}

bool BinaryFileWriter::writeByteBuffer(int8_t* buffer, std::size_t& length) {
	if ( this->file.is_open() == false ) {
		std::cout << "[BinaryFileWriter:write] Error: Binary file is not open." << std::endl;
		return false;
	}

	this->file.write(reinterpret_cast<const char*>(buffer), sizeof(int8_t) * length);
	return true;
}

bool BinaryFileWriter::writeByteBuffer(const int8_t* buffer, std::size_t& length) {
	if ( this->file.is_open() == false ) {
		std::cout << "[BinaryFileWriter:write] Error: Binary file is not open." << std::endl;
		return false;
	}

	this->file.write(reinterpret_cast<const char*>(buffer), sizeof(int8_t) * length);
	return true;
}

bool BinaryFileWriter::writeByteBufferUnsigned(uint8_t* buffer, std::size_t& length) {
	if ( this->file.is_open() == false ) {
		std::cout << "[BinaryFileWriter:write] Error: Binary file is not open." << std::endl;
		return false;
	}

	this->file.write(reinterpret_cast<const char*>(buffer), sizeof(uint8_t) * length);
	return true;
}

bool BinaryFileWriter::writeByteBufferUnsigned(const uint8_t* buffer, std::size_t& length) {
	if ( this->file.is_open() == false ) {
		std::cout << "[BinaryFileWriter:write] Error: Binary file is not open." << std::endl;
		return false;
	}

	this->file.write(reinterpret_cast<const char*>(buffer), sizeof(uint8_t) * length);
	return true;
}

bool BinaryFileWriter::writeBits8(int8_t value) {
	if ( this->file.is_open() == false ) {
		std::cout << "[BinaryFileWritter:write] Error: Binary file is not open." << std::endl;
		return false;
	}

	this->file.write(reinterpret_cast<char*>(&value), sizeof(int8_t));
	return true;
}

bool BinaryFileWriter::writeBits16(int16_t value) {
	if ( this->file.is_open() == false ) {
		std::cout << "[BinaryFileWritter:write] Error: Binary file is not open." << std::endl;
		return false;
	}

	this->file.write(reinterpret_cast<char*>(&value), sizeof(uint16_t));
	return true;
}

bool BinaryFileWriter::writeBits32(int32_t value) {
	if ( this->file.is_open() == false ) {
		std::cout << "[BinaryFileWritter:write] Error: Binary file is not open." << std::endl;
		return false;
	}

	this->file.write(reinterpret_cast<char*>(&value), sizeof(int32_t));
	return true;
}

bool BinaryFileWriter::writeBits64(int64_t value) {
	if ( this->file.is_open() == false ) {
		std::cout << "[BinaryFileWritter:write] Error: Binary file is not open." << std::endl;
		return false;
	}

	this->file.write(reinterpret_cast<char*>(&value), sizeof(int64_t));
	return true;
}

bool BinaryFileWriter::writeUnsignedBits8(uint8_t value) {
	if ( this->file.is_open() == false ) {
		std::cout << "[BinaryFileWritter:write] Error: Binary file is not open." << std::endl;
		return false;
	}

	this->file.write(reinterpret_cast<char*>(&value), sizeof(uint8_t));
	return true;
}

bool BinaryFileWriter::writeUnsignedBits16(uint16_t value) {
	if ( this->file.is_open() == false ) {
		std::cout << "[BinaryFileWritter:write] Error: Binary file is not open." << std::endl;
		return false;
	}

	this->file.write(reinterpret_cast<char*>(&value), sizeof(uint16_t));
	return true;
}

bool BinaryFileWriter::writeUnsignedBits32(uint32_t value) {
	if ( this->file.is_open() == false ) {
		std::cout << "[BinaryFileWritter:write] Error: Binary file is not open." << std::endl;
		return false;
	}

	this->file.write(reinterpret_cast<char*>(&value), sizeof(uint32_t));
	return true;
}

bool BinaryFileWriter::writeUnsignedBits64(uint64_t value) {
	if ( this->file.is_open() == false ) {
		std::cout << "[BinaryFileWritter:write] Error: Binary file is not open." << std::endl;
		return false;
	}

	this->file.write(reinterpret_cast<char*>(&value), sizeof(uint64_t));
	return true;
}

bool BinaryFileWriter::writeShort(short value) {
	if ( this->file.is_open() == false ) {
		std::cout << "[BinaryFileWritter:write] Error: Binary file is not open." << std::endl;
		return false;
	}

	this->file.write(reinterpret_cast<char*>(&value), sizeof(short));
	return true;
}

bool BinaryFileWriter::writeShortBuffer(short* buffer, std::size_t length) {
	if ( this->file.is_open() == false ) {
		std::cout << "[BinaryFileWriter:write] Error: Binary file is not open." << std::endl;
		return false;
	}

	this->file.write(reinterpret_cast<const char*>(buffer), sizeof(short) * length);
	return true;
}

bool BinaryFileWriter::writeShortBuffer(const short* buffer, std::size_t length) {
	if ( this->file.is_open() == false ) {
		std::cout << "[BinaryFileWriter:write] Error: Binary file is not open." << std::endl;
		return false;
	}

	this->file.write(reinterpret_cast<const char*>(buffer), sizeof(short) * length);
	return true;
}

bool BinaryFileWriter::writeShortUnsigned(unsigned short value) {
	if ( this->file.is_open() == false ) {
		std::cout << "[BinaryFileWritter:write] Error: Binary file is not open." << std::endl;
		return false;
	}

	this->file.write(reinterpret_cast<char*>(&value), sizeof(unsigned short));
	return true;
}

bool BinaryFileWriter::writeShortBufferUnsigned(unsigned short* const buffer, std::size_t length) {
	if ( this->file.is_open() == false ) {
		std::cout << "[BinaryFileWriter:write] Error: Binary file is not open." << std::endl;
		return false;
	}

	this->file.write(reinterpret_cast<const char*>(buffer), sizeof(unsigned short) * length);
	return true;
}

bool BinaryFileWriter::writeInt(int value) {
	if ( this->file.is_open() == false ) {
		std::cout << "[BinaryFileWritter:writeInt] Error: Binary file is not open." << std::endl;
		return false;
	}

	this->file.write(reinterpret_cast<char*>(&value), sizeof(int));
	return true;
}

bool BinaryFileWriter::writeInt32(int32_t value) {
	if ( this->file.is_open() == false ) {
		std::cout << "[BinaryFileWritter:writeInt32] Error: Binary file is not open." << std::endl;
		return false;
	}

	this->file.write(reinterpret_cast<char*>(&value), sizeof(int32_t));
	return true;
}

bool BinaryFileWriter::writeInt64(int64_t value) {
	if ( this->file.is_open() == false ) {
		std::cout << "[BinaryFileWritter:writeInt64] Error: Binary file is not open." << std::endl;
		return false;
	}

	this->file.write(reinterpret_cast<char*>(&value), sizeof(int64_t));
	return true;
}

bool BinaryFileWriter::writeUInt32(uint32_t value) {
	if ( this->file.is_open() == false ) {
		std::cout << "[BinaryFileWritter:writeUInt32] Error: Binary file is not open." << std::endl;
		return false;
	}

	this->file.write(reinterpret_cast<char*>(&value), sizeof(uint32_t));
	return true;
}

bool BinaryFileWriter::writeUInt64(uint64_t value) {
	if ( this->file.is_open() == false ) {
		std::cout << "[BinaryFileWritter:writeUInt64] Error: Binary file is not open." << std::endl;
		return false;
	}

	this->file.write(reinterpret_cast<char*>(&value), sizeof(uint64_t));
	return true;
}

bool BinaryFileWriter::wirteIntUnsigned(unsigned int value) {
	if ( this->file.is_open() == false ) {
		std::cout << "[BinaryFileWritter:write] Error: Binary file is not open." << std::endl;
		return false;
	}

	this->file.write(reinterpret_cast<char*>(&value), sizeof(unsigned int));
	return true;
}

bool BinaryFileWriter::writeIntBuffer(int* buffer, std::size_t length) {
	if ( this->file.is_open() == false ) {
		std::cout << "[BinaryFileWriter:write] Error: Binary file is not open." << std::endl;
		return false;
	}

	this->file.write(reinterpret_cast<const char*>(buffer), sizeof(int) * length);
	return true;
}

bool BinaryFileWriter::writeIntBuffer(const int* buffer, std::size_t length) {
	if ( this->file.is_open() == false ) {
		std::cout << "[BinaryFileWriter:write] Error: Binary file is not open." << std::endl;
		return false;
	}

	this->file.write(reinterpret_cast<const char*>(buffer), sizeof(int) * length);
	return true;
}

bool BinaryFileWriter::writeIntBufferUnsigned(const unsigned int* buffer, std::size_t length) {
	if ( this->file.is_open() == false ) {
		std::cout << "[BinaryFileWriter:write] Error: Binary file is not open." << std::endl;
		return false;
	}

	this->file.write(reinterpret_cast<const char*>(buffer), sizeof(unsigned int) * length);
	return true;
}

bool BinaryFileWriter::writeLong(long value) {
	if ( this->file.is_open() == false ) {
		std::cout << "[BinaryFileWritter:write] Error: Binary file is not open." << std::endl;
		return false;
	}

	this->file.write(reinterpret_cast<char*>(&value), sizeof(long));
	return true;
}

bool BinaryFileWriter::writeLong(unsigned long value) {
	if ( this->file.is_open() == false ) {
		std::cout << "[BinaryFileWritter:write] Error: Binary file is not open." << std::endl;
		return false;
	}

	this->file.write(reinterpret_cast<char*>(&value), sizeof(unsigned long));
	return true;
}

bool BinaryFileWriter::writeLongBuffer(long* buffer, std::size_t length) {
	if ( this->file.is_open() == false ) {
		std::cout << "[BinaryFileWriter:write] Error: Binary file is not open." << std::endl;
		return false;
	}

	this->file.write(reinterpret_cast<const char*>(buffer), sizeof(long) * length);
	return true;
}

bool BinaryFileWriter::writeLongBuffer(const long* buffer, std::size_t length) {
	if ( this->file.is_open() == false ) {
		std::cout << "[BinaryFileWriter:write] Error: Binary file is not open." << std::endl;
		return false;
	}

	this->file.write(reinterpret_cast<const char*>(buffer), sizeof(long) * length);
	return true;
}

bool BinaryFileWriter::writeLongBufferUnsigned(const unsigned long* buffer, std::size_t length) {
	if ( this->file.is_open() == false ) {
		std::cout << "[BinaryFileWriter:write] Error: Binary file is not open." << std::endl;
		return false;
	}

	this->file.write(reinterpret_cast<const char*>(buffer), sizeof(unsigned long) * length);
	return true;
}

bool BinaryFileWriter::writeFloat(float value) {
	if ( this->file.is_open() == false ) {
		std::cout << "[BinaryFileWriter:write] Error: Binary file is not open." << std::endl;
		return false;
	}

	this->file.write(reinterpret_cast<char*>(&value), sizeof(float));
	return true;
}

bool BinaryFileWriter::writeFloatBuffer(float* buffer, std::size_t length) {
	if ( this->file.is_open() == false ) {
		std::cout << "[BinaryFileWriter:write] Error: Binary file is not open." << std::endl;
		return false;
	}

	this->file.write(reinterpret_cast<const char*>(buffer), sizeof(float) * length);
	return true;
}

bool BinaryFileWriter::writeFloatBuffer(const float* buffer, std::size_t length) {
	if ( this->file.is_open() == false ) {
		std::cout << "[BinaryFileWriter:write] Error: Binary file is not open." << std::endl;
		return false;
	}

	this->file.write(reinterpret_cast<const char*>(buffer), sizeof(float) * length);
	return true;
}

bool BinaryFileWriter::writeDouble(double value) {
	if ( this->file.is_open() == false ) {
		std::cout << "[BinaryFileWriter:write] Error: Binary file is not open." << std::endl;
		return false;
	}

	this->file.write(reinterpret_cast<char*>(&value), sizeof(double));
	return true;
}

bool BinaryFileWriter::writeDouble(double* buffer, std::size_t length) {
	if ( this->file.is_open() == false ) {
		std::cout << "[BinaryFileWriter:write] Error: Binary file is not open." << std::endl;
		return false;
	}

	this->file.write(reinterpret_cast<const char*>(buffer), sizeof(double) * length);
	return true;
}

bool BinaryFileWriter::writeDouble(const double* buffer, std::size_t length) {
	if ( this->file.is_open() == false ) {
		std::cout << "[BinaryFileWriter:write] Error: Binary file is not open." << std::endl;
		return false;
	}

	this->file.write(reinterpret_cast<const char*>(buffer), sizeof(double) * length);
	return true;
}

bool BinaryFileWriter::writeString(const char* string) {
	if ( this->file.is_open() == false ) {
		std::cout << "[BinaryFileWriter:write] Error: Binary file is not open." << std::endl;
		return false;
	}

	this->file.write(reinterpret_cast<const char*>(string), sizeof(char) * std::strlen(string));
	this->write(NULL_CHAR);
	return true;
}

bool BinaryFileWriter::writeString(const std::string& string) {
	if ( this->file.is_open() == false ) {
		std::cout << "[BinaryFileWriter:write] Error: Binary file is not open." << std::endl;
		return false;
	}

	this->file.write(reinterpret_cast<const char*>(string.c_str()), sizeof(char) * string.length());
	this->write(NULL_CHAR);
	return true;
}

std::ofstream& BinaryFileWriter::getStream() {
	return this->file;
}

const std::ofstream& BinaryFileWriter::getStream() const {
	return this->file;
}

bool BinaryFileWriter::close() {
	if ( this->file.is_open() == false ) {
		std::cout << "[BinaryFileWriter:write] Error: Binary file is not open." << std::endl;
		return false;
	}

	this->file.close();
	return true;
}

}

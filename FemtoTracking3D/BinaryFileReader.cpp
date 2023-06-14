#include "BinaryFileReader.h"
#include <iostream>

namespace px {

static char NULL_CHAR = '\0';

BinaryFileReader::BinaryFileReader() {}

BinaryFileReader::BinaryFileReader(const BinaryFileReader&) {}

BinaryFileReader& BinaryFileReader::operator = (const BinaryFileReader& b) { return *this; }

BinaryFileReader::~BinaryFileReader() {
    if ( this->file.is_open() == true ) this->file.close();
}

bool BinaryFileReader::open(const std::string& filename) {
	if ( this->file.is_open() == true ) this->file.close();
	this->file.open(filename.c_str(), std::ios::in | std::ios::binary);
	if ( this->file.is_open() == true ) return true;
	else {
		std::cout << "Goodbit: " << this->file.goodbit << std::endl;
		std::cout << "Eofbit : " << this->file.eofbit << std::endl;
		std::cout << "Failbit: " << this->file.failbit << std::endl;
		std::cout << "RDState: " << this->file.rdstate() << std::endl;
		return false;
	}
}

bool BinaryFileReader::isOpen() const {
    if ( this->file.is_open() == true ) return true;
    return false;
}

bool BinaryFileReader::eof() const {
	if ( this->file.is_open() == false ) return false;
	if ( this->file.eof() ) return true;
	return false;
}

bool BinaryFileReader::hasNext() {
	if ( this->file.is_open() == false ) return false;
	if ( this->file.peek() == std::ifstream::traits_type::eof() ) return false;
	return true;
}

bool BinaryFileReader::readByte(uint8_t& value) {
	if ( this->file.is_open() == false ) {
		std::cout << "[BinaryFileReader:readByte] Error: Binary file is not open." << std::endl;
		return false;
	}

	this->file.read(reinterpret_cast<char*>(&value), sizeof(uint8_t));
	return true;
}

bool BinaryFileReader::readBits8(int8_t& value) {
	if ( this->file.is_open() == false ) {
		std::cout << "[BinaryFileReader:readBits8] Error: Binary file is not open." << std::endl;
		return false;
	}

	this->file.read(reinterpret_cast<char*>(&value), sizeof(int8_t));
	return true;
}

bool BinaryFileReader::readBits16(int16_t& value) {
	if ( this->file.is_open() == false ) {
		std::cout << "[BinaryFileReader:readBits16] Error: Binary file is not open." << std::endl;
		return false;
	}

	this->file.read(reinterpret_cast<char*>(&value), sizeof(int16_t));
	return true;
}

bool BinaryFileReader::readBits32(int32_t& value) {
	if ( this->file.is_open() == false ) {
		std::cout << "[BinaryFileReader:readBits32] Error: Binary file is not open." << std::endl;
		return false;
	}

	this->file.read(reinterpret_cast<char*>(&value), sizeof(int32_t));
	return true;
}

bool BinaryFileReader::readBits64(int64_t& value) {
	if ( this->file.is_open() == false ) {
		std::cout << "[BinaryFileReader:readBits64] Error: Binary file is not open." << std::endl;
		return false;
	}

	this->file.read(reinterpret_cast<char*>(&value), sizeof(int64_t));
	return true;
}

bool BinaryFileReader::readUnsignedByte(uint8_t& value) {
	if ( this->file.is_open() == false ) {
		std::cout << "[BinaryFileReader:readBits32] Error: Binary file is not open." << std::endl;
		return false;
	}

	this->file.read(reinterpret_cast<char*>(&value), sizeof(uint8_t));
	return true;
}

bool BinaryFileReader::readUnsignedBits8(uint8_t& value) {
	if ( this->file.is_open() == false ) {
		std::cout << "[BinaryFileReader:readBits32] Error: Binary file is not open." << std::endl;
		return false;
	}

	this->file.read(reinterpret_cast<char*>(&value), sizeof(uint8_t));
	return true;
}

bool BinaryFileReader::readUnsignedBits16(uint16_t& value) {
	if ( this->file.is_open() == false ) {
		std::cout << "[BinaryFileReader:readBits32] Error: Binary file is not open." << std::endl;
		return false;
	}

	this->file.read(reinterpret_cast<char*>(&value), sizeof(uint16_t));
	return true;
}

bool BinaryFileReader::readUnsignedBits32(uint32_t& value) {
	if ( this->file.is_open() == false ) {
		std::cout << "[BinaryFileReader:readBits32] Error: Binary file is not open." << std::endl;
		return false;
	}

	this->file.read(reinterpret_cast<char*>(&value), sizeof(uint32_t));
	return true;
}

bool BinaryFileReader::readUnsignedBits64(uint64_t& value) {
	if ( this->file.is_open() == false ) {
		std::cout << "[BinaryFileReader:readBits32] Error: Binary file is not open." << std::endl;
		return false;
	}

	this->file.read(reinterpret_cast<char*>(&value), sizeof(uint64_t));
	return true;
}

bool BinaryFileReader::readChar(char& value) {
	if ( this->file.is_open() == false ) {
		std::cout << "[BinaryFileReader:read] Error: Binary file is not open." << std::endl;
		return false;
	}

	this->file.read(&value, sizeof(char));
	return true;
}

bool BinaryFileReader::readCharBuffer(char* buffer, std::size_t length) {
	if ( this->file.is_open() == false ) {
		std::cout << "[BinaryFileReader:read] Error: Binary file is not open." << std::endl;
		return false;
	}

	this->file.read(buffer, length);
	return true;
}

bool BinaryFileReader::readShort(short& value) {
	if ( this->file.is_open() == false ) {
		std::cout << "[BinaryFileReader:read] Error: Binary file is not open." << std::endl;
		return false;
	}

	this->file.read(reinterpret_cast<char*>(&value), sizeof(short));
	return true;
}

bool BinaryFileReader::readShortBuffer(short* buffer, std::size_t length) {
	if ( this->file.is_open() == false ) {
		std::cout << "[BinaryFileReader:read] Error: Binary file is not open." << std::endl;
		return false;
	}

	this->file.read(reinterpret_cast<char*>(buffer), sizeof(short) * length);
	return true;
}

bool BinaryFileReader::readShortUnsigned(unsigned short& value) {
	if ( this->file.is_open() == false ) {
		std::cout << "[BinaryFileReader:read] Error: Binary file is not open." << std::endl;
		return false;
	}

	this->file.read(reinterpret_cast<char*>(&value), sizeof(unsigned short));
	return true;
}

bool BinaryFileReader::readShortBufferUnsigned(unsigned short* buffer, std::size_t length) {
	if ( this->file.is_open() == false ) {
		std::cout << "[BinaryFileReader:read] Error: Binary file is not open." << std::endl;
		return false;
	}

	this->file.read(reinterpret_cast<char*>(buffer), sizeof(unsigned short) * length);
	return true;
}

bool BinaryFileReader::readInt(int& value) {
	if ( this->file.is_open() == false ) {
		std::cout << "[BinaryFileReader:read] Error: Binary file is not open." << std::endl;
		return false;
	}

	this->file.read(reinterpret_cast<char*>(&value), sizeof(int));
	return true;
}

bool BinaryFileReader::readIntBuffer(int* buffer, std::size_t length) {
	if ( this->file.is_open() == false ) {
		std::cout << "[BinaryFileReader:read] Error: Binary file is not open." << std::endl;
		return false;
	}

	this->file.read(reinterpret_cast<char*>(buffer), sizeof(int) * length);
	return true;
}

bool BinaryFileReader::readLong(long& value) {
	if ( this->file.is_open() == false ) {
		std::cout << "[BinaryFileReader:read] Error: Binary file is not open." << std::endl;
		return false;
	}

	this->file.read(reinterpret_cast<char*>(&value), sizeof(long));
	return true;
}

bool BinaryFileReader::readLongBuffer(long* buffer, std::size_t length) {
	if ( this->file.is_open() == false ) {
		std::cout << "[BinaryFileReader:read] Error: Binary file is not open." << std::endl;
		return false;
	}

	this->file.read(reinterpret_cast<char*>(buffer), sizeof(long) * length);
	return true;
}

bool BinaryFileReader::readFloat(float& value) {
	if ( this->file.is_open() == false ) {
		std::cout << "[BinaryFileReader:read] Error: Binary file is not open." << std::endl;
		return false;
	}

	this->file.read(reinterpret_cast<char*>(&value), sizeof(float));
	return true;
}

bool BinaryFileReader::readFloatBuffer(float* buffer, std::size_t length) {
	if ( this->file.is_open() == false ) {
		std::cout << "[BinaryFileReader:read] Error: Binary file is not open." << std::endl;
		return false;
	}

	this->file.read(reinterpret_cast<char*>(buffer), sizeof(float) * length);
	return true;
}

bool BinaryFileReader::readDouble(double& value) {
	if ( this->file.is_open() == false ) {
		std::cout << "[BinaryFileReader:read] Error: Binary file is not open." << std::endl;
		return false;
	}

	this->file.read(reinterpret_cast<char*>(&value), sizeof(double));
	return true;
}

bool BinaryFileReader::readDoubleBuffer(double* buffer, std::size_t length) {
	if ( this->file.is_open() == false ) {
		std::cout << "[BinaryFileReader:read] Error: Binary file is not open." << std::endl;
		return false;
	}

	this->file.read(reinterpret_cast<char*>(buffer), sizeof(double) * length);
	return true;
}

bool BinaryFileReader::readString(std::string& string) {
	char currentChar;
	std::string temp;
	do {
		this->read(currentChar);
		if ( currentChar != NULL_CHAR ) temp.push_back(currentChar);
	}
	while ( currentChar != NULL_CHAR );
	string = temp;
	return true;
}

bool BinaryFileReader::readString(unsigned int length, std::string& string) {
	if ( length == 0 ) return false;
	char* buffer = new char[length];
	this->file.read(reinterpret_cast<char*>(buffer), sizeof(uint8_t) * length);
	string = std::string(buffer);
	delete [] buffer;
	this->read(NULL_CHAR);
	return true;
}

char BinaryFileReader::readChar() {
	char value = NULL_CHAR;
	this->readChar(value);
	return value;
}

short BinaryFileReader::readShort() {
	short value = 0;
	this->readShort(value);
	return value;
}

int BinaryFileReader::readInt() {
	int value;
	this->file.read(reinterpret_cast<char*>(&value), sizeof(int));
	return value;
}

int32_t BinaryFileReader::readInt32() {
	int32_t value;
	this->file.read(reinterpret_cast<char*>(&value), sizeof(int32_t));
	return value;
}

int64_t BinaryFileReader::readInt64() {
	int64_t value;
	this->file.read(reinterpret_cast<char*>(&value), sizeof(int64_t));
	return value;
}

uint32_t BinaryFileReader::readUInt32() {
	uint32_t value;
	this->file.read(reinterpret_cast<char*>(&value), sizeof(uint32_t));
	return value;
}

uint64_t BinaryFileReader::readUInt64() {
	uint64_t value;
	this->file.read(reinterpret_cast<char*>(&value), sizeof(uint64_t));
	return value;
}

long BinaryFileReader::readLong() {
	long value;
	this->file.read(reinterpret_cast<char*>(&value), sizeof(long));
	return value;
}

float BinaryFileReader::readFloat() {
    float value;
	this->file.read(reinterpret_cast<char*>(&value), sizeof(float));
	return value;
}

double BinaryFileReader::readDouble() {
	double value;
	this->file.read(reinterpret_cast<char*>(&value), sizeof(double));
	return value;
}

std::ifstream& BinaryFileReader::getStream() {
	return this->file;
}

const std::ifstream& BinaryFileReader::getStream() const {
	return this->file;
}

bool BinaryFileReader::close() {
	if ( this->file.is_open() == false ) {
		std::cout << "[BinaryFileReader:read] Error: Binary file is not open." << std::endl;
		return false;
	}

	this->file.close();
	return true;
}

bool BinaryFileReader::IsEmpty(const std::string& filename) {
	if ( filename.length() == 0 ) return true;
	std::ifstream in;
	in.open(filename.c_str());
	if ( in.is_open() == false ) return true;
	bool empty = true;
	if ( in.peek() == std::ifstream::traits_type::eof() ) empty = true;
	else empty = false;
	in.close();
	return empty;
}

}

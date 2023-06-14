
#ifndef PX_BINARY_FILE_READER_H
#define PX_BINARY_FILE_READER_H

#include <string>
#include <fstream>

namespace px {

class BinaryFileReader {
public:
	BinaryFileReader();
	~BinaryFileReader();

	bool open(const std::string& filename);
	bool isOpen() const;
	bool eof() const;
	bool hasNext();

	template <typename T>
	bool read(T& value);

	template <typename T>
	bool readBuffer(T* buffer, std::size_t length);

	bool readByte(uint8_t& value);
	bool readBits8(int8_t& value);
	bool readBits16(int16_t& value);
	bool readBits32(int32_t& value);
	bool readBits64(int64_t& value);

	bool readUnsignedByte(uint8_t& value);
	bool readUnsignedBits8(uint8_t& value);
	bool readUnsignedBits16(uint16_t& value);
	bool readUnsignedBits32(uint32_t& value);
	bool readUnsignedBits64(uint64_t& value);

	bool readChar(char& value);
	bool readCharBuffer(char* buffer, std::size_t length);
	bool readShort(short& value);
	bool readShortBuffer(short* buffer, std::size_t length);
	bool readShortUnsigned(unsigned short& value);
	bool readShortBufferUnsigned(unsigned short* buffer, std::size_t length);
	bool readInt(int& value);
	bool readIntBuffer(int* buffer, std::size_t length);
	bool readLong(long& value);
	bool readLongBuffer(long* buffer, std::size_t length);
	bool readFloat(float& value);
	bool readFloatBuffer(float* buffer, std::size_t length);
	bool readDouble(double& value);
	bool readDoubleBuffer(double* buffer, std::size_t length);
	bool readString(std::string& string);
	bool readString(unsigned int length, std::string& string);

	char readChar();
	short readShort();
	int readInt();
	int32_t readInt32();
	int64_t readInt64();
	uint32_t readUInt32();
	uint64_t readUInt64();
	long readLong();
	float readFloat();
	double readDouble();

	std::ifstream& getStream();
	const std::ifstream& getStream() const;

	bool close();

	static bool IsEmpty(const std::string& filename);

private:
	BinaryFileReader(const BinaryFileReader&);
	BinaryFileReader& operator = (const BinaryFileReader&);

	std::ifstream file;
};

template <typename T>
bool BinaryFileReader::read(T& value) {
	this->file.read(reinterpret_cast<char*>(&value), sizeof(T));
	return true;
}

template <typename T>
bool BinaryFileReader::readBuffer(T* buffer, std::size_t length) {
	this->file.read(reinterpret_cast<char*>(buffer), sizeof(T) * length);
	return true;
}

}

#endif


#ifndef PX_BINARY_FILE_WRITER_H
#define PX_BINARY_FILE_WRITER_H

#include <string>
#include <fstream>
#include <cstdint>

namespace px {

class BinaryFileWriter {
public:
	BinaryFileWriter();
	~BinaryFileWriter();

	bool open(const std::string& filename, bool append = false);
	bool isOpen() const;
	bool isEmpty();

	template <typename T>
	bool write(T value);

	template <typename T>
	bool writeBuffer(T* buffer, std::size_t length);
	template <typename T>
	bool writeBuffer(const T* buffer, std::size_t length);

	bool writeSizeT(std::size_t value);
	bool writeChar(char value);
	bool writeCharBufferNull(char* buffer, std::size_t length, bool appendNull = true);
	bool writeCharBuffer(const char* buffer, std::size_t length);

	bool writeByte(int8_t value);
	bool writeByteUnsigned(uint8_t value);
	bool writeByteBuffer(int8_t* buffer, std::size_t& length);
	bool writeByteBuffer(const int8_t* buffer, std::size_t& length);
	bool writeByteBufferUnsigned(uint8_t* buffer, std::size_t& length);
	bool writeByteBufferUnsigned(const uint8_t* buffer, std::size_t& length);

	bool writeBits8(int8_t value);
	bool writeBits16(int16_t value);
	bool writeBits32(int32_t value);
	bool writeBits64(int64_t value);
	bool writeUnsignedBits8(uint8_t value);
	bool writeUnsignedBits16(uint16_t value);
	bool writeUnsignedBits32(uint32_t value);
	bool writeUnsignedBits64(uint64_t value);

	bool writeShort(short value);
	bool writeShortBuffer(short* buffer, std::size_t length);
	bool writeShortBuffer(const short* buffer, std::size_t length);
	bool writeShortUnsigned(unsigned short value);
	bool writeShortBufferUnsigned(unsigned short* const buffer, std::size_t length);

	bool writeInt(int value);
	bool writeInt32(int32_t value);
	bool writeInt64(int64_t value);
	bool writeUInt32(uint32_t value);
	bool writeUInt64(uint64_t value);
    bool wirteIntUnsigned(unsigned int value);
	bool writeIntBuffer(int* buffer, std::size_t length);
	bool writeIntBuffer(const int* buffer, std::size_t length);
    bool writeIntBufferUnsigned(const unsigned int* buffer, std::size_t length);

	bool writeLong(long value);
	bool writeLong(unsigned long value);
	bool writeLongBuffer(long* buffer, std::size_t length);
	bool writeLongBuffer(const long* buffer, std::size_t length);
	bool writeLongBufferUnsigned(const unsigned long* buffer, std::size_t length);

	bool writeFloat(float value);
	bool writeFloatBuffer(float* buffer, std::size_t length);
	bool writeFloatBuffer(const float* buffer, std::size_t length);

	bool writeDouble(double value);
	bool writeDouble(double* buffer, std::size_t length);
	bool writeDouble(const double* buffer, std::size_t length);

	bool writeString(const char* string);
	bool writeString(const std::string& string);

	std::ofstream& getStream();
	const std::ofstream& getStream() const;

	bool close();

protected:
	BinaryFileWriter(const BinaryFileWriter&);
	BinaryFileWriter& operator = (const BinaryFileWriter&);

	std::ofstream file;
};

template <typename T>
bool BinaryFileWriter::write(T value) {
	this->file.write(reinterpret_cast<char*>(&value), sizeof(T));
	return true;
}

template <typename T>
bool BinaryFileWriter::writeBuffer(T* buffer, std::size_t length) {
	this->file.write(reinterpret_cast<char*>(buffer), sizeof(T) * length);
	return true;
}

template <typename T>
bool BinaryFileWriter::writeBuffer(const T* buffer, std::size_t length) {
	this->file.write(reinterpret_cast<const char*>(buffer), sizeof(T) * length);
	return true;
}

}

#endif

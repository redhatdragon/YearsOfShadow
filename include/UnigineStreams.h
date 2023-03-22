/* Copyright (C) 2005-2022, UNIGINE. All rights reserved.
 *
 * This file is a part of the UNIGINE 2 SDK.
 *
 * Your use and / or redistribution of this software in source and / or
 * binary form, with or without modification, is subject to: (i) your
 * ongoing acceptance of and compliance with the terms and conditions of
 * the UNIGINE License Agreement; and (ii) your inclusion of this notice
 * in any version of this software that you use or redistribute.
 * A copy of the UNIGINE License Agreement is available by contacting
 * UNIGINE. at http://unigine.com/
 */

// DO NOT EDIT DIRECTLY. This is an auto-generated file. Your changes will be lost.

#pragma once

#include "UniginePtr.h"
#include "UnigineString.h"
#include "UnigineMathLib.h"
#include "UniginePalette.h"

namespace Unigine
{

// byte order
enum {
	STREAM_LSB = 0,
	STREAM_MSB,
};


class UNIGINE_API Stream : public APIInterface
{
public:
	static bool convertible(Stream *obj) { return obj != nullptr; }

	enum TYPE
	{
		BLOB = 0,
		FILE,
		SOCKET,
		USER,	// User stream inherited from StreamBase
		NUM_STREAMS,
	};
	Stream::TYPE getType() const;
	size_t read(void *ptr, size_t size);
	size_t write(const void *ptr, size_t size);
	size_t readStream(const Ptr<Stream> &dest, size_t size);
	size_t writeStream(const Ptr<Stream> &src, size_t size);
	bool isOpened() const;
	bool isAvailable() const;
	void setByteOrder(int order);
	int getByteOrder() const;
	char readChar();
	bool writeChar(char value);
	unsigned char readUChar();
	bool writeUChar(unsigned char value);
	bool readBool();
	bool writeBool(bool value);
	int readInt2();
	bool writeInt2(int value);
	unsigned int readUInt2();
	bool writeUInt2(unsigned int value);
	short readShort();
	bool writeShort(short value);
	unsigned short readUShort();
	bool writeUShort(unsigned short value);
	int readInt();
	bool writeInt(int value);
	unsigned int readUInt();
	bool writeUInt(unsigned int value);
	long long readLong();
	bool writeLong(long long value);
	float readFloat();
	bool writeFloat(float value);
	double readDouble();
	bool writeDouble(double value);
	Math::vec2 readVec2();
	bool writeVec2(const Math::vec2 &value);
	Math::vec3 readVec3();
	bool writeVec3(const Math::vec3 &value);
	Math::vec4 readVec4();
	bool writeVec4(const Math::vec4 &value);
	Math::dvec2 readDVec2();
	bool writeDVec2(const Math::dvec2 &value);
	Math::dvec3 readDVec3();
	bool writeDVec3(const Math::dvec3 &value);
	Math::dvec4 readDVec4();
	bool writeDVec4(const Math::dvec4 &value);
	Math::ivec2 readIVec2();
	bool writeIVec2(const Math::ivec2 &value);
	Math::ivec3 readIVec3();
	bool writeIVec3(const Math::ivec3 &value);
	Math::ivec4 readIVec4();
	bool writeIVec4(const Math::ivec4 &value);
	Math::mat4 readMat4();
	bool writeMat4(const Math::mat4 &value);
	Math::mat4 readMat44();
	bool writeMat44(const Math::mat4 &value);
	Math::dmat4 readDMat4();
	bool writeDMat4(const Math::dmat4 &value);
	Math::quat readQuat();
	bool writeQuat(const Math::quat &value);
	Palette readPalette();
	int writePalette(const Palette &value);
	String readString();
	int readString(char *str, int str_size);
	bool writeString(const char *str);
	String readString2();
	int readString2(char *str, int str_size);
	bool writeString2(const char *str);
	String readToken();
	int readToken(char *str, int size);
	String readLine();
	int readLine(char *str, int str_size);
	int readShortArray(short *dest, int dest_size);
	bool writeShortArray(const short *src, int src_size);
	int readUShortArray(unsigned short *dest, int dest_size);
	bool writeUShortArray(const unsigned short *src, int src_size);
	int readIntArray(int *dest, int dest_size);
	bool writeIntArray(const int *src, int src_size);
	int readUIntArray(unsigned int *dest, int dest_size);
	bool writeUIntArray(const unsigned int *src, int src_size);
	int readLongArray(long long *dest, int dest_size);
	bool writeLongArray(const long long *src, int src_size);
	int readFloatArray(float *dest, int dest_size);
	bool writeFloatArray(const float *src, int src_size);
	int readDoubleArray(double *dest, int dest_size);
	bool writeDoubleArray(const double *src, int src_size);
	String gets();
	int puts(const char *str);
	bool isError() const;
	void read(unsigned char & value) const;
	bool write(unsigned char value) const;
	void read(char &value) const;
	bool write(char value) const;
	void read(short &value) const;
	bool write(short value) const;
	void read(unsigned short &value) const;
	bool write(unsigned short value) const;
	void read(bool &value) const;
	bool write(bool value) const;
	void read(int &value) const;
	bool write(int value) const;
	void read(unsigned int &value) const;
	bool write(unsigned int value) const;
	void read(long long &value) const;
	bool write(long long value) const;
	void read(float &value) const;
	bool write(float value) const;
	void read(double &value) const;
	bool write(double value) const;
	bool write(const char *value) const;
	void read(Math::vec2 &value) const;
	bool write(const Math::vec2 &value) const;
	void read(Math::vec3 &value) const;
	bool write(const Math::vec3 &value) const;
	void read(Math::vec4 &value) const;
	bool write(const Math::vec4 &value) const;
	void read(Math::dvec2 &value) const;
	bool write(const Math::dvec2 &value) const;
	void read(Math::dvec3 &value) const;
	bool write(const Math::dvec3 &value) const;
	void read(Math::dvec4 &value) const;
	bool write(const Math::dvec4 &value) const;
	void read(Math::ivec2 &value) const;
	bool write(const Math::ivec2 &value) const;
	void read(Math::ivec3 &value) const;
	bool write(const Math::ivec3 &value) const;
	void read(Math::ivec4 &value) const;
	bool write(const Math::ivec4 &value) const;
	void read(Math::mat4 &value) const;
	bool write(const Math::mat4 &value) const;
	void read(Math::dmat4 &value) const;
	bool write(const Math::dmat4 &value) const;
	void read(Math::quat &value) const;
	bool write(const Math::quat &value) const;
};
typedef Ptr<Stream> StreamPtr;

/// Unigine Base Stream class for creating user stream types
class UNIGINE_API StreamBase
{
	public:
		StreamBase();
		virtual ~StreamBase();
		Ptr<Stream> getStream() const;
		virtual int isOpened() = 0;
		virtual int isAvailable() = 0;
		virtual size_t read(void *ptr, size_t size, size_t nmemb) = 0;
		virtual size_t write(const void *ptr, size_t size, size_t nmemb) = 0;
	private:
		void *stream_wrapper;
};


class UNIGINE_API File : public Stream
{
public:
	static bool convertible(Stream *obj) { return obj && obj->getType() == Stream::FILE; }
	static Ptr<File> create();
	static Ptr<File> create(const char *name, const char *mode);
	bool open(const char *name, const char *mode) const;
	bool close() const;
	const char *getName() const;
	size_t getSize() const;
	int eof() const;
	int getc() const;
	int seekSet(size_t offset) const;
	int seekEnd(size_t offset) const;
	int seekCur(size_t offset) const;
	size_t tell() const;
	int flush() const;
	size_t readFile(void *ptr, size_t size, size_t nmemb) const;
	size_t writeFile(void *ptr, size_t size, size_t nmemb) const;
};
typedef Ptr<File> FilePtr;


class UNIGINE_API Blob : public Stream
{
public:
	static bool convertible(Stream *obj) { return obj && obj->getType() == Stream::BLOB; }
	static Ptr<Blob> create(size_t size = 0);
	void resize(size_t size) const;
	void allocate(size_t size) const;
	void reserve(size_t size) const;
	void clear() const;
	size_t getSize() const;
	unsigned char * getData() const;
	void setData(unsigned char *data, size_t size);
	int eof() const;
	int getc() const;
	int seekSet(size_t offset) const;
	int seekEnd(size_t offset) const;
	int seekCur(size_t offset) const;
	size_t tell() const;
	int flush() const;
	int decode(const char *src) const;
	String encode() const;
	int decodeZBase32(const char *src) const;
	String encodeZBase32() const;
	int compress(const Ptr<Stream> &dest, int quality) const;
	int decompress(const Ptr<Stream> &src) const;
	int compressLz4(const Ptr<Stream> &dest, int quality) const;
	int decompressLz4(const Ptr<Stream> &src) const;
	String getSHA1() const;
	String getSHA256() const;
	String getMD5() const;
	String getCRC32() const;
	unsigned char get(size_t offset) const;
	void set(size_t offset, unsigned char value) const;
};
typedef Ptr<Blob> BlobPtr;


class UNIGINE_API Socket : public Stream
{
public:
	static bool convertible(Stream *obj) { return obj && obj->getType() == Stream::SOCKET; }
	static Ptr<Socket> create(int type);
	static Ptr<Socket> create(int type, int port);
	static Ptr<Socket> create(int type, const char *host, int port);
	int open(int port) const;
	int open(const char *host, int port) const;
	int close() const;
	int send(int size) const;
	int recv(int size) const;
	int bind() const;
	int listen(int num) const;
	int listenMulticastGroup() const;
	int accept(const Ptr<Socket> &socket) const;
	int connect() const;
	int broadcast() const;
	int block() const;
	int nonblock() const;
	int nodelay() const;
	const char *getHost() const;
	int getPort() const;
	int getFD() const;
	int isReadyToRead(int timeout_usec = 0) const;
	int isReadyToWrite(int timeout_usec = 0) const;

	enum
	{
		SOCKET_STREAM = 0,	// Stream socket
		SOCKET_DGRAM,	// Datagram socket
	};
};
typedef Ptr<Socket> SocketPtr;

} // namespace Unigine

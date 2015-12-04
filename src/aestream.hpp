////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

#ifndef AESTREAM_HPP_INCLUDE_GUARD
#define AESTREAM_HPP_INCLUDE_GUARD 1

////////////////////////////////////////////////////////////////////////////////

#include "aeexcept.hpp"

////////////////////////////////////////////////////////////////////////////////

#include <cinttypes>

////////////////////////////////////////////////////////////////////////////////

AE_EXCEPTION(aeException, aeStreamError, "stream I/O error");

////////////////////////////////////////////////////////////////////////////////

class aeInputStream {
protected:
    aeInputStream() {}

public:
    virtual ~aeInputStream() {}

    template <typename T>
    void read(T &data) {
        if (read(&data, sizeof(T)) < sizeof(T)) {
            throw aeStreamError("incomplete read");
        }
    }

    virtual int64_t read(void *data, int64_t size) = 0;
    virtual int64_t seek(int64_t position) = 0;
    virtual int64_t tell() = 0;
    virtual int64_t length() = 0;
};

////////////////////////////////////////////////////////////////////////////////

class aeOutputStream {
protected:
    aeOutputStream() {}

public:
    virtual ~aeOutputStream() {}

    template <typename T>
    void write(const T &data) {
        if (write(&data, sizeof(T)) < sizeof(T)) {
            throw aeStreamError("incomplete write");
        }
    }

    virtual int64_t write(const void *data, int64_t size) = 0;
    virtual int64_t seek(int64_t position) = 0;
    virtual int64_t tell() = 0;
    virtual int64_t length() = 0;
};

////////////////////////////////////////////////////////////////////////////////

class aeFileInputStream : public aeInputStream {
public:
    aeFileInputStream(const std::string &filename);
    virtual ~aeFileInputStream();

    void close();

    virtual int64_t read(void *data, int64_t size);
    virtual int64_t seek(int64_t position);
    virtual int64_t tell();
    virtual int64_t length();

private:
    struct PHYSFS_File *mFile;
};

////////////////////////////////////////////////////////////////////////////////

class aeFileOutputStream : public aeOutputStream {
public:
    aeFileOutputStream(const std::string &filename);
    virtual ~aeFileOutputStream();

    void close();

    virtual int64_t write(const void *data, int64_t size);
    virtual int64_t seek(int64_t position);
    virtual int64_t tell();
    virtual int64_t length();

private:
    struct PHYSFS_File *mFile;
};

////////////////////////////////////////////////////////////////////////////////

class aeMemoryInputStream {
public:
    aeMemoryInputStream(const void *data, int64_t size);
    virtual ~aeMemoryInputStream();

    virtual int64_t read(void *data, int64_t size);
    virtual int64_t seek(int64_t position);
    virtual int64_t tell();
    virtual int64_t length();

private:
    const void *mData;
    int64_t mSize;
    int64_t mPosition;
};

////////////////////////////////////////////////////////////////////////////////

class aeMemoryOutputStream {
public:
    aeMemoryOutputStream(void *data, int64_t size);
    virtual ~aeMemoryOutputStream();

    virtual int64_t write(const void *data, int64_t size);
    virtual int64_t seek(int64_t position);
    virtual int64_t tell();
    virtual int64_t length();

private:
    void *mData;
    int64_t mSize;
    int64_t mPosition;
};

////////////////////////////////////////////////////////////////////////////////

#endif // AESTREAM_HPP_INCLUDE_GUARD

////////////////////////////////////////////////////////////////////////////////
//  EOF
////////////////////////////////////////////////////////////////////////////////


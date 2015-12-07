////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

#include "aestream.hpp"

////////////////////////////////////////////////////////////////////////////////

#include <physfs.h>

////////////////////////////////////////////////////////////////////////////////

class aePhysFS {
public:
    static void init(const char *argv0 = nullptr);

private:
    aePhysFS(const char *argv0);
    ~aePhysFS();
};

void aePhysFS::init(const char *argv0) {
    static aePhysFS instance(argv0);
}

aePhysFS::aePhysFS(const char *argv0) {
    if (!PHYSFS_init(argv0)) {
        throw aeStreamError(std::string("error initializing filesystem: ") + PHYSFS_getLastError());
    }

    const char *baseDir = PHYSFS_getBaseDir();

    if (!PHYSFS_mount(baseDir, "/", 0)) {
        throw aeStreamError(std::string("error mounting filesystem: ") + PHYSFS_getLastError());
    }
}

aePhysFS::~aePhysFS() {
    PHYSFS_deinit();
}

////////////////////////////////////////////////////////////////////////////////

aeFileInputStream::aeFileInputStream(const std::string &filename): mFile() {
    aePhysFS::init();
    mFile = PHYSFS_openRead(filename.c_str());
    if (!mFile) {
        throw aeStreamError(std::string("error opening file for reading: ") + PHYSFS_getLastError() + " (\"" + filename + "\")");
    }
}

aeFileInputStream::~aeFileInputStream() {
    close();
}

void aeFileInputStream::close() {
    if (mFile && !PHYSFS_close(mFile)) {
        throw aeStreamError(std::string("error closing file: ") + PHYSFS_getLastError());
    }
    mFile = nullptr;
}

int64_t aeFileInputStream::read(void *data, int64_t size) {
    return PHYSFS_readBytes(mFile, data, size);
}

int64_t aeFileInputStream::seek(int64_t position) {
    return PHYSFS_seek(mFile, position);
}

int64_t aeFileInputStream::tell() {
    return PHYSFS_tell(mFile);
}

int64_t aeFileInputStream::length() {
    return PHYSFS_fileLength(mFile);
}

////////////////////////////////////////////////////////////////////////////////

aeFileOutputStream::aeFileOutputStream(const std::string &filename): mFile() {
    aePhysFS::init();
    mFile = PHYSFS_openWrite(filename.c_str());
    if (!mFile) {
        throw aeStreamError(std::string("error opening file for writing: ") + PHYSFS_getLastError() + " (\"" + filename + "\")");
    }
}

aeFileOutputStream::~aeFileOutputStream() {
    close();
}

void aeFileOutputStream::close() {
    if (mFile && !PHYSFS_close(mFile)) {
        throw aeStreamError(std::string("error closing file: ") + PHYSFS_getLastError());
    }
    mFile = nullptr;
}

int64_t aeFileOutputStream::write(const void *data, int64_t size) {
    return PHYSFS_writeBytes(mFile, data, size);
}

int64_t aeFileOutputStream::seek(int64_t position) {
    return PHYSFS_seek(mFile, position);
}

int64_t aeFileOutputStream::tell() {
    return PHYSFS_tell(mFile);
}

int64_t aeFileOutputStream::length() {
    return PHYSFS_fileLength(mFile);
}

////////////////////////////////////////////////////////////////////////////////

aeMemoryInputStream::aeMemoryInputStream(
    const void *data, int64_t size
): mData(data), mSize(size), mPosition() {
}

aeMemoryInputStream::~aeMemoryInputStream() {
}

int64_t aeMemoryInputStream::read(void *data, int64_t size) {
    if (size > (mSize - mPosition)) {
        size = mSize - mPosition;
    }
    const uint8_t *tData = static_cast<const uint8_t*>(mData);
    std::copy(tData, tData + size, static_cast<uint8_t*>(data));
    return mPosition += size;
}

int64_t aeMemoryInputStream::seek(int64_t position) {
    if (position < 0) {
        position = 0;
    } else if (position > mSize) {
        position = mSize;
    }
    return mPosition = position;
}

int64_t aeMemoryInputStream::tell() {
    return mPosition;
}

int64_t aeMemoryInputStream::length() {
    return mSize;
}

////////////////////////////////////////////////////////////////////////////////

aeMemoryOutputStream::aeMemoryOutputStream(
    void *data, int64_t size
): mData(data), mSize(size), mPosition() {
}

aeMemoryOutputStream::~aeMemoryOutputStream() {
}

int64_t aeMemoryOutputStream::write(const void *data, int64_t size) {
    if (size > (mSize - mPosition)) {
        size = mSize - mPosition;
    }
    const uint8_t *tData = static_cast<const uint8_t*>(data);
    std::copy(tData, tData + size, static_cast<uint8_t*>(mData));
    return mPosition += size;
}

int64_t aeMemoryOutputStream::seek(int64_t position) {
    if (position < 0) {
        position = 0;
    } else if (position > mSize) {
        position = mSize;
    }
    return mPosition = position;
}

int64_t aeMemoryOutputStream::tell() {
    return mPosition;
}

int64_t aeMemoryOutputStream::length() {
    return mSize;
}

////////////////////////////////////////////////////////////////////////////////
//  EOF
////////////////////////////////////////////////////////////////////////////////


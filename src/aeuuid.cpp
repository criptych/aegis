////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

#include "aeuuid.hpp"

////////////////////////////////////////////////////////////////////////////////

#include <chrono>
#include <cstring>
#include <random>

////////////////////////////////////////////////////////////////////////////////

// All this for the C++11 equivalent of `srand(time(NULL))` ...
static uint64_t _seed(std::chrono::duration_cast<std::chrono::nanoseconds>(
    std::chrono::high_resolution_clock::now().time_since_epoch()
).count());
static std::mt19937 _gen(_seed);
static std::uniform_int_distribution<uint8_t> _dist;

aeUuid::aeUuid(Version version) {
    switch (version) {
        default:
        case Version::Random: {

            for (uint8_t &d : data) {
                d = _dist(_gen);
            }

            break;
        }

        case Version::MAC:
            //!@todo implement
            break;

        case Version::DCE:
            //!@todo implement
            break;

        case Version::MD5:
            //!@todo implement
            break;

        case Version::SHA1:
            //!@todo implement
            break;
    }

    data[6] = (data[6] & 0x0f) | ((version << 4) & 0xf0);
    data[8] = (data[8] & 0x3f) | (0x80);
}

aeUuid::aeUuid(
    uint32_t data1, uint16_t data2, uint16_t data3, uint8_t data4[8]
) {
    data[ 0] = (data1 >> 24) & 0xff;
    data[ 1] = (data1 >> 16) & 0xff;
    data[ 2] = (data1 >>  8) & 0xff;
    data[ 3] = (data1 >>  0) & 0xff;
    data[ 4] = (data2 >>  8) & 0xff;
    data[ 5] = (data2 >>  0) & 0xff;
    data[ 6] = (data3 >>  8) & 0xff;
    data[ 7] = (data3 >>  0) & 0xff;
    data[ 8] = data4[0];
    data[ 9] = data4[1];
    data[10] = data4[2];
    data[11] = data4[3];
    data[12] = data4[4];
    data[13] = data4[5];
    data[14] = data4[6];
    data[15] = data4[7];
}

aeUuid::operator std::string() const {
    char uuid[37];
    snprintf(
        uuid, sizeof(uuid),
        "%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x",
        data[ 0], data[ 1], data[ 2], data[ 3],
        data[ 4], data[ 5], data[ 6], data[ 7],
        data[ 8], data[ 9], data[10], data[11],
        data[12], data[13], data[14], data[15]
    );
    return uuid;
}

aeUuid::Version aeUuid::version() const {
    return static_cast<aeUuid::Version>((data[6] >> 4) & 0xf);
}

////////////////////////////////////////////////////////////////////////////////

bool operator == (const aeUuid &lhs, const aeUuid &rhs) {
    return 0 == std::memcmp(lhs.data, rhs.data, sizeof(lhs.data));
}

bool operator != (const aeUuid &lhs, const aeUuid &rhs) {
    return !(lhs == rhs);
}

////////////////////////////////////////////////////////////////////////////////
//  EOF
////////////////////////////////////////////////////////////////////////////////

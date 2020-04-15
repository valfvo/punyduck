#include <cstring>
#include "LQRawData.hpp"

static int num = 1;
static bool hostIsLittleEndian = (*(char *)&num == 1);

template<class TData>
TData LQRawData::parse() {
    LQsize dataSize = sizeof(TData);
    TData data;

    if (m_offset + dataSize <= m_size) {
        memcpy(&data, &m_data[m_offset], dataSize);
        m_offset += dataSize;
    }
    else {
        return TData();
    }

    if (hostIsLittleEndian) {
        data = __builtin_bswap32(data);
    }

    return data;
}

template<>
char* LQRawData::parse<char*>();

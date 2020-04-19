#include <cstring>
#include <cstdint>
#include "LQRawData.hpp"

template<class TData>
TData LQRawData::basicParse() {
    LQsize dataSize = sizeof(TData);
    TData data;

    if (m_offset + dataSize <= m_size) {
        memcpy(&data, &m_data[m_offset], dataSize);
        m_offset += dataSize;
    }
    else {
        return TData();
    }

    return data;
}

template<class TData>
TData LQRawData::parse() {
    return basicParse<TData>();
}

template<>
int16_t LQRawData::parse<int16_t>();

template<>
int32_t LQRawData::parse<int32_t>();

template<>
char* LQRawData::parse<char*>();

template<>
LQImageData LQRawData::parse<LQImageData>();

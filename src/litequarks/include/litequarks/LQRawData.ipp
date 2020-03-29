#include <cstring>
#include "LQRawData.hpp"

template<class TData>
TData LQRawData::parse() {
    LQsize dataSize = sizeof(TData);
    TData data;

    if (m_offset + dataSize <= m_size) {
        memcpy(&data, &m_data[m_offset], dataSize);
        m_offset += dataSize;
    }

    return data;
}

template<>
char* LQRawData::parse<char*>();

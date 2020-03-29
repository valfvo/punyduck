#include <litequarks/LQRawData.hpp>

#include <iostream>

LQRawData::LQRawData(char* data, LQsize size)
: m_data(data), m_size(size), m_offset(0)
{ }

template<>
char* LQRawData::parse<char*>() {
    auto dataSize = strlen(&m_data[m_offset]) + 1;  // +1: \0
    char* data = new char[dataSize];

    memcpy(data, &m_data[m_offset], dataSize);
    m_offset += dataSize;

    return data;
}

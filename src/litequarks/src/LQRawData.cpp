#include <litequarks/LQRawData.hpp>

#include <cstdint>
#include <algorithm>  // std::min, std::max

LQRawData::LQRawData(char* data)
: LQRawData(data, *(int*)data)
{
    // seek(sizeof(LQsize));
    seek(sizeof(int));
}

LQRawData::LQRawData(char* data, LQsize size)
: m_data(data), m_size(size), m_offset(0)
{ }

void LQRawData::seek(LQsize offset, int whence) {
    switch (whence) {
    case 0:
        m_offset = std::min(std::max(offset, LQsize(0)), m_size - 1);
        break;
    case 1:
        m_offset = std::min(std::max(m_offset + offset, LQsize(0)), m_size - 1);
        break;
    case 2:
        m_offset = std::min(std::max(m_size - 1 - offset, LQsize(0)), m_size - 1);
        break;
    default:
        break;
    }
}

template<>
char* LQRawData::parse<char*>() {
    auto dataSize = strlen(&m_data[m_offset]) + 1;  // +1: \0
    char* data = new char[dataSize];

    memcpy(data, &m_data[m_offset], dataSize);
    m_offset += dataSize;

    return data;
}

template<>
LQImageData LQRawData::parse<LQImageData>() {
    LQImageData image{
        parse<int32_t>(),
        parse<int32_t>(),
        parse<int16_t>(),
        nullptr
    };

    int dataSize = parse<int32_t>();
    auto pixels = new unsigned char[dataSize];

    memcpy(pixels, &m_data[m_offset], dataSize);
    m_offset += dataSize;

    image.pixels = pixels;

    return image;
}

#pragma once

#include "LQDeclaration.hpp"

class LQRawData {
public:
    LQRawData(char* data, LQsize size);

    template<class TData>
    TData parse();

private:
    char*  m_data;
    LQsize m_size;
    LQsize m_offset;
};

#include "LQRawData.ipp"

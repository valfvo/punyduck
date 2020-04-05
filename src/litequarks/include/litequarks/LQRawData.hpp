#pragma once

#include "LQDeclaration.hpp"

class LQRawData {
public:
    LQRawData(char* data);
    LQRawData(char* data, LQsize size);

    void seek(LQsize offset, int whence=1);

    // template<class TData>
    // void seek(int whence=1);

    template<class TData>
    TData parse();

private:
    char*  m_data;
    LQsize m_size;
    LQsize m_offset;
};

#include "LQRawData.ipp"

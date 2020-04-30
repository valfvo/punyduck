#pragma once

#include "LQViewable.hpp"
#include "LQEvent.hpp"

class LQDrop : public LQViewable {
public:
    LQDrop(LQNumber&& x, LQNumber&& y, LQNumber&& width, LQNumber&& height,
           GLint color);

    void onDrop(LQDropEvent& event);

    std::string getPath() const;

protected:
    std::string m_path;
};
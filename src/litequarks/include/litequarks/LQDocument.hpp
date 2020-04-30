#pragma once

#include "LQViewable.hpp"

class LQDocument : public LQViewable {
public:
    LQDocument(GLint color);

    void resizeHeightCallback() override;
};

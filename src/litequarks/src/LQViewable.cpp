#include <litequarks/LQViewable.hpp>

LQViewable::LQViewable()
: LQSurface()//, <-----------
//   m_left(0.0f, this, LQNumber::Kind::coords, moveToX), m_top(LQMetricKind_Coords),
//   m_w(LQMetricKind_Length), m_h(LQMetricKind_Length)
{
    // linkToLengths();
}

LQViewable::LQViewable(LQNumber&& x, LQNumber&& y,
                       LQNumber&& width, LQNumber&& height, GLint color)
: LQSurface(std::move(x), std::move(y), std::move(width), std::move(height), color)//,
//   m_left(this, LQMetricKind_Coords), m_top(this, LQMetricKind_Coords),
//   m_w(LQMetricKind_Length), m_h(LQMetricKind_Length)
{
    // linkToLengths();
}

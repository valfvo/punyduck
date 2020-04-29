#include <cstdlib>  // std::abs
#include <algorithm>  // std::max, std::min
#include <litequarks/LQAppController.hpp>
#include <litequarks/LQViewport.hpp>

using namespace LQUnit;

LQViewport::LQViewport(
    LQNumber&& _x, LQNumber&& _y, LQNumber&& _width, LQNumber&& _height,
    GLint color, float minGripSize)
: LQViewable(std::move(_x), std::move(_y),
             std::move(_width), std::move(_height), color),
  m_minGripSize(minGripSize)
{
    // m_width.linkQuark<LQViewport, recalc>(*this);
    m_height.linkQuark<LQViewport, recalc>(*this);
    lqOn<LQScrollEvent>(this, onScroll);
}

void LQViewport::onScroll(LQScrollEvent& event) {
    if (m_firstChild != m_lastChild) {
        auto* document = static_cast<LQViewable*>(firstChild());
        auto* track = static_cast<LQViewable*>(lastChild());
        auto* grip = static_cast<LQViewable*>(track->firstChild());

        float viewportScrollAreaSize = document->heightF() - heightF();
        document->y() = std::min(std::max(
            document->yF() + event.yoffset * 15_px, -viewportScrollAreaSize), 0.0f);

        float viewportPositionRatio = std::abs(document->yF()) / viewportScrollAreaSize;
        float trackScrollAreaSize = track->heightF() - grip->heightF();
        grip->y() = trackScrollAreaSize * viewportPositionRatio;
    }
}

void LQViewport::recalc() {
    resizeCallback();
    // std::cout << "le crash arrive !!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
    // std::cout << "viewport: " << &m_width << ' ' << &m_height << std::endl;
    auto* old_track = firstChild()->nextSibling();
    if (old_track) {  // there is a track & grip
        removeLastChild();
        // std::cout << "track: " << &static_cast<LQViewable*>(old_track)->m_x     
                //   << ' ' << &static_cast<LQViewable*>(old_track)->m_height << std::endl;
        delete old_track->firstChild();
        delete old_track;
    }
    // std::cout << "fin reset" << std::endl;
    auto* document = static_cast<LQViewable*>(firstChild());
    if (document->heightF() > heightF()) {  // scrollbar needed
        float trackSize = heightF();
        float viewportDocumentRatio = heightF() / document->heightF();
        float gripSize = std::max(trackSize * viewportDocumentRatio, m_minGripSize);

        float viewportScrollAreaSize = document->heightF() - heightF();
        float viewportPositionRatio = std::abs(document->yF()) / viewportScrollAreaSize;

        float trackScrollAreaSize = trackSize - gripSize;
        float gripPositionOnTrack = trackScrollAreaSize * viewportPositionRatio;

        appendChild(new LQViewable(width()-20_px, 0_px, 20_px, height()));
        lastChild()->
            appendChild(new LQViewable(0_px, gripPositionOnTrack, 20_px, gripSize, 0xff0000));
    }
    // std::cout << "attention v" << std::endl;
    // std::cout << "ah bah non ?!" << std::endl;
}

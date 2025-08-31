#include <other.h>

ConvexShape others::make_round_rect(const Vector2f size, float r, const int segs) {
    ConvexShape c;

    r = std::max(0.f, std::min(r, std::min(size.x, size.y) * 0.5f));
    const float w = size.x, h = size.y;
    const int n = 4 * segs;
    c.setPointCount(n);

    auto put_arc = [&](const int corner_id, const Vector2f center, const float start_deg) {
        for (int i=0; i<segs; i++) {
            const float t = (segs == 1) ? 0.f : static_cast<float>(i) / static_cast<float>(segs - 1);
            const float deg = start_deg + t * 90.f;
            const float rad = deg * 3.1415926535f / 180.f;
            const Vector2f p = center + Vector2f(std::cos(rad), std::sin(rad)) * r;
            c.setPoint(corner_id * segs + i, p);
        }
    };

    const Vector2f tl(r, r);
    const Vector2f tr(w-r, r);
    const Vector2f br(w-r, h-r);
    const Vector2f bl(r, h-r);

    put_arc(0, tl, 180.f);
    put_arc(1, tr, 270.f);
    put_arc(2, br,   0.f);
    put_arc(3, bl,  90.f);

    return c;
}
#ifndef OTHER_H
#define OTHER_H
#pragma once
#define NOMINMAX
#include <windows.h>
#include <shobjidl.h>
#include <shlobj.h>
#include <wrl/client.h>
#include <optional>
#include <SFML/Graphics.hpp>

using namespace sf;

namespace functions {
    ConvexShape make_round_rect(const Vector2f, float, const int);
    void reset_view_to_window();
    std::optional<std::wstring> browse_folder(const HWND parent);
}


#endif //OTHER_H

#pragma once

#include "ftxui/dom/node.hpp"

#include <cstdint>

namespace ftxui {

Element image_view(std::string_view url);
void setOnImageLoadedCallback(std::function<void()> cb);

void setImageCacheMaxSize(uint16_t size);
void setImageResizeCacheMaxSize(uint16_t size);
void setImageCharCacheMaxSize(uint16_t size);

}

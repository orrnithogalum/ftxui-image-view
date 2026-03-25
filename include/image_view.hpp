#pragma once

#include "ftxui/dom/node.hpp"

namespace ftxui {

Element image_view(std::string_view url);
void free_image_cache();

}

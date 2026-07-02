//
// Created by alexk on 25.06.2026.
//

#ifndef SAE_ALEXK_CITYBUILDER_API_INCLUDE_UTILS_UTILS_H_
#define SAE_ALEXK_CITYBUILDER_API_INCLUDE_UTILS_UTILS_H_

#include "bit"
#include "SFML/System/Vector2.hpp"

namespace api::utils {

struct Vec2iHash {
  size_t operator()(const sf::Vector2i node) const noexcept{
    return std::bit_cast<size_t>(node);
  }
};

}

#endif //SAE_ALEXK_CITYBUILDER_API_INCLUDE_UTILS_UTILS_H_

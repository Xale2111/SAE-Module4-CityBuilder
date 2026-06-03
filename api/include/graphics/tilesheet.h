//
// Created by alexk on 13.05.2026.
//

#ifndef CITYBUILDER_TILESHEET_H
#define CITYBUILDER_TILESHEET_H

#include <SFML/Graphics/Rect.hpp>
#include <unordered_map>
#include <SFML/Graphics/Texture.hpp>

namespace graphics {

template<typename T>
class TileSheet {
 public:
  [[nodiscard]] bool InitTileSheet(std::string_view path, int tile_step) {
    tile_step_ = tile_step;
    return texture.loadFromFile(path);
  }

  void AddTile(T type, int xIdx, int yIdx) {
    tile_rects[type] = ConstructRect(xIdx, yIdx, tile_step_, tile_step_);
  }

  [[nodiscard]] sf::FloatRect GetBounds(T type) {
    if (tile_rects.contains(type)) {
      return tile_rects.at(type);
    }
    return {};
  }

  [[nodiscard]] sf::Texture *GetTexture() { return &texture; };

 private:
  std::unordered_map<T, sf::FloatRect> tile_rects;
  sf::Texture texture;
  int tile_step_ = 0;

  [[nodiscard]] static sf::FloatRect ConstructRect(float xIdx, float yIdx, float width, float height) {
    return {{width * xIdx, yIdx * height}, {width, height}};
  }

};

}

#endif //CITYBUILDER_TILESHEET_H

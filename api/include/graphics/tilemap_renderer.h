//
// Created by alexk on 13.05.2026.
//

#ifndef CITYBUILDER_TILEMAP_RENDERER_H
#define CITYBUILDER_TILEMAP_RENDERER_H

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/VertexArray.hpp>

namespace graphics {

struct Tile{
  sf::Vector2f position;
  bool is_walkable = true;
};

class TilemapRenderer {
 public:
  void AddTile(sf::Vector2f pos, sf::Vector2f offset, sf::FloatRect texBounds);
  void Draw(sf::RenderWindow &window) const;
  void SetTexture(sf::Texture *texture);
  void Clear();

 private:
  const sf::Color color = sf::Color::White;

  sf::Vector2f gridSize_{128,128};
  sf::Vector2f gridOffset_{24,24};
  sf::Texture *texture_ = nullptr;
  sf::VertexArray vertices_ = sf::VertexArray(sf::PrimitiveType::Triangles);

};
}

#endif  // CITYBUILDER_TILEMAP_RENDERER_H

//
// Created by alexk on 13.05.2026.
//

#ifndef CITYBUILDER_TILEMAP_RENDERER_H
#define CITYBUILDER_TILEMAP_RENDERER_H

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/VertexArray.hpp>

namespace graphics {
class TilemapRenderer {

 private:
  const sf::Color color = sf::Color::White;

  sf::Vector2f gridSize_;
  sf::Vector2f gridOffset_;
  sf::Texture *texture_ = nullptr;
  sf::VertexArray vertices_ = sf::VertexArray(sf::PrimitiveType::Triangles);

 public:
  void AddTile(sf::Vector2f pos, sf::Vector2f offset, sf::FloatRect texBounds);
  void Draw(sf::RenderWindow &window) const;
  void SetTexture(sf::Texture *texture);
  void Clear();
};
}

#endif  // CITYBUILDER_TILEMAP_RENDERER_H

//
// Created by alexk on 13.05.2026.
//

#include "graphics/tilemap_renderer.h"

namespace graphics {
void TilemapRenderer::AddTile(sf::Vector2f pos, sf::Vector2f offset, sf::FloatRect texBounds) {

  tile_vertex_index_[EncodePosition(pos)] = static_cast<int>(vertices_.getVertexCount());

  vertices_.append(sf::Vertex(pos, color, texBounds.position));
  vertices_.append(sf::Vertex(pos + sf::Vector2f(offset.x, 0.f),
                              color,
                              texBounds.position + sf::Vector2f(texBounds.size.x, 0.f))); // + X
  vertices_.append(sf::Vertex(pos + offset, color, texBounds.position + texBounds.size));

  vertices_.append(sf::Vertex(pos, color, texBounds.position));
  vertices_.append(sf::Vertex(pos + offset, color, texBounds.position + texBounds.size));;
  vertices_.append(sf::Vertex(pos + sf::Vector2f(0.f, offset.y),
                              color,
                              texBounds.position + sf::Vector2f(0.f, texBounds.size.y))); // + Y
}

bool TilemapRenderer::UpdateTileTexture(sf::Vector2f pos, sf::FloatRect newTexBounds) {
  auto it = tile_vertex_index_.find(EncodePosition(pos));
  if (it == tile_vertex_index_.end()) return false;

  int idx = it->second;
  sf::Vector2f offset = {
      vertices_[idx + 1].texCoords.x - vertices_[idx].texCoords.x,
      vertices_[idx + 5].texCoords.y - vertices_[idx].texCoords.y
  };

  // Met à jour les 6 vertices
  vertices_[idx + 0].texCoords = newTexBounds.position;
  vertices_[idx + 1].texCoords = newTexBounds.position + sf::Vector2f(newTexBounds.size.x, 0.f);
  vertices_[idx + 2].texCoords = newTexBounds.position + newTexBounds.size;
  vertices_[idx + 3].texCoords = newTexBounds.position;
  vertices_[idx + 4].texCoords = newTexBounds.position + newTexBounds.size;
  vertices_[idx + 5].texCoords = newTexBounds.position + sf::Vector2f(0.f, newTexBounds.size.y);

  return true;
}

void TilemapRenderer::Draw(sf::RenderWindow &window) const {
  sf::RenderStates states;
  if (texture_) {
    states.texture = texture_;
  }
  if (vertices_.getVertexCount() > 0) {
    window.draw(vertices_, states);
  }
}

void TilemapRenderer::SetTexture(sf::Texture *texture) {
  texture_ = texture;
}

void TilemapRenderer::Clear() {
  vertices_.clear();
}

} // namespace graphics

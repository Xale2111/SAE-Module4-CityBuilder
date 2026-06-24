//
// Created by alexk on 17.06.2026.
//
#include "ai/a_start_graph.h"
#include <algorithm>
#include "data_utils.h"
#include <tracy/Tracy.hpp>

namespace api::ai {
void AStarGraph::AddNode(sf::Vector2i node) {
  graph_.emplace(node);
}

void AStarGraph::RemoveNode(const sf::Vector2i node) {
  graph_.erase(node);
}

bool AStarGraph::ContainsNode(const sf::Vector2i node) const {
  return graph_.contains(node);
}

std::vector<sf::Vector2i> AStarGraph::GetPath(sf::Vector2i start, sf::Vector2i end, std::span<sf::Vector2i> walkable) {
  ZoneScopedNC("AStarGraph::GetPath", tracy::Color::Green);
  std::vector<sf::Vector2i> path;

  std::vector<uint8_t> walkable_tiles(DataUtils::kTilemapWidth * DataUtils::kTilemapHeight);
  std::fill(walkable_tiles.begin(), walkable_tiles.end(), false);

  for (const auto &pixel_pos : walkable) {
    int tx = pixel_pos.x / DataUtils::kTileSize;
    int ty = pixel_pos.y / DataUtils::kTileSize;
    if (tx >= 0 && tx < DataUtils::kTilemapWidth && ty >= 0 && ty < DataUtils::kTilemapHeight) {
      walkable_tiles[CalculateIndex(tx,ty)] = true;
    }
  }

  std::vector<uint8_t> visited_tiles(DataUtils::kTilemapWidth * DataUtils::kTilemapHeight);
  std::fill(visited_tiles.begin(), visited_tiles.end(), false);


  int start_tx = start.x / DataUtils::kTileSize;
  int start_ty = start.y / DataUtils::kTileSize;
  int end_tx = end.x / DataUtils::kTileSize;
  int end_ty = end.y / DataUtils::kTileSize;

  if (start_tx < 0 || start_tx >= DataUtils::kTilemapWidth || start_ty < 0 || start_ty >= DataUtils::kTilemapHeight ||
      end_tx < 0 || end_tx >= DataUtils::kTilemapWidth || end_ty < 0 || end_ty >= DataUtils::kTilemapHeight) {
    return path;
  }

  if (!walkable_tiles[CalculateIndex(start_tx,start_ty)] || !walkable_tiles[CalculateIndex(end_tx,end_ty)]) {
    return path;
  }

  std::priority_queue<AStarVertex, std::vector<AStarVertex>, std::greater<>> open_queue;
  std::vector<AStarVertex> visited_vertices;

  open_queue.push(AStarVertex(start, 0, ManhattanDistance(start, end), -1));

  while (!open_queue.empty()) {
    AStarVertex node = open_queue.top();
    open_queue.pop();

    int node_tx = node.position.x / DataUtils::kTileSize;
    int node_ty = node.position.y / DataUtils::kTileSize;

    if (visited_tiles[CalculateIndex(node_tx,node_ty)]) continue;

    visited_tiles[CalculateIndex(node_tx,node_ty)] = true;
    visited_vertices.push_back(node);
    int visited_idx = visited_vertices.size() - 1;

    if (node.position == end) {
      auto pathVertex = node;
      while (pathVertex.parent_idx > -1) {
        path.push_back(pathVertex.position);
        pathVertex = visited_vertices[pathVertex.parent_idx];
      }
      std::ranges::reverse(path);
      return path;
    }

    for (sf::Vector2i neighbour : kNeighbours) {
      sf::Vector2i new_position = node.position + (neighbour * DataUtils::kTileSize);
      int n_tx = new_position.x / DataUtils::kTileSize;
      int n_ty = new_position.y / DataUtils::kTileSize;

      if (n_tx < 0 || n_tx >= DataUtils::kTilemapWidth || n_ty < 0 || n_ty >= DataUtils::kTilemapHeight) {
        continue;
      }

      if (!visited_tiles[CalculateIndex(n_tx,n_ty)] && walkable_tiles[CalculateIndex(n_tx,n_ty)]) {
        open_queue.push(AStarVertex(new_position,
                                    node.g + DataUtils::kTileSize,
                                    ManhattanDistance(new_position, end),
                                    visited_idx));
      }
    }
  }

  return path;
}
int AStarGraph::CalculateIndex(int x, int y) const {
  return y * DataUtils::kTilemapWidth + x;
}

int ManhattanDistance(sf::Vector2i a, sf::Vector2i b) {

  return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}
}
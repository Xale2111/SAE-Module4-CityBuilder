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

std::vector<sf::Vector2i> AStarGraph::GetPath(sf::Vector2i start, sf::Vector2i end, std::span<sf::Vector2i> walkable, std::vector<uint8_t>& cache_walkables, std::vector<uint8_t>& cache_visited) {
  ZoneScopedNC("AStarGraph::GetPath", tracy::Color::Green);
  std::vector<sf::Vector2i> path;

  std::fill(cache_walkables.begin(), cache_walkables.end(), false);

  for (const auto &pixel_pos : walkable) {
    int tx = pixel_pos.x / DataUtils::kTileSize;
    int ty = pixel_pos.y / DataUtils::kTileSize;
    if (tx >= 0 && tx < DataUtils::kTilemapWidth && ty >= 0 && ty < DataUtils::kTilemapHeight) {
      cache_walkables[CalculateIndex(tx,ty)] = true;
    }
  }

  std::fill(cache_visited.begin(), cache_visited.end(), false);


  int start_tx = start.x / DataUtils::kTileSize;
  int start_ty = start.y / DataUtils::kTileSize;
  int end_tx = end.x / DataUtils::kTileSize;
  int end_ty = end.y / DataUtils::kTileSize;

  if (start_tx < 0 || start_tx >= DataUtils::kTilemapWidth || start_ty < 0 || start_ty >= DataUtils::kTilemapHeight ||
      end_tx < 0 || end_tx >= DataUtils::kTilemapWidth || end_ty < 0 || end_ty >= DataUtils::kTilemapHeight) {
    return path;
  }

  if (!cache_walkables[CalculateIndex(start_tx,start_ty)]) {
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

    if (cache_visited[CalculateIndex(node_tx,node_ty)]) continue;

    cache_visited[CalculateIndex(node_tx,node_ty)] = true;
    visited_vertices.push_back(node);
    int visited_idx = visited_vertices.size() - 1;

    //TODO : FIX issue -> if the end is house, needs to walk on the house before hiding npc (so depending on what's the end, wither end or end-1)
    if (node.position == end) {
      if (node.parent_idx == -1) {
        return path;
      }
      int current_parent = node.parent_idx;

      while (current_parent >= 0 && current_parent < static_cast<int>(visited_vertices.size())) {
        path.push_back(visited_vertices[current_parent].position);
        current_parent = visited_vertices[current_parent].parent_idx;
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

      //TODO : FIX issue -> I removed the check for walkable tiles, so npc can walk on non-walkables (which can cause issue like starting on a non-walkable tile afterwards).
      // I need to check if walkable or not but if it's the resource destination, it will be non-walkable. (shitty idea, change walkable to true of the resource ?)
      if (!cache_visited[CalculateIndex(n_tx,n_ty)]) {
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
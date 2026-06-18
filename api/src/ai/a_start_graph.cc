//
// Created by alexk on 17.06.2026.
//
#include "ai/a_start_graph.h"
#include <algorithm>
#include "data_utils.h"

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

//TODO:give walkable tiles
std::vector<sf::Vector2i> AStarGraph::GetPath(sf::Vector2i start, sf::Vector2i end, std::span<sf::Vector2i> walkable) {
  std::vector<sf::Vector2i> path;
  std::priority_queue<AStarVertex, std::vector<AStarVertex>, std::greater<>> open_queue;
  std::vector<AStarVertex> visited_vertices;

  //TODO:check if start and end are on the map

  open_queue.push(AStarVertex(start, 0, ManhattanDistance(start, end),-1));


  while (!open_queue.empty()) {

    AStarVertex node = open_queue.top();
    open_queue.pop();
    visited_vertices.push_back(node);
    int visited_idx = visited_vertices.size() - 1;


    if (node.position == end) {
      // Reconstruct the path
      auto pathVertex = node;

      while(pathVertex.parent_idx > -1)
      {
        path.push_back(pathVertex.position);
        pathVertex = visited_vertices[pathVertex.parent_idx];
      }

      std::ranges::reverse(path);

      return path;
    }

    for (sf::Vector2i neighbour : kNeighbours) {


      sf::Vector2i new_position = node.position + (neighbour*DataUtils::kTileSize);

      auto visited = std::ranges::find_if(visited_vertices, [&](AStarVertex v) { return v.position == new_position; });

      if(visited == visited_vertices.end() && std::ranges::find_if(walkable, [&](sf::Vector2i v) { return v == new_position; }) != walkable.end())
      {
        open_queue.push(AStarVertex(new_position, node.g+1 ,ManhattanDistance(new_position, end),visited_idx));
      }

    }

  }

  path.push_back(start);
  path.push_back(end);

  return path;
}

int ManhattanDistance(sf::Vector2i a, sf::Vector2i b) {

  return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}
}
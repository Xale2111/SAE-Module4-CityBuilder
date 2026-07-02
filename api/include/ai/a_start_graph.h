//
// Created by alexk on 17.06.2026.
//

#ifndef SAE_ALEXK_CITYBUILDER_API_INCLUDE_AI_A_START_GRAPH_H_
#define SAE_ALEXK_CITYBUILDER_API_INCLUDE_AI_A_START_GRAPH_H_

#include <unordered_set>
#include <vector>
#include <SFML/System/Vector2.hpp>
#include <queue>
#include <span>
#include <array>

namespace api::ai{


struct AStarVertex{
  sf::Vector2i position = sf::Vector2i(-1, -1);
  int g = 0; //Dijkstra cost
  int h = 0; //heuristic value

  int parent_idx = -1;
  //AStarVertex* Parent = nullptr;

  [[nodiscard]] int F() const{return g+h;};

  bool operator>(const AStarVertex& other) const {return F() > other.F();};
  bool operator==(const AStarVertex &other) const{return F() == other.F();}
};

struct Vec2iHash{
  size_t operator()(const sf::Vector2i& node) const noexcept{
    size_t h = std::hash<int>{}(node.x);
    h ^= std::hash<int>{}(node.y) + 0x9e377969 + (h<<6) + (h >> 2);

    return h;
  }
};

struct AStarVertexHash{
  size_t operator()(const AStarVertex vertex) const noexcept{
    return Vec2iHash{}(vertex.position);
  }
};

class AStarGraph
{
 public:
  [[nodiscard]] std::vector<sf::Vector2i> GetPath(sf::Vector2i start, sf::Vector2i end, std::span<sf::Vector2i> walkable, std::vector<uint8_t>& cache_walkables, std::vector<uint8_t>& cache_visited);
 private:

  void AddNode(sf::Vector2i node);

  void RemoveNode(sf::Vector2i node);

  [[nodiscard]] bool ContainsNode(sf::Vector2i node) const;

  std::unordered_set<sf::Vector2i, Vec2iHash> graph_;

  [[nodiscard]] int CalculateIndex(int x, int y) const;

};
constexpr std::array<sf::Vector2i,4> kNeighbours{
    sf::Vector2i(0,-1), //NORTH
    sf::Vector2i(1,0),  //EAST
    sf::Vector2i(0,1),  //SOUTH
    sf::Vector2i(-1,0)  //WEST
};

int ManhattanDistance(sf::Vector2i a, sf::Vector2i b);

}

#endif //SAE_ALEXK_CITYBUILDER_API_INCLUDE_AI_A_START_GRAPH_H_

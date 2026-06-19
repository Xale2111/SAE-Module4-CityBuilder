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

  // 1. On crée une grille 2D de booleens pour les tuiles marchables (Taille: 100x100)
  std::vector<std::vector<bool>>
      is_tile_walkable(DataUtils::kTilemapWidth, std::vector<bool>(DataUtils::kTilemapHeight, false));

  // On remplit la grille UNE SEULE FOIS au début (Opération très rapide)
  for (const auto &pixel_pos : walkable) {
    int tx = pixel_pos.x / DataUtils::kTileSize;
    int ty = pixel_pos.y / DataUtils::kTileSize;
    if (tx >= 0 && tx < DataUtils::kTilemapWidth && ty >= 0 && ty < DataUtils::kTilemapHeight) {
      is_tile_walkable[tx][ty] = true;
    }
  }

  // 2. On crée une grille 2D pour la liste fermée (Visited) au lieu du find_if
  std::vector<std::vector<bool>>
      is_tile_visited(DataUtils::kTilemapWidth, std::vector<bool>(DataUtils::kTilemapHeight, false));

  // Convertir le départ et l'arrivée en index de tuiles pour nos vérifications
  int start_tx = start.x / DataUtils::kTileSize;
  int start_ty = start.y / DataUtils::kTileSize;
  int end_tx = end.x / DataUtils::kTileSize;
  int end_ty = end.y / DataUtils::kTileSize;

  // Sécurité : On vérifie les limites de la carte
  if (start_tx < 0 || start_tx >= DataUtils::kTilemapWidth || start_ty < 0 || start_ty >= DataUtils::kTilemapHeight ||
      end_tx < 0 || end_tx >= DataUtils::kTilemapWidth || end_ty < 0 || end_ty >= DataUtils::kTilemapHeight) {
    return path;
  }

  // Sécurité O(1) : Est-ce que le départ ou l'arrivée est dans un mur ?
  if (!is_tile_walkable[start_tx][start_ty] || !is_tile_walkable[end_tx][end_ty]) {
    return path;
  }

  std::priority_queue<AStarVertex, std::vector<AStarVertex>, std::greater<>> open_queue;
  std::vector<AStarVertex> visited_vertices; // On la garde uniquement pour reconstruire le chemin à la fin

  open_queue.push(AStarVertex(start, 0, ManhattanDistance(start, end), -1));

  while (!open_queue.empty()) {
    AStarVertex node = open_queue.top();
    open_queue.pop();

    int node_tx = node.position.x / DataUtils::kTileSize;
    int node_ty = node.position.y / DataUtils::kTileSize;

    // Si on a déjà traité cette tuile, on passe (évite les doublons de la queue)
    if (is_tile_visited[node_tx][node_ty]) continue;

    is_tile_visited[node_tx][node_ty] = true;
    visited_vertices.push_back(node);
    int visited_idx = visited_vertices.size() - 1;

    // Si on est arrivé
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

      // Vérification des limites pour le voisin
      if (n_tx < 0 || n_tx >= DataUtils::kTilemapWidth || n_ty < 0 || n_ty >= DataUtils::kTilemapHeight) {
        continue;
      }

      // MAGIE : Plus aucun find_if ! On interroge directement nos grilles de booléens en O(1)
      if (!is_tile_visited[n_tx][n_ty] && is_tile_walkable[n_tx][n_ty]) {
        open_queue.push(AStarVertex(new_position, node.g + DataUtils::kTileSize, ManhattanDistance(new_position, end), visited_idx));
      }
    }
  }

  return path; // Renvoie un chemin vide si aucun passage n'existe
}

int ManhattanDistance(sf::Vector2i a, sf::Vector2i b) {

  return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}
}
#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <SFML/Graphics.hpp>
#include <list>
#include <unordered_set>

#include "graph.h"
#include "grid.h"

class Dijkstra
{
public:
   Dijkstra(Graph& graph);
   void solveAlgorithm(const sf::Vector2i& src, const sf::Vector2i& target, Grid& map, sf::RenderWindow& window);
   void constructPath(Grid& map, sf::RenderWindow& window);
   void resetAlgorithm();

private:
   Graph&                    graph;
   sf::Vector2i              srcPos;
   sf::Vector2i              targetPos;
   sf::Text                  text;
   sf::Font                  font;
   bool                      targetReached;
   std::list<Node*>          openList;
   std::unordered_set<Node*> openSet;

   double nodeDistance(const Node* a, const Node* b);
};

#endif  // DIJKSTRA_H

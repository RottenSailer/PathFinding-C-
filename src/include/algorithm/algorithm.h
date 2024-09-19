#ifndef ALGO_H
#define ALGO_H

#include <SFML/Graphics.hpp>
#include <list>
#include <unordered_set>

#include "graph/graph.h"
#include "ui/grid.h"

class PathFindingAlgorithm
{
public:
   PathFindingAlgorithm(Graph& graph, Grid& grid, sf::RenderWindow& window);
   virtual void findPath(const sf::Vector2i& src, const sf::Vector2i& target);

   void constructPath(Grid& map, sf::RenderWindow& window);
   void resetAlgorithm();

protected:
   Graph&   graph;
   Grid&    grid;
   sf::Text text;
   sf::Font font;
   bool     targetReached;

   sf::Vector2i     srcPos;
   sf::Vector2i     targetPos;
   std::list<Node*> openList;

   sf::RenderWindow&         window;
   std::unordered_set<Node*> openSet;

   virtual double nodeDistance(Node* a, Node* b);
};

class AStar : public PathFindingAlgorithm
{
public:
   AStar(Graph& graph, Grid& grid, sf::RenderWindow& window);
   void findPath(const sf::Vector2i& src, const sf::Vector2i& target) override;

private:
   double nodeDistance(Node* a, Node* b) override;
};

class Dijkstra : public PathFindingAlgorithm
{
public:
   Dijkstra(Graph& graph, Grid& grid, sf::RenderWindow& window);
   void findPath(const sf::Vector2i& src, const sf::Vector2i& target) override;
};

class Kruskal
{
private:
   Graph& graph;
   void   generateMaze(Grid& map, sf::RenderWindow& window);
   struct Edge
   {
      int x1, x2;
      int y1, y2;
   };

public:
   Kruskal(Graph& graph);
   void resetMaze();
};

#endif  // ALGO_H

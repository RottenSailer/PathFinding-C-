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
   struct Edge
   {
      int from;
      int to;

      Edge(int f, int t) : from(f), to(t)
      {}
   };

   int width;
   int height;
   int totalCells;

   std::vector<Edge> edges;
   std::vector<int>  parent;

   Graph&            graph;
   Grid&             grid;
   sf::RenderWindow& window;

   void                     initializeUnionFind();
   int                      findSet(int cell);
   void                     unionSet(int cell1, int cell2);
   const std::vector<Edge>& getRemovedEdges() const;
   std::pair<int, int>      getCellCoordinates(int index) const;
   int                      getCellIndex(int row, int col) const;

   void generateAllEdges();
   void shuffleEdges();
   void removeWall(int x1, int y1, int x2, int y2);
   void addEdge(int x1, int y1, int x2, int y2);
   void resetMaze();

public:
   Kruskal(Graph& graph, Grid& grid, sf::RenderWindow& window);

   void generateMaze();
};

#endif  // ALGO_H

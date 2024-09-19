#ifndef ALGO_H
#define ALGO_H

#include <SFML/Graphics.hpp>
#include <list>
#include <unordered_set>

#include "graph/graph.h"
#include "ui/grid.h"

class Algorithm
{
public:
   Algorithm(Graph& graph);
   virtual void solveAlgorithm(const sf::Vector2i& src, const sf::Vector2i& target, Grid& map, sf::RenderWindow& window);

   void constructPath(Grid& map, sf::RenderWindow& window);
   void resetAlgorithm();

protected:
   Graph&                    graph;
   sf::Vector2i              srcPos;
   sf::Vector2i              targetPos;
   sf::Text                  text;
   sf::Font                  font;
   bool                      targetReached;
   std::list<Node*>          openList;
   std::unordered_set<Node*> openSet;

   virtual double nodeDistance(Node* a, Node* b);
};

class AStar : public Algorithm
{
public:
   AStar(Graph& graph);
   void solveAlgorithm(const sf::Vector2i& src, const sf::Vector2i& target, Grid& map, sf::RenderWindow& window) override;

private:
   double nodeDistance(Node* a, Node* b) override;
};

class Dijkstra : public Algorithm
{
public:
   Dijkstra(Graph& graph);
   void solveAlgorithm(const sf::Vector2i& src, const sf::Vector2i& target, Grid& map, sf::RenderWindow& window) override;
};

#endif  // ALGO_H

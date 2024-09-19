#ifndef GRAPH_H
#define GRAPH_H

#include <vector>

#include "node.h"

class Graph
{
public:
   Graph(int totalXTiles, int totalYTiles);
   Node& getNode(const sf::Vector2i& loc);
   Node& getNode(const int x, const int y);
   void  resetGraph();
   void  resetGraph(bool rememberObstacle);


private:
   int               totalXTiles;
   int               totalYTiles;
   std::vector<Node> nodes;
};

#endif  // GRAPH_H

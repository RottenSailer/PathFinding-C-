// Node.h
#ifndef NODE_H
#define NODE_H

#include <SFML/Graphics.hpp>
#include <vector>

struct Node
{
   Node(int x, int y);

   sf::Vector2i       location;
   bool               visited;
   Node*              parent;
   bool               isObstacle;
   double             hCost;
   double             gCost;
   double             fCost;
   std::vector<Node*> neighbours;
};

#endif  // NODE_H

#include "graph/graph.h"

#include <cassert>

Graph::Graph(int totalXTiles, int totalYTiles) : totalXTiles(totalXTiles), totalYTiles(totalYTiles)
{
   for (int x = 0; x < totalXTiles; ++x)
   {
      for (int y = 0; y < totalYTiles; ++y)
      {
         nodes.emplace_back(x, y);
      }
   }

   assert(nodes.size() == totalXTiles * totalYTiles);

   // Directions: N, NE, E, SE, S, SW, W, NW
   const int dx[]          = {0, 1, 1, 1, 0, -1, -1, -1};
   const int dy[]          = {-1, -1, 0, 1, 1, 1, 0, -1};
   const int numDirections = 8;

   // Set up neighbors
   for (int x = 0; x < totalXTiles; ++x)
   {
      for (int y = 0; y < totalYTiles; ++y)
      {
         int currentIndex = x * totalYTiles + y;  // Corrected indexing

         for (int dir = 0; dir < numDirections; ++dir)
         {
            int nx = x + dx[dir];
            int ny = y + dy[dir];

            // Check if neighbor is within bounds
            if (nx >= 0 && nx < totalXTiles && ny >= 0 && ny < totalYTiles)
            {
               int neighborIndex = nx * totalYTiles + ny;
               nodes[currentIndex].neighbours.emplace_back(&nodes[neighborIndex]);
            }
         }
      }
   }
}

Node& Graph::getNode(const sf::Vector2i& loc)
{
   int index = loc.x * totalYTiles + loc.y;
   assert(index >= 0 && index < nodes.size());
   return nodes[index];
}

Node& Graph::getNode(const int x, const int y)
{
   int index = x * totalYTiles + y;
   assert(index >= 0 && index < nodes.size());
   return nodes[index];
}

void Graph::resetGraph()
{
   for (auto& node : nodes)
   {
      node.fCost      = std::numeric_limits<double>::infinity();
      node.gCost      = std::numeric_limits<double>::infinity();
      node.hCost      = std::numeric_limits<double>::infinity();
      node.isObstacle = false;
      node.visited    = false;
      node.parent     = nullptr;
   }
}

void Graph::resetGraph(bool rememberObstacle)
{
   for (auto& node : nodes)
   {
      node.fCost   = std::numeric_limits<double>::infinity();
      node.gCost   = std::numeric_limits<double>::infinity();
      node.hCost   = std::numeric_limits<double>::infinity();
      node.visited = false;
      node.parent  = nullptr;
   }
}

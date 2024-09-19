#include "graph.h"

#include <cassert>

Graph::Graph(int totalXTiles, int totalYTiles) : totalXTiles(totalXTiles), totalYTiles(totalYTiles)
{
   for (int i = 0; i < totalXTiles; ++i)
   {
      for (int j = 0; j < totalYTiles; ++j)
      {
         nodes.emplace_back(i, j);
      }
   }

   assert(nodes.size() == totalXTiles * totalYTiles);

   for (int x = 0; x < totalXTiles; ++x)
   {
      for (int y = 0; y < totalYTiles; ++y)
      {
         if (y > 0)
         {
            nodes[x * totalXTiles + y].neighbours.emplace_back(&nodes[x * totalXTiles + (y - 1)]);
         }

         if (y < totalXTiles - 1)
         {
            nodes[x * totalXTiles + y].neighbours.emplace_back(&nodes[x * totalXTiles + (y + 1)]);
         }

         if (x > 0)
         {
            nodes[x * totalXTiles + y].neighbours.emplace_back(&nodes[(x - 1) * totalXTiles + y]);
         }

         if (x < totalXTiles - 1)
         {
            nodes[x * totalXTiles + y].neighbours.emplace_back(&nodes[(x + 1) * totalXTiles + y]);
         }

         if (y > 0 && x > 0)
         {
            nodes[x * totalXTiles + y].neighbours.emplace_back(&nodes[(x - 1) * totalXTiles + (y - 1)]);
         }

         if (y < totalXTiles - 1 && x > 0)
         {
            nodes[x * totalXTiles + y].neighbours.emplace_back(&nodes[(x - 1) * totalXTiles + (y + 1)]);
         }

         if (y > 0 && x < totalXTiles - 1)
         {
            nodes[x * totalXTiles + y].neighbours.emplace_back(&nodes[(x + 1) * totalXTiles + (y - 1)]);
         }

         if (y < totalXTiles - 1 && x < totalXTiles - 1)
         {
            nodes[x * totalXTiles + y].neighbours.emplace_back(&nodes[(x + 1) * totalXTiles + (y + 1)]);
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
      node.fCost      = std::numeric_limits<double>::infinity();
      node.gCost      = std::numeric_limits<double>::infinity();
      node.hCost      = std::numeric_limits<double>::infinity();
      node.visited    = false;
      node.parent     = nullptr;
   }
}

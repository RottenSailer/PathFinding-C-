#include <cmath>

#include "algorithm/algorithm.h"

AStar::AStar(Graph& graph, Grid& grid, sf::RenderWindow& window) : PathFindingAlgorithm(graph, grid, window)
{
   text.setString("Solving A* Algorithm...");
}

double AStar::nodeDistance(Node* a, Node* b)
{
   int dx = a->location.x - b->location.x;
   int dy = a->location.y - b->location.y;
   return std::sqrt(dx * dx + dy * dy);
}

void AStar::findPath(const sf::Vector2i& src, const sf::Vector2i& target)
{
   srcPos    = grid.getStart();
   targetPos = grid.getEnd();

   for (int x = 0; x < grid.getMapSize(); ++x)
   {
      for (int y = 0; y < grid.getMapSize(); ++y)
      {
         if (grid.isWall(x, y))
         {
            graph.getNode(sf::Vector2i(x, y)).isObstacle = true;
         }
      }
   }

   auto compareFCost = [](Node* leftNode, Node* rightNode) { return leftNode->fCost < rightNode->fCost; };

   Node* startNode  = &graph.getNode(srcPos);
   startNode->gCost = 0;

   Node* endNode    = &graph.getNode(targetPos);
   startNode->hCost = nodeDistance(startNode, endNode);
   startNode->fCost = startNode->gCost + startNode->hCost;

   openList.push_back(startNode);
   openSet.insert(startNode);

   while (!openList.empty() && !targetReached)
   {
      openList.sort(compareFCost);
      Node* currentNode = openList.front();
      openList.pop_front();
      openSet.erase(currentNode);

      currentNode->visited = true;

      if (!grid.isWall(currentNode->location.x, currentNode->location.y))
      {
         grid.colourVisitedTile(currentNode->location);
      }

      if (currentNode->location == targetPos)
      {
         targetReached = true;
         break;
      }

      grid.draw(window);
      window.display();

      for (auto neighbor : currentNode->neighbours)
      {
         if (neighbor->visited || neighbor->isObstacle)
         {
            continue;
         }

         double tentativeGCost = currentNode->gCost + nodeDistance(currentNode, neighbor);

         if (tentativeGCost < neighbor->gCost)
         {
            neighbor->parent = currentNode;
            neighbor->gCost  = tentativeGCost;
            neighbor->hCost  = nodeDistance(neighbor, endNode);
            neighbor->fCost  = neighbor->gCost + neighbor->hCost;

            if (openSet.find(neighbor) == openSet.end())
            {
               if (!grid.isWall(neighbor->location.x, neighbor->location.y))
               {
                  grid.colourVisitingTile(neighbor->location);
               }

               openSet.insert(neighbor);
               openList.push_back(neighbor);
            }
         }
      }
   }
}

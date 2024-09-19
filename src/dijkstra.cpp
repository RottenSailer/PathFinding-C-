#include "algorithm/algorithm.h"

Dijkstra::Dijkstra(Graph& graph) : Algorithm(graph)
{
   text.setString("Solving Dijkstra's Algorithm...");
}

void Dijkstra::solveAlgorithm(const sf::Vector2i& src, const sf::Vector2i& target, Grid& map, sf::RenderWindow& window)
{
   srcPos    = map.getStart();
   targetPos = map.getEnd();

   for (int i = 0; i < 50; i++)
   {
      for (int j = 0; j < 50; j++)
      {
         if (map.isWall(i, j))
         {
            graph.getNode(sf::Vector2i(i, j)).isObstacle = true;
         }
      }
   }

   auto compareGCost = [](Node* leftNode, Node* rightNode) { return leftNode->gCost < rightNode->gCost; };

   Node* startNode  = &graph.getNode(srcPos);
   startNode->gCost = 0;

   Node* endNode = &graph.getNode(targetPos);

   openList.push_back(startNode);
   openSet.insert(startNode);

   while (!openList.empty() && !targetReached)
   {
      openList.sort(compareGCost);
      Node* currentNode = openList.front();
      openList.pop_front();
      openSet.erase(currentNode);
      currentNode->visited = true;

      if (!map.isWall(currentNode))
      {
         map.colourVisitedTile(currentNode->location);
      }

      if (currentNode->location == targetPos)
      {
         targetReached = true;
      }

      map.draw(window);
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

            if (openSet.find(neighbor) == openSet.end())
            {
               openSet.insert(neighbor);
               openList.push_back(neighbor);

               if (!map.isWall(neighbor->location.x, neighbor->location.y))
               {
                  map.colourVisitingTile(neighbor->location);
               }
            }
         }
      }
   }
}

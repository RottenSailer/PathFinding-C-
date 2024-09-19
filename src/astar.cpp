#include "astar.h"
#include <cmath>

AStar::AStar(Graph& graph) : graph(graph), targetReached(false)
{
   // if (!font.loadFromMemory(arial_ttf, arial_ttf_len))
   // {
   //    std::cout << "Could not load font.." << std::endl;
   // }
   text.setFont(font);
   text.setString("Solving A* Algorithm...");
   text.setPosition(sf::Vector2f(5, 0));
   text.setFillColor(sf::Color::Green);
   text.setOutlineColor(sf::Color::Black);
   text.setOutlineThickness(2);
   text.setCharacterSize(25);
}

void AStar::resetAlgorithms()
{
   openList.clear();
   openSet.clear();
   targetReached = false;
}

double AStar::nodeDistance(Node* a, Node* b)
{
   int dx = a->location.x - b->location.x;
   int dy = a->location.y - b->location.y;
   return std::sqrt(dx * dx + dy * dy);
}

void AStar::solveAlgorithm(const sf::Vector2i& src, const sf::Vector2i& target, Grid& map, sf::RenderWindow& window)
{
   srcPos    = map.getStart();
   targetPos = map.getEnd();

   for (int x = 0; x < map.getMapSize(); ++x)
   {
      for (int y = 0; y < map.getMapSize(); ++y)
      {
         if (map.isWall(x, y))
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

      if (!map.isWall(currentNode->location.x, currentNode->location.y))
      {
         map.colourVisitedTile(currentNode->location);
      }

      if (currentNode->location == targetPos)
      {
         targetReached = true;
         break;
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
            neighbor->hCost  = nodeDistance(neighbor, endNode);
            neighbor->fCost  = neighbor->gCost + neighbor->hCost;

            if (openSet.find(neighbor) == openSet.end())
            {
               if (!map.isWall(neighbor->location.x, neighbor->location.y))
               {
                  map.colourVisitingTile(neighbor->location);
               }

               openSet.insert(neighbor);
               openList.push_back(neighbor);
            }
         }
      }
   }
}

void AStar::constructPath(Grid& map, sf::RenderWindow& window)
{
   Node* traverse = &graph.getNode(targetPos);
   if (traverse != nullptr)
   {
      while (traverse->parent != nullptr)
      {
         map.setTileColor(traverse->location, sf::Color::Red);
         traverse = traverse->parent;
         map.draw(window);
         window.display();
      }
   }
}

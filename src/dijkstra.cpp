#include "dijkstra.h"
// #include "arial.h"

Dijkstra::Dijkstra(Graph& graph) : graph(graph), targetReached(false)
{
   // if (!font.loadFromMemory(arial_ttf, arial_ttf_len))
   // {
   //    std::cerr << "Could not load font.." << std::endl;
   // }
   text.setFont(font);
   text.setString("Solving Dijkstra's Algorithm...");
   text.setPosition(sf::Vector2f(5, 0));
   text.setFillColor(sf::Color::Green);
   text.setOutlineColor(sf::Color::Black);
   text.setOutlineThickness(2);
}

void Dijkstra::resetAlgorithm()
{
   openList.clear();
   openSet.clear();
   targetReached = false;
}

double Dijkstra::nodeDistance(const Node* a, const Node* b)
{
   return 1.0;
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

void Dijkstra::constructPath(Grid& map, sf::RenderWindow& window)
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

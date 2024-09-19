
#include "algorithm/algorithm.h"

PathFindingAlgorithm::PathFindingAlgorithm(Graph& graph, Grid& grid, sf::RenderWindow& window)
  : graph(graph), grid(grid), window(window), targetReached(false)
{
   text.setFont(font);
   text.setString("Solving A* PathFindingAlgorithm...");
   text.setPosition(sf::Vector2f(5, 0));
   text.setFillColor(sf::Color::Green);
   text.setOutlineColor(sf::Color::Black);
   text.setOutlineThickness(2);
   text.setCharacterSize(25);
}

void PathFindingAlgorithm::resetAlgorithm()
{
   openList.clear();
   openSet.clear();
   targetReached = false;
}

double PathFindingAlgorithm::nodeDistance(Node* a, Node* b)
{
   return 1.0;
}

void PathFindingAlgorithm::findPath(const sf::Vector2i& src, const sf::Vector2i& target)
{}

void PathFindingAlgorithm::constructPath(Grid& grid, sf::RenderWindow& window)
{
   Node* traverse = &graph.getNode(targetPos);
   if (traverse != nullptr)
   {
      while (traverse->parent != nullptr)
      {
         grid.setTileColor(traverse->location, sf::Color::Red);
         traverse = traverse->parent;
         grid.draw(window);
         window.display();
      }
   }
}

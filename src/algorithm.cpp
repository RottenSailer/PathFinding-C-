
#include "algorithm/algorithm.h"

Algorithm::Algorithm(Graph& graph) : graph(graph), targetReached(false)
{
   text.setFont(font);
   text.setString("Solving A* Algorithm...");
   text.setPosition(sf::Vector2f(5, 0));
   text.setFillColor(sf::Color::Green);
   text.setOutlineColor(sf::Color::Black);
   text.setOutlineThickness(2);
   text.setCharacterSize(25);
}

void Algorithm::resetAlgorithm()
{
   openList.clear();
   openSet.clear();
   targetReached = false;
}

double Algorithm::nodeDistance(Node* a, Node* b)
{
   return 1.0;
}

void Algorithm::solveAlgorithm(const sf::Vector2i& src, const sf::Vector2i& target, Grid& map, sf::RenderWindow& window)
{}

void Algorithm::constructPath(Grid& map, sf::RenderWindow& window)
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

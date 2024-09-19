#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

#include "algorithm/algorithm.h"
#include "embedding/arial.h"
#include "graph/graph.h"
#include "ui/button.h"  // Include the Button class
#include "ui/grid.h"
#include "ui/legendbuilder.h"

int main()
{
   sf::Font font;
   if (!font.loadFromMemory(arial_ttf, arial_ttf_len))
   {
      std::cerr << "Could not load font.." << std::endl;
      return -1;
   }
   int       windowSize = 900;
   int       margin     = 50;
   int       mapSize    = 50;
   int       stepSize   = (windowSize - margin * 2) / mapSize;
   sf::Color buttonColor(200, 200, 200);
   Grid      grid(stepSize, mapSize, margin);
   Graph     graph(mapSize, mapSize);

   sf::RenderWindow window(sf::VideoMode(windowSize, windowSize), "PathFinder", sf::Style::Close);
   Dijkstra         dijkstra(graph, grid, window);
   AStar            astar(graph, grid, window);
   Kruskal          kruskal(graph, grid, window);

   float gap    = 50.f;
   float border = 65.f;

   ButtonFactory buttonFactory(buttonColor, sf::Color::Black, font);
   Button*       clearButton    = buttonFactory.createButton({(border * 1) + (gap * 0 * 2), 6.f}, "CLEAR");     // 50
   Button*       mazeButton     = buttonFactory.createButton({(border * 2) + (gap * 1 * 2), 6.f}, "MAZE");      // 70
   Button*       resetButton    = buttonFactory.createButton({(border * 3) + (gap * 2 * 2), 6.f}, "RESET");     // 90
   Button*       dijkstraButton = buttonFactory.createButton({(border * 4) + (gap * 3 * 2), 6.f}, "DIJKSTRA");  // 80
   Button*       astarButton    = buttonFactory.createButton({(border * 5) + (gap * 4 * 2), 6.f}, "A*");        // 100

   LegendBuilder legend(windowSize - 200.f, 80.f, font);
   legend.addItem(grid.wallColour, "Wall");
   legend.addItem(grid.startColour, "Start");
   legend.addItem(grid.endColour, "End");
   legend.addItem(grid.visitedTileColour, "Visited");
   legend.addItem(grid.shortestPathColour, "Path");

   bool isMovingStart = false;
   bool isMovingEnd   = false;

   while (window.isOpen())
   {
      sf::Event event;
      while (window.pollEvent(event))
      {
         if (event.type == sf::Event::Closed)
            window.close();

         if (event.type == sf::Event::MouseButtonPressed)
         {
            if (event.mouseButton.button == sf::Mouse::Left)
            {
               if (clearButton->isMouseOver(window))
               {
                  grid.removeWalls();
               }
               else if (mazeButton->isMouseOver(window))
               {
                  kruskal.generateMaze();
               }
               else if (resetButton->isMouseOver(window))
               {
                  grid.gridReset();
                  graph.resetGraph();
                  dijkstra.resetAlgorithm();
                  astar.resetAlgorithm();
               }
               else if (dijkstraButton->isMouseOver(window))
               {
                  graph.resetGraph(true);
                  grid.colorReset();
                  dijkstra.resetAlgorithm();
                  dijkstra.findPath(grid.getStart(), grid.getEnd());
                  dijkstra.constructPath(grid, window);
               }
               else if (astarButton->isMouseOver(window))
               {
                  graph.resetGraph(true);
                  grid.colorReset();
                  astar.resetAlgorithm();
                  astar.findPath(grid.getStart(), grid.getEnd());
                  astar.constructPath(grid, window);
               }
               else
               {
                  // Handle initial wall placement or moving start/end positions
                  sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                  int          mx       = mousePos.x - margin;
                  int          my       = mousePos.y - margin;

                  if (mx >= 0 && my >= 0 && mx < windowSize - 2 * margin && my < windowSize - 2 * margin)
                  {
                     int x = mx / stepSize;
                     int y = my / stepSize;

                     if (grid.checkStart(x, y))
                     {
                        isMovingStart = true;
                     }
                     else if (grid.checkEnd(x, y))
                     {
                        isMovingEnd = true;
                     }
                     else
                     {
                        grid.putWall(x, y);
                     }
                  }
               }
            }
            else if (event.mouseButton.button == sf::Mouse::Right)
            {
               // Remove wall
               sf::Vector2i mousePos = sf::Mouse::getPosition(window);
               int          mx       = mousePos.x - margin;
               int          my       = mousePos.y - margin;

               if (mx >= 0 && my >= 0 && mx < windowSize - 2 * margin && my < windowSize - 2 * margin)
               {
                  int x = mx / stepSize;
                  int y = my / stepSize;
                  grid.removeWall(x, y);
               }
            }
         }
         else if (event.type == sf::Event::MouseButtonReleased)
         {
            if (event.mouseButton.button == sf::Mouse::Left)
            {
               isMovingStart = false;
               isMovingEnd   = false;
            }
         }
         else if (event.type == sf::Event::MouseMoved)
         {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            int          mx       = mousePos.x - margin;
            int          my       = mousePos.y - margin;

            if (mx >= 0 && my >= 0 && mx < windowSize - 2 * margin && my < windowSize - 2 * margin)
            {
               int x = mx / stepSize;
               int y = my / stepSize;

               if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
               {
                  if (isMovingStart)
                  {
                     grid.updateStart(x, y);
                  }
                  else if (isMovingEnd)
                  {
                     grid.updateEnd(x, y);
                  }
                  else
                  {
                     if (!grid.checkStart(x, y) && !grid.checkEnd(x, y))
                     {
                        grid.putWall(x, y);
                     }
                  }
               }
               else if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
               {
                  grid.removeWall(x, y);
               }
            }
         }
      }

      window.clear(sf::Color::White);
      grid.draw(window);
      buttonFactory.updateAll(window);
      buttonFactory.drawAll(window);

      legend.draw(window);
      window.display();
   }

   return 0;
}

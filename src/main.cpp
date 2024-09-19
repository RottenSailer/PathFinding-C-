#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

#include "algorithm/algorithm.h"
#include "embedding/arial.h"
#include "graph/graph.h"
#include "ui/button.h"  // Include the Button class
#include "ui/grid.h"

int main()
{
   sf::Font font;
   if (!font.loadFromMemory(arial_ttf, arial_ttf_len))
   {
      std::cerr << "Could not load font.." << std::endl;
      return -1;
   }

   int windowSize = 900;
   int margin     = 50;
   int mapSize    = 50;
   int stepSize   = (windowSize - margin * 2) / mapSize;

   sf::RenderWindow window(sf::VideoMode(windowSize, windowSize), "PathFinder", sf::Style::Close);

   Grid     map(stepSize, mapSize, margin);
   Graph    graph(mapSize, mapSize);
   Dijkstra dijkstra(graph);
   AStar    astar(graph);

   // Create buttons
   Button clearButton({100.f, 40.f}, {50.f, 6.f}, sf::Color(200, 200, 200), sf::Color::Black, "CLEAR", font);
   Button mazeButton({100.f, 40.f}, {230.f, 6.f}, sf::Color(200, 200, 200), sf::Color::Black, "MAZE", font);
   Button resetButton({100.f, 40.f}, {410.f, 6.f}, sf::Color(200, 200, 200), sf::Color::Black, "RESET", font);
   Button dijkstraButton({120.f, 40.f}, {590.f, 6.f}, sf::Color(200, 200, 200), sf::Color::Black, "DIJKSTRA", font);
   Button astarButton({60.f, 40.f}, {790.f, 6.f}, sf::Color(200, 200, 200), sf::Color::Black, "A*", font);

   // Legend setup
   float    legendX = windowSize - 200.f;
   sf::Text legendTitle("Legend:", font, 16);
   legendTitle.setFillColor(sf::Color::Black);
   legendTitle.setPosition(legendX + 10.f, 80.f);

   // Legend background rectangle
   sf::RectangleShape legendBackground(sf::Vector2f(180.f, 200.f));
   legendBackground.setFillColor(sf::Color(255, 255, 255, 200));
   legendBackground.setOutlineColor(sf::Color::Black);
   legendBackground.setOutlineThickness(2.f);
   legendBackground.setPosition(legendX, 70.f);

   // Wall legend
   sf::RectangleShape wallLegend(sf::Vector2f(20.f, 20.f));
   wallLegend.setFillColor(map.wallColour);
   wallLegend.setPosition(windowSize - 190.f, 110.f);

   sf::Text wallText("Wall", font, 14);
   wallText.setFillColor(sf::Color::Black);
   wallText.setPosition(windowSize - 160.f, 110.f);

   // Start legend
   sf::RectangleShape startLegend(sf::Vector2f(20.f, 20.f));
   startLegend.setFillColor(map.startColour);
   startLegend.setPosition(windowSize - 190.f, 140.f);

   sf::Text startText("Start", font, 14);
   startText.setFillColor(sf::Color::Black);
   startText.setPosition(windowSize - 160.f, 140.f);

   // End legend
   sf::RectangleShape endLegend(sf::Vector2f(20.f, 20.f));
   endLegend.setFillColor(map.endColour);
   endLegend.setPosition(windowSize - 190.f, 170.f);

   sf::Text endText("End", font, 14);
   endText.setFillColor(sf::Color::Black);
   endText.setPosition(windowSize - 160.f, 170.f);

   // Visited tile legend
   sf::RectangleShape visitedLegend(sf::Vector2f(20.f, 20.f));
   visitedLegend.setFillColor(map.visitedTileColour);
   visitedLegend.setPosition(windowSize - 190.f, 200.f);

   sf::Text visitedText("Visited", font, 14);
   visitedText.setFillColor(sf::Color::Black);
   visitedText.setPosition(windowSize - 160.f, 200.f);

   // Path legend
   sf::RectangleShape pathLegend(sf::Vector2f(20.f, 20.f));
   pathLegend.setFillColor(map.shortestPathColour);
   pathLegend.setPosition(windowSize - 190.f, 230.f);

   sf::Text pathText("Path", font, 14);
   pathText.setFillColor(sf::Color::Black);
   pathText.setPosition(windowSize - 160.f, 230.f);

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
               if (clearButton.isMouseOver(window))
               {
                  map.removeWalls();
               }
               else if (mazeButton.isMouseOver(window))
               {
                  map.generateMaze(window);
               }
               else if (resetButton.isMouseOver(window))
               {
                  map.gridReset();
                  graph.resetGraph();
                  dijkstra.resetAlgorithm();
                  astar.resetAlgorithm();
               }
               else if (dijkstraButton.isMouseOver(window))
               {
                  graph.resetGraph(true);
                  map.colorReset();
                  dijkstra.resetAlgorithm();
                  dijkstra.solveAlgorithm(map.getStart(), map.getEnd(), map, window);
                  dijkstra.constructPath(map, window);
               }
               else if (astarButton.isMouseOver(window))
               {
                  graph.resetGraph(true);
                  map.colorReset();
                  astar.resetAlgorithm();
                  astar.solveAlgorithm(map.getStart(), map.getEnd(), map, window);
                  astar.constructPath(map, window);
               }
               else
               {
                  sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                  int          mx       = mousePos.x - margin;
                  int          my       = mousePos.y - margin;

                  // Check if mouse is on the grid
                  if (mx >= 0 && my >= 0 && mx < windowSize - 2 * margin && my < windowSize - 2 * margin)
                  {
                     int x = mx / stepSize;
                     int y = my / stepSize;

                     if (map.checkStart(x, y))
                     {
                        // Move start position
                        while (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                        {
                           sf::Vector2i newMousePos = sf::Mouse::getPosition(window);
                           int          newMx       = newMousePos.x - margin;
                           int          newMy       = newMousePos.y - margin;

                           if (newMx >= 0 && newMy >= 0 && newMx < windowSize - 2 * margin && newMy < windowSize - 2 * margin)
                           {
                              int newX = newMx / stepSize;
                              int newY = newMy / stepSize;
                              map.updateStart(newX, newY);
                           }
                           map.draw(window);
                           window.display();
                        }
                     }
                     else if (map.checkEnd(x, y))
                     {
                        // Move end position
                        while (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                        {
                           sf::Vector2i newMousePos = sf::Mouse::getPosition(window);
                           int          newMx       = newMousePos.x - margin;
                           int          newMy       = newMousePos.y - margin;

                           if (newMx >= 0 && newMy >= 0 && newMx < windowSize - 2 * margin && newMy < windowSize - 2 * margin)
                           {
                              int newX = newMx / stepSize;
                              int newY = newMy / stepSize;
                              map.updateEnd(newX, newY);
                           }
                           map.draw(window);
                           window.display();
                        }
                     }
                     else
                     {
                        map.putWall(x, y);
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
                  map.removeWall(x, y);
               }
            }
         }
      }

      // Update buttons
      clearButton.update(window);
      mazeButton.update(window);
      resetButton.update(window);
      dijkstraButton.update(window);
      astarButton.update(window);

      // Drawing
      window.clear(sf::Color::White);
      map.draw(window);

      // Draw buttons
      clearButton.draw(window);
      mazeButton.draw(window);
      resetButton.draw(window);
      dijkstraButton.draw(window);
      astarButton.draw(window);

      // Draw legend background first
      window.draw(legendBackground);

      // Draw legend
      window.draw(legendTitle);
      window.draw(wallLegend);
      window.draw(wallText);
      window.draw(startLegend);
      window.draw(startText);
      window.draw(endLegend);
      window.draw(endText);
      window.draw(visitedLegend);
      window.draw(visitedText);
      window.draw(pathLegend);
      window.draw(pathText);

      window.display();
   }

   return 0;
}

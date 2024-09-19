#ifndef GRID_H
#define GRID_H

#include <graph/node.h>

#include <SFML/Graphics.hpp>
#include <vector>

class Grid
{
public:
   Grid(int stepSize, int mapSize, int margin);
   ~Grid();

   bool isWall(int x, int y) const;
   bool isWall(Node* node) const;

   void         addPath(std::vector<sf::Vector2i>& path, sf::RenderWindow& window);
   void         gridReset();
   void         draw(sf::RenderWindow& window);
   void         putWall(int x, int y);
   void         removeWall(int x, int y);
   void         removeWalls();
   void         updateStart(int x, int y);
   void         updateEnd(int x, int y);
   bool         checkStart(int x, int y) const;
   bool         checkEnd(int x, int y) const;
   int          getMapSize() const;
   void         colourVisitedTile(const sf::Vector2i& loc);
   void         colourVisitingTile(const sf::Vector2i& loc);
   void         setTileColor(const sf::Vector2i& loc, const sf::Color& color);
   void         setTileColor(const int index, const sf::Color& color);
   sf::Vector2i getStart() const;
   sf::Vector2i getEnd() const;
   void         initMap();
   void         colorReset();

   sf::Color emptyColour;
   sf::Color wallColour;
   sf::Color startColour;
   sf::Color endColour;
   sf::Color visitedTileColour;
   sf::Color visitingTileColour;
   sf::Color shortestPathColour;

private:
   sf::Vector2i start;
   sf::Vector2i end;
   int          stepSize;
   int          mapSize;
   int          margin;

   std::vector<int>                 occupants;
   std::vector<sf::RectangleShape*> rectangles;
};

#endif  // GRID_H

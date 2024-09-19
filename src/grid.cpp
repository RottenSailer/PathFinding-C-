#include "ui/grid.h"

Grid::Grid(int stepSize, int mapSize, int margin)
  : stepSize(stepSize),
    mapSize(mapSize),
    margin(margin),
    emptyColour(sf::Color::White),
    wallColour(sf::Color::Black),
    startColour(sf::Color::Green),
    endColour(sf::Color::Blue),
    visitedTileColour(sf::Color::Cyan),
    visitingTileColour(sf::Color::Yellow),
    shortestPathColour(sf::Color::Red)
{
   start = sf::Vector2i(0, 0);
   end   = sf::Vector2i(mapSize - 1, mapSize - 1);

   this->rectangles.reserve(mapSize * mapSize);
   this->occupants.reserve(mapSize * mapSize);

   for (int i = 0; i < mapSize; ++i)
   {
      for (int j = 0; j < mapSize; ++j)
      {
         rectangles.push_back(new sf::RectangleShape(sf::Vector2f(stepSize, stepSize)));
      }
   }

   initMap();
}

Grid::~Grid()
{
   for (auto rect : rectangles)
   {
      delete rect;
   }
}

void Grid::initMap()
{
   for (int i = 0; i < mapSize; ++i)
   {
      for (int j = 0; j < mapSize; ++j)
      {
         auto* rect = rectangles[i * mapSize + j];

         auto index = i * mapSize + j;

         if (i == start.x && j == start.y)
         {
            rect->setFillColor(startColour);
            occupants[index] = 3;
         }
         else if (i == end.x && j == end.y)
         {
            rect->setFillColor(endColour);
            occupants[index] = 4;
         }
         else
         {
            rect->setFillColor(emptyColour);
            occupants[index] = 1;
         }

         rect->setPosition(sf::Vector2f(i * stepSize + margin, j * stepSize + margin));
         rect->setOutlineThickness(1.0f);
         rect->setOutlineColor(sf::Color::Black);
      }
   }
}

void Grid::colorReset()
{
   for (int i = 0; i < mapSize; ++i)
   {
      for (int j = 0; j < mapSize; ++j)
      {
         auto index = i * mapSize + j;
         auto* rect = rectangles[index];

         if (i == start.x && j == start.y)
         {
            rect->setFillColor(startColour);
            occupants[index] = 3;
         }
         else if (i == end.x && j == end.y)
         {
            rect->setFillColor(endColour);
            occupants[index] = 4;
         }
         else if (occupants[index] == 0)
         {
            rect->setFillColor(wallColour);
         }
         else
         {
            rect->setFillColor(emptyColour);
            occupants[index] = 1;
         }
      }
   }
}

void Grid::gridReset()
{
   occupants.assign(mapSize * mapSize, 1);
   initMap();
}

void Grid::draw(sf::RenderWindow& window)
{
   for (const auto rect : rectangles)
   {
      window.draw(*rect);
   }
}

bool Grid::isWall(Node* node) const
{
   auto x = node->location.x;
   auto y = node->location.y;

   if (x < 0 || x >= mapSize || y < 0 || y >= mapSize)
      return true;
   return occupants[x * mapSize + y] == 0;
}

bool Grid::isWall(int x, int y) const
{
   if (x < 0 || x >= mapSize || y < 0 || y >= mapSize)
      return true;
   return occupants[x * mapSize + y] == 0;
}

void Grid::putWall(int x, int y)
{
   if (x < 0 || x >= mapSize || y < 0 || y >= mapSize)
      return;
   int index        = x * mapSize + y;
   occupants[index] = 0;
   setTileColor(index, wallColour);
}

void Grid::removeWall(int x, int y)
{
   if (x < 0 || x >= mapSize || y < 0 || y >= mapSize)
      return;
   int index = x * mapSize + y;
   if (occupants[index] == 0)
   {
      occupants[index] = 1;
      setTileColor(index, emptyColour);
   }
}

void Grid::removeWalls()
{
   for (int i = 0; i < mapSize * mapSize; ++i)
   {
      if (occupants[i] == 0)
      {
         occupants[i] = 1;
         setTileColor(i, emptyColour);
      }
   }
}

void Grid::updateStart(int x, int y)
{
   if (x < 0 || x >= mapSize || y < 0 || y >= mapSize)
      return;

   // Reset old start
   int oldIndex        = start.x * mapSize + start.y;
   occupants[oldIndex] = 1;
   setTileColor(oldIndex, emptyColour);

   // Set new start
   start               = sf::Vector2i(x, y);
   int newIndex        = x * mapSize + y;
   occupants[newIndex] = 3;
   setTileColor(newIndex, startColour);
}

bool Grid::checkStart(int x, int y) const
{
   return start.x == x && start.y == y;
}

bool Grid::checkEnd(int x, int y) const
{
   return end.x == x && end.y == y;
}

void Grid::updateEnd(int x, int y)
{
   if (x < 0 || x >= mapSize || y < 0 || y >= mapSize)
      return;

   // Reset old end
   int oldIndex        = end.x * mapSize + end.y;
   occupants[oldIndex] = 1;
   setTileColor(oldIndex, emptyColour);

   // Set new end
   end                 = sf::Vector2i(x, y);
   int newIndex        = x * mapSize + y;
   occupants[newIndex] = 4;
   setTileColor(newIndex, endColour);
}

sf::Vector2i Grid::getStart() const
{
   return start;
}

sf::Vector2i Grid::getEnd() const
{
   return end;
}

int Grid::getMapSize() const
{
   return mapSize;
}

void Grid::colourVisitedTile(const sf::Vector2i& loc)
{
   if (checkStart(loc.x, loc.y) || checkEnd(loc.x, loc.y))
      return;

   setTileColor(loc, visitedTileColour);
}

void Grid::colourVisitingTile(const sf::Vector2i& loc)
{
   if (checkStart(loc.x, loc.y) || checkEnd(loc.x, loc.y))
      return;

   setTileColor(loc, visitingTileColour);
}

void Grid::setTileColor(const sf::Vector2i& loc, const sf::Color& color)
{
   int index = loc.x * mapSize + loc.y;
   rectangles.at(index)->setFillColor(color);
}

void Grid::setTileColor(const int index, const sf::Color& color)
{
   rectangles.at(index)->setFillColor(color);
}

void Grid::addPath(std::vector<sf::Vector2i>& path, sf::RenderWindow& window)
{
   for (auto loc : path)
   {
      setTileColor(loc, shortestPathColour);
      draw(window);
      window.display();
   }
}

void Grid::generateMaze(sf::RenderWindow& window)
{
   initMap();

   for (int i = 0; i < mapSize; ++i)
   {
      for (int j = 0; j < mapSize; ++j)
      {
         int index    = i * mapSize + j;
         int r        = rand() % 4;
         int safeArea = 2;
         if (r == 0 && !((i < safeArea && j < safeArea) || (i > mapSize - safeArea && j > mapSize - safeArea)))
         {
            occupants[index] = 0;
            setTileColor(index, wallColour);
            draw(window);
            window.display();
         }
      }
   }
}

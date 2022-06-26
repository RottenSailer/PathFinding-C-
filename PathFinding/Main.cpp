#include <SFML/Graphics.hpp>
#include <vector>
#include<iostream>
#include <stdlib.h>



class grid {

public:

    grid(int stepSize, int mapSize, int margin);
    ~grid();
    bool isWall(int x, int y);
    void addPath(std::vector<sf::Vector3i>& path);
    void draw(sf::RenderWindow& window);
    void generateRandomMap();
    void putWall(int x, int y);
    void removeWall(int x, int y);
    void removeWalls();
    void setStart(int x, int y);
    void setEnd(int x, int y);
    int getMapSize() const;
    sf::Vector2i getStart() const;
    sf::Vector2i getEnd() const;

private:
    int stepSize, mapSize, margin;
    sf::Vector2i start;
    sf::Vector2i end;

    std::vector<sf::RectangleShape*> rectangles;
    void initMap();
    std::vector<int> occupants;
};



grid::grid(int stepSize, int mapSize, int margin) {
    this->stepSize = stepSize;
    this->mapSize = mapSize;
    this->margin = margin;

    start = sf::Vector2i(0, 0);
    end = sf::Vector2i(mapSize-1,mapSize-1);

    this->rectangles.reserve(mapSize * mapSize);
    this->occupants.reserve(mapSize * mapSize);
    for (int i = 0; i < mapSize; ++i)
        for (int j = 0; j < mapSize; ++j)
            rectangles.push_back(new sf::RectangleShape(sf::Vector2f(stepSize, stepSize)));
    initMap();
}

grid::~grid()
{
    for (int i = 0; i < mapSize; ++i)
    {
        for (int j = 0; j < mapSize; ++j)
        {
            delete (rectangles.at(i * mapSize + j));
        }
    }
}

void grid::draw(sf::RenderWindow &window) {
    for (int i = 0; i < mapSize; ++i)
    {
        for (int j = 0; j < mapSize; ++j)
        {
            window.draw(sf::RectangleShape(*rectangles.at(i * mapSize + j)));
        }
    }

}

bool grid::isWall(int x, int y)
{
    if (x > mapSize - 1 || y > mapSize - 1 || y < 0 || x < 0)
        return 1;
    return occupants.at(x * mapSize + y) == 0;
}


void grid::addPath(std::vector<sf::Vector3i>& path)
{
    for (auto step : path)
    {
        int i = step.x;
        int j = step.y;
        occupants.at(i * mapSize + j) = 2;
        rectangles.at(i * mapSize + j)->setFillColor(sf::Color::Red);
    }
}

void grid::generateRandomMap()
{
    srand(3);
    for (int i = 0; i < mapSize; ++i)
    {
        for (int j = 0; j < mapSize; ++j)
        {
            int r = rand() % 4;
            rectangles.at(i * mapSize + j)->setPosition(sf::Vector2f(i * stepSize + margin, j * stepSize + margin));
            int safeArea = 2;
            if (r == 0 && !((i < safeArea && j < safeArea) || (i > mapSize - safeArea && j > mapSize - safeArea)))
            {
                rectangles.at(i * mapSize + j)->setFillColor(sf::Color::Black);
                occupants.at(i * mapSize + j) = 0;
            }
        }
    }
}

void grid::putWall(int x, int y)
{
    if (x + 1 > mapSize || y + 1 > mapSize || x < 0 || y < 0)
        return;
    occupants.at(x * mapSize + y) = 0;
    rectangles.at(x * mapSize + y)->setFillColor(sf::Color::Black);
}

void grid::removeWall(int x, int y)
{
    if (x + 1 > mapSize || y + 1 > mapSize || x < 0 || y < 0)
        return;
    if (occupants.at(x * mapSize + y) == 0)
    {
        occupants.at(x * mapSize + y) = 1;
        rectangles.at(x * mapSize + y)->setFillColor(sf::Color::White);
    }
}

void grid::removeWalls()
{
    initMap();
}

void grid::initMap()
{
    for (int i = 0; i < mapSize; ++i)
    {
        for (int j = 0; j < mapSize; ++j)
        {
            if (i == start.x && j == start.y)
            {
                rectangles.at(i * mapSize + j)->setFillColor(sf::Color::Green);
                occupants.push_back(3);
            }
            else if (i == end.x && j == end.y)
            {
                rectangles.at(i * mapSize + j)->setFillColor(sf::Color::Blue);
                occupants.push_back(4);
            }
            else
            {
                rectangles.at(i * mapSize + j)->setFillColor(sf::Color::White);
                occupants.push_back(1);
            }
            rectangles.at(i * mapSize + j)->setPosition(sf::Vector2f(i * stepSize + margin, j * stepSize + margin));
            rectangles.at(i * mapSize + j)->setOutlineThickness(1.0f);
            rectangles.at(i * mapSize + j)->setOutlineColor(sf::Color::Black);
        }
    }
}

void grid::setStart(int x, int y)
{
    if (x + 1 > mapSize || y + 1 > mapSize || x < 0 || y < 0)
        return;
    start = sf::Vector2i(x, y);
    initMap();
}

void grid::setEnd(int x, int y)
{
    if (x + 1 > mapSize || y + 1 > mapSize || x < 0 || y < 0)
        return;
    end = sf::Vector2i(x, y);
    initMap();
}

sf::Vector2i grid::getStart() const
{
    return start;
}

sf::Vector2i grid::getEnd() const
{
    return end;
}

int grid::getMapSize() const
{
    return mapSize;
}


/*----------------------------------------------------------------*/

void findNeighbors(sf::Vector3i current, std::vector<sf::Vector3i>& temp, grid& map)
{
    temp.erase(temp.begin(), temp.end());
    if (!map.isWall(current.x, current.y - 1))
        temp.emplace_back(current.x, current.y - 1, 1);
    if (!map.isWall(current.x + 1, current.y))
        temp.emplace_back(current.x + 1, current.y, 1);
    if (!map.isWall(current.x, current.y + 1))
        temp.emplace_back(current.x, current.y + 1, 1);
    if (!map.isWall(current.x - 1, current.y))
        temp.emplace_back(current.x - 1, current.y, 1);
}

void dijkstrasAlgorithm(sf::Vector3i& current, std::vector<bool>& visited, std::vector<sf::Vector3i>& path, grid& map)
{
    std::vector<sf::Vector3i> temp;
    findNeighbors(current, temp, map);
    for (auto neighbor : temp)
    {
    }
}

bool isUsedCoord(int x, int y, std::vector<sf::Vector2i>& coords)
{
    for (int i = 0; i < coords.size(); ++i)
    {
        if (coords.at(i).x == x && coords.at(i).y == y)
            return true;
    }
    return false;
}

void checkCoord(grid& map, int x, int y, int z, std::vector<sf::Vector2i>& usedCoords, std::vector<sf::Vector3i>& lastStep, std::vector<sf::Vector3i>& path)
{
    if (!map.isWall(x, y) && !isUsedCoord(x, y, usedCoords))
    {
        lastStep.emplace_back(x, y, z);
        usedCoords.emplace_back(x, y);
    }
}

void findPath(std::vector<sf::Vector3i>& path, grid& map)
{
    std::vector<sf::Vector3i> lastStep;
    std::vector<sf::Vector2i> usedCoords;
    usedCoords.emplace_back(path.at(0).x, path.at(0).y);
    lastStep.push_back(path.at(0));

    int counter = 0;
    while (counter < 2000)
    {
        int size = lastStep.size();
        for (int i = 0; i < size; ++i)
        {
            auto step = lastStep.at(i);
            checkCoord(map, step.x, step.y - 1, path.size() - size + 1 + i, usedCoords, lastStep, path);
            checkCoord(map, step.x + 1, step.y, path.size() - size + 1 + i, usedCoords, lastStep, path);
            checkCoord(map, step.x, step.y + 1, path.size() - size + 1 + i, usedCoords, lastStep, path);
            checkCoord(map, step.x - 1, step.y, path.size() - size + 1 + i, usedCoords, lastStep, path);
            checkCoord(map, step.x + 1, step.y - 1, path.size() - size + 1 + i, usedCoords, lastStep, path);
            checkCoord(map, step.x + 1, step.y + 1, path.size() - size + 1 + i, usedCoords, lastStep, path);
            checkCoord(map, step.x - 1, step.y + 1, path.size() - size + 1 + i, usedCoords, lastStep, path);
            checkCoord(map, step.x - 1, step.y - 1, path.size() - size + 1 + i, usedCoords, lastStep, path);
        }

        lastStep.erase(lastStep.begin(), lastStep.begin() + size);
        for (auto step : lastStep)
        {
            path.push_back(step);
            if (step.x == map.getStart().x && step.y == map.getStart().y)
            {
                std::cout << "Finished" << std::endl;
                return;
            }
        }
        ++counter;
    }
}

void clearPaths(std::vector<sf::Vector3i>& path, grid& map)
{
    std::vector<sf::Vector3i> temp;
    sf::Vector3i start;
    for (auto step : path)
    {
        if (step.x == map.getStart().x && step.y == map.getStart().y)
        {
            start = step;
        }
    }
    temp.push_back(start);
    int i = start.z;
    while (i > 0)
    {
        //std::cout << path.at(i - 1).x << " " << path.at(i - 1).y << std::endl;
        temp.push_back(path.at(i - 1));
        i = path.at(i - 1).z;
    }
    path.erase(path.begin(), path.end());
    for (auto step : temp)
    {
        path.push_back(step);
    }
}

int main()
{
    sf::Font font;
    if (!font.loadFromFile("arial.ttf"))
    {
        std::cout << "Could not load font.." << std::endl;
        return 0;
    }

    sf::RenderWindow window(sf::VideoMode(1280, 1280), "PathFinder", sf::Style::Close);

    sf::Text text;
    text.setFont(font);
    text.setString("  C - Clear map            S - Start position               E - End position            R - Random map          B - Begin pathing");
    text.setFillColor(sf::Color::Black);
    text.setCharacterSize(24);

    int mapSize = 50;
    int margin = 50;
    grid map((1280 - margin * 2) / mapSize, mapSize, margin);

    std::vector<sf::Vector3i> path;

    bool pathDone = false;

    while (window.isOpen())
    {
        sf::Event Event;

        while (window.pollEvent(Event))
        {
            if (Event.type == sf::Event::Closed)
                window.close();

            if (Event.type == sf::Event::KeyPressed && !pathDone)
            {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
                {
                    map.removeWalls();
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
                {
                    map.generateRandomMap();
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
                {
                    int mx = sf::Mouse::getPosition(window).x - margin;
                    int my = sf::Mouse::getPosition(window).y - margin;
                    if (!(mx < 0 || my < 0 || mx > 1280 - margin || my > 1280 - margin))
                    {
                        int x = mx / ((1280 - 2 * margin) / mapSize);
                        int y = my / ((1280 - 2 * margin) / mapSize);
                        map.setStart(x, y);
                    }
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
                {
                    int mx = sf::Mouse::getPosition(window).x - margin;
                    int my = sf::Mouse::getPosition(window).y - margin;
                    if (!(mx < 0 || my < 0 || mx > 1280 - margin || my > 1280 - margin))
                    {
                        int x = mx / ((1280 - 2 * margin) / mapSize);
                        int y = my / ((1280 - 2 * margin) / mapSize);
                        map.setEnd(x, y);
                    }
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::B))
                {
                    //starting coords of our algorithm
                    path.emplace_back(map.getEnd().x, map.getEnd().y, 0);
                    findPath(path, map);
                    clearPaths(path, map);
                    map.addPath(path);
                    pathDone = true;
                }
            }

            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                int mx = sf::Mouse::getPosition(window).x - margin;
                int my = sf::Mouse::getPosition(window).y - margin;
                if (!(mx < 0 || my < 0 || mx > 1280 - margin || my > 1280 - margin))
                {
                    int x = mx / ((1280 - 2 * margin) / mapSize);
                    int y = my / ((1280 - 2 * margin) / mapSize);
                    map.putWall(x, y);
                }
            }
            else if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
            {
                int mx = sf::Mouse::getPosition(window).x - margin;
                int my = sf::Mouse::getPosition(window).y - margin;
                if (!(mx < 0 || my < 0 || mx > 1280 - margin || my > 1280 - margin))
                {
                    int x = mx / ((1280 - 2 * margin) / mapSize);
                    int y = my / ((1280 - 2 * margin) / mapSize);
                    map.removeWall(x, y);
                }
            }
        }

        window.clear(sf::Color::White);
        map.draw(window);
        if (!pathDone)
            window.draw(text);
        window.display();
    }
}

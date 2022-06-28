#include <SFML/Graphics.hpp>
#include <vector>
#include<iostream>
#include <stdlib.h>
#include<thread>
#include <string>
#include <assert.h>
#include <unordered_set>
#include <list>
#include <algorithm>


class grid {

public:

    grid(int stepSize, int mapSize, int margin);
    ~grid();
    bool isWall(int x, int y);
    void addPath(std::vector<sf::Vector3i>& path, sf::RenderWindow& window);
    void gridReset();
    void draw(sf::RenderWindow& window);
    void generateMaze(grid& map,sf::RenderWindow& createwindow);
    void putWall(int x, int y);
    void removeWall(int x, int y);
    void removeWalls();
    void updateStart(int x, int y);
    bool checkStart(int x, int y);
    bool checkEnd(int x, int y);
    void updateEnd(int x, int y);
    int getMapSize() const;
    void ColourVisitedTile(sf::Vector2i loc);
    void ColourVisitingTile(sf::Vector2i loc);
    void setTileColor(sf::Vector2i loc, const sf::Color& color);
    sf::Vector2i getStart() const;
    sf::Vector2i getEnd() const;
    std::vector<sf::Vector2i> obstacles;
    void initMap();
    std::vector<int> occupants;
    std::vector<sf::RectangleShape*> rectangles;
    int stepSize, mapSize, margin;
    std::vector<sf::Vector2i> visitedNodes;

private:
    sf::Vector2i start;
    sf::Vector2i end;
    std::vector<sf::RectangleShape> TileMap;


    sf::Color emptyColour = sf::Color::White;
    sf::Color wallColour = sf::Color::Black;
    sf::Color startColour = sf::Color::Red;
    sf::Color endColour = sf::Color(255, 69, 0);
    sf::Color visitedTileColour = sf::Color::Cyan;
    sf::Color visitngTileColour = sf::Color::Yellow;
    sf::Color shortestPath = sf::Color::Yellow;



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

void grid::gridReset()
{
    occupants.clear();
    initMap();
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

void grid::updateStart(int x, int y)
{
    if (x + 1 > mapSize || y + 1 > mapSize || x < 0 || y < 0)
        return;
    start = sf::Vector2i(x, y);
    initMap();
}

bool grid::checkStart(int x, int y)
{
    if (start.x == x && start.y == y) {
        return true;
    }
    return false;

}

bool grid::checkEnd(int x, int y)
{
    if (end.x == x && end.y == y) {
        return true;
    }
    return false;

}

void grid::updateEnd(int x, int y)
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
   
        temp.push_back(path.at(i - 1));
        i = path.at(i - 1).z;
    }
    path.erase(path.begin(), path.end());
    for (auto step : temp)
    {
        path.push_back(step);
    }
}



struct node {
    node(int x, int y);
    sf::Vector2i nodeloc;	
    bool Visited = false;
    bool mazeEdge[4];
    node* parent = nullptr;	
    bool isObstacle = false;
    double hCost = INFINITY;			
    double gCost = INFINITY;	
    double fCost;	
    std::vector<node*> neighbours;
};

class Graph {

private:
    int totalXTiles;
    int totalYTiles;
    std::vector<node> nodes;

public:
    Graph();
    node& getNode(sf::Vector2i loc);
    void resetGraph();
};

class AStar {
public:
    AStar(Graph& graph);
    virtual void solveAlgorithm(sf::Vector2i src, sf::Vector2i target, grid& map, sf::RenderWindow& createwindow);
    void constructPath(grid& grid, sf::RenderWindow& createwindow);
    void resetAlgorithms();

private:
    Graph& graph;
    sf::Vector2i srcpos;
    sf::Vector2i targetpos;
    sf::Text text;
    sf::Font font;
    bool targetreached = false;
    std::list<node*> pq;
    std::unordered_set<node*> openSet;
    double nodedistance(node* a, node* b);
};

class Dijkstra {

public:
    Dijkstra(Graph& graph);
    virtual void SolveAlgorithm(sf::Vector2i src, sf::Vector2i target, grid& map, sf::RenderWindow& createwindow);
    void constructPath(grid& grid, sf::RenderWindow& createwindow);
    void resetAlgorithm();


private:
    Graph& graph;
    sf::Vector2i srcpos;
    sf::Vector2i targetpos;
    sf::Text text;
    sf::Font font;
    bool targetreached = false;
    std::list<node*> pq;
    std::unordered_set<node*> openSet;
    double nodedistance(node* a, node* b);
};


node::node(int x, int y) {
    nodeloc.x = x;
    nodeloc.y = y;
}

void Dijkstra::resetAlgorithm() {
    pq.clear();
    openSet.clear();
    targetreached = false;
}

void AStar::resetAlgorithms() {
    pq.clear();
    openSet.clear();
    targetreached = false;
}


Graph::Graph()
{
    this->totalXTiles = 50;
    this->totalYTiles = 50;

    for (int i = 0; i < totalXTiles; ++i) {
        for (int j = 0; j < totalYTiles; ++j) {
            nodes.emplace_back(i, j);
        }
    }

    assert(nodes.size() == totalXTiles * totalYTiles);

    for (int x = 0; x < totalXTiles; ++x) {
        for (int y = 0; y < totalYTiles; ++y) {

            if (y > 0)
                nodes[x * totalXTiles + y].neighbours.emplace_back(&nodes[x * totalXTiles + (y - 1)]);
            if (y < totalXTiles - 1)
                nodes[x * totalXTiles + y].neighbours.emplace_back(&nodes[x * totalXTiles + (y + 1)]);
            if (x > 0)
                nodes[x * totalXTiles + y].neighbours.emplace_back(&nodes[(x - 1) * totalXTiles + y]);
            if (x < totalXTiles - 1)
                nodes[x * totalXTiles + y].neighbours.emplace_back(&nodes[(x + 1) * totalXTiles + y]);


            if (y > 0 && x > 0)
                nodes[x * totalXTiles + y].neighbours.emplace_back(&nodes[(x - 1) * totalXTiles + (y - 1)]);
            if (y < totalXTiles - 1 && x > 0)
                nodes[x * totalXTiles + y].neighbours.emplace_back(&nodes[(x - 1) * totalXTiles + (y + 1)]);
            if (y > 0 && x < totalXTiles - 1)
                nodes[x * totalXTiles + y].neighbours.emplace_back(&nodes[(x + 1) * totalXTiles + (y - 1)]);
            if (y < totalXTiles - 1 && x < totalXTiles - 1)
                nodes[x * totalXTiles + y].neighbours.emplace_back(&nodes[(x + 1) * totalXTiles + (y + 1)]);
        }
    }
}



void grid::generateMaze(grid& map,sf::RenderWindow& createwindow)
{
    initMap();

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
                map.draw(createwindow);
                createwindow.display();

                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
        }
    }
}

Dijkstra::Dijkstra(Graph& graph)
    :graph(graph)
{
    if (!font.loadFromFile("arial.ttf")) {
        std::cout << "Could not load font.." << std::endl;
    }
    text.setFont(font);
    text.setString("Solving Dijsktra..");
    text.setPosition(sf::Vector2f(5, 0));
    text.setFillColor(sf::Color::Green);
    text.setOutlineColor(sf::Color::Black);
    text.setOutlineThickness(2);
    text.setCharacterSize(25);
}

AStar::AStar(Graph& graph)
    :graph(graph)
{
    if (!font.loadFromFile("arial.ttf")) {
        std::cout << "Could not load font.." << std::endl;
    }
    text.setFont(font);
    text.setString("Solving AStar..");
    text.setPosition(sf::Vector2f(5, 0));
    text.setFillColor(sf::Color::Green);
    text.setOutlineColor(sf::Color::Black);
    text.setOutlineThickness(2);
    text.setCharacterSize(25);
}

void Graph::resetGraph()
{
    for (auto& node : nodes) {
        node.fCost = INFINITY;
        node.gCost = INFINITY;
        node.hCost = INFINITY;
        node.isObstacle = false;
        node.Visited = false;
        node.parent = nullptr;
    }
}

node& Graph::getNode(sf::Vector2i loc)
{
    assert(loc.x * 50 + loc.y < 50 * 50);
    assert(loc.x * 50 + loc.y >= 0);
    return nodes[loc.x * 50 + loc.y];
}


void AStar::solveAlgorithm(sf::Vector2i src, sf::Vector2i target, grid& map, sf::RenderWindow& createwindow)
{
    this->srcpos = map.getStart();
    this->targetpos = map.getEnd();;


    for (int i = 0; i < 50; i++) {
        for (int j = 0; j < 50; j++) {
            if ((map.isWall(i, j))) {
                graph.getNode(sf::Vector2i(i, j)).isObstacle = true;
            }
        }
    }

    auto fCostcomparator = [](node* leftnode, node* rightnode)
    {
        return leftnode->fCost < rightnode->fCost;
    };

    node* srcNode = &(graph.getNode(srcpos));
    srcNode->gCost = 0;
    node* targetNode = &(graph.getNode(targetpos));
    srcNode->hCost = nodedistance(srcNode, targetNode);
    pq.emplace_back(srcNode);				

    while (!pq.empty() && !targetreached) {		
        pq.sort(fCostcomparator);
        node* curr = pq.front();
        pq.pop_front();							
        curr->Visited = true;

        if (!map.isWall(curr->nodeloc.x, curr->nodeloc.y)) {
            map.ColourVisitedTile(curr->nodeloc);
        }

        if (curr->nodeloc == targetpos) {	
            targetreached = true;
        }


        map.draw(createwindow);
        createwindow.display();

        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        for (auto& neighbour : curr->neighbours) {

            if (neighbour->Visited || neighbour->isObstacle) {
                continue;
            }

            else {
                double estimatedgCost = curr->gCost + nodedistance(curr, neighbour);
                if (estimatedgCost < neighbour->gCost) {		
                    neighbour->parent = curr;
                    neighbour->gCost = estimatedgCost;
                    neighbour->hCost = nodedistance(neighbour, targetNode);
                    neighbour->fCost = neighbour->gCost + neighbour->hCost;
                    auto find = openSet.find(neighbour);
                    if (find == openSet.end()) {		
                        if (!map.isWall(neighbour->nodeloc.x, neighbour->nodeloc.y))
                            map.ColourVisitingTile(neighbour->nodeloc);
                        pq.emplace_back(neighbour);
                        openSet.insert(neighbour);
                    }
                }
            }
        }
    }
}

void Dijkstra::constructPath(grid& grid, sf::RenderWindow& createwindow)
{
    node* traverse = &graph.getNode(targetpos);
    if (traverse != nullptr) {
        while (traverse->parent != nullptr) {
            grid.setTileColor(traverse->nodeloc, sf::Color::Red);
            traverse = traverse->parent;
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            grid.draw(createwindow);
            createwindow.display();
        }
    }
}



void Dijkstra::SolveAlgorithm(sf::Vector2i src, sf::Vector2i target, grid& map, sf::RenderWindow& createwindow)
{

    this->srcpos = map.getStart();
    this->targetpos = map.getEnd();;


    for (int i = 0;i<50;i++) {
        for (int j = 0; j < 50; j++) {
            if (map.isWall(i,j)) {
                graph.getNode(sf::Vector2i(i,j)).isObstacle = true;
            }
        }
    }

    auto gCostcomparator = [](node* leftnode, node* rightnode)
    {
        return leftnode->gCost < rightnode->gCost;
    };

    node* srcNode = &(graph.getNode(srcpos));
    srcNode->gCost = 0;
    node* targetNode = &(graph.getNode(targetpos));
    pq.emplace_back(srcNode);
    while (!pq.empty() && !targetreached) {		
        pq.sort(gCostcomparator);
        node* curr = pq.front();
        pq.pop_front();								
        curr->Visited = true;

        if (!map.isWall(curr->nodeloc.x, curr->nodeloc.y)) {
            map.ColourVisitedTile(curr->nodeloc);
        }
        if (curr->nodeloc == targetpos) {		
            std::cout << "reached" << std::endl;
            targetreached = true;
        }

        map.draw(createwindow);
        createwindow.display();
        std::this_thread::sleep_for(std::chrono::milliseconds(5));

        for (auto& neighbour : curr->neighbours) {

            if (neighbour->Visited || neighbour->isObstacle) {
                continue;	
            }

            else {
                double estimatedgCost = curr->gCost + nodedistance(curr, neighbour);
                if (estimatedgCost < neighbour->gCost) {			
                    neighbour->parent = curr;
                    neighbour->gCost = estimatedgCost;
                    auto find = openSet.find(neighbour);
                    if (find == openSet.end()) {		
                        if (!map.isWall(neighbour->nodeloc.x, neighbour->nodeloc.y))
                            map.ColourVisitingTile(neighbour->nodeloc);
                        pq.emplace_back(neighbour);
                        openSet.insert(neighbour);
                    }
                }
            }
        }
        /*
        */
    }

    return;
}

void AStar::constructPath(grid& grid, sf::RenderWindow& createwindow)
{
    node* traverse = &graph.getNode(targetpos);
    if (traverse != nullptr) {
        while (traverse->parent != nullptr) {
            grid.setTileColor(traverse->nodeloc, sf::Color::Red);
            traverse = traverse->parent;
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            grid.draw(createwindow);
            createwindow.display();
        }
    }
}


void grid::ColourVisitedTile(sf::Vector2i loc)
{
    if (loc.x == start.x && loc.y == start.y)
        return;
    if (loc.x == end.x && loc.y == end.y)
        return;
    setTileColor(loc, visitedTileColour);
}

void grid::ColourVisitingTile(sf::Vector2i loc)
{
    if (loc.x == start.x && loc.y == start.y)
        return;
    if (loc.x == end.x && loc.y == end.y)
        return;
    setTileColor(loc, visitngTileColour);
}

void grid::setTileColor(sf::Vector2i loc, const sf::Color& color)
{
        rectangles.at(loc.x * mapSize + loc.y)->setFillColor(color);
}

double Dijkstra::nodedistance(node* a, node* b)
{
    int dx = (a->nodeloc.x - b->nodeloc.x);
    int dy = (a->nodeloc.y - b->nodeloc.y);
    return sqrt((dx * dx) + (dy * dy));			
}


double AStar::nodedistance(node* a, node* b)
{
    int dx = (a->nodeloc.x - b->nodeloc.x);
    int dy = (a->nodeloc.y - b->nodeloc.y);
    return sqrt((dx * dx) + (dy * dy));			
}


void grid::addPath(std::vector<sf::Vector3i>& path,sf::RenderWindow& window)
{
    for (auto step : path)
    {
        int i = step.x;
        int j = step.y;
        occupants.at(i * mapSize + j) = 2;
        rectangles.at(i * mapSize + j)->setFillColor(sf::Color::Red);
        draw(window);
        window.display();
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
}

int main()
{
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")){
        std::cout << "Could not load font.." << std::endl;
        return 0;
    }

    int windowSize = 1280;
    sf::RenderWindow window(sf::VideoMode(windowSize, windowSize), "PathFinder", sf::Style::Close);

    sf::Text clearMap;
    clearMap.setFillColor(sf::Color::Black);
    clearMap.setFont(font);
    clearMap.setString("CLEAR");
    clearMap.setFillColor(sf::Color::Black);
    clearMap.setCharacterSize(18);

    sf::RenderTexture renTex_clear;
    renTex_clear.create(static_cast<unsigned int>(clearMap.getGlobalBounds().width), static_cast<unsigned int>(font.getLineSpacing(35)));
    renTex_clear.clear(sf::Color::Transparent);
    renTex_clear.draw(clearMap);
    renTex_clear.display();

    sf::Text randomMap;
    randomMap.setFillColor(sf::Color::Black);
    randomMap.setFont(font);
    randomMap.setString("MAZE");
    randomMap.setFillColor(sf::Color::Black);
    randomMap.setCharacterSize(18);

    sf::RenderTexture renTex_random;
    renTex_random.create(static_cast<unsigned int>(clearMap.getGlobalBounds().width), static_cast<unsigned int>(font.getLineSpacing(35)));
    renTex_random.clear(sf::Color::Transparent);
    renTex_random.draw(randomMap);
    renTex_random.display();

    sf::Text dijk;
    dijk.setFillColor(sf::Color::Black);
    dijk.setFont(font);
    dijk.setString("DIJK");
    dijk.setFillColor(sf::Color::Black);
    dijk.setCharacterSize(18);

    sf::RenderTexture renTex_dijk;
    renTex_dijk.create(static_cast<unsigned int>(clearMap.getGlobalBounds().width), static_cast<unsigned int>(font.getLineSpacing(35)));
    renTex_dijk.clear(sf::Color::Transparent);
    renTex_dijk.draw(dijk);
    renTex_dijk.display();


    sf::Text reset;
    reset.setFillColor(sf::Color::Black);
    reset.setFont(font);
    reset.setString("RESET");
    reset.setFillColor(sf::Color::Black);
    reset.setCharacterSize(18);

    sf::RenderTexture renTex_reset;
    renTex_reset.create(static_cast<unsigned int>(clearMap.getGlobalBounds().width), static_cast<unsigned int>(font.getLineSpacing(35)));
    renTex_reset.clear(sf::Color::Transparent);
    renTex_reset.draw(reset);
    renTex_reset.display();


    sf::Text Astar;
    Astar.setFillColor(sf::Color::Black);
    Astar.setFont(font);
    Astar.setString("Astar");
    Astar.setFillColor(sf::Color::Black);
    Astar.setCharacterSize(18);

    sf::RenderTexture renTex_Astar;
    renTex_Astar.create(static_cast<unsigned int>(clearMap.getGlobalBounds().width), static_cast<unsigned int>(font.getLineSpacing(35)));
    renTex_Astar.clear(sf::Color::Transparent);
    renTex_Astar.draw(Astar);
    renTex_Astar.display();



    sf::Sprite clear;
    clear.setTexture(renTex_clear.getTexture());
    clear.setPosition(100.0f,6.0f);

    sf::Sprite random;
    random.setTexture(renTex_random.getTexture());
    random.setPosition(300.0f, 6.0f);

    sf::Sprite dijks;
    dijks.setTexture(renTex_dijk.getTexture());
    dijks.setPosition(700.0f, 6.0f);

    sf::Sprite resets;
    resets.setTexture(renTex_reset.getTexture());
    resets.setPosition(500.0f, 6.0f);

    sf::Sprite Astars;
    Astars.setTexture(renTex_Astar.getTexture());
    Astars.setPosition(900.0f, 6.0f);

    float clearButtonWidth = clear.getLocalBounds().width;
    float clearButtonHeight = clear.getLocalBounds().height;

    float randomButtonWidth = random.getLocalBounds().width;
    float randomButtonHeight = random.getLocalBounds().height;

    float dijksButtonWidth = dijks.getLocalBounds().width;
    float dijksButtonHeight = dijks.getLocalBounds().height;

    float exitButtonWidth = resets.getLocalBounds().width;
    float exitButtonHeight = resets.getLocalBounds().height;

    float AstarsButtonWidth = Astars.getLocalBounds().width;
    float AstarsButtonHeight = Astars.getLocalBounds().height;

    int mapSize = 50;
    int margin = 50;
    grid map((1280 - margin * 2) / mapSize, mapSize, margin);

    Graph graph = Graph();
    Dijkstra dijkstra = Dijkstra(graph);
    AStar astar = AStar(graph);

    std::vector<sf::Vector3i> path;

    bool pathDone = false;

    while (window.isOpen())
    {
        sf::Event Event;

        while (window.pollEvent(Event))
        {

            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                int mx = sf::Mouse::getPosition(window).x - margin;
                int my = sf::Mouse::getPosition(window).y - margin;
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
                if (!(mx < 0 || my < 0 || mx > windowSize - margin || my > windowSize - margin))
                {
                    int x = mx / ((1280 - 2 * margin) / mapSize);
                    int y = my / ((1280 - 2 * margin) / mapSize);
                    std::cout << x << std::endl;
                    std::cout << y << std::endl;

                    if (map.checkStart(x, y)) {
                        while (window.waitEvent(Event)) {
                            mx = sf::Mouse::getPosition(window).x - margin;
                            my = sf::Mouse::getPosition(window).y - margin;
                            if (!(mx < 0 || my < 0 || mx > 1280 - margin || my > 1280 - margin))
                            {
                                x = mx / ((1280 - 2 * margin) / mapSize);
                                y = my / ((1280 - 2 * margin) / mapSize);
                                map.updateStart(x, y);
                                map.draw(window);
                                window.display();
                            }
                            if (Event.type == sf::Event::MouseButtonReleased) {
                                break;
                            }
                        }

                    }
                    else if (map.checkEnd(x, y)) {
                        while (window.waitEvent(Event)) {
                            int mx = sf::Mouse::getPosition(window).x - margin;
                            int my = sf::Mouse::getPosition(window).y - margin;
                            if (!(mx < 0 || my < 0 || mx > 1280 - margin || my > 1280 - margin))
                            {
                                int x = mx / ((1280 - 2 * margin) / mapSize);
                                int y = my / ((1280 - 2 * margin) / mapSize);
                                map.updateEnd(x, y);
                                map.draw(window);
                                window.display();
                            }
                            if (Event.type == sf::Event::MouseButtonReleased) {
                                break;
                            }
                        }

                    }
                    else {
                        map.putWall(x, y);
                    }
                }
                else if (clear.getGlobalBounds().contains(mousePosF)) {
                    map.removeWalls();
                    map.initMap();
                    
                }
                else if (resets.getGlobalBounds().contains(mousePosF)) {
                    path.clear();
                    bool pathDone = false;
                    graph.resetGraph();
                    dijkstra.resetAlgorithm();
                    astar.resetAlgorithms();
                    map.gridReset();

                }
                else if (random.getGlobalBounds().contains(mousePosF)) {
                    map.generateMaze(map,window);
                }
                else if (dijks.getGlobalBounds().contains(mousePosF)) {
                    dijkstra.SolveAlgorithm(map.getStart(),map.getEnd(),map,window);
                    dijkstra.constructPath(map,window);
                   
                }

                else if (Astars.getGlobalBounds().contains(mousePosF)) {
                    astar.solveAlgorithm(map.getStart(), map.getEnd(), map, window);
                    astar.constructPath(map, window);

                }
            }
            
            else if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
            {
                int mx = sf::Mouse::getPosition(window).x - margin;
                int my = sf::Mouse::getPosition(window).y - margin;
                if (!(mx < 0 || my < 0 || mx > windowSize - margin || my > windowSize - margin))
                {
                    int x = mx / ((1280 - 2 * margin) / mapSize);
                    int y = my / ((1280 - 2 * margin) / mapSize);
                    map.removeWall(x, y);
                }
            }

        }

        window.clear(sf::Color::White);
        map.draw(window);
        window.draw(clear);
        window.draw(random);
        window.draw(dijks);
        window.draw(resets);
        window.draw(Astars);
        window.display();
    }
}

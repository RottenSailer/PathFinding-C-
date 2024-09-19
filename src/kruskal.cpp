#include "algorithm/kruskal.h"

#include <algorithm>
#include <random>
#include <vector>

Kruskal::Kruskal(Graph& graph, Grid& grid, sf::RenderWindow& window) : graph(graph), grid(grid), window(window)
{
   width      = grid.getMapSize();
   height     = grid.getMapSize();
   totalCells = width * height;
}

void Kruskal::initializeUnionFind()
{
   parent.resize(totalCells);
   for (int i = 0; i < totalCells; ++i)
   {
      parent[i] = i;
   }
}

int Kruskal::findSet(int cell)
{
   if (parent[cell] != cell)
   {
      parent[cell] = findSet(parent[cell]);
   }
   return parent[cell];
}

void Kruskal::unionSet(int cell1, int cell2)
{
   int root1 = findSet(cell1);
   int root2 = findSet(cell2);
   if (root1 != root2)
   {
      parent[root2] = root1;
   }
}

void Kruskal::generateAllEdges()
{
   edges.clear();

   for (int row = 0; row < height; ++row)
   {
      for (int col = 0; col < width; ++col)
      {
         int current = getCellIndex(row, col);

         if (col < width - 2)
         {
            int right = getCellIndex(row, col + 2);
            edges.emplace_back(current, right);
         }

         if (row < height - 2)
         {
            int bottom = getCellIndex(row + 2, col);
            edges.emplace_back(current, bottom);
         }
      }
   }
}

void Kruskal::shuffleEdges()
{
   std::random_device rd;
   std::mt19937       g(rd());
   std::shuffle(edges.begin(), edges.end(), g);
}

void Kruskal::resetMaze()
{
   initializeUnionFind();
   generateAllEdges();
   shuffleEdges();

   sf::Vector2i start = grid.getStart();
   sf::Vector2i end   = grid.getEnd();

   for (int row = 0; row < height; ++row)
   {
      for (int col = 0; col < width; ++col)
      {
         if (col == start.x and row == start.y)
         {
            continue;
         }
         else if (col == end.x and row == end.y)
         {
            continue;
         }
         // else if (row % 2 == 0 and col % 2 == 0)
         // {
         //    continue;
         // }

         graph.getNode(col, row).isObstacle = true;
         grid.putWall(col, row);
      }
   }

   grid.draw(window);
   window.display();
}

void Kruskal::removeWall(int x1, int y1, int x2, int y2)
{
   int wallX = (x1 + x2) / 2;
   int wallY = (y1 + y2) / 2;

   graph.getNode(x1, y1).isObstacle = false;
   grid.removeWall(x1, y1);
   grid.draw(window);
   window.display();

   graph.getNode(x2, y2).isObstacle = false;
   grid.removeWall(x2, y2);
   grid.draw(window);
   window.display();

   graph.getNode(wallX, wallY).isObstacle = false;
   grid.removeWall(wallX, wallY);
   grid.draw(window);
   window.display();
}

int Kruskal::getNeighbourDir(int cell1, int cell2)
{
   int row1 = cell1 / width, col1 = cell1 % width;
   int row2 = cell2 / width, col2 = cell2 % width;

   if (col1 < col2)
      return 1;
   if (col1 > col2)
      return 3;
   if (row1 < row2)
      return 2;
   return 0;
}

// Connect cells in the spanning tree
void Kruskal::connectCells(int cell1, int cell2, std::vector<std::vector<int>>& spanning_tree)
{
   int direction                   = getNeighbourDir(cell1, cell2);
   spanning_tree[cell1][direction] = 1;

   int opposite_direction                   = getNeighbourDir(cell2, cell1);
   spanning_tree[cell2][opposite_direction] = 1;
}

void Kruskal::generateMaze()
{
   resetMaze();
   std::vector<std::vector<int>> spanning_tree(totalCells, std::vector<int>(4, 0));  // PARENT, LEFT, RIGHT, CHILD

   DisjointSet disjoint(totalCells);

   int validCellCount = (width / 2) * (height / 2);
   int connectedCells = 1;

   while (connectedCells < validCellCount)
   {
      int  edge_idx = rand() % edges.size();
      auto edge     = edges[edge_idx];
      int  cell1    = edge.from;
      int  cell2    = edge.to;

      if (disjoint.find(cell1) != disjoint.find(cell2))
      {
         disjoint.unionSet(cell1, cell2);
         connectCells(cell1, cell2, spanning_tree);

         auto coord1 = getCellCoordinates(cell1);
         auto coord2 = getCellCoordinates(cell2);
         removeWall(coord1.first, coord1.second, coord2.first, coord2.second);

         connectedCells++;
      }
   }
}

const std::vector<Kruskal::Edge>& Kruskal::getRemovedEdges() const
{
   return edges;
}

int Kruskal::getCellIndex(int row, int col) const
{
   return row * width + col;
}

std::pair<int, int> Kruskal::getCellCoordinates(int index) const
{
   return {index / width, index % width};
}

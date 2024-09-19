// Kruskal.cpp

//
#include <algorithm>
#include <random>
#include <vector>

#include "algorithm/algorithm.h"

Kruskal::Kruskal(Graph& graph, Grid& grid, sf::RenderWindow& window) : graph(graph), grid(grid), window(window)
{
   width      = grid.getMapSize();
   height     = grid.getMapSize();
   totalCells = width * height;

   resetMaze();
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

         if (col < width - 1)
         {
            int right = getCellIndex(row, col + 1);
            edges.emplace_back(current, right);
         }

         if (row < height - 1)
         {
            int bottom = getCellIndex(row + 1, col);
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

   for (int row = 0; row < height; ++row)
   {
      for (int col = 0; col < width; ++col)
      {
         graph.getNode(col, row).isObstacle = true;
         grid.putWall(col, row);
      }
   }
}

void Kruskal::removeWall(int x1, int y1, int x2, int y2)
{
   graph.getNode(x1, y1).isObstacle = false;
   grid.removeWall(x1, y1);
   graph.getNode(x2, y2).isObstacle = false;
   grid.removeWall(x2, y2);
}

void Kruskal::generateMaze()
{
   resetMaze();

   for (const auto& edge : edges)
   {
      int set1 = findSet(edge.from);
      int set2 = findSet(edge.to);

      if (set1 != set2)
      {
         unionSet(set1, set2);

         auto coord1 = getCellCoordinates(edge.from);
         auto coord2 = getCellCoordinates(edge.to);

         removeWall(coord1.first, coord1.second, coord2.first, coord2.second);
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


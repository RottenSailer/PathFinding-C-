#pragma once

#include <SFML/Graphics.hpp>

#include "graph/graph.h"
#include "ui/grid.h"




class DisjointSet
{
public:
   DisjointSet(int n)
   {
      parent.resize(n);
      rank.resize(n, 0);
      for (int i = 0; i < n; ++i)
         parent[i] = i;
   }

   int find(int u)
   {
      if (parent[u] != u)
         parent[u] = find(parent[u]);
      return parent[u];
   }

   void unionSet(int u, int v)
   {
      int rootU = find(u);
      int rootV = find(v);
      if (rootU != rootV)
      {
         if (rank[rootU] > rank[rootV])
            parent[rootV] = rootU;
         else if (rank[rootU] < rank[rootV])
            parent[rootU] = rootV;
         else
         {
            parent[rootV] = rootU;
            rank[rootU]++;
         }
      }
   }

private:
   std::vector<int> parent;
   std::vector<int> rank;
};


class Kruskal
{
private:
   struct Edge
   {
      int from;
      int to;

      Edge(int f, int t) : from(f), to(t)
      {}
   };

   int width;
   int height;
   int totalCells;

   std::vector<Edge> edges;
   std::vector<int>  parent;

   Graph&            graph;
   Grid&             grid;
   sf::RenderWindow& window;

   void initializeUnionFind();
   int  findSet(int cell);
   void unionSet(int cell1, int cell2);
   int  getCellIndex(int row, int col) const;
   int  getNeighbourDir(int cell1, int cell2);
   void connectCells(int cell1, int cell2, std::vector<std::vector<int>>& spanning_tree);
   void generateAllEdges();
   void shuffleEdges();
   void removeWall(int x1, int y1, int x2, int y2);
   void addEdge(int x1, int y1, int x2, int y2);
   void resetMaze();

   const std::vector<Edge>& getRemovedEdges() const;
   std::pair<int, int>      getCellCoordinates(int index) const;

public:
   Kruskal(Graph& graph, Grid& grid, sf::RenderWindow& window);

   void generateMaze();
};

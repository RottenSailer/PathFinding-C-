#include "graph/node.h"

Node::Node(int x, int y)
  : location(x, y),
    visited(false),
    parent(nullptr),
    isObstacle(false),
    hCost(std::numeric_limits<double>::infinity()),
    gCost(std::numeric_limits<double>::infinity()),
    fCost(std::numeric_limits<double>::infinity())
{}

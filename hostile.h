#ifndef HOSTILE_H
#define HOSTILE_H

#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <algorithm>
#include <string>

#include "gameelement.h"

struct Point
{
    int x, y;

    Point(int _x, int _y) : x(_x), y(_y) {}
};

class Hostile
{
private:
    struct Node
    {
        Point point;
        int distance;

        Node(Point _point, int _distance) : point(_point), distance(_distance) {}

        bool operator>(const Node& other) const {
            return distance > other.distance;
        }
    };

    std::vector<std::vector<int>> matrix_;

    bool isCellValidForMovement(int x, int y);

    Point position_;
    int previousElement_  = 0;

public:
    Hostile();
    Hostile(std::vector<std::vector<int>>& inputMatrix);

    std::vector<std::string> getPath(const Point& end);
    int getPreviousElement();
        int Primer();
    void setPreviousElement(int element);
    void setPosition(Point& pos);
    void setMatrix(std::vector<std::vector<int>>& inputMatrix);
    Point getPosition();
};

#endif // HOSTILE_H

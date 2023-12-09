// hostile.h
#ifndef HOSTILE_H
#define HOSTILE_H

#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <algorithm>

struct Point {
    int x, y;

    Point(int _x, int _y) : x(_x), y(_y) {}
};

class Hostile {

private:
    std::vector<std::vector<int>> matrix_;

    bool isValid(int x, int y);
    Point position_;

public:
    Hostile();
    Hostile(std::vector<std::vector<int>>& inputMatrix);

    std::vector<char> getPath(const Point& end);
    void setPosition(Point& pos);
    void setMatrix(std::vector<std::vector<int>>& inputMatrix);
    Point getPosition();
    int temp = 0;
};

#endif // HOSTILE_H

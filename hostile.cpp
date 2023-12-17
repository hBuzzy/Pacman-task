#include "hostile.h"

using namespace std;

Hostile::Hostile(std::vector<std::vector<int>>& inputMatrix) : position_(0, 0)
{}

Hostile::Hostile() : position_(0, 0)
{}

bool Hostile::isCellValidForMovement(int x, int y)
{
    int rows = matrix_.size();
    int columns = matrix_[0].size();
    return x >= GameElement::Empty && x < columns && y >= GameElement::Empty && y < rows && matrix_[y][x] != GameElement::Wall && matrix_[y][x] != GameElement::Hostile;
}

std::vector<Directions> getMoveDirections(const Point& start, const Point& end)
{
    int dx = end.x - start.x;
    int dy = end.y - start.y;

    std::vector<Directions> directions;

    if (dx > 0)
    {
        directions.push_back(Directions(Directions::Right));
    } else if (dx < 0)
    {
        directions.push_back(Directions(Directions::Left));
    }

    if (dy > 0)
    {
        directions.push_back(Directions(Directions::Down));
    }
    else if (dy < 0)
    {
        directions.push_back(Directions(Directions::Up));
    }

    return directions;
}

std::vector<Directions> Hostile::getPath(const Point& end)
{
    int rows = matrix_.size();
    int columns = matrix_[0].size();

    std::vector<Directions> directions;

    vector<vector<int>> distance(rows, vector<int>(columns, INT_MAX));
    vector<vector<Point>> path(rows, vector<Point>(columns, Point(-1, -1)));
    priority_queue<Node, vector<Node>, greater<Node>> priorityQueue;

    priorityQueue.push(Node(position_, 0));
    distance[position_.y][position_.x] = 0;

    int dx[] = {-1, 1, 0, 0};
    int dy[] = {0, 0, -1, 1};

    while (!priorityQueue.empty())
    {
        Node current = priorityQueue.top();
        priorityQueue.pop();

        if (current.point.x == end.x && current.point.y == end.y)
        {

            vector<Point> pathPoints;
            Point currentPoint = end;

            while (!(currentPoint.x == position_.x && currentPoint.y == position_.y))
            {
                pathPoints.push_back(currentPoint);
                currentPoint = path[currentPoint.y][currentPoint.x];
            }

            reverse(pathPoints.begin(), pathPoints.end());

            for (const Point& point : pathPoints)
            {
                int dx = point.x - currentPoint.x;
                int dy = point.y - currentPoint.y;

                if (dx == -1)
                {
                    directions.push_back(Directions(Directions::Left));
                }
                else if (dx == 1)
                {
                    directions.push_back(Directions(Directions::Right));
                }
                else if (dy == -1)
                {
                    directions.push_back(Directions(Directions::Up));
                }
                else if (dy == 1)
                {
                    directions.push_back(Directions(Directions::Down));
                }

                currentPoint = point;
            }
            cout << endl;

            return directions;
        }

        int stepWeight = 1;
        const int kDirections = 4;

        for (int i = 0; i < kDirections; ++i)
        {
            int nextX = current.point.x + dx[i];
            int nextY = current.point.y + dy[i];

            if (isCellValidForMovement(nextX, nextY))
            {
                int newDistance = current.distance + stepWeight;

                if (newDistance < distance[nextY][nextX])
                {
                    distance[nextY][nextX] = newDistance;
                    path[nextY][nextX] = current.point;
                    priorityQueue.push(Node(Point(nextX, nextY), newDistance));
                }
            }
        }
    }

    return std::vector<Directions>();
}

void Hostile::setPosition(Point& pos)
{
    position_ = pos;
}

void Hostile::setMatrix(std::vector<std::vector<int> > &matrix)
{
    matrix_ = matrix;
}

Point Hostile::getPosition()
{
    return position_;
}

int Hostile::Primer()
{

    vector<vector<int>> inputMatrix =
    {
        {0, 0, 0, 0, 0},
        {0, 1, 0, 1, 0},
        {0, 1, 0, 0, 1},
        {0, 1, 1, 0, 1},
        {1, 0, 0, 0, 0}
    };

    Hostile hostile(inputMatrix);

    Point start(4, 0);
    Point end(0, 3);

    hostile.setPosition(start);
    std::vector<Directions> pathDirections = hostile.getPath(end);

        if (pathDirections.empty())
        {
            cout << "No path found." << endl;
        }
        else
        {
            cout << "Shortest Path Directions: ";
            for (Directions direction : pathDirections)
            {
                cout << direction << " ";
            }
            cout << endl;
        }

    return 0;
}

void Hostile::setPreviousElement(int element)
{
    previousElement_ = element;
}

int Hostile::getPreviousElement()
{
   return previousElement_;
}

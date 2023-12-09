#include "hostile.h"

using namespace std;



Hostile::Hostile(std::vector<std::vector<int>>& inputMatrix) : position_(0, 0) {

}

Hostile::Hostile() : position_(0, 0) {

}

struct Node {
    Point point;
    int distance;

    Node(Point _point, int _distance) : point(_point), distance(_distance) {}

    // Перегрузка оператора "больше" для приоритетной очереди
    bool operator>(const Node& other) const {
        return distance > other.distance;
    }
};

bool Hostile::isValid(int x, int y) {
    int rows = matrix_.size();
    int cols = matrix_[0].size();
    return x >= 0 && x < cols && y >= 0 && y < rows && matrix_[y][x] != 1 && matrix_[y][x] != 3;
}

vector<char> getMoveDirections(const Point& start, const Point& end) {
    int dx = end.x - start.x;
    int dy = end.y - start.y;

    vector<char> directions;

    if (dx > 0) {
        directions.push_back('R');  // Вправо
    } else if (dx < 0) {
        directions.push_back('L');  // Влево
    }

    if (dy > 0) {
        directions.push_back('D');  // Вниз
    } else if (dy < 0) {
        directions.push_back('U');  // Вверх
    }

    return directions;
}

vector<char> Hostile::getPath(const Point& end) {
    int rows = matrix_.size();
    int cols = matrix_[0].size();

    vector<char> directions;

    vector<vector<int>> distance(rows, vector<int>(cols, INT_MAX));
    vector<vector<Point>> path(rows, vector<Point>(cols, Point(-1, -1)));
    priority_queue<Node, vector<Node>, greater<Node>> pq;

    pq.push(Node(position_, 0));
    distance[position_.y][position_.x] = 0;

    // Возможные смещения влево, вправо, вверх и вниз
    int dx[] = {-1, 1, 0, 0};
    int dy[] = {0, 0, -1, 1};

    while (!pq.empty()) {
        Node current = pq.top();
        pq.pop();

        if (current.point.x == end.x && current.point.y == end.y) {
            // Мы достигли конечной точки
            vector<Point> pathPoints;
            Point currentPoint = end;

            while (!(currentPoint.x == position_.x && currentPoint.y == position_.y)) {
                pathPoints.push_back(currentPoint);
                currentPoint = path[currentPoint.y][currentPoint.x];
            }

            reverse(pathPoints.begin(), pathPoints.end());

            for (const Point& point : pathPoints) {
                int dx = point.x - currentPoint.x;
                int dy = point.y - currentPoint.y;

                if (dx == -1) {
                    directions.push_back('L');
                } else if (dx == 1) {
                    directions.push_back('R');
                } else if (dy == -1) {
                    directions.push_back('U');
                } else if (dy == 1) {
                    directions.push_back('D');
                }

                currentPoint = point;
            }
            cout << endl;

            return directions;
        }

        for (int i = 0; i < 4; ++i) {
            int nextX = current.point.x + dx[i];
            int nextY = current.point.y + dy[i];

            if (isValid(nextX, nextY)) {
                int newDistance = current.distance + 1; // Вес каждого шага равен 1

                if (newDistance < distance[nextY][nextX]) {
                    distance[nextY][nextX] = newDistance;
                    path[nextY][nextX] = current.point;
                    pq.push(Node(Point(nextX, nextY), newDistance));
                }
            }
        }
    }

    // Если конечная точка недостижима, возвращаем пустой путь
    return vector<char>();
}

void Hostile::setPosition(Point& pos)
{
    position_ = pos;
}

void Hostile::setMatrix(std::vector<std::vector<int> > &inputMatrix)
{
    matrix_ = inputMatrix;
}

Point Hostile::getPosition()
{
    return position_;
}

int Primer() {
    // Пример использования
    vector<vector<int>> inputMatrix = {
        {0, 0, 0, 0, 0},
        {0, 1, 0, 1, 0},
        {0, 1, 0, 0, 1},
        {0, 1, 1, 0, 1},
        {1, 0, 0, 0, 0}
    };

    Hostile hostileObject(inputMatrix);

    Point start(4, 0);
    Point end(0, 3);

    hostileObject.setPosition(start);
    vector<char> pathDirections = hostileObject.getPath(end);

        if (pathDirections.empty()) {
            cout << "No path found." << endl;
        } else {
            cout << "Shortest Path Directions: ";
            for (char direction : pathDirections) {
                cout << direction << " ";
            }
            cout << endl;
        }

    return 0;
}

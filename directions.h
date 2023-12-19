#ifndef DIRECTIONS_H
#define DIRECTIONS_H

#include <string>

class Directions {
public:
    static const std::string Left;
    static const std::string Right;
    static const std::string Up;
    static const std::string Down;

    Directions(const std::string& type);

    std::string getType() const;

    friend std::ostream& operator<<(std::ostream& os, const Directions& direction);
    friend bool operator==(const Directions& left, const Directions& right);

private:
    std::string directionsType_;
};

#endif // DIRECTIONS_H

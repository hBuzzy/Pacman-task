#include "directions.h"

const std::string Directions::Left = "Left";
const std::string Directions::Right = "Right";
const std::string Directions::Up = "Up";
const std::string Directions::Down = "Down";

Directions::Directions(const std::string& type) : directionsType_(type) {}

std::string Directions::getType() const
{
    return directionsType_;
}

std::ostream& operator<<(std::ostream& os, const Directions& direction)
{
    os << direction.getType();
    return os;
}

bool operator==(const Directions& left, const Directions& right)
{
    return left.getType() == right.getType();
}

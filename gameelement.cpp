#include "gameelement.h"

GameElement::GameElement(ElementType type) : elementType_(type) {}

GameElement::ElementType GameElement::getType() const
{
    return elementType_;
}

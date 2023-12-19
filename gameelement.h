#ifndef GAMEELEMENT_H
#define GAMEELEMENT_H

class GameElement {
public:
    enum ElementType {
        Empty = 0,
        Wall = 1,
        Puckman = 2,
        Hostile = 3,
        Coin = 4
    };

    GameElement(ElementType type);

    ElementType getType() const;

private:
    ElementType elementType_;
};

#endif // GAMEELEMENT_H

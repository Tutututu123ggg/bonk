#ifndef BRICKCONTAINER_HPP
#define BRICKCONTAINER_HPP

#include <gui_generated/containers/BrickContainerBase.hpp>

class BrickContainer : public BrickContainerBase
{
public:
    BrickContainer();
    virtual ~BrickContainer() {}
    virtual void initialize();

    // --- CODE THÊM ---
    void setHP(int hp);
    int getHP();
    void setXY(int x, int y);
    // ----------------

protected:
    int health;
};

#endif // BRICKCONTAINER_HPP

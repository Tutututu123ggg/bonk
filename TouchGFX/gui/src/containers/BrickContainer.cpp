#include <gui/containers/BrickContainer.hpp>
#include <touchgfx/Color.hpp> // Để dùng hàm đổi màu

BrickContainer::BrickContainer()
{
}

void BrickContainer::initialize()
{
    BrickContainerBase::initialize();
}

// Gọi hàm gốc setXY của TouchGFX
void BrickContainer::setXY(int x, int y)
{
    BrickContainerBase::setXY(x, y);
}

int BrickContainer::getHP()
{
    return health;
}

void BrickContainer::setHP(int hp)
{
    health = hp;

    // Đổi màu viên gạch theo máu
    if (health >= 4)
    {
        box1.setColor(touchgfx::Color::getColorFromRGB(255, 0, 0));     // Đỏ
    }
    else if (health == 3)
    {
        box1.setColor(touchgfx::Color::getColorFromRGB(255, 165, 0));   // Cam
    }
    else if (health == 2)
    {
        box1.setColor(touchgfx::Color::getColorFromRGB(0, 0, 255));     // Xanh lam
    }
    else if (health == 1)
    {
        box1.setColor(touchgfx::Color::getColorFromRGB(0, 200, 0));     // Xanh lá
    }

    // Cập nhật số HP
    Unicode::snprintf(textHPBuffer, TEXTHP_SIZE, "%d", health);
    textHP.invalidate();
    box1.invalidate();
}


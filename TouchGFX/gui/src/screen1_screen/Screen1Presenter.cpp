#include <gui/screen1_screen/Screen1View.hpp>
#include <gui/screen1_screen/Screen1Presenter.hpp>

Screen1Presenter::Screen1Presenter(Screen1View& v)
    : view(v)
{
}

void Screen1Presenter::activate()
{
}

void Screen1Presenter::deactivate()
{
}
void Screen1Presenter::saveHighScore(int s) {
    model->setHighScore(s);
}
int Screen1Presenter::getHighScore()
{
    return model->getHighScore();
}
// --- ĐỊNH NGHĨA HÀM Ở ĐÂY ---
// Phải có "Screen1Presenter::" ở đằng trước
int Screen1Presenter::getMoveDirection()
{
    // Bây giờ nó đã hiểu "model" là gì (vì model nằm trong lớp cha của Presenter)
    return model->getMoveDirection();
}
// ----------------------------

#include <gui/screen2_screen/Screen2View.hpp>

Screen2View::Screen2View()
{

}

void Screen2View::setupScreen()
{
	Screen2ViewBase::setupScreen();
	// Lấy điểm cao nhất từ Model thông qua Presenter
	int topScore = presenter->getHighScore();
	Unicode::snprintf(textHighScoreBuffer, TEXTHIGHSCORE_SIZE, "%d", topScore);
	textHighScore.invalidate();
}
void Screen2View::startGameClicked()
{
    // Lệnh chuyển từ Screen2 sang Screen1
}
void Screen2View::tearDownScreen()
{
    Screen2ViewBase::tearDownScreen();
}

#include <gui/screen1_screen/Screen1View.hpp>
#include <touchgfx/Color.hpp>
#include "stm32f4xx_hal.h"

extern RNG_HandleTypeDef hrng;

// Khai báo các biến từ phía C (main.c)
extern "C" {
    extern volatile uint8_t play_bonk;
    extern volatile uint8_t play_sad_music;
}

Screen1View::Screen1View()
{
}

void Screen1View::setupScreen()
{
    Screen1ViewBase::setupScreen();

    currentScore = 0;
    updateScore(0); // Hàm này sẽ tự lấy highScore từ Presenter để hiển thị

    paddle.setWidth(PADDLE_MAX_W);
    paddle.invalidate();

    gameOverTimer = 0;
    isWaitingReset = false;

    initLevel();
    resetBall();
}

void Screen1View::tearDownScreen()
{
    Screen1ViewBase::tearDownScreen();
}

// --- HÀM RANDOM ---
int Screen1View::getRandomNumber(int min, int max)
{
    uint32_t random32bit = HAL_RNG_GetRandomNumber(&hrng);
    return (random32bit % (max - min + 1)) + min;
}

// --- CẬP NHẬT ĐIỂM ---
void Screen1View::updateScore(int points)
{
    currentScore += points;

    // Lấy kỷ lục hiện tại từ Model thông qua Presenter
    int topScore = presenter->getHighScore();

    if (currentScore > topScore)
    {
        // Nếu phá kỷ lục, lưu ngay vào Model
        presenter->saveHighScore(currentScore);
        topScore = currentScore;
    }

    // Cập nhật hiển thị lên màn hình
    Unicode::snprintf(textHighScoreBuffer, TEXTHIGHSCORE_SIZE, "%d", topScore);
    textHighScore.invalidate();

    Unicode::snprintf(textScoreBuffer, TEXTSCORE_SIZE, "%d", currentScore);
    textScore.invalidate();
}

// --- KHỞI TẠO LEVEL ---
void Screen1View::initLevel()
{
    int screenWidth = 240;
    int totalBricksW = (BRICK_COLS * BRICK_W) + ((BRICK_COLS - 1) * BRICK_GAP);
    int startX = (screenWidth - totalBricksW) / 2;
    int startY = 20;

    int index = 0;
    activeBrickCount = 0;

    for (int r = 0; r < BRICK_ROWS; r++)
    {
        for (int c = 0; c < BRICK_COLS; c++)
        {
            if(index >= TOTAL_BRICKS) break;

            int x = startX + c * (BRICK_W + BRICK_GAP);
            int y = startY + r * (BRICK_H + BRICK_GAP);

            bricks[index].setXY(x, y);
            int hp = getRandomNumber(1, 4);
            bricks[index].setHP(hp);
            bricks[index].setVisible(true);
            bricks[index].invalidate();

            if (bricks[index].getParent() == 0) add(bricks[index]);

            activeBrickCount++;
            index++;
        }
    }
}

void Screen1View::nextLevel()
{
    initLevel();
    int currentW = paddle.getWidth();
    if (currentW > PADDLE_MIN_W)
    {
        int newW = currentW*7/10;
        if (newW < PADDLE_MIN_W) newW = PADDLE_MIN_W;
        paddle.setWidth(newW);
        paddle.invalidate();
    }
}

void Screen1View::resetGame()
{
    resetBall();
    currentScore = 0;
    updateScore(0);
    paddle.setWidth(PADDLE_MAX_W);
    paddle.invalidate();
    initLevel();
}

void Screen1View::resetBall()
{
    ballX = 113;
    ballY = 200;
    ballVX = 3.0f;
    ballVY = -3.0f;
    ball.moveTo((int)ballX, (int)ballY);
}

bool Screen1View::checkCollision(touchgfx::Drawable& obj, float bX, float bY, float bW, float bH)
{
    return (bX < obj.getX() + obj.getWidth() &&
            bX + bW > obj.getX() &&
            bY < obj.getY() + obj.getHeight() &&
            bY + bH > obj.getY());
}

// 1. Logic kiểm tra qua màn
void Screen1View::checkSafeSpawn()
{
    if (activeBrickCount <= 0 && ballY > 150)
    {
        nextLevel();
    }
}

// 2. Logic di chuyển Paddle
void Screen1View::updatePaddle()
{
    int direction = presenter->getMoveDirection();
    if (direction != 0)
    {
        int speed = 8;
        int currentX = paddle.getX();
        currentX += (direction * speed);

        if (currentX < 0) currentX = 0;
        if (currentX > 240 - paddle.getWidth()) currentX = 240 - paddle.getWidth();

        paddle.moveTo(currentX, paddle.getY());
    }
}

// 3. Logic vật lý bóng
void Screen1View::updateBallPhysics()
{
    if (isWaitingReset) return;

    ballX += ballVX;

    if (ballX <= 0) {
        ballX = 0; ballVX = -ballVX;
        play_bonk = 1;
    } else if (ballX + ball.getWidth() >= 240) {
        ballX = 240 - ball.getWidth(); ballVX = -ballVX;
        play_bonk = 1;
    }

    for(int i=0; i<TOTAL_BRICKS; i++) {
        if (bricks[i].isVisible() && checkCollision(bricks[i], ballX, ballY, ball.getWidth(), ball.getHeight())) {
            if (ballVX > 0) ballX = bricks[i].getX() - ball.getWidth() - 1;
            else ballX = bricks[i].getX() + bricks[i].getWidth() + 1;

            ballVX = -ballVX;
            play_bonk = 1;
            int hp = bricks[i].getHP() - 1;
            if (hp > 0) bricks[i].setHP(hp);
            else {
                bricks[i].setVisible(false);
                bricks[i].invalidate();
                updateScore(10);
                activeBrickCount--;
            }
            break;
        }
    }

    ballY += ballVY;

    if (ballY <= 0) {
        ballY = 0; ballVY = -ballVY;
        play_bonk = 1;
    }

    // Rớt xuống đáy -> Game Over (Chuyển sang trạng thái chờ 2s)
    if (ballY > 320) {
        play_sad_music = 1;
        isWaitingReset = true;
        gameOverTimer = 0;
        return;
    }

    if (checkCollision(paddle, ballX, ballY, ball.getWidth(), ball.getHeight())) {
        ballY = paddle.getY() - ball.getHeight() - 1;
        ballVY = -std::abs(ballVY);
        play_bonk = 1;

        float centerPaddle = paddle.getX() + (paddle.getWidth() / 2.0f);
        float centerBall = ballX + (ball.getWidth() / 2.0f);
        float offset = (centerBall - centerPaddle) / (paddle.getWidth() / 2.0f);
        ballVX = offset * 4.0f;
    }

    for(int i=0; i<TOTAL_BRICKS; i++) {
        if (bricks[i].isVisible() && checkCollision(bricks[i], ballX, ballY, ball.getWidth(), ball.getHeight())) {
            if (ballVY > 0) ballY = bricks[i].getY() - ball.getHeight() - 1;
            else ballY = bricks[i].getY() + bricks[i].getHeight() + 1;
            play_bonk = 1;
            ballVY = -ballVY;

            int hp = bricks[i].getHP() - 1;
            if (hp > 0) bricks[i].setHP(hp);
            else {
                bricks[i].setVisible(false);
                bricks[i].invalidate();
                updateScore(10);
                activeBrickCount--;
            }
            break;
        }
    }

    ball.moveTo((int)ballX, (int)ballY);
}

void Screen1View::handleTickEvent()
{
    if (isWaitingReset)
    {
        gameOverTimer++;
        if (gameOverTimer >= 120) // Đợi đủ 2 giây nhạc buồn
        {
            isWaitingReset = false;

            // CHUYỂN SCREEN: Quay lại màn hình bắt đầu
            static_cast<FrontendApplication*>(Application::getInstance())->gotoScreen2ScreenNoTransition();
        }
    }
    else
    {
        checkSafeSpawn();
        updatePaddle();
        updateBallPhysics();
    }
}

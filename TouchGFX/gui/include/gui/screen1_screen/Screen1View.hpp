#ifndef SCREEN1VIEW_HPP
#define SCREEN1VIEW_HPP

#include <gui_generated/screen1_screen/Screen1ViewBase.hpp>
#include <gui/screen1_screen/Screen1Presenter.hpp>
#include <gui/containers/BrickContainer.hpp>
#include <cmath>
#include <cstdlib>

// --- CẤU HÌNH GAME ---
#define BRICK_ROWS 4
#define BRICK_COLS 5
#define BRICK_W 40
#define BRICK_H 15
#define BRICK_GAP 5
#define TOTAL_BRICKS (BRICK_ROWS * BRICK_COLS)

#define PADDLE_MAX_W 80
#define PADDLE_MIN_W 22
#define PADDLE_SHRINK_STEP 10

class Screen1View : public Screen1ViewBase
{
public:
    Screen1View();
    virtual ~Screen1View() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    virtual void handleTickEvent();
private:
    int gameOverTimer; // Biến đếm frame để delay
    bool isWaitingReset; // Trạng thái đang chờ reset
protected:
    BrickContainer bricks[TOTAL_BRICKS];

    float ballX, ballY;
    float ballVX, ballVY;

    int currentScore;
    //static int highScore;
    int activeBrickCount;

    // --- CÁC HÀM LOGIC GAME (ĐÃ CHIA NHỎ) ---
    void initLevel();
    void nextLevel();
    void resetBall();
    void resetGame();
    void updateScore(int points);

    // 3 Hàm mới để tách logic trong handleTickEvent
    void updatePaddle();       // Xử lý di chuyển thanh đỡ
    void updateBallPhysics();  // Xử lý vật lý bóng (di chuyển + va chạm)
    void checkSafeSpawn();     // Kiểm tra điều kiện qua màn an toàn

    int getRandomNumber(int min, int max);
    bool checkCollision(touchgfx::Drawable& obj, float bX, float bY, float bW, float bH);
};

#endif // SCREEN1VIEW_HPP

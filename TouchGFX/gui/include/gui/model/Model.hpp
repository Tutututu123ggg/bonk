#ifndef MODEL_HPP
#define MODEL_HPP

class ModelListener;

class Model
{
public:
    Model();

    void bind(ModelListener* listener)
    {
        modelListener = listener;
    }

    void tick();
    int getMoveDirection();
    void setHighScore(int score) { highScore = score; }
    int getHighScore() { return highScore; }
protected:
    ModelListener* modelListener;
    int currentDirection;
    int highScore;
};

#endif // MODEL_HPP

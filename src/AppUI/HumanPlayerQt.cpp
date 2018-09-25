#include "HumanPlayerQt.h"
#include <vector>


using namespace AppUI;

const std::map < size_t, std::vector<Qt::Key>> HumanPlayerQt::playerKeys = {
    std::pair<size_t,std::vector<Qt::Key>>(0,{Qt::Key::Key_W, Qt::Key::Key_S, Qt::Key::Key_A, Qt::Key::Key_D}),
    std::pair<size_t,std::vector<Qt::Key>>(1,{Qt::Key::Key_Up, Qt::Key::Key_Down, Qt::Key::Key_Left, Qt::Key::Key_Right})

};

HumanPlayerQt::HumanPlayerQt(Qt::Key up, Qt::Key down, Qt::Key left, Qt::Key right):
    QObject(),m_up(up),m_down(down),m_left(left),m_right(right)
{
}

HumanPlayerQt::HumanPlayerQt(std::vector<Qt::Key> keys):
    m_up(keys[0]),m_down(keys[1]),m_left(keys[2]),m_right(keys[3])
{

}

HumanPlayerQt::~HumanPlayerQt()
{}

SnakeMove HumanPlayerQt::GetNextAction(const GameState & gameState)
{
    SnakeMove move = GetUserInput();

    TranslateMoveToSnakeView(move, gameState);
    return move != SnakeMove::DOWN ? move : SnakeMove::FORWARD;
}

void HumanPlayerQt::processKeyEvent(QKeyEvent * keyEvent)
{
    if (keyEvent->key() == m_up) {
        m_direction = InputDirection::UP;
    } else if (keyEvent->key() == m_down) {
        m_direction = InputDirection::DOWN;
    } else if (keyEvent->key() == m_left) {
        m_direction = InputDirection::LEFT;
    } else if (keyEvent->key() == m_right) {
        m_direction = InputDirection::RIGHT;
    } 
}

SnakeMove HumanPlayerQt::GetUserInput() const
{
    switch (m_direction) {
        case InputDirection::UP: {
            return SnakeMove::FORWARD;
            break;
        }
        case InputDirection::DOWN: {
            return SnakeMove::DOWN;
            break;
        }
        case InputDirection::LEFT: {
            return SnakeMove::RIGHT;
            break;
        }
        case InputDirection::RIGHT: {
            return SnakeMove::LEFT;
            break;
        }
        default: {
            return SnakeMove::FORWARD;
            break;
        }
    };
}

void HumanPlayerQt::TranslateMoveToSnakeView(SnakeMove & move, const GameState & gameState) const
{
    const auto& snake = gameState.GetSnake(GetSnakeNumber());
    auto orientation = snake.GetOrientation();
    RotateLeft(move);
    if (orientation == Coordinate::RIGHT) {
        RotateLeft(move);
    } else if (orientation == Coordinate::DOWN) {
        RotateLeft(move);
        RotateLeft(move);
    } else if (orientation == Coordinate::LEFT) {
        RotateLeft(move);
        RotateLeft(move);
        RotateLeft(move);
    }
}

void HumanPlayerQt::RotateLeft(SnakeMove & move) const
{
    switch (move) {
        case SnakeMove::FORWARD:
            move = SnakeMove::LEFT;
            return;
        case SnakeMove::LEFT:
            move = SnakeMove::DOWN;
            return;
        case SnakeMove::DOWN:
            move = SnakeMove::RIGHT;
            return;
        case SnakeMove::RIGHT:
            move = SnakeMove::FORWARD;
            return;
    }
}


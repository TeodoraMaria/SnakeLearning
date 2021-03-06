#pragma once

#include "GameLogic/IPlayer.h"

#include <qobject.h>
#include <qwidget.h>
#include <qgraphicsitem.h>
#include <qevent.h>
#include <map>

namespace AppUI
{
    enum class InputDirection
    {
        DEFAULT,
        UP,
        DOWN,
        LEFT,
        RIGHT
    };

    class HumanPlayerQt :public QObject,public IPlayer
    {
        Q_OBJECT
    public:
        HumanPlayerQt(Qt::Key up, Qt::Key down, Qt::Key left,Qt::Key right);
        HumanPlayerQt(std::vector<Qt::Key> keys);
        ~HumanPlayerQt();

        // Inherited via IPlayer
        virtual SnakeMove GetNextAction(const GameState & gameState) override;

        void processKeyEvent(QKeyEvent* keyEvent );

        static const std::map<size_t,std::vector<Qt::Key>> playerKeys;

    private:
        SnakeMove GetUserInput() const;
        void TranslateMoveToSnakeView(SnakeMove& move, const GameState& gameState) const;
        void RotateLeft(SnakeMove& move) const;

        const Qt::Key m_up;
        const Qt::Key m_down;
        const Qt::Key m_left;
        const Qt::Key m_right;

        InputDirection m_direction = InputDirection::DEFAULT;
    };

}
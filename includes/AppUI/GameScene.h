#include "AppUI/IScene.h"
#include "GameLogic/Game.h"
#include "GraphicBoard.h"
#include <string>
#include <vector>
#include <qtimer.h>
#include <qevent.h>

namespace AppUI
{
    class GameScene :public IScene
    {
        Q_OBJECT
    public:

        GameScene(const std::string& name);
        ~GameScene();

        // Inherited via IScene
        virtual void createScene() override;
        virtual void release() override;

    protected:
        bool eventFilter(QObject* obj, QEvent* event)override;

    private slots:
        void runRound();
        void backButtonPressed();

    private:
        bool m_gameStarted=false;

        QTimer m_timer;
        QWidget* m_centralWidget;

        GraphicBoard* m_board;
        Game* m_game;

        std::vector<IPlayerPtr> m_players;

    };

}
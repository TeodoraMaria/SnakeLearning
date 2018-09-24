#pragma once
#include "AppUI/IScene.h"
#include "GameLogic/Game.h"
#include "GraphicBoard.h"
#include "OptionsScene.h"
#include "GameSettings.h"
#include "ui_GameScene.h"


#include <string>
#include <vector>
#include <qtimer.h>
#include <qevent.h>

namespace AppUI
{
    struct OptionsValues
    {
        size_t humanPlayers = 0;
        size_t geneticBots = 0;
        size_t supervizedBots = 0;
        size_t qLearningBots = 0;
        size_t normalBots = 0;
    };

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

    public slots:
        void updateSettings(const GameSettings& settings);

    protected slots:
        void runRound();
        void backButtonPressed();
        void updateScoreBoard();
        

    private:
        void addPlayersToTheGame();

        bool m_gameStarted=false;

        std::shared_ptr<Ui_GameScene> ui;

        QTimer m_timer;
        size_t m_timerSpeed = 100;
        QWidget* m_centralWidget;

        GraphicBoard* m_board;
        Game* m_game;

        GameSettings m_gameSettings;

        std::vector<IPlayerPtr> m_players;
        std::map<size_t, std::string> m_playerNames;
    };

}
#include "GameScene.h"
#include "ui_GameScene.h"
#include "GraphicCell.h"
#include "ApplicationModel.h"
#include "HumanPlayerQt.h"
#include "AI/HardCoded/SingleBot.hpp"

#include <memory>
#include <qstring.h>
#include <qgraphicsscene.h>
#include <qgraphicsitem.h>
#include <memory>


using namespace AppUI;

GameScene::GameScene(const std::string& name)
{
    m_sceneName = name; 
}

GameScene::~GameScene()
{
    
}

void GameScene::createScene()
{
    auto ui = std::make_unique<Ui_GameScene>();
    ui->setupUi(m_mainWindow.get());
    m_centralWidget = ui->centralwidget;
    ui->graphicsView->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    m_board = new GraphicBoard(550, 550);
    ui->graphicsView->setScene(m_board);
    
    GameOptions options;
    options.boardLength = 25;
    options.boardWidth = 25;
    options.numFoods = 25;
    options.saveGameplay = true;

    m_players = std::vector<IPlayerPtr>(
    {
         std::make_shared<HumanPlayerQt>(Qt::Key::Key_W, Qt::Key::Key_S, Qt::Key::Key_A, Qt::Key::Key_D),
         std::make_shared<HumanPlayerQt>(Qt::Key::Key_Up, Qt::Key::Key_Down, Qt::Key::Key_Left, Qt::Key::Key_Right),
         /*
         std::make_shared<AI::HardCoded::SingleBot>(),
         std::make_shared<AI::HardCoded::SingleBot>(),
         std::make_shared<AI::HardCoded::SingleBot>(),
         std::make_shared<AI::HardCoded::SingleBot>(),
         std::make_shared<AI::HardCoded::SingleBot>(),
         std::make_shared<AI::HardCoded::SingleBot>(),
         std::make_shared<AI::HardCoded::SingleBot>(),
         std::make_shared<AI::HardCoded::SingleBot>(),
         std::make_shared<AI::HardCoded::SingleBot>(),
         std::make_shared<AI::HardCoded::SingleBot>(),
         std::make_shared<AI::HardCoded::SingleBot>(),
         std::make_shared<AI::HardCoded::SingleBot>(),
         std::make_shared<AI::HardCoded::SingleBot>(),
         std::make_shared<AI::HardCoded::SingleBot>(),
         */
         //std::make_shared<AI::HardCoded::SingleBot>(),
         //std::make_shared<AI::HardCoded::SingleBot>(),
         //std::make_shared<AI::HardCoded::SingleBot>(),   
    });

    m_game = new Game(options, m_players);
    m_game->InitGame();
    m_board->updateBoard(m_game->GetGameState());

    m_timer.setInterval(300);

    m_mainWindow.get()->installEventFilter(this);

    QObject::connect(&m_timer, SIGNAL(timeout()), this, SLOT(runRound())); 
    QObject::connect(ui->pushButtonBack, SIGNAL(released()), this, SLOT(backButtonPressed()));
}

void GameScene::release()
{
   // m_mainWindow.get()->removeEventFilter(this);
}

bool GameScene::eventFilter(QObject * obj, QEvent * event)
{
    if (event->type() == QEvent::KeyPress) {
       
        if(m_gameStarted==false){
            m_gameStarted = true;
            m_timer.start();
            
        }
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);

        for (const auto& player : m_players) {
            auto human = dynamic_cast<HumanPlayerQt*>(player.get());
            if (human != nullptr) {
                human->processKeyEvent(keyEvent);
            }
        }
        return QObject::eventFilter(obj, event);
    } else {
        return QObject::eventFilter(obj, event);
    }
    return false;
}

void GameScene::backButtonPressed()
{
    emit sceneChange(ApplicationModel::startMenuSceneName);
}

void GameScene::runRound()
{  
    m_game->RunRound();
    m_board->updateBoard(m_game->GetGameState());  
}


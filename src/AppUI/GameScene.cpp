#include "GameScene.h"
#include "ui_GameScene.h"
#include "OptionsScene.h"
#include "GraphicCell.h"
#include "ApplicationModel.h"
#include "HumanPlayerQt.h"
#include "AI/HardCoded/SingleBot.hpp"
#include "AI/GeneticAlgorithm/GeneticBot.h"

#include "ConfigLoading/GeneticBotJson.h"


#include <sstream>
#include <memory>
#include <qstring.h>
#include <qgraphicsscene.h>
#include <qgraphicsitem.h>
#include <qstringlistmodel.h>
#include <qpalette.h>
#include <json.hpp>

using namespace AppUI;

GameScene::GameScene(const std::string& name)
{
    m_sceneName = name;
    
    m_timer.setInterval(m_timerSpeed);
    QObject::connect(&m_timer, SIGNAL(timeout()), this, SLOT(runRound()));
    QObject::connect(&m_timer, SIGNAL(timeout()), this, SLOT(updateScoreBoard()));
}

GameScene::~GameScene()
{
    
}

void GameScene::createScene()
{
    ui = std::make_shared<Ui_GameScene>();
    ui->setupUi(m_mainWindow.get());
    m_centralWidget = ui->centralwidget;

    ui->graphicsView->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    //ui->graphicsView->setAlignment(Qt::AlignCenter);
    m_board = new GraphicBoard(550, 550);
    ui->graphicsView->setScene(m_board);
        
    GameOptions options;
    options.boardLength = m_gameSettings.mapWidth;
    options.boardWidth =m_gameSettings.mapHeight;
    options.numFoods = m_gameSettings.foodCount;

    addPlayersToTheGame();

    m_game = new Game(options, m_players);
    m_game->InitGame();
    
    m_board->updateBoard(m_game->GetGameState());

    updateScoreBoard();

    m_mainWindow.get()->installEventFilter(this);

    QObject::connect(ui->pushButtonBack, SIGNAL(released()), this, SLOT(backButtonPressed()));
}

void GameScene::release()
{
    m_timer.stop();
    m_gameStarted = false;
    m_players.clear();
    delete m_game;
    delete m_centralWidget;
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

void GameScene::updateSettings(const GameSettings& settings)
{
    m_gameSettings = settings;
}


void GameScene::backButtonPressed()
{
    emit sceneChange(ApplicationModel::startMenuSceneName);
}

void GameScene::addPlayersToTheGame()
{
    size_t count = 11;
    for (size_t i = 0; i < m_gameSettings.nbHumanPlayers; i++) {
        m_players.push_back(std::make_shared<HumanPlayerQt>(HumanPlayerQt::playerKeys.at(i)));
        m_playerNames.emplace(count++, "Player" + std::to_string(i+1)+":");
    }

    for (size_t i = 0; i < m_gameSettings.nbGeneticBots; i++) {
        
        const auto filePath = "D:\\fac\\snake\\aux_files\\genetic\\TrainedGenetic.json";
       
        std::ifstream fileStream;

        fileStream.open(filePath);
        if (!fileStream.is_open()) {
            throw "Failed to open file.";
        }

        nlohmann::json fileJsonContent;

        fileStream >> fileJsonContent;
        std::shared_ptr<AI::GeneticAlgorithm::GeneticBot> player = fileJsonContent.get<std::shared_ptr<AI::GeneticAlgorithm::GeneticBot>>();
        m_players.push_back(player);


        m_playerNames.emplace(count++, "Genetic bot" + std::to_string(i + 1) + ":");
    }

    for (size_t i = 0; i < m_gameSettings.nbNormalBots; i++) {
        m_players.push_back(std::make_shared<AI::HardCoded::SingleBot>());

        m_playerNames.emplace(count++, "Normal bot" + std::to_string(i + 1) + ":");
    }

    for (size_t i = 0; i < m_gameSettings.nbQlearningBots; i++) {

        m_playerNames.emplace(count++, "Qlearning bot" + std::to_string(i + 1) + ":");
    }

    for (size_t i = 0; i < m_gameSettings.nbSupervisedBots; i++) {

        m_playerNames.emplace(count++, "Supervised bot" + std::to_string(i + 1) + ":");
    }

    
}

void GameScene::updateScoreBoard()
{
    ui->listWidgetScore->clear();
    std::vector<Snake> snakes=m_game->GetAllSnakes();

    std::sort(snakes.begin(), snakes.end(), [](auto& snake1,auto& snake2) {
        return snake1.GetScore()>snake2.GetScore();
    });

    for (size_t i = 0; i < snakes.size();i++) {
        Snake snake = snakes.at(i);
        std::stringstream score;
        score << m_playerNames[snake.GetSnakeNumber()]<<" "<<snake.GetScore();

        ui->listWidgetScore->addItem(QString::fromUtf8(score.str().c_str()));

        //TODO make function
        size_t red = ((snake.GetSnakeNumber() * 100 & 0xC0) >> 6) * 64;
        size_t green = ((snake.GetSnakeNumber() * 100 & 0x30) >> 4) * 64;
        size_t blue = ((snake.GetSnakeNumber() * 100 & 0x0C) >> 2) * 64;

        ui->listWidgetScore->item(i)->setForeground(QColor(red,green,blue));
    }
}

void GameScene::runRound()
{  
    m_game->RunRound();
    m_board->updateBoard(m_game->GetGameState());  
}


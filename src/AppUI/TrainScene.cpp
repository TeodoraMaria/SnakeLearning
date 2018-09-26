#include "TrainScene.h"
#include "ConfigLoading/GeneticBotJson.h" 
#include "ApplicationModel.h"

#include <QtConcurrent/qtconcurrentrun.h>
#include <memory>
#include <fstream>
#include <iomanip> 
#include <json.hpp>

#include "Utils/MathUtils.h"

#include <memory>
#include <thread>
#include <qcoreapplication.h>
#include <QtConcurrent/qtconcurrentrun.h>
#include <qfuturesynchronizer.h>


using namespace AppUI;


TrainScene::TrainScene(const std::string& name)
{
    m_sceneName = name;
    qRegisterMetaType<std::vector<double>>("std::vector<double>");
    qRegisterMetaType<size_t>("size_t");
    qRegisterMetaType<GameState>("GameState");

    QObject::connect(this, SIGNAL(graphValues(const std::vector<double>&)), this, SLOT(updateGraph(const std::vector<double>&)));
    QObject::connect(this, SIGNAL(loadingBar(double)), this, SLOT(updateLoadingBar(double)));
    QObject::connect(this, SIGNAL(gameState(GameState)), this, SLOT(updateGameScene(GameState)));
}

TrainScene::~TrainScene()
{
    delete m_chart;
    delete m_board;
}

void TrainScene::createScene()
{
    ui = std::make_shared<Ui_TrainScene>();
    ui->setupUi(m_mainWindow.get());

    m_centralWidget = ui->centralwidget;
    
    m_maxFitnessValues = new QLineSeries();
   // m_maxFitnessValues->setName("Max Fitness");
    m_avgFitnessValues = new QLineSeries();
    //m_avgFitnessValues->setName("Avg. Fitness");

    m_graphValues.resize(200);



    //TODO fix this
    m_chart = new QChart();
    m_board = new GraphicBoard(300, 300);
    
    m_chart->legend()->setVisible(true);
    m_chart->legend()->setAlignment(Qt::AlignBottom);
    
    for (auto& lineSeries : m_graphValues) {
        lineSeries = new QLineSeries();
        m_chart->addSeries(lineSeries);
    }

    m_chart->legend()->hide();
    m_chart->addSeries(m_maxFitnessValues);
    m_chart->addSeries(m_avgFitnessValues);
    m_chart->createDefaultAxes();

    m_chart->setTitle("Snake Generations");
   
    ui->graphicsView->setScene(m_board);

    ui->chartView->setChart(m_chart);
    ui->chartView->repaint();

    QStringList list = (QStringList() << "Genetic" << "Tabular QLearning" << "Deep QLearning"<<"Supervised");

    ui->comboBoxAlgorithm->addItems(list);
    

    QObject::connect(ui->pushButtonStart, SIGNAL(released()), this, SLOT(startButtonPressed()));
    QObject::connect(ui->pushButtonBack, SIGNAL(released()), this, SLOT(backButtonPressed()));
   
    QObject::connect(ui->pushButtonDisplay, SIGNAL(released()), this, SLOT(switchDisplayEnabled()));   
}

void TrainScene::release()
{
    m_chart->removeAllSeries();
    m_graphX = 0;
    m_graphY = 0;
}

void TrainScene::backButtonPressed()
{
    emit sceneChange(ApplicationModel::startMenuSceneName);
}

void TrainScene::startButtonPressed()
{
    //m_geneticAlg.setEpisodes(ui->spinBoxEpisodes->value());   

    AI::ITrainer::TrainCallbacks trainCallbacks;
    auto numEpisodes = trainCallbacks.numEpisodes = ui->spinBoxEpisodes->value();
    trainCallbacks.emitStepEpisode = [&, numEpisodes](size_t episode) {
        double value = static_cast<double>(Utils::Math::normalize(episode, 0, numEpisodes, 0, 100));
        emit(loadingBar(value));
    };

    trainCallbacks.emitGraphValues = [&](std::vector<double> values) {
        emit(graphValues(values));
    };

    trainCallbacks.emitDisplayGame = [&](IPlayerPtr player, size_t numOfSteps=150) {
        if (!m_displayEnabled) {
            return;
        }
        
        static GameOptions options;
        options.boardLength = 25;
        options.boardWidth = 25;
        options.numFoods = 10;

        auto game = Game(options, {player});
        //  m_game= new Game(options, players);
        game.InitGame();

        for (size_t i = 0; i < numOfSteps; i++) {
            auto currentScore = game.GetAllSnakes().at(0).GetScore();
            game.RunRound();
            emit gameState(game.GetGameState());
            std::this_thread::sleep_for(std::chrono::milliseconds(10));

            if (game.GetAllSnakes().at(0).GetScore() != currentScore) {
                i = 0;
            }
            if (game.EveryoneIsDead()) {
                break;
            }
        }
    };

    auto func = [&]() {
        auto bot = std::dynamic_pointer_cast<AI::GeneticAlgorithm::GeneticBot>(m_geneticAlg.Train(trainCallbacks));
        const auto filePath = "D:\\fac\\snake\\aux_files\\genetic\\TrainedGenetic.json";
        std::ofstream outFileStream(filePath);

        if (!outFileStream.is_open()) {
            std::cout << "Failed to open " << filePath << std::endl;
            return;
        }
        try {
            outFileStream << std::setw(2) << nlohmann::json(bot.get());
            std::cout << "was written" << std::endl;
            outFileStream.close();
        } catch (...) {
            std::cout << "Failed to save player." << std::endl;
        }
    };
    QFuture<void> future = QtConcurrent::run(func);
    ui->pushButtonStart->setDisabled(true);
    ui->pushButtonBack->setDisabled(true);
}

void TrainScene::updateGraph(const std::vector<double>& values)
{
    /*
    m_maxFitnessValues->append(m_graphX, values[0]);
    if (values.size() > 1) {
        m_avgFitnessValues->append(m_graphX, values[1]);
    }
    */

    for (size_t i = 0; i < values.size(); i++) {
        m_graphValues[i]->append(m_graphX,values[i]);
    }


    ui->chartView->chart()->axisY()->setRange(0, (long long)m_graphY);
    ui->chartView->chart()->axisX()->setRange(0, (long long)m_graphX);
    m_graphX++;

    auto maxValue = *std::max_element(values.begin(), values.end());

    m_graphY = maxValue > m_graphY ? maxValue : m_graphY;

    ui->chartView->chart()->update();      
}

void TrainScene::updateLoadingBar(double value)
{
    ui->progressBar->setValue(value);
    if (value == 100) {
        ui->pushButtonStart->setDisabled(false);
        ui->pushButtonBack->setDisabled(false);
    }
}

void AppUI::TrainScene::updateGameScene(GameState gamestate)
{
    m_board->updateBoard(gamestate);
}

void TrainScene::switchDisplayEnabled()
{
    m_displayEnabled = !m_displayEnabled;
}



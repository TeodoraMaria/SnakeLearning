#include "TrainScene.h"
#include "ConfigLoading/GeneticBotJson.h" 
#include "ApplicationModel.h"

#include <QtConcurrent/qtconcurrentrun.h>
#include <memory>
#include <fstream>
#include <iomanip> 
#include <json.hpp>


using namespace AppUI;


TrainScene::TrainScene(const std::string& name)
{
    m_sceneName = name;
    qRegisterMetaType<std::vector<double>>("std::vector<double>");
    qRegisterMetaType<size_t>("size_t");
    qRegisterMetaType<GameState>("GameState");

    QObject::connect(&m_geneticAlg, SIGNAL(graphValues(const std::vector<double>&)), this, SLOT(updateGraph(const std::vector<double>&)));
    QObject::connect(&m_geneticAlg, SIGNAL(loadingBar(double)), this, SLOT(updateLoadingBar(double)));
    QObject::connect(&m_geneticAlg, SIGNAL(gameState(GameState)), this, SLOT(updateGameScene(GameState))); 
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
    m_maxFitnessValues->setName("Max Fitness");
    m_avgFitnessValues = new QLineSeries();
    m_avgFitnessValues->setName("Avg. Fitness");

    //TODO fix this
    m_chart = new QChart();
    m_board = new GraphicBoard(300, 300);
    
    m_chart->legend()->setVisible(true);
    m_chart->legend()->setAlignment(Qt::AlignBottom);
    
    m_chart->addSeries(m_maxFitnessValues);
    m_chart->addSeries(m_avgFitnessValues);
    m_chart->createDefaultAxes();

    m_chart->setTitle("Snake Generations");
   
    ui->graphicsView->setScene(m_board);

    ui->chartView->setChart(m_chart);
    ui->chartView->repaint();
    QObject::connect(ui->pushButtonStart, SIGNAL(released()), this, SLOT(startButtonPressed()));
    QObject::connect(ui->pushButtonBack, SIGNAL(released()), this, SLOT(backButtonPressed()));
    QObject::connect(ui->pushButtonBack, SIGNAL(released()), &m_geneticAlg, SLOT(endGame()));
    QObject::connect(ui->pushButtonDisplay, SIGNAL(released()), &m_geneticAlg, SLOT(switchDisplayEnabled()));   
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
    m_geneticAlg.setEpisodes(ui->spinBoxEpisodes->value());

    auto func = [&]() {
        AI::GeneticAlgorithm::GeneticBot& bot = dynamic_cast<AI::GeneticAlgorithm::GeneticBot&>(*m_geneticAlg.Train());
        const auto filePath = "D:\\fac\\snake\\aux_files\\genetic\\TrainedGenetic.json";
        std::ofstream outFileStream(filePath);

        if (!outFileStream.is_open()) {
            std::cout << "Failed to open " << filePath << std::endl;
            return;
        }

        try {
            outFileStream << std::setw(2) << nlohmann::json(&bot);
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
    m_maxFitnessValues->append(m_graphX, values[0]);
    m_avgFitnessValues->append(m_graphX, values[1]);

    ui->chartView->chart()->axisX()->setRange(0, (long long)m_graphX);
    ui->chartView->chart()->axisY()->setRange(0, (long long)m_graphY);
    m_graphX++;
    m_graphY = values[0] > m_graphY ? values[0] : m_graphY;

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



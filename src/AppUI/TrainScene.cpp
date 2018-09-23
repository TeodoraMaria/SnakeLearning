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
    QObject::connect(&m_geneticAlg, SIGNAL(graphValues(const std::vector<double>&)), this, SLOT(updateGraph(const std::vector<double>&)));
}

TrainScene::~TrainScene()
{

}

void TrainScene::createScene()
{
    ui = std::make_shared<Ui_TrainScene>();
    ui->setupUi(m_mainWindow.get());

    m_centralWidget = ui->centralwidget;

    m_maxFitnessValues = new QLineSeries();
    m_avgFitnessValues = new QLineSeries();

    //*series << QPointF(11, 1) << QPointF(13, 3) << QPointF(17, 6) << QPointF(18, 3) << QPointF(20, 2);

    m_chart = new QChart();
    m_chart->legend()->hide();
    m_chart->addSeries(m_maxFitnessValues);
    m_chart->addSeries(m_avgFitnessValues);
    m_chart->createDefaultAxes();

    m_chart->setTitle("Simple line chart example");

    ui->chartView->setChart(m_chart);
    ui->chartView->repaint();
    QObject::connect(ui->pushButtonStart, SIGNAL(released()), this, SLOT(startButtonPressed()));
    QObject::connect(ui->pushButtonBack, SIGNAL(released()), this, SLOT(backButtonPressed()));
    
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
    auto func = [&]() {
        AI::GeneticAlgorithm::GeneticBot& bot = dynamic_cast< AI::GeneticAlgorithm::GeneticBot&>(*m_geneticAlg.Train());
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
    
}

void TrainScene::updateGraph(const std::vector<double>& values)
{
    m_maxFitnessValues->append(m_graphX, values[0]);
    m_avgFitnessValues->append(m_graphX, values[1]);

    m_graphX++;
    m_graphY = values[0] > m_graphY ? values[0] : m_graphY;

    ui->chartView->chart()->axisX()->setRange(0, m_graphX);
    ui->chartView->chart()->axisY()->setRange(0, m_graphY);

    ui->chartView->chart()->update();
}



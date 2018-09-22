#include "TrainScene.h"

#include <memory>

using namespace AppUI;


TrainScene::TrainScene(const std::string& name)
{
    m_sceneName = name;
}

TrainScene::~TrainScene()
{

}

void TrainScene::createScene()
{
    auto ui = std::make_unique<Ui_TrainScene>();
    ui->setupUi(m_mainWindow.get());

    m_centralWidget = ui->centralwidget;

    series = new QLineSeries();

    series->append(0, 6);
    series->append(2, 4);
    series->append(3, 8);
    series->append(7, 4);
    series->append(10, 5);
    //*series << QPointF(11, 1) << QPointF(13, 3) << QPointF(17, 6) << QPointF(18, 3) << QPointF(20, 2);

    QChart *chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->setTitle("Simple line chart example");

    ui->chartView->setChart(chart);
    QObject::connect(ui->pushButtonStart, SIGNAL(released()), this, SLOT(updateGraph()));
}

void TrainScene::release()
{

}

void TrainScene::backButtonPressed()
{

}

void TrainScene::startButtonPressed()
{

}

void TrainScene::updateGraph()
{
    series->clear();

    *series << QPointF(rand()%10, rand() % 10) 
        << QPointF(rand() % 10, rand() % 10) 
        << QPointF(rand() % 10, rand() % 10) 
        << QPointF(rand() % 10, rand() % 10) 
        << QPointF(rand() % 10, rand() % 10);
        
}



#include "OptionsScene.h"
#include "GameScene.h"
#include "ui_OptionsScene.h"
#include "ApplicationModel.h"

#include <memory>

using namespace AppUI;


OptionsScene::OptionsScene(const std::string& name)
{
    m_sceneName = name;
}
OptionsScene::~OptionsScene()
{

}
void OptionsScene::createScene()
{
    m_ui = std::make_shared<Ui_OptionsScene>();
    m_ui->setupUi(m_mainWindow.get());

    m_centralWidget = m_ui->centralwidget;

    m_ui->spinBoxHumans->setValue(m_gameSettings.nbHumanPlayers);
    m_ui->spinBoxGeneticBots->setValue(m_gameSettings.nbGeneticBots);
    m_ui->spinBoxQlearningBots->setValue(m_gameSettings.nbQlearningBots);
    m_ui->spinBoxSupervisedBots->setValue(m_gameSettings.nbSupervisedBots);
    m_ui->spinBoxNormalBots->setValue(m_gameSettings.nbNormalBots);

    m_ui->spinBoxMapWidth->setValue(m_gameSettings.mapWidth);
    m_ui->spinBoxMapHeight->setValue(m_gameSettings.mapHeight);
    m_ui->spinBoxFoodCount->setValue(m_gameSettings.foodCount);


    QObject::connect(m_ui->pushButton, SIGNAL(released()), this, SLOT(quitButtonPressed()));
}

void OptionsScene::release()
{

}


void OptionsScene::quitButtonPressed()
{   
    m_gameSettings.nbHumanPlayers = m_ui->spinBoxHumans->value();
    m_gameSettings.nbGeneticBots = m_ui->spinBoxGeneticBots->value();
    m_gameSettings.nbQlearningBots = m_ui->spinBoxQlearningBots->value();
    m_gameSettings.nbSupervisedBots = m_ui->spinBoxSupervisedBots->value();
    m_gameSettings.nbNormalBots = m_ui->spinBoxNormalBots->value();

    m_gameSettings.mapWidth = m_ui->spinBoxMapWidth->value();
    m_gameSettings.mapHeight = m_ui->spinBoxMapHeight->value();
    m_gameSettings.foodCount = m_ui->spinBoxFoodCount->value();

    emit settingsChanged(m_gameSettings);
    emit sceneChange(ApplicationModel::startMenuSceneName);
}


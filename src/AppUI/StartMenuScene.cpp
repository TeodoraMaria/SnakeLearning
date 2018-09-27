#include "StartMenuScene.h"
#include "GameScene.h"
#include "OptionsScene.h"
#include "ApplicationModel.h"
#include "ui_StartMenuScene.h"
#include <memory>

using namespace AppUI;

StartMenuScene::StartMenuScene(const std::string & name)
{
    m_sceneName = name;
}

StartMenuScene::~StartMenuScene()
{

}

void StartMenuScene::createScene()
{
    auto ui = std::make_unique<Ui_StartMenuScene>();
    ui->setupUi(m_mainWindow.get());
    m_mainWindow.get()->setWindowTitle("Snake Learning");
    
    m_centralWidget = ui->centralwidget;

    QObject::connect(ui->pushButtonStart, SIGNAL(released()), this, SLOT(startButtonPressed()));
    QObject::connect(ui->pushButtonOptions, SIGNAL(released()), this, SLOT(optionsButtonPressed()));
    QObject::connect(ui->pushButtonExit, SIGNAL(released()), this, SLOT(quitButtonPressed()));
    QObject::connect(ui->pushButtonTrain, SIGNAL(released()), this, SLOT(trainButtonPressed()));
}

void StartMenuScene::release()
{
    delete m_centralWidget;
}

void StartMenuScene::startButtonPressed()
{
    emit sceneChange(ApplicationModel::gameSceneName);
}
void StartMenuScene::optionsButtonPressed()
{
    emit sceneChange(ApplicationModel::optionsSceneName);
}

void StartMenuScene::quitButtonPressed()
{
    QCoreApplication::quit();
}

void StartMenuScene::trainButtonPressed()
{
    emit sceneChange(ApplicationModel::trainSceneName);
}


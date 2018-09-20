#include "OptionsScene.h"
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
    auto ui = std::make_unique<Ui_OptionsScene>();
    ui->setupUi(m_mainWindow.get());

    m_centralWidget = ui->centralwidget;

    QObject::connect(ui->pushButton, SIGNAL(released()), this, SLOT(quitButtonPressed()));
}

void OptionsScene::release()
{

}

void OptionsScene::quitButtonPressed()
{
    emit sceneChange(ApplicationModel::startMenuSceneName);
}


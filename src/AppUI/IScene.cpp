#include "IScene.h"

using namespace AppUI;

void IScene::drawScene()
{
    QMetaObject::connectSlotsByName(m_mainWindow.get());
}

std::string IScene::getSceneName()
{
    return m_sceneName;
}

std::unique_ptr<QMainWindow>& IScene::getWindow()
{
    return m_mainWindow;
}

void IScene::setWindow(std::unique_ptr<QMainWindow>& mainWindow)
{
    m_mainWindow=std::move(mainWindow);
}

void IScene::show() const
{
    m_mainWindow->show();
}

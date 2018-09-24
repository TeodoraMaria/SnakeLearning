#include "Application.h"

using namespace AppUI;

Application::Application(ApplicationModel * appModel)
{
    m_appModel = appModel;
    m_appModel->defineScenes();
    m_appModel->defineInitialScene();

}

Application::~Application()
{

}

void Application::start(const std::string & appTitle, size_t width, size_t height)
{
    m_mainWindow = std::make_unique<QMainWindow>();
    m_mainWindow->setWindowTitle(QString::fromUtf8(appTitle.c_str()));
    m_mainWindow->resize(width, height);

    m_sceneManager = new SceneManager(m_appModel->getScenes(),m_mainWindow, m_appModel->getInitialSceneName());
}

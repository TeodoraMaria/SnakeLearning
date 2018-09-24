#include "SceneManager.h"
#include <qcoreapplication.h>

using namespace AppUI;

SceneManager::SceneManager(std::map<const std::string, IScene*> scenes,
                           std::unique_ptr<QMainWindow>& mainWindow,
                           const std::string & initialScene)
    :m_scenes(scenes),
    m_mainWindow(std::move(mainWindow)),
    m_initialSceneName(initialScene),
    m_currentSceneName("")
{
    for (const auto& scene : m_scenes) {
        QObject::connect(scene.second, SIGNAL(sceneChange(const std::string&)), this, SLOT(setActiveScene(const std::string &)));
    }

    setActiveScene(m_initialSceneName);

    QCoreApplication* app = QCoreApplication::instance();
    connect(app, SIGNAL(aboutToQuit()), this, SLOT(releaseCurrent()));
}

SceneManager::~SceneManager()
{

}

void SceneManager::setActiveScene(const std::string& sceneName)
{
    if (m_currentSceneName != sceneName) {
        IScene* currentScene = nullptr;

        if (m_currentSceneName != "") {
            currentScene = m_scenes.find(m_currentSceneName)->second;
            m_mainWindow = std::move(currentScene->getWindow());
        }

        IScene* nextScene = m_scenes.find(sceneName)->second;

        if (nextScene != nullptr) {
            if (currentScene != nullptr) {
                currentScene->release();
            }

            nextScene->setWindow(m_mainWindow);
            nextScene->createScene();
            nextScene->drawScene();
            nextScene->show();
            m_currentSceneName = sceneName;
        }
    }
}

void SceneManager::releaseCurrent() const
{
    const auto& currentScene = m_scenes.find(m_currentSceneName)->second;
    if (currentScene != nullptr)
        currentScene->release();
}

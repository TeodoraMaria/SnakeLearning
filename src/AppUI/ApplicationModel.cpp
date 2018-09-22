#include "ApplicationModel.h"
#include "StartMenuScene.h"
#include "GameScene.h"
#include "OptionsScene.h"
#include "TrainScene.h"
#include "IScene.h"

using namespace AppUI;

const std::string ApplicationModel::startMenuSceneName = std::string("StartMenuScene");
const std::string ApplicationModel::gameSceneName = std::string("GameScene");
const std::string ApplicationModel::optionsSceneName= std::string("OptionsScene");
const std::string ApplicationModel::trainSceneName = std::string("TrainScene");

ApplicationModel* ApplicationModel::instance;

ApplicationModel::ApplicationModel()
{}

ApplicationModel::~ApplicationModel()
{
    delete instance;
}

ApplicationModel* ApplicationModel::getInstance()
{
    if (instance == nullptr) {
        instance = new ApplicationModel();
    }
    return instance;
}

void ApplicationModel::defineScenes()
{
    auto startMenuScene = new StartMenuScene(startMenuSceneName);
    m_scenes.emplace(startMenuSceneName, startMenuScene);

    auto optionsScene = new OptionsScene(optionsSceneName);
    m_scenes.emplace(optionsSceneName, optionsScene);

    auto gameScene = new GameScene(gameSceneName);
    m_scenes.emplace(gameSceneName, gameScene);

    auto trainScene = new TrainScene(trainSceneName);
    m_scenes.emplace(trainSceneName, trainScene);

}

void ApplicationModel::defineInitialScene()
{
    m_initialScene = startMenuSceneName;
}

std::map<const std::string, IScene*> ApplicationModel::getScenes() const
{
    return m_scenes;
}

std::string ApplicationModel::getInitialSceneName() const
{
    return startMenuSceneName;
}

IScene * ApplicationModel::getInitialScene() const
{
    return m_scenes.find(m_initialScene)->second;
}

IScene * ApplicationModel::getScene(const std::string & name) const
{
    return m_scenes.find(name)->second;
}

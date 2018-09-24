#pragma once
#include "AppUI/IScene.h"
#include <qmainwindow.h>
#include <map>
#include <memory>

namespace AppUI
{
    class SceneManager:public QObject
    {
        Q_OBJECT
    public:
        SceneManager(std::map<const std::string, IScene*> scenes, 
                     std::unique_ptr<QMainWindow>& mainWindow, 
                     const std::string& initialScene);

        ~SceneManager();

    private:
        std::map<const std::string, IScene*>  m_scenes;

        std::unique_ptr<QMainWindow> m_mainWindow;

        std::string m_initialSceneName;
        std::string m_currentSceneName;

    private slots:
        void setActiveScene(const std::string& sceneName);
        void releaseCurrent() const;

    };
}
#pragma once
#include <qmainwindow.h>
#include <qobject.h>
#include <memory>

namespace AppUI
{
    class IScene : public QObject
    {
        Q_OBJECT;

    signals:
        void sceneChange(const std::string& sceneName);

    public:

        virtual void createScene() = 0;
        virtual void drawScene();
        virtual void release() = 0;
        virtual std::string getSceneName();
        virtual std::unique_ptr<QMainWindow>& getWindow();
        virtual void setWindow(std::unique_ptr<QMainWindow>& mainWindow);


        void show() const;

    protected:
        std::unique_ptr<QMainWindow> m_mainWindow;
        std::string m_sceneName;
    };
}
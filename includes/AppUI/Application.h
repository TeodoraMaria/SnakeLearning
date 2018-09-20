#pragma once
#include "AppUI/ApplicationModel.h"
#include "AppUI/SceneManager.h"

#include <qmainwindow.h>
#include <string>
#include <memory>

namespace AppUI
{
    class Application
    {
    public:
        Application(ApplicationModel* appModel);
        ~Application();

        void start(const std::string& appTitle, size_t width, size_t height);

    private:
        ApplicationModel* m_appModel;
        std::unique_ptr<QMainWindow> m_mainWindow;
        SceneManager* m_sceneManager;
    };
}
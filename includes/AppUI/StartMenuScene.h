#pragma once

#include "AppUI/IScene.h"
#include "qobject.h"

namespace AppUI
{
    class StartMenuScene:public IScene
    {
        Q_OBJECT
    public:

        StartMenuScene(const std::string& name);

        // Inherited via IScene
        virtual void createScene() override;
        virtual void release() override;

        ~StartMenuScene();

    protected slots:
        void startButtonPressed();
        void optionsButtonPressed();
        void quitButtonPressed();
    private:

        QWidget* m_centralWidget;
    };
}
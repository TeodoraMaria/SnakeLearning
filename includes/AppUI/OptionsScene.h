#pragma once

#include "AppUI/IScene.h"
#include "qobject.h"
#include <string>

namespace AppUI
{
    class OptionsScene :public IScene
    {
        Q_OBJECT
    public:

        OptionsScene(const std::string& name);
        ~OptionsScene();

        // Inherited via IScene
        virtual void createScene() override;
        virtual void release() override;


        protected slots:
        void quitButtonPressed();

    private:

        QWidget* m_centralWidget;
    };
}
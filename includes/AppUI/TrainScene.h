#pragma once

#include "AppUI/IScene.h"
#include "qobject.h"
#include <string>
#include "ui_TrainScene.h"

namespace AppUI
{

    class TrainScene :public IScene
    {
        Q_OBJECT
    public:

        TrainScene(const std::string& name);
        ~TrainScene();

        // Inherited via IScene
        virtual void createScene() override;
        virtual void release() override;

    protected slots:
        void backButtonPressed();
        void startButtonPressed();
        void updateGraph();

    private:

        QLineSeries* series;

        QWidget* m_centralWidget;
    };
}
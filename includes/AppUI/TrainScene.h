#pragma once

#include "AppUI/IScene.h"
#include "GraphicBoard.h"
#include "AI/ITrainer.hpp"

#include "qobject.h"
#include "ui_TrainScene.h"
#include <string>
#include <memory>

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


    signals:
        void loadingBar(double value);
        void graphValues(const std::vector<double>& values);
        void gameState(GameState gamestate);

    protected slots:
        void backButtonPressed();
        void startButtonPressed();
        void updateGraph(const std::vector<double>& values);
        void updateLoadingBar(double value);
        void updateGameScene(GameState gamestate);
        void switchDisplayEnabled();


    private:
        QChart* m_chart;

        std::vector<QLineSeries*> m_graphValues;
        QWidget* m_centralWidget;

        GraphicBoard* m_board;

        size_t m_graphX = 0;
        size_t m_graphY = 0;

        std::shared_ptr<Ui_TrainScene> ui;

        std::unique_ptr<AI::ITrainer> m_trainer;

        bool m_isTraining = false;
        bool m_displayEnabled = false;
    };
}
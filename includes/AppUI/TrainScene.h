#pragma once

#include "AppUI/IScene.h"
#include "GraphicBoard.h"
#include "AI/GeneticAlgorithm/GeneticTrainerMultiSnake.h"
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

    protected slots:
        void backButtonPressed();
        void startButtonPressed();
        void updateGraph(const std::vector<double>& values);

    private:
        QChart* m_chart;
        QLineSeries* m_maxFitnessValues;
        QLineSeries* m_avgFitnessValues;
        QWidget* m_centralWidget;

        GraphicBoard* m_board;

        size_t m_graphX = 0;
        size_t m_graphY = 0;

        std::shared_ptr<Ui_TrainScene> ui;

        AI::GeneticAlgorithm::GeneticTrainerMultiSnake m_geneticAlg;

    };
}
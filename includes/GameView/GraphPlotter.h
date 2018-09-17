#pragma once

#include <vector>

namespace GameView
{
    struct Vertex
    {
        float x;
        float y;
    };


    class GraphPlotter
    {
    public:
        GraphPlotter();

        void addValue(float value);
        void plot();

    private:
        size_t m_currentIndex=0;

        float m_maxValueSoFar=0;

        const size_t m_maxValues = 300;

        const float m_maxYValue = -200;
        const float m_minYValue = 0;

        const int m_xOffset = 600;
        const int m_yOffset = 400;
        static const size_t m_numBackgroundVertices=4;
        static const size_t m_numAxisVertices = 4;

        void drawBackground();
        void drawAxes();
        void drawGraph();

        void checkIfMaxValue(float value);

        void scaleXValues();
        void scaleYValues(float value);

        std::vector<Vertex> m_graphVertices;
        
        Vertex m_backgroundVertices[m_numBackgroundVertices];
        Vertex m_axisVertices[m_numAxisVertices];
    };
}
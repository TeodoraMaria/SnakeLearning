#include "GL/glew.h"
#include <vector>

namespace GameView
{
    struct Point
    {
        GLfloat x;
        GLfloat y;
    };


    class GraphPlotter
    {
    public:
        GraphPlotter();

        void plot();

    private:

        std::vector<Point> m_vertices;


    };


}
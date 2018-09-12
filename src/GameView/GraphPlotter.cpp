#include "GraphPlotter.h"
#include "Utils/MathUtils.h"
#include "GL/glew.h"

using namespace GameView;

GraphPlotter::GraphPlotter()
{

    m_axisVertices[0].x = 0;
    m_axisVertices[0].y = 0;


    m_axisVertices[1].x = 300;
    m_axisVertices[1].y = 0;

    m_axisVertices[2].x = 0;
    m_axisVertices[2].y = 0;

    m_axisVertices[3].x = 0;
    m_axisVertices[3].y = -200;


    m_backgroundVertices[0].x = -10;
    m_backgroundVertices[0].y = 10;

    m_backgroundVertices[1].x = -10;
    m_backgroundVertices[1].y = -210;

    m_backgroundVertices[2].x = 310;
    m_backgroundVertices[2].y = -210;

    m_backgroundVertices[3].x = 310;
    m_backgroundVertices[3].y = 10;


}


void GraphPlotter::addValue(float value)
{
    Vertex vertex;
    vertex.y = value;
        m_graphVertices.push_back(vertex);

    if (m_graphVertices.size() == m_maxValues) {
        m_graphVertices.erase(m_graphVertices.begin());
    }   
    for (size_t i = 0; i < m_graphVertices.size(); i++) {
        m_graphVertices[i].x = i*10;
    }
}

void GraphPlotter::plot()
{
    glTranslatef(m_xOffset, m_yOffset, 0);
    drawBackground();
    drawAxes();
    if (m_graphVertices.size() > 0) {
        drawGraph();
    }
    
    glTranslatef(-m_xOffset, -m_yOffset, 0);
}

void GraphPlotter::drawBackground()
{
    GLuint vbo;

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, m_numBackgroundVertices * sizeof(Vertex), m_backgroundVertices, GL_DYNAMIC_DRAW);

    GLuint attribute_coord2d = 0;
    glEnableVertexAttribArray(attribute_coord2d);
    glVertexAttribPointer(
        attribute_coord2d,   // attribute
        2,                   // number of elements per vertex, here (x,y)
        GL_FLOAT,            // the type of each element
        GL_FALSE,            // take our values as-is
        0,                   // no space between values
        0                    // use the vertex buffer object
    );

    glColor3d(1, 1, 1);
    glDrawArrays(GL_QUADS, 0, m_numBackgroundVertices);

    glDisableVertexAttribArray(attribute_coord2d);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glDeleteBuffers(m_numBackgroundVertices * sizeof(Vertex),&vbo);
    
}

void GraphPlotter::drawAxes()
{
    GLuint vbo;

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, m_numAxisVertices * sizeof(Vertex), m_axisVertices, GL_DYNAMIC_DRAW);

    GLuint attribute_coord2d = 0;
    glEnableVertexAttribArray(attribute_coord2d);
    glVertexAttribPointer(
        attribute_coord2d,   // attribute
        2,                   // number of elements per vertex, here (x,y)
        GL_FLOAT,            // the type of each element
        GL_FALSE,            // take our values as-is
        0,                   // no space between values
        0                    // use the vertex buffer object
    );

    glColor3d(0, 0, 0);
    glLineWidth(2.5);
    glDrawArrays(GL_LINE_STRIP, 0, m_numAxisVertices);
    glLineWidth(1);

    glDisableVertexAttribArray(attribute_coord2d);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glDeleteBuffers(m_numAxisVertices * sizeof(Vertex), &vbo);
}

void GraphPlotter::drawGraph()
{
    GLuint vbo;

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, m_graphVertices.size() * sizeof(Vertex), &m_graphVertices.front(), GL_DYNAMIC_DRAW);

    GLuint attribute_coord2d = 0;
    glEnableVertexAttribArray(attribute_coord2d);
    glVertexAttribPointer(
        attribute_coord2d,   // attribute
        2,                   // number of elements per vertex, here (x,y)
        GL_FLOAT,            // the type of each element
        GL_FALSE,            // take our values as-is
        0,                   // no space between values
        0                    // use the vertex buffer object
    );

    glColor3d(0, 0.5, 0);
    glDrawArrays(GL_LINE_STRIP, 0, m_graphVertices.size());

    glDisableVertexAttribArray(attribute_coord2d);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glDeleteBuffers(m_graphVertices.size() * sizeof(Vertex), &vbo);
}

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <glew.h>
#pragma comment(lib,"C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\VC\\Tools\\MSVC\\14.28.29910\\lib\\x86\\glew32.lib")
#pragma comment (lib, "legacy_stdio_definitions.lib")
#include <glut.h>
#include "math_3d.h"

GLuint VBO; // ОБЪЕКТЫ ВЕРШИННОГО ШЕЙДЕРА
GLuint gWorldLocation;

static const char* pVS = "                                                          \n\
#version 330                                                                        \n\
                                                                                    \n\
layout (location = 0) in vec3 Position;                                             \n\
                                                                                    \n\
uniform mat4 gWorld;                                                                \n\
                                                                                    \n\
out vec4 Color;                                                                     \n\
                                                                                    \n\
void main()                                                                         \n\
{                                                                                   \n\
    gl_Position = gWorld * vec4(Position, 1.0);                                     \n\
}";
// gl_Position указывает на положение вершины. Используется в вершинном шейдере в качестве выходного параметр

static void RenderSceneCB()
{
    glClear(GL_COLOR_BUFFER_BIT);
    static float Scale = 0.0f;
    Scale = 1.2f;
    
    // мировая матрица
    Matrix4f World;
    World.m[0][0] = sinf(Scale); 
    World.m[0][1] = 0.0f; 
    World.m[0][2] = 0.0f;        
    World.m[0][3] = 0.0f;
    World.m[1][0] = 0.0f; 
    World.m[1][1] = sinf(Scale); 
    World.m[1][2] = 0.0f;       
    World.m[1][3] = 0.0f;
    World.m[2][0] = 0.0f;
    World.m[2][1] = 0.0f;
    World.m[2][2] = sinf(Scale); 
    World.m[2][3] = 0.0f;
    World.m[3][0] = 0.0f;
    World.m[3][1] = 0.0f;
    World.m[3][2] = 0.0f;        
    World.m[3][3] = 1.0f;

    glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, &World.m[0][0]); // загружаем матрицу в шейдер
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableVertexAttribArray(0);
    glutSwapBuffers();
}


static void InitializeGlutCallbacks()
{
    glutDisplayFunc(RenderSceneCB);
    glutIdleFunc(RenderSceneCB);
}

static void CreateVertexBuffer()
{
    Vector3f Vertices[3]; // все координаты в нормализованном диапазоне координат
    Vertices[0] = Vector3f(-1.0f, -1.0f, 0.0f);
    Vertices[1] = Vector3f(1.0f, -1.0f, 0.0f);
    Vertices[2] = Vector3f(0.0f, 1.0f, 0.0f);

    glGenBuffers(1, &VBO);   // СОЗДАЁМ БУФЕР ДЛЯ ВЕРШИННОГО ШЕЙДЕРА
    glBindBuffer(GL_ARRAY_BUFFER, VBO);  // СВЯЗЫВАЕМ НАШ БУФЕР С ВСТРОЕННЫМ
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW); // копируем ранее определенные данные вершин в память буфера
    // опирует vertices в память видеокарты. Первый аргумент указывает, что мы копируем вершины в связанный буфер, второй - размер массива, третий- адрес откуда копировать
}

static void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
    GLuint ShaderObj = glCreateShader(ShaderType);  // создаём шейдер ( передаём тип вершинного шейднра )
    const GLchar* p[1];
    p[0] = pShaderText;
    GLint Lengths[1];
    Lengths[0] = strlen(pShaderText);
    
    // прикрепляем исходный код шейдера к объекту шейдера и компилируем его
    glShaderSource(ShaderObj, 1, p, Lengths); // ( Объект шейдера для компиляции, Количество строк, Фактический исходный код вершинного шейдера )
    glCompileShader(ShaderObj);
    
    glAttachShader(ShaderProgram, ShaderObj);
}

static void CompileShaders()
{
    // СОЗДАЁМ ОБЪЕКТ ШЕЙДЕРНОЙ ПРОГРАММЫ
    GLuint ShaderProgram = glCreateProgram();
    
    AddShader(ShaderProgram, pVS, GL_VERTEX_SHADER);
    
    // прикрепляем ранее скомпилированные шейдеры к объекту программы
    glLinkProgram(ShaderProgram);
    glValidateProgram(ShaderProgram);
    
    // вызываем программный объект
    glUseProgram(ShaderProgram);

}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(1024, 768);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("Лабораторная работа №8");
    InitializeGlutCallbacks();
    GLenum res = glewInit();
    glClearColor(0.3f, 0.0f, 0.5f, 0.0f);
    CreateVertexBuffer();
    CompileShaders();
    glutMainLoop();
    return 0;
}

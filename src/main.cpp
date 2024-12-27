
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>

#include "window.h"
#include "shader_program.h"
#include "vertices_data.h"
#include "shapes.h"


void printGLInfo();

#define GL_CHECK_ERROR checkGLError(__FILE__, __LINE__)
void checkGLError(const char* file, int line);

void changeRGB(GLfloat* color);
void changePos(GLfloat* pos, float& cx, float& cy, float& dx, float& dy);

std::string readFile(const char* path);

static const char BASIC_VERTEX_SHADER_TEXT[] = "./shaders/basic.vs.glsl";
static const char BASIC_FRAGMENT_SHADER_TEXT[] = "./shaders/basic.fs.glsl";


static const char COLOR_FRAGMENT_SHADER_TEXT[] = "./shaders/color.fs.glsl";
static const char COLOR_VERTEX_SHADER_TEXT[] = "./shaders/color.vs.glsl";

static const char TRANSFORM_FRAGMENT_SHADER_TEXT[] = "./shaders/transform.fs.glsl";
static const char TRANSFORM_VERTEX_SHADER_TEXT[] = "./shaders/transform.vs.glsl";


int main(int argc, char* argv[])
{
    Window w;
    if (!w.init())
        return -1;

    GLenum rev = glewInit();
    if (rev != GLEW_OK)
    {
        std::cout << "Could not initialize glew! GLEW_Error: " << glewGetErrorString(rev) << std::endl;
        return -2;
    }

    printGLInfo();
    ShaderProgram basicShaderProgram;
    // TODO Partie 1: Instancier les shader programs ici.
    // ... basic;
    { // Les accolades vont permettre de détruire le code des shaders plus rapidement
        // Vous devez lire le code des shaders dans "shaders/"
        // avec la fonction "readFile".
        std::string basicVertexShaderText = readFile(BASIC_VERTEX_SHADER_TEXT);
        std::string basicFragmentShaderText = readFile(BASIC_FRAGMENT_SHADER_TEXT);

        Shader vertexShader(GL_VERTEX_SHADER, basicVertexShaderText.c_str());
        Shader fragmentShader(GL_FRAGMENT_SHADER, basicFragmentShaderText.c_str());

        // Vous pouvez par la suite instancier vos shaders, les attacher et les lier
        // au programme.
        basicShaderProgram.attachShader(vertexShader);
        basicShaderProgram.attachShader(fragmentShader);
        basicShaderProgram.link();
    }
    ShaderProgram colorShaderProgram;
    // ... color;
    {
        // ...
        std::string colorVertexShaderText = readFile(COLOR_VERTEX_SHADER_TEXT);
        std::string colorFragmentShaderText = readFile(COLOR_FRAGMENT_SHADER_TEXT);

        Shader vertexShader(GL_VERTEX_SHADER, colorVertexShaderText.c_str());
        Shader fragmentShader(GL_FRAGMENT_SHADER, colorFragmentShaderText.c_str());

        colorShaderProgram.attachShader(vertexShader);
        colorShaderProgram.attachShader(fragmentShader);
        colorShaderProgram.link();
    }

    // TODO Partie 2: Shader program de transformation.
    ShaderProgram transformShaderProgram;
    // ... transform;
    // ... location;
    {
        GL_CHECK_ERROR;
        // ...
        std::string transformVertexShaderText = readFile(TRANSFORM_VERTEX_SHADER_TEXT);
        std::string transformFragmentShaderText = readFile(TRANSFORM_FRAGMENT_SHADER_TEXT);

        Shader vertexShader(GL_VERTEX_SHADER, transformVertexShaderText.c_str());
        Shader fragmentShader(GL_FRAGMENT_SHADER, transformFragmentShaderText.c_str());

        transformShaderProgram.attachShader(vertexShader);
        transformShaderProgram.attachShader(fragmentShader);
        transformShaderProgram.link();

        GL_CHECK_ERROR;
    }

    // Variables pour la mise à jour, ne pas modifier.
    float cx = 0, cy = 0;
    float dx = 0.019f;
    float dy = 0.0128f;

    float angleDeg = 0.0f;

    // Tableau non constant de la couleur
    GLfloat onlyColorTriVertices[] = {
        // TODO Partie 1: Rempliser adéquatement le tableau.
        // Vous pouvez expérimenter avec une couleur uniforme
        // de votre choix ou plusieurs différentes en chaque points.
        0.2f, 0.7f, 0.0f,
        0.2f, 0.7f, 0.0f,
        0.2f, 0.7f, 0.0f
    };

    // TODO Partie 1: Instancier vos formes ici.
    // ...
    GL_CHECK_ERROR;
    BasicShapeArrays triangle(triVertices, sizeof(triVertices));
    BasicShapeArrays square(squareVertices, sizeof(squareVertices));
    BasicShapeArrays coloredTriangle(colorTriVertices, sizeof(colorTriVertices));
    BasicShapeArrays coloredSquare(colorSquareVertices, sizeof(colorSquareVertices));
    BasicShapeElements coloredSquareV2(colorSquareVerticesReduced, sizeof(colorSquareVerticesReduced), indexes, sizeof(indexes));
    BasicShapeMultipleArrays movingTriangle(triVertices, sizeof(triVertices), onlyColorTriVertices, sizeof(onlyColorTriVertices));

    // basic forme
    triangle.enableAttribute(0, 3, 0, 0);
    square.enableAttribute(0, 3, 0, 0);

    // color forme
    coloredTriangle.enableAttribute(0, 3, 6 * sizeof(float), 0);
    coloredTriangle.enableAttribute(1, 3, 6 * sizeof(float), 3 * sizeof(float));
  
    coloredSquare.enableAttribute(0, 3, 6 * sizeof(float), 0);
    coloredSquare.enableAttribute(1, 3, 6 * sizeof(float), 3 * sizeof(float));

    coloredSquareV2.enableAttribute(0, 3, 6 * sizeof(float), 0);
    coloredSquareV2.enableAttribute(1, 3, 6 * sizeof(float), 3 * sizeof(float));

    //triangle bougeant
    movingTriangle.enablePosAttribute(0, 3, 3 * sizeof(float), 0);
    movingTriangle.enableColorAttribute(1, 3, 3 * sizeof(float), 0);
  
    // TODO Partie 2: Instancier le cube ici.
    // ...
    BasicShapeElements cube(cubeVertices, sizeof(cubeVertices), cubeIndexes, sizeof(cubeIndexes));
    cube.enableAttribute(0, 3, 6 * sizeof(float), 0);
    cube.enableAttribute(1, 3, 6 * sizeof(float), 3 * sizeof(float));
  
    // TODO Partie 1: Donner une couleur de remplissage aux fonds.
    glClearColor(1.0, 1.0, 1.0, 1.0);
    GL_CHECK_ERROR;

    // TODO Partie 2: Activer le depth test.
    glEnable(GL_DEPTH_TEST);

    int selectShape = 0;
    bool isRunning = true;
    while (isRunning)
    {
        if (w.shouldResize())
            glViewport(0, 0, w.getWidth(), w.getHeight());

        // TODO Partie 1: Nettoyer les tampons appropriées.
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (w.getKey(Window::Key::T))
        {
            selectShape = ++selectShape < 7 ? selectShape : 0;
            std::cout << "Selected shape: " << selectShape << std::endl;
        }

        // TODO Partie 1: Mise à jour des données du triangle
        changeRGB(&onlyColorTriVertices[0]);
        changeRGB(&onlyColorTriVertices[3]);
        changeRGB(&onlyColorTriVertices[6]);

        GLfloat* posPtr = movingTriangle.mapPosData();
        changePos(posPtr, cx, cy, dx, dy);
        movingTriangle.unmapPosData();

        // TODO Partie 1: Utiliser le bon shader programme selon la forme.
        // N'hésiter pas à utiliser le fallthrough du switch case.
        switch (selectShape)
        {
        case 0:
        case 1:
            basicShaderProgram.use();
            break;
        case 2:
        case 3:
        case 4:
        case 5:
            colorShaderProgram.use();
            break;
        case 6:
            transformShaderProgram.use();
            break;
        }

        // TODO Partie 2: Calcul des matrices et envoyer une matrice résultante mvp au shader.
        if (selectShape == 6)
        {
            float time = (float) w.getTick() / 1000.0f;
            angleDeg += 0.5f;
            // Utiliser glm pour les calculs de matrices.
            // glm::mat4 matrix;
            glm::mat4 matrix = glm::mat4(1.0f);
            matrix = glm::rotate(matrix, glm::radians(angleDeg), glm::vec3(0.1f, 1.0f, 0.1f));
            glm::mat4 view = glm::mat4(1.0f);
            view = glm::translate(view, glm::vec3(0.0f, -0.5f, -2.0f));
            glm::mat4 projection = glm::perspective(glm::radians(70.0f), w.getWidth() / (float)w.getHeight(), 0.1f, 10.0f);
            unsigned int modelLoc = transformShaderProgram.getUniformLoc("model");
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(matrix));

            unsigned int viewLoc = transformShaderProgram.getUniformLoc("view");
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

            unsigned int projectionLoc = transformShaderProgram.getUniformLoc("projection");
            glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
        }

        // TODO Partie 1: Dessiner la forme sélectionnée.
        switch (selectShape)
        {
        case 0:
            triangle.draw(GL_TRIANGLES, 3);
          break;
        case 1:
            square.draw(GL_TRIANGLES, 6);
            break;
        case 2:
            coloredTriangle.draw(GL_TRIANGLES, 3);
            break;
        case 3:
            coloredSquare.draw(GL_TRIANGLES, 6);
            break;
        case 4:
            coloredSquareV2.draw(GL_TRIANGLES, 6);
            break;
        case 5:
            movingTriangle.updateColorData(onlyColorTriVertices, sizeof(onlyColorTriVertices));
            movingTriangle.draw(GL_TRIANGLES, 3);
            break;
        case 6:
            cube.draw(GL_TRIANGLES, 36);
            break;
        }
        w.swap();
        w.pollEvent();
        isRunning = !w.shouldClose() && !w.getKey(Window::Key::ESC);
    }

    return 0;
}



void checkGLError(const char* file, int line)
{
    GLenum error;
    while ((error = glGetError()) != GL_NO_ERROR)
    {
        std::cerr << "GL_ERROR, File " << file << " (Line " << line << "): ";
        switch (error)
        {
        case GL_INVALID_ENUM:
            std::cerr << "GL_INVALID_ENUM";
            break;
        case GL_INVALID_VALUE:
            std::cerr << "GL_INVALID_VALUE";
            break;
        case GL_INVALID_OPERATION:
            std::cerr << "GL_INVALID_OPERATION";
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            std::cerr << "GL_INVALID_FRAMEBUFFER_OPERATION";
            break;
        case GL_OUT_OF_MEMORY:
            std::cerr << "GL_OUT_OF_MEMORY";
            break;
        case GL_STACK_UNDERFLOW:
            std::cerr << "GL_STACK_UNDERFLOW";
            break;
        case GL_STACK_OVERFLOW:
            std::cerr << "GL_STACK_OVERFLOW";
            break;
        default:
            std::cerr << "Unknown gl error occured!";
            break;
        }
        std::cerr << std::endl;
    }
}

void printGLInfo()
{
    std::cout << "OpenGL info:" << std::endl;
    std::cout << "    Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "    Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "    Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "    Shading version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}


void changeRGB(GLfloat* color)
{
    unsigned char r = color[0] * 255;
    unsigned char g = color[1] * 255;
    unsigned char b = color[2] * 255;

    if (r > 0 && b == 0)
    {
        r--;
        g++;
    }
    if (g > 0 && r == 0)
    {
        g--;
        b++;
    }
    if (b > 0 && g == 0)
    {
        r++;
        b--;
    }
    color[0] = r / 255.0f;
    color[1] = g / 255.0f;
    color[2] = b / 255.0f;
}

void changePos(GLfloat* pos, float& cx, float& cy, float& dx, float& dy)
{
    if ((cx < -1 && dx < 0) || (cx > 1 && dx > 0))
        dx = -dx;
    pos[0] += dx;
    pos[3] += dx;
    pos[6] += dx;
    cx += dx;
    if ((cy < -1 && dy < 0) || (cy > 1 && dy > 0))
        dy = -dy;
    pos[1] += dy;
    pos[4] += dy;
    pos[7] += dy;
    cy += dy;
}


std::string readFile(const char* path)
{
    std::ifstream file(path);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

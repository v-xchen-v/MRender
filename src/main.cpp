#include "draw2D.h"

#include <GLFW/glfw3.h>
#include "math.h"

#include "iostream"
#include "vector"

using namespace std;

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Show Me Window", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        // for(float i=0;i<10;i+=0.01)
        // {
        //     drawPoint(i,i,1.0, 0.0, 0.0);
        // }
        // drawLint_Print(0,0,1,1,1.0,0.0,0.0);
        drawLine_DDA(1,1,0,0,1.0,0.0,0.0);
        drawTriangle(0,0.5,-1,-0.25,-0.5,0.0,1.0,0.0,0.0);
        // drawTriangle(-0.5,-0.5,0.5,-0.5,0.0,0.0,1.0,0.0,0.0);
        // drawTriangle(-0.25,0.5,0.25,0.5,0,0,1.0,0.0,0.0);
        // drawTriangle_flatTop(0,0,-0.5,-0.5,0.5,-0.5,1.0,0.0,0.0);
        // drawTriangle_flatBottom(-0.25,0.5,0.25,0.5,0,0,1.0,0.0,0.0);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
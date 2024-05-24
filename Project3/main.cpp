#pragma once
#include "libs.h"

void framebuffer_resize_callback(GLFWwindow* window, int fbW, int fbH) {
    glViewport(0, 0, fbW, fbH);
}

int main() {

    // Init GLFW
    if (!glfwInit()) {
        std::cerr << "ERROR::MAIN.CPP::Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Create Window
    const int WINDOW_WIDTH = 640;
    const int WINDOW_HEIGHT = 480;
    int framebufferWidth = 0;
    int framebufferHeight = 0;

    // Window Options [using version 4.4]
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE); // resisable
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // MAC OS

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Project3", NULL, NULL);
    if (!window) {
        std::cerr << "ERROR::MAIN.CPP::Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Set window context
    glfwSetFramebufferSizeCallback(window, framebuffer_resize_callback);
    // glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight); // same size as window
    // glViewport(0, 0, framebufferWidth, framebufferHeight); // how much window we are drawing on

    glfwMakeContextCurrent(window); // IMPORTANT

    // Initialize GLEW (NEEDS WINDOW CONTEXT)
    glewExperimental = GL_TRUE; // enable modern OpenGL functionality

    // Error
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "ERROR::MAIN.CPP::GLEW_INIT_FAILED" << std::endl;
        glfwTerminate();
        return -1;
    }

    // MAIN PROGRAM LOOP
    while (!glfwWindowShouldClose(window)) { // As long as window is open
        
        // UPDATE INPUT ---
        glfwPollEvents();

        // Game Update ---

        // DRAW ---

        // Clear all buffers
        glClearColor(0.f, 0.f, 0.f, 1.f); // Black
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        // Draw

        // End Draw
        glfwSwapBuffers(window); // Swap back & front buffer
        glFlush();
    }

    // END OF PROGRAM
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

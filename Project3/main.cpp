#pragma once
#include "libs.h"

// Callback function for resizable window
void framebuffer_resize_callback(GLFWwindow* window, int fbW, int fbH) {
    glViewport(0, 0, fbW, fbH);
}

bool loadShaders(GLuint &program) {
    bool loadSuccess = true;
    char infoLog[512];
    GLint success;

    std::string temp = "";
    std::string src = "";

    std::ifstream in_file;

    // VERTEX ---
    in_file.open("vertex_core.glsl");

    if (in_file.is_open()) { // read file
        while (std::getline(in_file, temp))
            src += temp + "\n";

    }
    else { // failed to open
        std::cout << "ERROR::LOADSHADERS::COULD_NOT_OPEN_VERTEX_FILE" << "\n";
        loadSuccess = false;
    }

    in_file.close(); // close file

    // Create vertex shader in the background
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER); // get / init shader ID

    // Set source
    const GLchar* vertSrc = src.c_str();
    glShaderSource(vertexShader, 1, &vertSrc, NULL); // Set source to new shader
    glCompileShader(vertexShader); // Compile

    // Error check compile shaders
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success); // get check
    if (!success) { // FAIL
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::LOADSHADERS::COULD_NOT_COMPILE_VERTEX_SHADER" << "\n";
        std::cout << infoLog << "\n"; // Print what happened
        loadSuccess = false;
    }

    temp = "";
    src = "";

    // FRAGMENT ---
    in_file.open("fragment_core.glsl");

    if (in_file.is_open()) { // read file
        while (std::getline(in_file, temp))
            src += temp + "\n";

    }
    else { // failed to open
        std::cout << "ERROR::LOADSHADERS::COULD_NOT_OPEN_FRAGMENT_FILE" << "\n";
        loadSuccess = false;
    }

    in_file.close(); // close file

    // Create fragment shader in the background
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); // get / init shader ID

    // Set source
    const GLchar* fragSrc = src.c_str();
    glShaderSource(fragmentShader, 1, &fragSrc, NULL); // Set source to new shader
    glCompileShader(fragmentShader); // Compile

    // Error check compile shaders
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success); // get check
    if (!success) { // FAIL
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::LOADSHADERS::COULD_NOT_COMPILE_FRAGMENT_SHADER" << "\n";
        std::cout << infoLog << "\n"; // Print what happened
        loadSuccess = false;
    }

    // Program
    program = glCreateProgram();

    // Attach & link shaders to prgm
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    // Error check linking
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) { // FAIL
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cout << "ERROR::COULD_NOT_LINK_PROGRAM" << "\n";
        std::cout << infoLog << "\n"; // print what happened
        loadSuccess = false;
    }


    // End [Once created, linked to program & are local. Free up memory.]
    glUseProgram(0); // Resets(binding) what program we are using
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return loadSuccess;
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

    // Window Options [using GLSL version 4.4]
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
    glfwSetFramebufferSizeCallback(window, framebuffer_resize_callback); // for resizable window
    // glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight); // same size as window
    // glViewport(0, 0, framebufferWidth, framebufferHeight); // how much window we are drawing on

    glfwMakeContextCurrent(window); // IMPORTANT

    // Initialize GLEW (NEEDS WINDOW CONTEXT)
    glewExperimental = GL_TRUE; // enable modern OpenGL functionality

    // Error
    if (glewInit() != GLEW_OK) {
        std::cerr << "ERROR::MAIN.CPP::GLEW_INIT_FAILED" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Initialize Shader
    GLuint core_program;
    if (!loadShaders(core_program))
        glfwTerminate();













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

    // Delete program
    glDeleteProgram(core_program);
    return 0;
}

#include "Game.h"




void Game::initGLFW() {


    if (!glfwInit() == GLFW_FALSE) {
        std::cout << "ERROR::GLFW_INIT_FAILED" << "\n";
        glfwTerminate();
    }

}

void Game::initWindow(
    const char* title,
    bool resizable) {
    // Window Options [using GLSL version 4.4]
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, this->GL_VERSION_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, this->GL_VERSION_MINOR);
    glfwWindowHint(GLFW_RESIZABLE, resizable); // resisable
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // MAC OS

    this->window = glfwCreateWindow(this->WINDOW_WIDTH, this->WINDOW_HEIGHT, title, NULL, NULL);

    if (this->window == nullptr) {
        std::cout << "ERROR::GLFW_WINDOW_INIT_FAILED" << "\n";
        glfwTerminate();

    }

    // Set window context
    glfwGetFramebufferSize(window, &this->framebufferWidth, &this->framebufferHeight); // same size as window
    glfwSetFramebufferSizeCallback(window, Game::framebuffer_resize_callback); // for resizable window

    // glViewport(0, 0, framebufferWidth, framebufferHeight); // how much window we are drawing on

    glfwMakeContextCurrent(window); // IMPORTANT
}

void Game::initGLEW() {
    // Initialize GLEW (NEEDS WINDOW CONTEXT)
    glewExperimental = GL_TRUE; // enable modern OpenGL functionality

    // Error
    if (glewInit() != GLEW_OK) {
        std::cout << "ERROR::MAIN.CPP::GLEW_INIT_FAILED" << "\n";
        glfwTerminate();
    }
}


void Game::initOpenGLOptions() {
    // OpenGL Options
    glEnable(GL_DEPTH_TEST); // enable use of Z & W coordinate
    glEnable(GL_CULL_FACE); // don't draw whats not shown
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW); // draw vertices counter clockwise
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // fill shape with color [DEFAULT:FILL]
    glEnable(GL_BLEND); // blend colors
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


Game::Game(
    const char* title,
    const int WINDOW_WIDTH, const int WINDOW_HEIGHT,
    int GL_VERSION_MAJOR, int GL_VERSION_MINOR,
    bool resizable) : WINDOW_WIDTH(WINDOW_WIDTH), WINDOW_HEIGHT(WINDOW_HEIGHT),
    GL_VERSION_MAJOR(GL_VERSION_MAJOR), GL_VERSION_MINOR(GL_VERSION_MINOR) {

    // Init variables
    this->framebufferHeight = WINDOW_HEIGHT;
    this->framebufferWidth = WINDOW_WIDTH;

    this->initGLFW();
	this->initWindow(title, resizable);
    this->initGLEW();
    this->initOpenGLOptions();
}

Game::~Game() {
    glfwDestroyWindow(this->window);
    glfwTerminate();
}


// Accessors
int Game::getWindowShouldClose() {
    return glfwWindowShouldClose(this->window);
}

// Modifiers
void Game::setWindowShouldClose() {
    glfwSetWindowShouldClose(this->window, GLFW_TRUE);
}

// Functions
void Game::update() {


}

void Game::render() {


}

// Callback function for resizable window
void Game::framebuffer_resize_callback(GLFWwindow* window, int fbW, int fbH) {
    glViewport(0, 0, fbW, fbH);
};
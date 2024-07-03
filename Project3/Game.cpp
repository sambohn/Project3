#include "Game.h"




void Game::initGLFW() {


    if (glfwInit() == GLFW_FALSE) {
        std::cout << "ERROR::GLFW_INIT_FAILED" << "\n";
        glfwTerminate();
    }

}

void Game::initWindow(const char* title, bool resizable) {
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

    glfwMakeContextCurrent(window); // Set OpenGL context here

    // Now set framebuffer size and callback
    glfwGetFramebufferSize(window, &this->framebufferWidth, &this->framebufferHeight);
    glfwSetFramebufferSizeCallback(window, Game::framebuffer_resize_callback);
}

void Game::initGLEW() {
    glewExperimental = GL_TRUE; // Enable modern OpenGL functionality

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

void Game::initMatrices() {
    // Create view matrix
    // CAMERA [STATIC VECTOR & POS]
    this->ViewMatrix = glm::lookAt(this->camPosition, this->camPosition + this->camFront, this->worldUp);

    // Create projection matrix

    this->ProjectionMatrix = glm::perspective(
        glm::radians(fov),
        static_cast<float>(this->framebufferWidth) / this->framebufferHeight,
        this->nearPlane,
        this->farPlane); // Not resized until game loop. Send to shader
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

    // Create view matrix
    // CAMERA [STATIC VECTOR & POS]
    this->camPosition = glm::vec3(0.f, 0.f, 1.f);
    this->worldUp = glm::vec3(0.f, 1.f, 0.f); // Up
    this->camFront = glm::vec3(0.f, 0.f, -1.f); // Forward
    this->ViewMatrix = glm::mat4(1.f);

    // Create projection matrix
    this->fov = 90.f;
    this->nearPlane = 0.1f; // Not 0. Want slightly behind cam to avoid clipping
    this->farPlane = 1000.f;
    this->ProjectionMatrix = glm::mat4(1.f);

    this->initGLFW();
	this->initWindow(title, resizable);
    this->initGLEW();
    this->initOpenGLOptions();

    this->initMatrices();
    this->initShaders();
    this->initTextures();
    this->initMaterials(); // Textures before materials
    this->initMeshes();
    this->initLights(); // Lights before uniforms
    this->initUniforms();


}





Game::~Game() {
    glfwDestroyWindow(this->window);
    glfwTerminate();

    // Delete memory
    for (size_t i = 0; i < this->shaders.size(); i++)
        delete this->shaders[i];
    for (size_t i = 0; i < this->textures.size(); i++)
        delete this->textures[i];
    for (size_t i = 0; i < this->materials.size(); i++)
        delete this->materials[i];
    for (size_t i = 0; i < this->meshes.size(); i++)
        delete this->meshes[i];
    for (size_t i = 0; i < this->lights.size(); i++)
        delete this->lights[i];
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

    // Update input
    glfwPollEvents();
    this->updateInput(this->window, *this->meshes[MESH_QUAD]);
}

void Game::render() {

    // Game Update ---


    // DRAW ---

    // Clear all buffers
    glClearColor(0.f, 0.f, 0.f, 1.f); // Black
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    this->updateUniforms();

    this->shaders[SHADER_CORE_PROGRAM]->use();

    // Activate texture (binding)
    this->textures[TEX_FISH0]->bind(0);
    this->textures[TEX_FISH1]->bind(1);



    // Draw

    this->meshes[MESH_QUAD]->render(this->shaders[SHADER_CORE_PROGRAM]);

    // End Draw
    glfwSwapBuffers(this->window); // Swap back & front buffer
    glFlush();

    // unbinding [RESET]
    glBindVertexArray(0);
    glUseProgram(0);
    glActiveTexture(0);
    glBindTexture(GL_TEXTURE_2D, 0);

}

// Callback function for resizable window
void Game::framebuffer_resize_callback(GLFWwindow* window, int fbW, int fbH) {
    glViewport(0, 0, fbW, fbH);
}

void Game::initShaders() {
    this->shaders.push_back(new Shader(this->GL_VERSION_MAJOR, this->GL_VERSION_MINOR, "vertex_core.glsl", "fragment_core.glsl"));
}

void Game::initTextures() {
    // TEXTURE0 INIT
    this->textures.push_back(new Texture("Images/fish.png", GL_TEXTURE_2D));

    // TEXTURE1 INIT
    this->textures.push_back(new Texture("Images/yoyo.png", GL_TEXTURE_2D));
}

void Game::initMaterials() {

    this->materials.push_back(new Material (glm::vec3(0.1f), glm::vec3(1.f), glm::vec3(1.f),
        0, 
        1));
}

void Game::initMeshes() {
    this->meshes.push_back(new Mesh(new Quad()));
}

void Game::initLights() {
    this->lights.push_back(new glm::vec3(0.f, 0.f, -1.f));
}

void Game::initUniforms() {
    // Init Uniforms

    // send to shader [ Init uniforms ]
    this->shaders[SHADER_CORE_PROGRAM]->setMat4fv(ViewMatrix, "ViewMatrix");
    this->shaders[SHADER_CORE_PROGRAM]->setMat4fv(ProjectionMatrix, "ProjectionMatrix");

    // Send light pos -> fragment shader
    this->shaders[SHADER_CORE_PROGRAM]->setVec3f(*this->lights[0], "lightPos0");
    this->shaders[SHADER_CORE_PROGRAM]->setVec3f(this->camPosition, "cameraPos");

}

void Game::updateUniforms() {

    // Update uniforms (textures)
    this->shaders[SHADER_CORE_PROGRAM]->set1i(0, "texture0"); // Bind shader program before sending data!!!
    this->shaders[SHADER_CORE_PROGRAM]->set1i(1, "texture1");
    this->materials[MAT_1]->sendToShader(*this->shaders[SHADER_CORE_PROGRAM]); // update texture in shader [Fragment]

    // get correct view plane every frame
    glfwGetFramebufferSize(this->window, &this->framebufferWidth, &this->framebufferHeight);
    this->ProjectionMatrix = glm::mat4(1.f);
    this->ProjectionMatrix = glm::perspective(
        glm::radians(fov),
        static_cast<float>(this->framebufferWidth) / this->framebufferHeight,
        this->nearPlane,
        this->farPlane);
    this->shaders[SHADER_CORE_PROGRAM]->setMat4fv(this->ProjectionMatrix, "ProjectionMatrix");

    // Use a program
    this->shaders[SHADER_CORE_PROGRAM]->use(); // tell what shaders to use
}

// update input function
void Game::updateInput(GLFWwindow* window) {

    // Close window on [ESC] pressed
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

void Game::updateInput(GLFWwindow* window, Mesh& mesh) {

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        mesh.move(glm::vec3(0.f, 0.f, -0.001f));
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        mesh.move(glm::vec3(-0.001f, 0.f, 0.f));;
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        mesh.move(glm::vec3(0.f, 0.f, 0.001f));
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        mesh.move(glm::vec3(0.001f, 0.f, 0.f));
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        mesh.rotate(glm::vec3(0.f, 0.05f, 0.f));
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        mesh.rotate(glm::vec3(0.f, -0.05f, 0.f));
    }
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
        mesh.scaleUp(glm::vec3(-0.01f, -0.01f, 0.f));
    }
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
        mesh.scaleUp(glm::vec3(0.01f, 0.01f, 0.f));
    }
    if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS) { // RESET POS
        mesh.setPosition(glm::vec3(0.f));
        mesh.setRotation(glm::vec3(0.f));
        mesh.setScale(glm::vec3(1.f));
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINES);
    }
}
;
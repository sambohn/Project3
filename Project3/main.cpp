#pragma once
#include "libs.h"


// update input function
void updateInput(GLFWwindow* window) {

    // Close window on [ESC] pressed
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

void updateInput(GLFWwindow* window, Mesh &mesh) {

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

// Callback function for resizable window
void framebuffer_resize_callback(GLFWwindow* window, int fbW, int fbH) {
    glViewport(0, 0, fbW, fbH);
}

bool loadShaders(GLuint& program) {
    bool loadSuccess = true;
    char infoLog[512];
    GLint success;

    // Read from file
    std::string temp = ""; // Get each line
    std::string src = ""; // Gource

    std::ifstream in_file;

    // VERTEX ---
    in_file.open("vertex_core.glsl"); // GLSL is a shader coding language.
    if (in_file.is_open()) { // Read file
        while (std::getline(in_file, temp))
            src += temp + "\n";
    }
    else { // Failed to open
        std::cout << "ERROR::LOADSHADERS::COULD_NOT_OPEN_VERTEX_FILE" << "\n";
        loadSuccess = false;
    }
    in_file.close(); // close file

    // Create vertex shader in the background
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER); // get & init shader ID

    // Set source for shader
    const GLchar* vertSrc = src.c_str(); // C type string format
    glShaderSource(vertexShader, 1, &vertSrc, NULL); // Set source to new shader
    glCompileShader(vertexShader); // Compile shader

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
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE); // resisable
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // MAC OS

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Fish.exe", NULL, NULL);
    if (!window) {
        std::cerr << "ERROR::MAIN.CPP::Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Set window context
    glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight); // same size as window
    glfwSetFramebufferSizeCallback(window, framebuffer_resize_callback); // for resizable window

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

    // OpenGL Options
    glEnable(GL_DEPTH_TEST); // enable use of Z & W coordinate
    glEnable(GL_CULL_FACE); // don't draw whats not shown
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW); // draw vertices counter clockwise
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // fill shape with color [DEFAULT:FILL]
    glEnable(GL_BLEND); // blend colors
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    // Initialize Shader

    Shader core_program("vertex_core.glsl", "fragment_core.glsl");


    //GLuint core_program;
    //if (!loadShaders(core_program))
    //    glfwTerminate();

    // MODEL

    // MODEL MESH
    Mesh test(&Triangle());


    // TEXTURE0 INIT
    Texture texture0("Images/fish.png", GL_TEXTURE_2D, 0);

    // TEXTURE1 INIT
    Texture texture1("Images/yoyo.png", GL_TEXTURE_2D, 1);

    // MATERIAL 0
    Material material0(glm::vec3(0.1f), glm::vec3(1.f), glm::vec3(1.f), texture0.getTextureUnit(), texture1.getTextureUnit());

    

    // Create view matrix
    // CAMERA [STATIC VECTOR & POS]
    glm::vec3 camPosition(0.f, 0.f, 1.f);
    glm::vec3 worldUp = glm::vec3(0.f, 1.f, 0.f); // Up
    glm::vec3 camFront = glm::vec3(0.f, 0.f, -1.f); // Forward
    glm::mat4 ViewMatrix(1.f);
    ViewMatrix = glm::lookAt(camPosition, camPosition + camFront, worldUp);

    // Create projection matrix
    float fov = 90.f;
    float nearPlane = 0.1f; // Not 0. Want slightly behind cam to avoid clipping
    float farPlane = 1000.f;
    glm::mat4 ProjectionMatrix(1.f); // INITIALIZE!!!

    ProjectionMatrix = glm::perspective(
        glm::radians(fov),
        static_cast<float>(framebufferWidth) / framebufferHeight,
        nearPlane,
        farPlane); // Not resized until game loop. Send to shader

    // Lights
    glm::vec3 lightPos0(0.f, 0.f, -1.f);

    // Init Uniforms
    core_program.use();

    // send to shader [ Init uniforms ]
    core_program.setMat4fv(ViewMatrix, "ViewMatrix");
    core_program.setMat4fv(ProjectionMatrix, "ProjectionMatrix");

    // Send light pos -> fragment shader
    core_program.setVec3f(lightPos0, "lightPos0");
    core_program.setVec3f(camPosition, "cameraPos");





    // MAIN PROGRAM LOOP
    while (!glfwWindowShouldClose(window)) { // As long as window is open

        // UPDATE INPUT ---
        glfwPollEvents();
        updateInput(window, test);

        // Game Update ---
        updateInput(window);

        // DRAW ---

        // Clear all buffers
        glClearColor(0.f, 0.f, 0.f, 1.f); // Black
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        // Update uniforms (textures)
        core_program.set1i(texture0.getTextureUnit(), "texture0"); // Bind shader program before sending data!!!
        core_program.set1i(texture1.getTextureUnit(), "texture1");
        material0.sendToShader(core_program); // update texture in shader [Fragment]

        // Move, rotate and scale
        // position.z -= 0.001f;
        // rotation.y += 0.05f;

        // get correct view plane every frame
        glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
        ProjectionMatrix = glm::mat4(1.f);
        ProjectionMatrix = glm::perspective(
            glm::radians(fov),
            static_cast<float>(framebufferWidth) / framebufferHeight,
            nearPlane,
            farPlane);
        core_program.setMat4fv(ProjectionMatrix, "ProjectionMatrix");

        // Use a program
        core_program.use(); // tell what shaders to use

        // Activate texture (binding)
        texture0.bind();
        texture1.bind();


        // Draw

        test.render(&core_program);

        // End Draw
        glfwSwapBuffers(window); // Swap back & front buffer
        glFlush();

        // unbinding
        glBindVertexArray(0);
        glUseProgram(0);
        glActiveTexture(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    // END OF PROGRAM
    glfwDestroyWindow(window);
    glfwTerminate();

    // Delete program
    return 0;
}
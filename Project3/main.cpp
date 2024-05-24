#pragma once
#include "libs.h"



Vertex vertices[] =
{
    // Position                         // Color                        // Texcoords

    glm::vec3(-0.5f, 0.5f, 0.f),         glm::vec3(1.f, 0.f, 0.f),       glm::vec2(0.f, 1.f),
    glm::vec3(-0.5f, -0.5f, 0.f),        glm::vec3(0.f, 1.f, 0.f),       glm::vec2(0.f, 0.f),
    glm::vec3(0.5f, -0.5f, 0.f),         glm::vec3(0.f, 0.f, 1.f),       glm::vec2(1.f, 0.f),
    glm::vec3(0.5f, 0.5f, 0.f),          glm::vec3(1.f, 1.f, 0.f),       glm::vec2(1.f, 1.f)
    
};
unsigned nrOfVertices = sizeof(vertices) / sizeof(Vertex);

GLuint indices[] =
{
    0, 1, 2, // Triangle 1
    0, 2, 3  // Triangle 2
};
unsigned nrOfIndices = sizeof(indices) / sizeof(GLuint);

// update input function
void updateInput(GLFWwindow* window) {

    // Close window on [ESC] pressed
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

// Callback function for resizable window
void framebuffer_resize_callback(GLFWwindow* window, int fbW, int fbH) {
    glViewport(0, 0, fbW, fbH);
}

bool loadShaders(GLuint &program) {
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

    // OpenGL Options
    glEnable(GL_DEPTH_TEST); // enable use of Z & W coordinate
    glEnable(GL_CULL_FACE); // don't draw whats not shown
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW); // draw vertices counter clockwise
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // fill shape with color [DEFAULT:FILL]
    glEnable(GL_BLEND); // blend colors
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    // Initialize Shader
    GLuint core_program;
    if (!loadShaders(core_program))
        glfwTerminate();

    // MODEL

    // VAO, VBO, EBO
    // GEN VAO, & BIND
    GLuint VAO; // vertex Array Object
    glCreateVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // GEN VBO & BIND & SEND DATA
    GLuint VBO; // Vertex Buffer Object
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // Data sent to GPU

    // GEN EBO & BIND & SEND DATA
    GLuint EBO; // Element Buffer Object
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); // Do once (All on GPU side)

    // SET VERTEXATRIPOINTERS & ENABLE (Input Assembly)
    //GLuint attribloc = glGetAttribLocation(core_program);
    
    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);

    // Color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
    glEnableVertexAttribArray(1);

    // Texcoord
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texcoord));
    glEnableVertexAttribArray(2);

    // BIND VAO 0
    glBindVertexArray(0); // Unbind any active Array

    // TEXTURE INIT
    int image_width = 0;
    int image_height = 0;
    unsigned char* image = SOIL_load_image("Images/fish.png", &image_width, &image_height, NULL, SOIL_LOAD_RGBA);

    // Get texture ID
    GLuint texture0;
    glGenTextures(1, &texture0);
    glBindTexture(GL_TEXTURE_2D, texture0);
    
    if (image) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D); // makes smaller & biger versions for distance

        // Repeat texture to fill canvas
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // S=Xcoord T=Ycoord
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR); // antialiasing
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // MAGnification, MINification (no mipmap)


    }
    else {
        std::cout << "ERROR::TEXTURE_LOADING_FAILED" << "\n";
    }

    // Texture cleanup
    glActiveTexture(0); // No active texture
    glBindTexture(GL_TEXTURE_2D, 0); // Unbind all textures
    SOIL_free_image_data(image); // Free loaded texture from memory














    // MAIN PROGRAM LOOP
    while (!glfwWindowShouldClose(window)) { // As long as window is open
        
        // UPDATE INPUT ---
        glfwPollEvents();

        // Game Update ---
        updateInput(window);

        // DRAW ---

        // Clear all buffers
        glClearColor(0.f, 0.f, 0.f, 1.f); // Black
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        // Use a program
        glUseProgram(core_program); // tell what shaders we want to use

        // Update uniforms (textures)
        glUniform1i(glGetUniformLocation(core_program, "texture0"), 0); // Bind shader program before sending data!!!

        // Activate texture (binding)
        glActiveTexture(GL_TEXTURE0); // tex in spot 0
        glBindTexture(GL_TEXTURE_2D, texture0);

        // Bind vertex array object
        glBindVertexArray(VAO);

        // Draw
        //glDrawArrays(GL_TRIANGLES, 0, nrOfVertices); // Draw all points. Not optimanl.
        glDrawElements(GL_TRIANGLES, nrOfIndices, GL_UNSIGNED_INT, 0);

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
    glDeleteProgram(core_program);
    return 0;
}

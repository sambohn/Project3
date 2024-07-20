#pragma once
#include "libs.h"
#include "Camera.h"


// Enumerations
enum shader_enum { SHADER_CORE_PROGRAM = 0 };
enum texture_enum { TEX_CORAL0, TEX_CORAL_SPECULAR0, TEX_FISH1, TEX_FISH_SPECULAR1};
enum material_enum { MAT_1 = 0 };
enum mesh_enum { MESH_QUAD = 0 };


class Game
{
private:
	// Variables
		// Window
	GLFWwindow* window;
	const int WINDOW_WIDTH;
	const int WINDOW_HEIGHT;
	int framebufferWidth;
	int framebufferHeight;

	// OpenGL Context
	const int GL_VERSION_MAJOR;
	const int GL_VERSION_MINOR;

	// Delta time [ independent of frame rate]
	float dt;
	float curTime;
	float lastTime;

	// Mouse & Cursor
	double lastMouseX;
	double lastMouseY;
	double mouseX;
	double mouseY;
	double mouseOffsetX;
	double mouseOffsetY;
	bool firstMouse;

	// Camera
	Camera camera;

	// Matrices
	glm::mat4 ViewMatrix;
	glm::mat4 ProjectionMatrix;
	glm::vec3 camPosition;
	glm::vec3 worldUp;
	glm::vec3 camFront;

	

	float fov;
	float nearPlane; // Not 0. Want slightly behind cam to avoid clipping
	float farPlane;

	// Shaders
	std::vector<Shader*> shaders;

	// Textures
	std::vector<Texture*> textures;

	// Materials
	std::vector<Material*> materials;

	// Models
	std::vector<Model*> models;

	// Lights
	std::vector<glm::vec3*> lights;


	// Private functions
	void initGLFW();
	void initWindow(const char* title, bool resizable);
	void initGLEW(); // After context creation
	void initOpenGLOptions();
	void initMatrices();
	void initShaders();
	void initTextures();
	void initMaterials();
	void initMeshes();
	void initModels();
	void initLights();
	void initUniforms();

	void updateUniforms();

	// Static variables

public:
	Game(const char* title,
		const int WINDOW_WIDTH, const int WINDOW_HEIGHT,
		int GL_VERSION_MAJOR, int GL_VERSION_MINOR,
		bool resizable);

	virtual ~Game();

	// Accessors
	int getWindowShouldClose();

	// Modifiers
	void setWindowShouldClose();

	// Functions
	void updateDt();
	void updateMouseInput();
	void updateKeyboardInput();
	void updateInput();
	void update();
	void render();

	// Static functions
	static void framebuffer_resize_callback(GLFWwindow* window, int fbW, int fbH);



};


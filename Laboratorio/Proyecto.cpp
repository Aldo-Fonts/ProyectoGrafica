/*---------------------------------------------------------------------------------------------*/
// Autor:					Aldo Hernandez Fontes
// Fecha de creacion:		Octubre 11, 2021
// Ultima actualizacion:	Noviembre 24, 2021
// Script:					Proyecto.cpp
// Descripcion:				El script presente contiene todo el código necesario para compilar
//							y  visualizar la  aplicación de tal manera que  se pueden apreciar
//							los comandos correspondientes para la realizacion del programa.
/*---------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------*/
// Author:					Aldo Hernandez Fontes
// Creation date:			Octobre 11, 2021
// Last update:				Novembre 24, 2021
// Script:					Proyecto.cpp
// Description:				The  present  script  contains the code necessary  to  compile  and
//							visualize the app in such a way that the corresponding commands for
//							implementation of the program can be appreciated.
/*---------------------------------------------------------------------------------------------*/
// Librarie(s)
#include <Windows.h>
#include <glad/glad.h>
#include <glfw3.h>						// Main
#include <stdlib.h>		
#include <glm/glm.hpp>					// Camera and model
#include <glm/gtc/matrix_transform.hpp>	// Camera and model
#include <glm/gtc/type_ptr.hpp>
#include <time.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>					// Texture

#define SDL_MAIN_HANDLED
#include <SDL/SDL.h>
#include <shader_m.h>
#include <camera.h>
#include <modelAnim.h>
#include <model.h>
#include <Skybox.h>
#include <iostream>
#include <fstream>

//#pragma comment(lib, "winmm.lib")

// Function(s)
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//void my_input(GLFWwindow *window);
void my_input(GLFWwindow* window, int key, int scancode, int action, int mods);
void animate(void);
void reset(void);
void loadKeyFrames(void);

// Settings
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;
GLFWmonitor *monitors;

void getResolution(void);

// Camera
Camera camera(glm::vec3(0.0f, 10.0f, 90.0f));
float MovementSpeed = 0.1f;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// Timing
const int FPS = 60;
const int LOOP_TIME = 1000 / FPS; // = 16 milisec // 1000 millisec == 1 sec
double	deltaTime = 0.0f,
		lastFrame = 0.0f;

// Lighting
glm::vec3 lightPosition(0.0f, 4.0f, -10.0f);
glm::vec3 lightDirection(-1.0f, 0.0f, 0.0f);
double MyVariable = 0.0;

// Variable(s)
// Audio
bool	banderaAudio = false,
		banderaAudioCarePackage = true,
		sound = true,
		sound0 = true;

// Animacion (modelo jerarquico)
float	buzonDoorRot = 0.1f,
		buzonPlumaRot = 0.1;

// Animation 1
float	coreRotY = 0.0f,
		tapePosZ = 0.0f,
		tapePosY = 0.0f,
		tapePosX = 0.0f,
		tapeRot	 = 0.0f;

// Animation 2
float	cryptoX = 0.0f,
		cryptoY = 0.0f,
		cryptoZ = 0.0f,
		cryptoRot = 0.0f,
		alpha = 0.0f;

// Animation 3
float	carePackageX = 0.0f,
		carePackageY = 0.0f,
		carePackageZ = 0.0f,
		carePackageRot = 0.0f,
		beta = 0.0f;

// Animation 4. KeyFrames
float	posX = 0.0f,
		posY = 0.0f,
		posZ = 0.0f,
		rotY = 0.0f,
		QZ = 0.0f,
		PDX = 0.0f,
		PDY = 0.0f,
		PDZ = 0.0f,
		PDR = 0.0f,
		PIX = 0.0f,
		PIY = 0.0f,
		PIZ = 0.0f,
		PIR = 0.0f;

float	posXInc = 0.0f,
		posYInc = 0.0f,
		posZInc = 0.0f,
		rotYInc = 0.0f,
		QZInc = 0.0f,
		PDXInc = 0.0f,
		PDYInc = 0.0f,
		PDZInc = 0.0f,
		PDRInc = 0.0f,
		PIXInc = 0.0f,
		PIYInc = 0.0f,
		PIZInc = 0.0f,
		PIRInc = 0.0f;

// Animation variables
bool	animacion = false,
		cryptoAnimation = false,
		carePackageAnimation = false,
		buzonAnimation = false;

// Auxiliares
int		estado = 0,
		estadoC = 0,
		estadoB = 0;

float	aux0 = 0.0,
		aux1 = 0.0,
		aux2 = 0.0,
		aux3 = 0.0,
		aux4 = 0.0;
bool	bandera = true,
		Reversa = false,
		loadFrames = true;

#define MAX_FRAMES 9	// Frames
int i_max_steps = 60;	// Intermediate frames between keyframes
int i_curr_steps = 0;	

// Structure that saves the frames
typedef struct _frame
{
	// Variables to SAVE KeyFrames
	float posX;
	float posY;
	float posZ;
	float rotY;
	float QZ;
	float PDX;
	float PDY;
	float PDZ;
	float PDR;
	float PIX;
	float PIY;
	float PIZ;
	float PIR;
}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 0;			// Introduce data
bool play = false;
int playIndex = 0;

// Store the actual frame
void saveFrame(void)
{
	ofstream file;
	file.open("posx.txt", ios::out | ios::ate | ios::app);
	file << std::to_string(posX) + "\n";
	file.close();
	
	file.open("posy.txt", ios::out | ios::ate | ios::app);
	file << std::to_string(posY) + "\n";
	file.close();

	file.open("posz.txt", ios::out | ios::ate | ios::app);
	file << std::to_string(posZ) + "\n";
	file.close();

	file.open("roty.txt", ios::out | ios::ate | ios::app);
	file << std::to_string(rotY) + "\n";
	file.close();

	file.open("qz.txt", ios::out | ios::ate | ios::app);
	file << std::to_string(QZ) + "\n";
	file.close();

	file.open("pdx.txt", ios::out | ios::ate | ios::app);
	file << std::to_string(PDX) + "\n";
	file.close();

	file.open("pdy.txt", ios::out | ios::ate | ios::app);
	file << std::to_string(PDY) + "\n";
	file.close();

	file.open("pdz.txt", ios::out | ios::ate | ios::app);
	file << std::to_string(PDZ) + "\n";
	file.close();

	file.open("pdr.txt", ios::out | ios::ate | ios::app);
	file << std::to_string(PDR) + "\n";
	file.close();

	file.open("pix.txt", ios::out | ios::ate | ios::app);
	file << std::to_string(PIX) + "\n";
	file.close();

	file.open("piy.txt", ios::out | ios::ate | ios::app);
	file << std::to_string(PIY) + "\n";
	file.close();

	file.open("piz.txt", ios::out | ios::ate | ios::app);
	file << std::to_string(PIZ) + "\n";
	file.close();

	file.open("pir.txt", ios::out | ios::ate | ios::app);
	file << std::to_string(PIR) + "\n";
	file.close();

	std::cout << "Frame Index = " << FrameIndex << std::endl;
	// Data object is saved (positions and rotations)
	KeyFrame[FrameIndex].posX = posX;
	KeyFrame[FrameIndex].posY = posY;
	KeyFrame[FrameIndex].posZ = posZ;
	KeyFrame[FrameIndex].rotY = rotY;

	KeyFrame[FrameIndex].QZ = QZ;

	KeyFrame[FrameIndex].PDX = PDX;
	KeyFrame[FrameIndex].PDY = PDY;
	KeyFrame[FrameIndex].PDZ = PDZ;
	KeyFrame[FrameIndex].PDR = PDR;

	KeyFrame[FrameIndex].PIX = PIX;
	KeyFrame[FrameIndex].PIY = PIY;
	KeyFrame[FrameIndex].PIZ = PIZ;
	KeyFrame[FrameIndex].PIR = PIR;

	// FrameIndex increases
	FrameIndex++;
}

// Execute animation 
void resetElements(void)
{
	// The position and values came from Frame 0 
	posX = KeyFrame[0].posX;
	posY = KeyFrame[0].posY;
	posZ = KeyFrame[0].posZ;
	rotY = KeyFrame[0].rotY;

	QZ = KeyFrame[0].QZ;

	PDX = KeyFrame[0].PDX;
	PDY = KeyFrame[0].PDY;
	PDZ = KeyFrame[0].PDZ;
	PDR = KeyFrame[0].PDR;

	PIX = KeyFrame[0].PIX;
	PIY = KeyFrame[0].PIY;
	PIZ = KeyFrame[0].PIZ;
	PIR = KeyFrame[0].PIR;
}

// It obtains the increase the position between frames
void interpolation(void)
{
	posXInc = (KeyFrame[playIndex + 1].posX - KeyFrame[playIndex].posX) / i_max_steps;
	posYInc = (KeyFrame[playIndex + 1].posY - KeyFrame[playIndex].posY) / i_max_steps;
	posZInc = (KeyFrame[playIndex + 1].posZ - KeyFrame[playIndex].posZ) / i_max_steps;
	rotYInc = (KeyFrame[playIndex + 1].rotY - KeyFrame[playIndex].rotY) / i_max_steps;

	QZInc = (KeyFrame[playIndex + 1].QZ - KeyFrame[playIndex].QZ) / i_max_steps;

	PDXInc = (KeyFrame[playIndex + 1].PDX - KeyFrame[playIndex].PDX) / i_max_steps;
	PDYInc = (KeyFrame[playIndex + 1].PDY - KeyFrame[playIndex].PDY) / i_max_steps;
	PDZInc = (KeyFrame[playIndex + 1].PDZ - KeyFrame[playIndex].PDZ) / i_max_steps;
	PDRInc = (KeyFrame[playIndex + 1].PDR - KeyFrame[playIndex].PDR) / i_max_steps;

	PIXInc = (KeyFrame[playIndex + 1].PIX - KeyFrame[playIndex].PIX) / i_max_steps;
	PIYInc = (KeyFrame[playIndex + 1].PIY - KeyFrame[playIndex].PIY) / i_max_steps;
	PIZInc = (KeyFrame[playIndex + 1].PIZ - KeyFrame[playIndex].PIZ) / i_max_steps;
	PIRInc = (KeyFrame[playIndex + 1].PIR - KeyFrame[playIndex].PIR) / i_max_steps;
}

void loadKeyFrames(void)
{
	int i = 0;
	// Pos X
	std::ifstream file;
	file.open("posx.txt");
	std::string str;
	while (std::getline(file,str)) 
	{
		KeyFrame[i].posX = std::stof(str);
		i++;
		FrameIndex++;
	}
	file.close();

	// Pos Y
	i = 0;
	file.open("posy.txt");
	while (std::getline(file, str))
	{
		KeyFrame[i].posY = std::stof(str);
		i++;
	}
	file.close();

	// Pos Z
	i = 0;
	file.open("posz.txt");
	while (std::getline(file, str))
	{
		KeyFrame[i].posZ = std::stof(str);
		i++;
	}
	file.close();

	// Rot Y
	i = 0;
	file.open("roty.txt");
	while (std::getline(file, str))
	{
		KeyFrame[i].rotY = std::stof(str);
		i++;
	}
	file.close();

	// QZ
	i = 0;
	file.open("qz.txt");
	while (std::getline(file, str))
	{
		KeyFrame[i].QZ = std::stof(str);
		i++;
	}
	file.close();

	// PDX
	i = 0;
	file.open("pdx.txt");
	while (std::getline(file, str))
	{
		KeyFrame[i].PDX = std::stof(str);
		i++;
	}
	file.close();

	// PDY
	i = 0;
	file.open("pdy.txt");
	while (std::getline(file, str))
	{
		KeyFrame[i].PDY = std::stof(str);
		i++;
	}
	file.close();

	// PDZ
	i = 0;
	file.open("pdz.txt");
	while (std::getline(file, str))
	{
		KeyFrame[i].PDZ = std::stof(str);
		i++;
	}
	file.close();

	// PDR
	i = 0;
	file.open("pdr.txt");
	while (std::getline(file, str))
	{
		KeyFrame[i].PDR = std::stof(str);
		i++;
	}
	file.close();

	// PIX
	i = 0;
	file.open("pix.txt");
	while (std::getline(file, str))
	{
		KeyFrame[i].PIX = std::stof(str);
		i++;
	}
	file.close();

	// PIY
	i = 0;
	file.open("piy.txt");
	while (std::getline(file, str))
	{
		KeyFrame[i].PIY = std::stof(str);
		i++;
	}
	file.close();

	// PIZ
	i = 0;
	file.open("piz.txt");
	while (std::getline(file, str))
	{
		KeyFrame[i].PIZ = std::stof(str);
		i++;
	}
	file.close();

	// PIR
	i = 0;
	file.open("posx.txt");
	while (std::getline(file, str))
	{
		KeyFrame[i].PIR = std::stof(str);
		i++;
	}
	file.close();
}

void animate(void)
{
	if (play)
	{
		if (i_curr_steps >= i_max_steps)	// End of animation between frames?
		{
			playIndex++;
			if (playIndex > FrameIndex - 2)	// End of total animation?
			{
				std::cout << "Animation ended" << std::endl;
				playIndex = 0;
				play = false;
			}
			else							// Next frame interpolations
			{
				i_curr_steps = 0; // Reset counter
								  // Interpolation
				interpolation();
			}
		}
		else
		{
			// Draw animation
			posX += posXInc;
			posY += posYInc;
			posZ += posZInc;
			rotY += rotYInc;

			QZ += QZInc;

			PDX += PDXInc;
			PDY += PDYInc;
			PDZ += PDZInc;
			PDR += PDRInc;

			PIX += PIXInc;
			PIY += PIYInc;
			PIZ += PIZInc;
			PIR += PIRInc;

			i_curr_steps++;
		}
	}

	if (animacion){
		if ((estado == 0) || (estado == 1) || (estado == 2)){
			if (coreRotY < 360) {
				coreRotY = coreRotY + 5.0;
			}
			else {
				coreRotY = 0.0f;
				estado++;
			}
		}
		if (estado == 3) {
			if (tapePosY > -0.4) {
				aux0 = aux0 + 0.3;
				aux1 = aux1 - 0.6;
				aux2 = aux2 + 0.05;
				tapePosX = cos(50) * aux0;
				tapePosZ = cos(50) * aux1;
				tapePosY = (sin(50) * aux2) - (pow(aux2, 2));
				tapeRot = tapeRot + 7.0;
			}
			else {
				tapeRot = 110;
				tapePosY = 9;
				estado = 4;
			}
		}
	}
	if (cryptoAnimation)
	{
		if (estadoC == 0)
		{
			if (cryptoX <= 50)
			{
				cryptoX++;
				cryptoZ++;
			}
			else 
			{
				cryptoRot = 45.0f;
				estadoC++;
			}
		}
		if (estadoC == 1)
		{
			if (alpha <= 180) 
			{
				alpha++;
				cryptoX = (50 * cos(glm::radians(alpha+270))) + 50.0f;
				cryptoZ = -(50 * sin(glm::radians(alpha+270)));
				cryptoRot = (1.25 * alpha) + 45;
			}
			else
			{
				alpha = 0.0f;
				cryptoRot = 270.0f;
				estadoC++;
			}	
		}
		if (estadoC == 2)
		{
			if (cryptoX >= -50)
			{
				cryptoX--;
				cryptoZ++;
			}
			else 
			{
				cryptoRot = 225.0f;
				estadoC++;
			}
		}
		if (estadoC == 3)
		{
			if (alpha <= 180)
			{
				alpha++;
				cryptoX = -(50 * cos(glm::radians(alpha + 270))) - 50.f;
				cryptoZ = -(50 * sin(glm::radians(alpha + 270)));
				cryptoRot = -(1.25 * alpha) + 225;
			}
			else
			{
				alpha = 0.0f;
				cryptoRot = 0.0f;
				estadoC = 0;
			}
		}
	}

	if (carePackageAnimation)
	{
		if ((carePackageY + 1000.0f) > 1)
		{
			beta += 0.1f;
			carePackageX = 20 * cos(beta);
			carePackageZ = 20 * sin(beta);
			carePackageY -= 4.5f;
			carePackageRot = 45 * beta;
		}
		else
			banderaAudioCarePackage = false;
		
	}

	if (buzonAnimation)
	{
		if (estadoB == 0) 
		{
			if (buzonDoorRot <= 90)
				buzonDoorRot++;
			else
				estadoB = 1;
		}
		if (estadoB == 1) {
			if (buzonPlumaRot <= 90)
				buzonPlumaRot++;
			else
				buzonAnimation = false;
		}
	}
}

void reset()
{
	animacion = false;
	bandera = true;
	estado = 0;
}

void getResolution()
{
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;
}


int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	// --------------------
	monitors = glfwGetPrimaryMonitor();
	getResolution();

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CGeIHC", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwSetWindowPos(window, 0, 30);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, my_input);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile shaders
	// -------------------------
	Shader staticShader("Shaders/shader_Lights.vs", "Shaders/shader_Lights.fs");	// Static objects
	Shader skyboxShader("Shaders/skybox.vs", "Shaders/skybox.fs");					// Scene 
	Shader animShader("Shaders/anim.vs", "Shaders/anim.fs");						// Animated objects

	vector<std::string> faces
	{
		"resources/skybox/right.jpg",
		"resources/skybox/left.jpg",
		"resources/skybox/top.jpg",
		"resources/skybox/bottom.jpg",
		"resources/skybox/front.jpg",
		"resources/skybox/back.jpg"
	};

	Skybox skybox = Skybox(faces);

	// Shader configuration
	// --------------------
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

	// load models
	// -----------
	Model piso("resources/objects/piso/piso.obj");
	Model pilita("resourcesProject/pilita/pila.obj");
	Model supplie("resourcesProject/supplie/Suplie.obj");
	Model building("resourcesProject/building/building.obj");
	//Model vidrio("resourcesProject/building/Prueba/vidrio.obj");
	Model core("resourcesProject/LootTick/Core.obj");
	Model tape("resourcesProject/LootTick/Tape.obj");
	Model base("resourcesProject/LootTick/Base.obj");
	Model crypto("resourcesProject/drone/crypto.obj");
	Model ammo("resourcesProject/Ammo/ammo.obj");
	Model carePackage("resourcesProject/carePackage/carePackage.obj");
	Model buzonCore("resourcesProject/buzon/core.obj");
	Model buzonDoor("resourcesProject/buzon/door.obj");
	Model buzonPluma("resourcesProject/buzon/pluma.obj");
	Model psyduckCore("resourcesProject/psyduck/core.obj");
	Model psyduckPD("resourcesProject/psyduck/patitaD.obj");
	Model psyduckPI("resourcesProject/psyduck/patitaI.obj");
	Model psyduckQ("resourcesProject/psyduck/question.obj");
	Model DeathBox("resourcesProject/DeathBox/DeathBox.obj");
	Model Respawn("resourcesProject/Respawn/Respawn.obj");
	Model DOC("resourcesProject/doc/doc.obj");
	Model Nessys("resourcesProject/Nessy/Nessys.obj");
	Model PokeBalls("resourcesProject/PokeBalls/PokeBalls.obj");
	Model Pallet("resourcesProject/Pallet/Pallet.obj");

	// Inicialización de KeyFrames
	for (int i = 0; i < MAX_FRAMES; i++)
	{
		KeyFrame[i].posX = 0;
		KeyFrame[i].posY = 0;
		KeyFrame[i].posZ = 0;

		KeyFrame[i].QZ = 0;

		KeyFrame[i].PDX = 0;
		KeyFrame[i].PDY = 0;
		KeyFrame[i].PDZ = 0;
		KeyFrame[i].PDR = 0;

		KeyFrame[i].PIX = 0;
		KeyFrame[i].PIY = 0;
		KeyFrame[i].PIZ = 0;
		KeyFrame[i].PIR = 0;
	}

	// Draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		skyboxShader.setInt("skybox", 0);
		
		// per-frame time logic
		// --------------------
		lastFrame = SDL_GetTicks();

		// input
		// -----
		//my_input(window);
		animate();

		// render
		// ------
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// don't forget to enable shader before setting uniforms
		staticShader.use();
		//===================================================================================
		// Setup Advanced Lights
		//===================================================================================
		// The light that becomes from a spece of the scene
		staticShader.setVec3("viewPos", camera.Position);
		staticShader.setVec3("dirLight.direction", lightDirection);
		staticShader.setVec3("dirLight.ambient", glm::vec3(0.5f, 0.5f, 0.5f));
		staticShader.setVec3("dirLight.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));
		
		//===================================================================================
		// A kind of spotlight in the center of the scene
		// HOUSE'S SPOTLIGHT
		staticShader.setVec3("pointLight[0].position", glm::vec3(-17.5f, 12.0f, -68.0f));
		staticShader.setVec3("pointLight[0].ambient", glm::vec3(0.5f, 0.5f, 0.5f));
		staticShader.setVec3("pointLight[0].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("pointLight[0].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[0].constant", 0.6f);
		staticShader.setFloat("pointLight[0].linear", 0.009f);
		staticShader.setFloat("pointLight[0].quadratic", 0.0032f);
		/*
		//===================================================================================
		// A kind of spotlight that can have a diferent postion
		// SHIELD BATTERY
		staticShader.setVec3("pointLight[1].position", glm::vec3(45.3f, 1.75f, 0.9f));
		staticShader.setVec3("pointLight[1].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].diffuse", glm::vec3(0.0f, 0.0f, 1.0f));
		staticShader.setVec3("pointLight[1].specular", glm::vec3(0.0f, 0.0f, 1.0f));
		staticShader.setFloat("pointLight[1].constant", 1.0f);
		staticShader.setFloat("pointLight[1].linear", 0.09f);
		staticShader.setFloat("pointLight[1].quadratic", 0.0f);

		//===================================================================================
		// A kind of spotlight that can have a diferent postion
		// LAMP
		staticShader.setVec3("pointLight[2].position", glm::vec3(-50.0f, 0.25f, -50.0f));
		staticShader.setVec3("pointLight[2].ambient", glm::vec3(0.025f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[2].diffuse", glm::vec3(0.1f, 0.1f, 0.1f));
		staticShader.setVec3("pointLight[2].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[2].constant", 0.005f);
		staticShader.setFloat("pointLight[2].linear", 0.005f);
		staticShader.setFloat("pointLight[2].quadratic", 0.0032f);
		
		staticShader.setVec3("pointLight[3].position", glm::vec3(-50.0f, 4.25f, -50.0f));
		staticShader.setVec3("pointLight[3].ambient", glm::vec3(0.025f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[3].diffuse", glm::vec3(0.1f, 0.1f, 0.1f));
		staticShader.setVec3("pointLight[3].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[3].constant", 0.005f);
		staticShader.setFloat("pointLight[3].linear", 0.005f);
		staticShader.setFloat("pointLight[3].quadratic", 0.0032f);
		*/
		//===================================================================================
		staticShader.setFloat("material_shininess", 32.0f);
		//===================================================================================
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 tmp0 = glm::mat4(1.0f);
		glm::mat4 tmp1 = glm::mat4(1.0f);
		glm::mat4 tmp2 = glm::mat4(1.0f);

		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
		glm::mat4 view = camera.GetViewMatrix();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);

		//// Light
		glm::vec3 lightColor = glm::vec3(0.6f);
		glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.75f);
		

		// -------------------------------------------------------------------------------------------------------------------------
		// Personaje Animacion
		// -------------------------------------------------------------------------------------------------------------------------
		//Remember to activate the shader with the animation
		animShader.use();
		animShader.setMat4("projection", projection);
		animShader.setMat4("view", view);
	
		animShader.setVec3("material.specular", glm::vec3(0.5f));
		animShader.setFloat("material.shininess", 32.0f);
		animShader.setVec3("light.ambient", ambientColor);
		animShader.setVec3("light.diffuse", diffuseColor);
		animShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
		animShader.setVec3("light.direction", lightDirection);
		animShader.setVec3("viewPos", camera.Position);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-40.3f, 1.75f, 0.3f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(1.2f));	// it's a bit too big for our scene, so scale it down
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		animShader.setMat4("model", model);
		//animacionPersonaje.Draw(animShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Segundo Personaje Animacion
		// -------------------------------------------------------------------------------------------------------------------------

		model = glm::translate(glm::mat4(1.0f), glm::vec3(40.3f, 1.75f, 0.3f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.5f));	// it's a bit too big for our scene, so scale it down
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		animShader.setMat4("model", model);
		//ninja.Draw(animShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Escenario
		// -------------------------------------------------------------------------------------------------------------------------
		staticShader.use();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		piso.Draw(staticShader);
		
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.5, 0.0f)); 
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		tmp2 = model;
		staticShader.setMat4("model", model);
		building.Draw(staticShader);
		// -------------------------------------------------------------------------------------------------------------------------
		// Shields
		// -------------------------------------------------------------------------------------------------------------------------
		model = glm::translate(glm::mat4(1.0f), glm::vec3(30.0f, 0.25f, 0.9f));
		model = glm::scale(model, glm::vec3(0.5f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		pilita.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(12.5f, 35.0f, 5.0f));
		model = glm::scale(model, glm::vec3(0.5f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		pilita.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-20.0f, 0.25f, -10.0f));
		model = glm::scale(model, glm::vec3(0.5f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		pilita.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-15.0f, 33.5f, 7.0f));
		model = glm::scale(model, glm::vec3(0.5f));
		staticShader.setMat4("model", model);
		pilita.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.5f, 50.0f));
		model = glm::scale(model, glm::vec3(0.5f));
		staticShader.setMat4("model", model);
		pilita.Draw(staticShader);
		// -------------------------------------------------------------------------------------------------------------------------
		// Ammo
		// -------------------------------------------------------------------------------------------------------------------------
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.5f, 50.0f));
		model = glm::scale(model, glm::vec3(0.5f));
		staticShader.setMat4("model", model);
		pilita.Draw(staticShader);
		// -------------------------------------------------------------------------------------------------------------------------
		// DOC
		// -------------------------------------------------------------------------------------------------------------------------
		model = glm::translate(glm::mat4(1.0f), glm::vec3(25.0f, -1.5f, -25.0f));
		model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		DOC.Draw(staticShader);
		// -------------------------------------------------------------------------------------------------------------------------
		// Death Box
		// -------------------------------------------------------------------------------------------------------------------------
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.5f, 0.0f));
		staticShader.setMat4("model", model);
		DeathBox.Draw(staticShader);
		// -------------------------------------------------------------------------------------------------------------------------
		// Nessy
		// -------------------------------------------------------------------------------------------------------------------------
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.5f, 0.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		Nessys.Draw(staticShader);
		// -------------------------------------------------------------------------------------------------------------------------
		// PokeBalls
		// -------------------------------------------------------------------------------------------------------------------------
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.5f, 0.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		PokeBalls.Draw(staticShader);
		// -------------------------------------------------------------------------------------------------------------------------
		// Pallet
		// -------------------------------------------------------------------------------------------------------------------------
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.5f, 0.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		Pallet.Draw(staticShader);
		// -------------------------------------------------------------------------------------------------------------------------
		// Respawn
		// -------------------------------------------------------------------------------------------------------------------------
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-90.0f, -1.5f, 90.0f));
		staticShader.setMat4("model", model);
		Respawn.Draw(staticShader);
		// -------------------------------------------------------------------------------------------------------------------------
		// Supplie
		// -------------------------------------------------------------------------------------------------------------------------
		model = glm::translate(glm::mat4(1.0f), glm::vec3(25.5f, -1.75f, 43.5f));
		model = glm::rotate(model, glm::radians(-60.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(7.5f, 7.5f, 7.5f));
		staticShader.setMat4("model", model);
		supplie.Draw(staticShader);
		
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-25.5f, -1.75f, 43.5f));
		model = glm::rotate(model, glm::radians(-120.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(7.5f, 7.5f, 7.5f));
		staticShader.setMat4("model", model);
		supplie.Draw(staticShader);
		// -------------------------------------------------------------------------------------------------------------------------
		// Loot Tick
		// Modelado jerárquico para la animación
		// -------------------------------------------------------------------------------------------------------------------------
		model = glm::translate(glm::mat4(1.0f), glm::vec3(2.0, -1.5f, -70.0f));
		tmp1 = model;
		model = glm::rotate(model, glm::radians(coreRotY), glm::vec3(0.0f, 1.0f, 0.0f));
		tmp0 = model;
		staticShader.setMat4("model", model);
		core.Draw(staticShader);

		model = glm::translate(tmp0, glm::vec3(tapePosX, tapePosY, tapePosZ));
		model = glm::rotate(model, glm::radians(tapeRot), glm::vec3(-1.0f, 0.0f, -1.0f));
		staticShader.setMat4("model", model);
		tape.Draw(staticShader);

		model = glm::translate(tmp1, glm::vec3(0.0f, 0.0f, 0.0));
		staticShader.setMat4("model", model);
		base.Draw(staticShader);
		// -------------------------------------------------------------------------------------------------------------------------
		// Crypto's drone
		// -------------------------------------------------------------------------------------------------------------------------
		model = glm::translate(glm::mat4(1.0f), glm::vec3(cryptoX, cryptoY + 100.0f, cryptoZ));
		model = glm::rotate(model, glm::radians(cryptoRot + 45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		crypto.Draw(staticShader);
		// -------------------------------------------------------------------------------------------------------------------------
		// Care Package
		// -------------------------------------------------------------------------------------------------------------------------
		model = glm::translate(glm::mat4(1.0f), glm::vec3(carePackageX + 200.0f, carePackageY + 1000.0f, carePackageZ));
		model = glm::rotate(model, glm::radians(carePackageRot), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		carePackage.Draw(staticShader);
		// -------------------------------------------------------------------------------------------------------------------------
		// Buzon
		// -------------------------------------------------------------------------------------------------------------------------
		model = glm::translate(glm::mat4(1.0f), glm::vec3(50.0f, 0.0f, 185.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		tmp0 = model;
		staticShader.setMat4("model", model);
		buzonCore.Draw(staticShader);

		model = glm::translate(tmp0, glm::vec3(0.0f, 32.011f, 16.5f));
		model = glm::rotate(model, glm::radians(buzonDoorRot), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		buzonDoor.Draw(staticShader);

		model = glm::translate(tmp0, glm::vec3(11.5f, 43.99f, 0.0f));
		model = glm::rotate(model, glm::radians(buzonPlumaRot), glm::vec3(-1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		buzonPluma.Draw(staticShader);
		// -------------------------------------------------------------------------------------------------------------------------
		// Psyduck
		// -------------------------------------------------------------------------------------------------------------------------
		model = glm::translate(glm::mat4(1.0f), glm::vec3(posX - 50.0f, posY + 12.75f, posZ + 200.0f));
		model = glm::rotate(model, glm::radians(rotY + 45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		tmp0 = model;
		staticShader.setMat4("model", model);
		psyduckCore.Draw(staticShader);

		model = glm::translate(tmp0, glm::vec3(0.0f, QZ, 0.0f));
		staticShader.setMat4("model", model);
		psyduckQ.Draw(staticShader);

		model = glm::translate(tmp0, glm::vec3(PDX - 3.5f, PDY - 11.85, PDZ + 0.45));
		model = glm::rotate(model, glm::radians(PDR), glm::vec3(1.0f, 0.0f, 1.0f));
		staticShader.setMat4("model", model);
		psyduckPD.Draw(staticShader);

		model = glm::translate(tmp0, glm::vec3(PIX + 3.5f, PIY - 11.85, PIZ + 0.45));
		model = glm::rotate(model, glm::radians(PIR), glm::vec3(1.0f, 0.0f, -1.0f));
		staticShader.setMat4("model", model);
		psyduckPI.Draw(staticShader);
		// -------------------------------------------------------------------------------------------------------------------------
		// Caja Transparente --- Siguiente Práctica
		// -------------------------------------------------------------------------------------------------------------------------
		/*
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		model = glm::translate(tmp2, glm::vec3(0.0f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		vidrio.Draw(staticShader);
		glDisable(GL_BLEND); 
		*/
		// -------------------------------------------------------------------------------------------------------------------------
		// Termina Escenario
		// -------------------------------------------------------------------------------------------------------------------------

		//-------------------------------------------------------------------------------------
		// draw skybox as last
		// -------------------
		skyboxShader.use();
		skybox.Draw(skyboxShader, view, projection, camera);

		// Limitar el framerate a 60
		deltaTime = SDL_GetTicks() - lastFrame; // time for full 1 loop
		//std::cout <<"frame time = " << frameTime << " milli sec"<< std::endl;
		if (deltaTime < LOOP_TIME)
		{
			SDL_Delay((int)(LOOP_TIME - deltaTime));
		}

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	skybox.Terminate();

	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, (float)deltaTime);

	// Audio
	if (key == GLFW_KEY_X && action == GLFW_PRESS)
	{
		banderaAudio ^= true;
		if (banderaAudio)
		{
			sound0 = PlaySound(TEXT("resourcesProject/Audio/Lifeline.wav"), NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
		}
		else
		{
			sound0 = PlaySound(NULL, 0, 0);
		}
		
	}

	// Loot tick animation
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		animacion ^= true;

	// Zubat animation
	if (key == GLFW_KEY_Z && action == GLFW_PRESS)
		cryptoAnimation ^= true;

	// Care Package animation
	if (key == GLFW_KEY_C && action == GLFW_PRESS)
	{
		carePackageAnimation ^= true;
		if (carePackageAnimation)
		{
			if(banderaAudioCarePackage)
			{
				sound = PlaySound(TEXT("resourcesProject/Audio/CarePackage.wav"), NULL, SND_ASYNC | SND_FILENAME);
				banderaAudio = false;
			}
		}
	}

	// Buzon animation
	if (key == GLFW_KEY_V && action == GLFW_PRESS)
		buzonAnimation ^= true;
	//To play KeyFrame animation 
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		if (loadFrames)
		{
			loadKeyFrames(); 
			std::cout << "Load Key Frames COMPLETE" << std::endl;
			loadFrames = false;
		}
		if (play == false && (FrameIndex > 1))
		{
			std::cout << "Play animation" << std::endl;
			resetElements();
			//First Interpolation				
			interpolation();

			play = true;
			playIndex = 0;
			i_curr_steps = 0;
		}
		else
		{
			play = false;
			std::cout << "Not enough Key Frames" << std::endl;
		}
	}

	//To Save a KeyFrame
	/*
	if (key == GLFW_KEY_L && action == GLFW_PRESS)
	{
		if (FrameIndex < MAX_FRAMES)
		{
			saveFrame();
		}
	}

	// Temporal KeyFrame
	
	if (key == GLFW_KEY_U && action == GLFW_PRESS)
		Reversa ^= true;

	if (key == GLFW_KEY_E && action == GLFW_PRESS) 
	{
		if(Reversa)
			posX -= 0.1;
		else
			posX += 0.1;
	}

	if (key == GLFW_KEY_R && action == GLFW_PRESS)
	{
		if (Reversa)
			posY -= 0.1;
		else
			posY += 0.1;
	}

	if (key == GLFW_KEY_T && action == GLFW_PRESS)
	{
		if (Reversa)
			posZ -= 0.1;
		else
			posZ += 0.1;
	}

	if (key == GLFW_KEY_Y && action == GLFW_PRESS)
	{
		if (Reversa)
			rotY -= 1;
		else
			rotY += 1;
	}
	
	if (key == GLFW_KEY_J && action == GLFW_PRESS)
	{
		if (Reversa)
			QZ -= 0.1;
		else
			QZ += 0.1;
	}

	if (key == GLFW_KEY_F && action == GLFW_PRESS)
	{
		if (Reversa)
			PDX += 0.1;
		else
			PDX += 0.1;
	}

	if (key == GLFW_KEY_G && action == GLFW_PRESS)
	{
		if (Reversa)
			PDY -= 0.1;
		else
			PDY += 0.1;
	}


	if (key == GLFW_KEY_H && action == GLFW_PRESS) {
		if (Reversa)
			PDZ -= 0.1;
		else
			PDZ += 0.1;
	}

	if (key == GLFW_KEY_I && action == GLFW_PRESS)
	{
		if (Reversa)
			PDR -= 1;
		else
			PDR += 1;
	}


	if (key == GLFW_KEY_B && action == GLFW_PRESS)
	{
		if (Reversa)
			PIX -= 0.1;
		else
			PIX += 0.1;
	}


	if (key == GLFW_KEY_N && action == GLFW_PRESS)
	{
		if (Reversa)
			PIY -= 0.1;
		else
			PIY += 0.1;
	}

	if (key == GLFW_KEY_M && action == GLFW_PRESS)
	{
		if (Reversa)
			PIZ -= 0.1;
		else
			PIZ += 0.1;
	}


	if (key == GLFW_KEY_O && action == GLFW_PRESS)
	{
		if (Reversa)
			PIR -= 1;
		else
			PIR += 1;
	}
	*/
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}
// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}
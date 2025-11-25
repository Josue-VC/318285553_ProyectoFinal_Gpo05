// ProyectoF.cpp
// Proyecto Final - Casa de Billy y Mandy
// Computación Gráfica e interaccion humano-computadora
//Alumno:Josué Vázquez Cárdenas 

//Librerias utilizadas
#include <iostream>
#include <cmath>

// GLEW
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// SOIL2 for texture loading
#include "SOIL2/SOIL2.h"

// Custom Headers
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "modelAnim.h"


// Function prototypes
//Registra movimientos en el teclado
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
//Registra movimiento en el mouse
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
//Detecta el movimiento en teclas presionadas 
void DoMovement();
//Funcion para analisis de las animaciones
void Animation();
//Dibuja las primitivas del cubo
void ObjetoDraw(glm::mat4 base, glm::vec3 escala, glm::vec3 traslado, GLint uniformModel);

// Window dimensions
const GLuint WIDTH = 1600, HEIGHT = 900;
int SCREEN_WIDTH, SCREEN_HEIGHT;


// Camera
//Posicion inicial de la camara
Camera camera(glm::vec3(-5.0f, 5.0f, 37.0f));
GLfloat lastX = WIDTH / 2.0f;//Posicion del mouse en X
GLfloat lastY = HEIGHT / 2.0f;//Posicion del mouse en Y
bool keys[1024];//Arreglo para registro de teclas
bool firstMouse = true; //Evitar salto de camara al entrar a la venta

// Light attributes
bool active; // For debug light
glm::vec3 Light1 = glm::vec3(0); // For debug light color animation
glm::vec3 PosIni(-0.0f, 0.2f, -0.0f);

// Positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(0.0f, 5.0f, 0.0f),glm::vec3(0.0f, 0.0f, 0.0f),glm::vec3(0.0f, 0.0f, 0.0f),glm::vec3(0.0f, 0.0f, 0.0f)};

// Cube vertices (Pos, Normal, TexCoords)
float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};

// --- Variables de textura ---
GLuint sofaTexture;//Textura del sofa
GLuint lampBaseTexture;
GLuint lampShadeTexture;//Textura de lampara
GLuint tvCasingTexture;//Textura de la tv
GLuint tvScreenTexture;//Textura de la tv apagada
GLuint inodoroTexture;//Textura del inodoro
GLuint plantaTexture;//Textura de la planta interior
GLuint arbustoTexture;//Textura del arbusto
GLuint tvStaticTexture;//Textura del portal

//Variables para Animaciones Simples 
//Variables de puertas
float lampWobble = 0.0f;
bool lampAnimationActive = true;
bool lampLightActive = true; // Interruptor para la luz

// Variables de animacion de puertas
float doorE_angle = 0.0f;
float doorS_angle = 0.0f;
float doorB_angle = 0.0f;

bool doorE_open = false;
bool doorS_open = false;
bool doorB_open = false;

//Variables Animacion compleja Tv
bool tvAnimationActive = false; // Inicia apagada
float tvAnimTime = 0.0f;      // Temporizador para la animacion

// Variables globales para la lampara
bool lampState = true;       // true = prendida false = apagada
bool nKeyProcessed = false;  // Evitar errores con la pulsacion de teclas

// Deltatime
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

int main()
{
	// Init GLFW
	glfwInit();
	//Creacion de la ventana de visualizacion
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Proyecto Final - Casa Billy y Mandy", nullptr, nullptr);

	//Prueba de error, se genera en casa de que la pantalla no se muestre
	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}
	//Especificaciones de pantalla
	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);
	//Registros de CALLBACKS, para teclado y mouse
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);

	//Inicializacion de GLEW
	glewExperimental = GL_TRUE;
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	// Shaders
	Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");//Shader para la iluminacion Phong
	Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");//Shader para objetos simples
	Shader modelShader("Shader/modelLoading.vs", "Shader/modelLoading.frag");//Shader para la carga de modelos 
	Shader animShader("Shader/anim.vs", "Shader/anim.frag");//Shader para la animacion de Billy

	// Carga de Modelos
	Model casa((char*)"Models/Casa/Casa.obj");//Modelo de la casa 

	//Modelo de las puertas
	Model PuertaE((char*)"Models/Puertas/PuertaE.obj");
	Model PuertaS((char*)"Models/Puertas/PuertaS.obj");
	Model PuertaB((char*)"Models/Puertas/PuertaB.obj");
	//Modelo arbustos
	Model Arbustos((char*)"Models/Arbustos/Arbustos.obj");

	//--------------Modelos baño

	//Modelo de la alfombra
	Model Alfombra((char*)"Models/Baño/Alfombra/Alfombra.obj");
	//Modelo bañera
	Model Bañera((char*)"Models/Baño/Banera/Banera.obj");
	//Modelo Taza
	Model Taza((char*)"Models/Baño/Taza/Taza.obj");
	//Modelo Toalla
	Model Toalla((char*)"Models/Baño/Toalla/Toalla.obj");
	//Modelo cortina
	Model Cortina((char*)"Models/Baño/Cortina/Cortina.obj");

	//---------------Modelos Sala
	//Modelo Retrato
	Model Retrato((char*)"Models/Sala/Retrato/Retrato.obj");
	//Model Lampara Colgante
	Model LamparaC((char*)"Models/Sala/LamparaC/LamparaC.obj");
	//Model Sofa
	Model Sillon((char*)"Models/Sala/Sofa/Sofa.obj");
	//Model Lampara
	Model Lampara((char*)"Models/Sala/Lampara/Lampara.obj");
	//Model Tv
	Model Tv((char*)"Models/Sala/Tv/Tv.obj");
	//Modelo pantalla TV
	Model PantallaTv((char*)"Models/Sala/Tv/Pantalla.obj");
	Model Fantasma((char*)"Models/Espectro/ghost2.obj");//Modelo del espectro
	ModelAnim animacionPersonaje("Animaciones/Billy/movimiento.dae");//Modelo de la animacion del personaje
	animacionPersonaje.initShaders(animShader.Program);//Animacion del personaje

	// VAO y VBO para los cubos (primitivas)
	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO); //Creacion del (VAO) para la configuracion
	glGenBuffers(1, &VBO);//Creacion del (VBO) para almacenar los datos de los vertices
	glBindVertexArray(VAO);//Activacion del VAO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);//Activacion del VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	// Vertex Attribs
	//Atributos de posiciones
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	//Atributos de Normales
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	//Atributos de texturas
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// Set texture units
	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.specular"), 1);

	// --- Carga de Texturas ---
	int width, height;//Dimensiones de la textura
	unsigned char* image;//Puntero a los datos de pixeles


	glBindTexture(GL_TEXTURE_2D, 0);//Desvincula la textura actual


	// Matriz de Proyección
	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{
			// Gestion de tiempo
			GLfloat currentFrame = (float)glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			glfwPollEvents();//Verifica eventos de entrada 
			DoMovement();//Movimientos de la camara
			Animation();//Llamada a los huesos de la animacion de (Billy caminando)

			// Limpieza de buffers
			glClearColor(0.5f, 0.8f, 1.0f, 1.0f);//Color de fondo
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//Limpieza del buffer de fondo
			glEnable(GL_DEPTH_TEST);//Evitar que los objetos de atras no se dibujen por delante

			// --- MATRICES COMUNES ---
			glm::mat4 view = camera.GetViewMatrix();//Matriz de la camara
			glm::mat4 model = glm::mat4(1.0f);//Matriz del modelo
																										
			//Angulo de vision 
			glm::mat4 projection = glm::perspective(camera.GetZoom(), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);

			//---------Configuracion del shader de iluminacion
			lightingShader.Use();

			// Matrices y Cámara
			glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
			//Envio de la posicion de la camara
			glUniform3f(glGetUniformLocation(lightingShader.Program, "viewPos"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

			// Material base
			glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);//Concentracion en el brillo de los objetos
			glUniform2f(glGetUniformLocation(lightingShader.Program, "texTiling"), 1.0f, 1.0f);//Reseteo del tiling de texturas a 1.0

			// Luz direccional sol
			glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.1f, -0.3f);//Luz direccional
			glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.9f, 0.9f, 0.9f); // Iluminacion ambiental
			glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.6f, 0.6f, 0.6f);// Iluminacion difusa
			glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.3f, 0.3f, 0.3f);// Iluminacion especular

			//----Animacion de la lampara


			// ---Luz de punto de la lampara
			// Ubicacion de la lampara
			glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), 0.0f, 3.5f, 0.0f);

			if (lampState)//Logica de la lampara
			{//Encendido
				glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), 0.3f, 0.3f, 0.3f);//Iluminacion ambiental
				glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), 1.5f, 1.4f, 1.0f);//Iluminacion difusa color amarillo
				glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1.0f, 1.0f, 1.0f);//Especular:Reflejo blanco puro
			}
			else
			{//Apagado
				//Toda la iluminacion se pone en 0, para que no exista iluminacion
				glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), 0.0f, 0.0f, 0.0f);
				glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), 0.0f, 0.0f, 0.0f);
				glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 0.0f, 0.0f, 0.0f);
			}

			// --- Atenuacion modificada

			glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
			// La luz se atenua conforme a la distancia que se encuentra
			glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.22f);
			// Limpia el radio de iluminacion generado
			glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.20f);

																									
			// ------------------------------Dibujo de modelos

			model = glm::mat4(1.0f);//Matriz identidad para casa
			glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
			casa.Draw(lightingShader);//Dibujo de la casa

			// === Puertas ===
			glm::mat4 modelDoorE = glm::mat4(1.0f);//Matriz de puerta entrada
			glm::vec3 hingePosE = glm::vec3(6.0f, 0.0f, -4.8f);//Coordenada de la bisagra
			modelDoorE = glm::translate(modelDoorE, -hingePosE);//Movimiento de la puerta con la bisagra como eje central
			modelDoorE = glm::rotate(modelDoorE, glm::radians(doorE_angle), glm::vec3(0.0f, 0.5f, 0.0f));//Rotacion de la puerta
			modelDoorE = glm::translate(modelDoorE, hingePosE);//Retorno de la puerta a posicion original
			glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelDoorE));
			PuertaE.Draw(lightingShader);//Dibujo de la puerta entrada

			glm::mat4 modelDoorS = glm::mat4(1.0f);//Matriz de puerta Salida
			glm::vec3 hingePosS = glm::vec3(5.3f, 0.0f, 2.35f);//Coordenada de la bisagra
			modelDoorS = glm::translate(modelDoorS, -hingePosS);//Movimiento de la puerta con la bisagra como eje central
			modelDoorS = glm::rotate(modelDoorS, glm::radians(doorS_angle), glm::vec3(0.0f, 0.5f, 0.0f));//Rotacion de la puerta
			modelDoorS = glm::translate(modelDoorS, hingePosS);//Retorno de la puerta a posicion original
			glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelDoorS));
			PuertaS.Draw(lightingShader);//Dibujo de la puerta sala

			glm::mat4 modelDoorB = glm::mat4(1.0f);//Matriz de puerta Baño
			glm::vec3 hingePosB = glm::vec3(3.3f, 0.0f, 5.25f);//Coordenada de la bisagra
			modelDoorB = glm::translate(modelDoorB, -hingePosB);//Movimiento de la puerta con la bisagra como eje central
			modelDoorB = glm::rotate(modelDoorB, glm::radians(doorB_angle), glm::vec3(0.0f, 0.5f, 0.0f));//Rotacion de la puerta
			modelDoorB = glm::translate(modelDoorB, hingePosB);//Retorno de la puerta a posicion original
			glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelDoorB));
			PuertaB.Draw(lightingShader);//Dibujo de la puerta baño

		
			// ---------------Objetos Baño
			glm::mat4 modelAlfombra = glm::mat4(1.0f);//Matriz de la alfombra
			glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelAlfombra));
			Alfombra.Draw(lightingShader);//Dibuja alfombra

			glEnable(GL_CULL_FACE);//Activa el descarte de caras
			glCullFace(GL_BACK);//Descarta las caras de atras
			glFrontFace(GL_CCW);//Definir el frente como sentido anti-horario
			glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 2.0f);//Aumento de dispersion de la luz

			// Cortina
			glm::mat4 modelCortina = glm::mat4(1.0f);//Matriz de la cortina
			glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelCortina));
			Cortina.Draw(lightingShader);//Dibujo de la cortina

			// Toalla
			glm::mat4 modelToalla = glm::mat4(1.0f);//Matriz de la toalla
			glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelToalla));
			Toalla.Draw(lightingShader);//Dibujo de la toalla

			glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 128.0f);//Disminucion de la dispersion de luz

			// Bañera
			glm::mat4 modelBañera = glm::mat4(1.0f);//Matriz de la bañera
			glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelBañera));
			Bañera.Draw(lightingShader);//Dibujo de la bañera

			// Taza
			glm::mat4 modelTaza = glm::mat4(1.0f);//Matriz de la taza
			glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelTaza));
			Taza.Draw(lightingShader);//Dibujo de la taza

			//Reestablecemos la dispercion de la luz para el resto de objetos
			glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);

			//Objetos Sala
		
			glm::mat4 modelLamparaC = glm::mat4(1.0f); // Matriz de lampara colgante
			glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelLamparaC));
			LamparaC.Draw(lightingShader);//Dibujo de lampara colgante

			glm::mat4 modelSillon = glm::mat4(1.0f); // Matriz del sillon
			glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelSillon));
			Sillon.Draw(lightingShader);//Dibujo del sillon

			glm::mat4 modelRetrato = glm::mat4(1.0f); // Matriz del retrato
			glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelRetrato));
			Retrato.Draw(lightingShader);//Dibujo del retrato

			glm::mat4 modelTv = glm::mat4(1.0f); // Matriz de la television
			glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelTv));
			Tv.Draw(lightingShader);//Dibujo de la television

			glm::mat4 modelArbustos = glm::mat4(1.0f); // Matriz de los arbustos
			glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelArbustos));
			Arbustos.Draw(lightingShader);//Dibujo de los arbustos

			glDisable(GL_CULL_FACE);//Desactivar CULL FACE

			glm::mat4 modelPantalla = glm::mat4(1.0f); // Matriz de la pantalla
			glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelPantalla));
			PantallaTv.Draw(lightingShader);//Dibujo de la pantalla

			// === Lampara Animada 
			glm::vec3 realLampPos = glm::vec3(2.0f, 0.0f, -1.0f);//Punto central de movimiento
			glm::mat4 rotationMat = glm::rotate(glm::mat4(1.0f), glm::radians(lampWobble), glm::vec3(0.0f, 0.0f, 1.0f));//Matriz de rotacion balanceo de izquierda a derecha
			glm::mat4 modelLamp = glm::mat4(1.0f);//Logica del pivote
			modelLamp = glm::translate(modelLamp, realLampPos);//Movimiento al punto del pivote
			modelLamp = modelLamp * rotationMat;//Aplicar rotacion
			modelLamp = glm::translate(modelLamp, -realLampPos);//Regresa a su posicion inicial

			glUniform2f(glGetUniformLocation(lightingShader.Program, "texTiling"), 1.0f, 1.0f);
			glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelLamp));

			glEnable(GL_CULL_FACE);//Activa el descarte de caras
			glCullFace(GL_BACK);//Descarta las caras de atras
			Lampara.Draw(lightingShader);//Dibujo de lampara
			glDisable(GL_CULL_FACE);//Desactivar CULL FACE

			// ---------Animacion del Fantasma 
			if (tvAnimationActive)
			{
				glm::vec3 posicionTV = glm::vec3(-1.5f, 0.5f, 4.0f);//Coordenadas de la pantalla
				float emergeZ = 0.4f + (tvAnimTime * 0.5f);//Movimiento en el eje Z
				float wiggleX = sin(tvAnimTime * 8.0f) * 0.3f;//Movimiento de izquierda a derecha usando la funcion seno

				glm::mat4 modelFantasma = glm::mat4(1.0f); //Matriz del fantasma
				//Movimiento compuesto en el eje z para avanzar
				//Y al mismo tiempo aplicar el movimiento de izquierda a derecha
				modelFantasma = glm::translate(modelFantasma, posicionTV + glm::vec3(wiggleX, 1.0f, -emergeZ));
				//Ajuste de posicion del fantasma
				modelFantasma = glm::rotate(modelFantasma, glm::radians(-270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
				//Ajuste del tamaño del fantasma
				modelFantasma = glm::scale(modelFantasma, glm::vec3(0.28f));

				glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelFantasma));
				Fantasma.Draw(lightingShader);//Dibujo del fantasma
			}

			// -------Animacion de Billy
			animShader.Use();

			//Ajuste de ubicacion de las variables
			GLint modelLoc = glGetUniformLocation(animShader.Program, "model");
			GLint viewLoc = glGetUniformLocation(animShader.Program, "view");
			GLint projLoc = glGetUniformLocation(animShader.Program, "projection");

			//Envio de matrices de la camara al shader
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

			//Configuracion del modelo de Billy
			model = glm::mat4(1.0f);//Matriz de Billy
			model = glm::translate(model, glm::vec3(-5.0f, 0.0f, 15.0f));//Ajuste de ubicacion de billy
			model = glm::scale(model, glm::vec3(1.8f));//Tamaño de billy
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

			animacionPersonaje.Draw(animShader);//Dibujo del personaje
			glBindVertexArray(0);//Desvinculacion del VAO

			glfwSwapBuffers(window);//Intercambio de buffers
		}

		glfwTerminate();//Fin del while y cierre de la ventana GLFW
		return 0;
		}

		// DoMovement
		void DoMovement()
		{
			// Controles de Cámara
			if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])//Movimiento hacia adelante
			{
				camera.ProcessKeyboard(FORWARD, deltaTime);
			}
			if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])//Movimiento hacia atras
			{
				camera.ProcessKeyboard(BACKWARD, deltaTime);
			}
			if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])//Movimiento hacia la izquierda
			{
				camera.ProcessKeyboard(LEFT, deltaTime);
			}
			if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])//Movimiento hacia la derecha
			{
				camera.ProcessKeyboard(RIGHT, deltaTime);
			}

		}
		//Funcion para control de las animaciones 
		void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
		{
			// Controles de Puertas
			if (action == GLFW_PRESS)
			{
				if (key == GLFW_KEY_1)
					doorE_open = !doorE_open; // apertura de la puerta de la entrada

				if (key == GLFW_KEY_2)
					doorS_open = !doorS_open; // apertura de la puerta de la sala

				if (key == GLFW_KEY_3)
					doorB_open = !doorB_open; // apertura de la puerta del baño
			}

			// Control de Animacion de Lampara
			if (key == GLFW_KEY_M && action == GLFW_PRESS)
			{
				lampAnimationActive = !lampAnimationActive; // Movimiento de la lampara
			}

			// Control de Luz de Lampara
			if (key == GLFW_KEY_N && action == GLFW_PRESS)
			{
				lampState = !lampState; // Interruptor de la LUZ de la lampara
			}

			// Control Animacion Compleja TV ---
			if (key == GLFW_KEY_P && action == GLFW_PRESS)
			{
				if (!tvAnimationActive)//Solo inicia mientras la tv no este activa
				{
					tvAnimationActive = true;//Activa la bandera
					tvAnimTime = 0.0f; // Reinicia el tiempo de la animacion
				}
			}

			// Cerrar ventana
			if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
			{
				glfwSetWindowShouldClose(window, GL_TRUE);//Señal de cierre de ventana
			}

			// Actualizar array de teclas
			if (key >= 0 && key < 1024)
			{
				if (action == GLFW_PRESS)
				{
					keys[key] = true;//Marca la tecla como presionada
				}
				else if (action == GLFW_RELEASE)
				{
					keys[key] = false;//Marca la tecla como soltada
				}
			}
			
		}

		void Animation() {

			// Animacion de Puertas
			float doorSpeed = 1.5f; // velocidad en grados por frame

			// Puerta Entrada
			if (doorE_open && doorE_angle < 90.0f)
				doorE_angle += doorSpeed;//Abrir
			else if (!doorE_open && doorE_angle > 0.0f)
				doorE_angle -= doorSpeed;//Cerrar

			// Puerta Salida
			if (doorS_open && doorS_angle < 90.0f)
				doorS_angle += doorSpeed;//Abrir
			else if (!doorS_open && doorS_angle > 0.0f)
				doorS_angle -= doorSpeed;//Cerrar

			// Puerta Baño
			if (doorB_open && doorB_angle < 90.0f)
				doorB_angle += doorSpeed;//Abrir
			else if (!doorB_open && doorB_angle > 0.0f)
				doorB_angle -= doorSpeed;//Cerrar

			// Animacion Simple de Lampara
			if (lampAnimationActive)
			{
				lampWobble = sin((float)glfwGetTime() * 2.0f) * 5.0f;//Angulo de tambaleo de la lampara
			}
			else
			{
				lampWobble = 0.0f;//En caso de estar activa resetea 
			}

			// ---  Actualizacion de Animacion Compleja TV ---
			if (tvAnimationActive)
			{
				tvAnimTime += deltaTime; // Incrementa el temporizador

				// Detener la animacion despues de 5 segundos
				if (tvAnimTime > 5.0f)
				{
					tvAnimationActive = false;//Desactiva la bandera
					tvAnimTime = 0.0f; //Resetea el temporizador
				}
			}
		}

		// MouseCallback
		void MouseCallback(GLFWwindow* window, double xPos, double yPos)
		{
			if (firstMouse)
			{
				lastX = (float)xPos;//Establece la posicion inicial en X
				lastY = (float)yPos;//Establece la posicion inicial en Y
				firstMouse = false;//Desactiva la bandera
			}
			GLfloat xOffset = (float)xPos - lastX;//Calculo del movimiento del mouse
			GLfloat yOffset = lastY - (float)yPos;//Se invierte el eje Y
			lastX = (float)xPos;//Almacena la posicion para el proximo fotograma
			lastY = (float)yPos;
			camera.ProcessMouseMovement(xOffset, yOffset);//Envia la posicion de la camara para calcular los nuevos angulos
		}

		// Función para dibujar primitivas de cubo
		void ObjetoDraw(glm::mat4 base, glm::vec3 escala, glm::vec3 traslado, GLint uniformModel)
		{
			glm::mat4 modelo = glm::mat4(1.0f);//Inicializa la matriz
			modelo = glm::translate(modelo, traslado);//Aplica la posicion de cada pieza
			modelo = glm::scale(modelo, escala);//Ajusta el tamaño de cada pieza
			modelo = base * modelo;
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelo));//Envia la matriz al shader
			glDrawArrays(GL_TRIANGLES, 0, 36);//Dibuja el cubo con 36 vertices definidos en el VAO
		}







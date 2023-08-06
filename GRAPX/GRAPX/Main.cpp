#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include "glm/glm.hpp"
#include "obj_mesh.h";
#include "shader.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "skybox.h"
#include "CameraController.h"


int main()
{

	stbi_set_flip_vertically_on_load(true);

#pragma region Initialization
	//initialize glfw
	if (glfwInit() != GLFW_TRUE) {
		fprintf(stderr, "Failed to initialized! \n");
		return -1;
	}

	// set opengl version to 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create window 
	GLFWwindow* window;
	window = glfwCreateWindow(1024, 768, "Joseph Santos", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to load window! \n");
		return -1;
	}
	glfwMakeContextCurrent(window);

	//initialize glew
	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}
#pragma endregion


#pragma region Mesh Loading

	ObjData cat;
	//LoadObjFile(&cat, "girl OBJ.obj");
	LoadObjFile(&cat, "Cat/12221_Cat_v1_l3.obj");
	GLfloat catOffsets[] = { 0.0f, 0.0f, 0.0f };
	LoadObjToMemory(
		&cat,
		1.0f,
		catOffsets
	);

	ObjData bird;
	LoadObjFile(&bird, "Bird/12213_Bird_v1_l3.obj");
	GLfloat birdOffsets[] = { 0.0f, 0.0f, 0.0f };
	LoadObjToMemory(
		&bird,
		1.0f,
		birdOffsets
	);

	ObjData duck;
	LoadObjFile(&duck, "Duck/12248_Bird_v1_L2.obj");
	GLfloat duckOffsets[] = { 0.0f, 0.0f, 0.0f };
	LoadObjToMemory(
		&duck,
		1.0f,
		duckOffsets
	);

	ObjData parrot;
	LoadObjFile(&parrot, "Parrot/10032_Parrot_V1_L3.obj");
	GLfloat parrotOffsets[] = { 0.0f, 0.0f, 0.0f };
	LoadObjToMemory(
		&parrot,
		1.0f,
		parrotOffsets
	);

	std::vector<std::string> faces
	{
		//"right.png",
		//"left.png",
		//"bottom.png",
		//"top.png",
		//"front.png",
		//"back.png"
		
		"Daylight Box_Right.bmp",
		"Daylight Box_Left.bmp",
		"Daylight Box_Bottom.bmp",
		"Daylight Box_Top.bmp",
		"Daylight Box_Front.bmp",
		"Daylight Box_Back.bmp"
	};

	std::vector<std::string> faces2
	{
		"right.png",
		"left.png",
		"bottom.png",
		"top.png",
		"front.png",
		"back.png"

	};
	SkyBoxData skybox = loadSkyBox("Assets/skybox", faces);
	SkyBoxData skybox2 = loadSkyBox("Assets/skybox", faces2);
#pragma endregion

#pragma region Shader Loading

	//load skybox shader program
	GLuint skyboxShaderProgram = LoadShaders("Shaders/skybox_vertex.shader", "Shaders/skybox_fragment.shader");

	//load shader program
	GLuint shaderProgram = LoadShaders("Shaders/phong_vertex.shader", "Shaders/phong_directional_fragment.shader");
	glUseProgram(shaderProgram);


	GLuint colorLoc = glGetUniformLocation(shaderProgram, "u_color");
	glUniform3f(colorLoc, 1.0f, 1.0f, 1.0f);


	// initialize MVP
	GLuint modelTransformLoc = glGetUniformLocation(shaderProgram, "u_model");
	GLuint viewLoc = glGetUniformLocation(shaderProgram, "u_view");
	GLuint projectionLoc = glGetUniformLocation(shaderProgram, "u_projection");

	//initialize normal transformation
	GLuint normalTransformLoc = glGetUniformLocation(shaderProgram, "u_normal");
	GLuint cameraPosLoc = glGetUniformLocation(shaderProgram, "u_camera_pos");
	GLuint ambientColorLoc = glGetUniformLocation(shaderProgram, "u_ambient_color");
	glUniform3f(ambientColorLoc, 0.1f, 0.1f, 0.1f);

	glm::mat4 catObj = glm::mat4(1.0f); // identity
	glm::mat4 birdObj = glm::mat4(1.0f); // identity
	glm::mat4 duckObj = glm::mat4(1.0f); // identity
	glm::mat4 parrotObj = glm::mat4(1.0f); // identity

	glUniformMatrix4fv(modelTransformLoc, 1, GL_FALSE, glm::value_ptr(catObj));
	glUniformMatrix4fv(modelTransformLoc, 1, GL_FALSE, glm::value_ptr(birdObj));
	glUniformMatrix4fv(modelTransformLoc, 1, GL_FALSE, glm::value_ptr(duckObj));
	glUniformMatrix4fv(modelTransformLoc, 1, GL_FALSE, glm::value_ptr(parrotObj));

	// define projection matrix
	glm::mat4 projection = glm::mat4(1.0f);
	//glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

	//setup light shading
	//POINT LIGHT
GLuint lightPosLoc = glGetUniformLocation(shaderProgram, "u_light_pos");
//DIRECTIONAL LIGHT
GLuint lightDirLoc = glGetUniformLocation(shaderProgram, "u_light_dir");
GLuint diffuseTexLoc = glGetUniformLocation(shaderProgram, "texture_diffuse");
GLuint normalTexLoc = glGetUniformLocation(shaderProgram, "texture_normal");

glUniform1i(diffuseTexLoc, 0);
glUniform1i(normalTexLoc, 1);
glUniform3f(lightPosLoc, 0.1f, 2.0f, 0.25f);
glUniform3f(lightDirLoc, 1.0f, 1.0f, 1.0f);




//flag for shading
GLuint modelIdLoc = glGetUniformLocation(shaderProgram, "u_model_id");
glUniform1f(modelIdLoc, 1.0f);


#pragma endregion

// set bg color to green
glClearColor(0.4f, 0.4f, 0.0f, 0.0f);

// var for rotations
  // var for rotations
float xFactor = 0.0f;
float xSpeed = 1.0f;
float currentTime = glfwGetTime();
float prevTime = 0.0f;
float deltaTime = 0.0f;
float rotFactor = 0.0f;
float currX = 0.0f;
float currY = 0.0f;

CameraController::initialize();

glfwSetCursorPosCallback(window, CameraController::getInstance()->mouse_callback);
glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


//depth testing
glEnable(GL_DEPTH_TEST);
//glDepthFunc(GL_ALWAYS); // set the depth test function

//face culling
glEnable(GL_CULL_FACE);
//glCullFace(GL_BACK); // set which face to cull
//glFrontFace(GL_CCW); // set the front face orientation

//std::cout << moon.textures[0] << std::endl;
//glfwSetCursorPosCallback(window, cursor_position_callback);


while (!glfwWindowShouldClose(window))
{

#pragma region Viewport
	float ratio;
	int width, height;
	bool ortho = false;

	glfwGetFramebufferSize(window, &width, &height);
	ratio = width / (float)height;

	glViewport(0, 0, width, height);
#pragma endregion

#pragma region Projection
	// Orthopgraphic projection but make units same as pixels. origin is lower left of window
	// projection = glm::ortho(0.0f, (GLfloat)width, 0.0f, (GLfloat)height, 0.1f, 10.0f); // when using this scale objects really high at pixel unity size

	// Orthographic with stretching
	//projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 10.0f);

	// Orthographic with corection for stretching, resize window to see difference with previous example
	//projection = glm::ortho(-ratio, ratio, -1.0f, 1.0f, 0.1f, 10.0f);


	projection = glm::perspective(glm::radians(90.0f), ratio, 0.1f, 10.0f);


	// Perspective Projection

	//projection = glm::ortho(-15.0f, 15.0f, -15.0f, 15.0f, 0.1f, 125.0f);



	// Set projection matrix in shader
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));



#pragma endregion

#pragma region View

	currentTime = glfwGetTime();
	deltaTime = currentTime - prevTime;
	prevTime = currentTime;
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS){
	currX = currX;
	currY = currY;
	}

	else {
		currX += deltaTime * 0.45f;
		currY += deltaTime * 0.45f;

	}
		
		glUniform3f(lightDirLoc, glm::sin(currX), glm::cos(currY), 0.0f);
	
		

		glm::mat4 view = glm::lookAt(CameraController::getInstance()->cameraPos,
			CameraController::getInstance()->cameraPos + CameraController::getInstance()->cameraFront,
			CameraController::getInstance()->cameraUp);

		//w,a,s,d movement of the camera
		CameraController::getInstance()->moveCam(window, deltaTime);

		glUniformMatrix4fv(modelTransformLoc, 1, GL_FALSE, glm::value_ptr(catObj));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		
#pragma endregion

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//toggle to render wit GL_FILL or GL_LINE
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

#pragma region Draw

		//DRAWING SKYBOX
		
		if (prevTime <= 30) {
			DrawSkybox(skybox, skyboxShaderProgram, view, projection);
			glUseProgram(shaderProgram);
		}
		else if(prevTime > 30) {
			DrawSkybox(skybox2, skyboxShaderProgram, view, projection);
			glUseProgram(shaderProgram);
		}
		else if (prevTime == 60) {
			prevTime = 0;
		}
		
		
		
		//draw Cat
		glUniform1f(modelIdLoc, 1.0f);
		glBindVertexArray(cat.vaoId);

		// transforms
		catObj = glm::mat4(1.0f); // identity
		catObj = glm::translate(catObj, glm::vec3(0.0f, -4.0f, 1.0f)); // matrix * translate_matrix
		catObj = glm::rotate(catObj, glm::radians(270.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		catObj = glm::scale(catObj, glm::vec3(0.1f, 0.1f, 0.1f));

		//send to shader
		glm::mat4 normalTrans = glm::transpose(glm::inverse(catObj));
		glUniformMatrix4fv(normalTransformLoc, 1, GL_FALSE, glm::value_ptr(normalTrans));
		glUniformMatrix4fv(modelTransformLoc, 1, GL_FALSE, glm::value_ptr(catObj));


		glActiveTexture(GL_TEXTURE0);
		GLuint catTexture = cat.textures[cat.materials[0].diffuse_texname];
		glBindTexture(GL_TEXTURE_2D, catTexture);
		glActiveTexture(GL_TEXTURE1);
		GLuint normalTexture = cat.textures[cat.materials[0].bump_texname];
		glBindTexture(GL_TEXTURE_2D, normalTexture);
		//drawbackpack
		glDrawElements(GL_TRIANGLES, cat.numFaces, GL_UNSIGNED_INT, (void*)0);

		



		
		//https://free3d.com/3d-model/cat-v1--522281.html//
		
		
		//draw Bird
		glUniform1f(modelIdLoc, 1.0f);
		glBindVertexArray(bird.vaoId);

		// transforms
		birdObj = glm::mat4(1.0f); // identity
		birdObj = glm::translate(birdObj, glm::vec3(-6.0f, 6.0f, 1.0f)); // matrix * translate_matrix
		birdObj = glm::rotate(birdObj, glm::radians(135.0f), glm::vec3(0.0f, 1.0f, 1.0f));
		birdObj = glm::scale(birdObj, glm::vec3(0.25f, 0.25f, 0.25f));

		//send to shader
		glm::mat4 normalTrans2 = glm::transpose(glm::inverse(birdObj));
		glUniformMatrix4fv(normalTransformLoc, 1, GL_FALSE, glm::value_ptr(normalTrans2));
		glUniformMatrix4fv(modelTransformLoc, 1, GL_FALSE, glm::value_ptr(birdObj));


		glActiveTexture(GL_TEXTURE0);
		GLuint birdTexture = bird.textures[bird.materials[0].diffuse_texname];
		glBindTexture(GL_TEXTURE_2D, birdTexture);
		glActiveTexture(GL_TEXTURE1);
		GLuint normalTexture1 = bird.textures[bird.materials[0].bump_texname];
		glBindTexture(GL_TEXTURE_2D, normalTexture1);
		
		//drawbackpack
		glDrawElements(GL_TRIANGLES, bird.numFaces, GL_UNSIGNED_INT, (void*)0);


		//https://free3d.com/3d-model/bird-v1--875504.html//

		//draw Duck
		glUniform1f(modelIdLoc, 1.0f);
		glBindVertexArray(duck.vaoId);

		// transforms
		duckObj = glm::mat4(1.0f); // identity
		duckObj = glm::translate(duckObj, glm::vec3(-4.0f, -4.0f, 1.0f)); // matrix * translate_matrix
		duckObj = glm::rotate(duckObj, glm::radians(270.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		duckObj = glm::scale(duckObj, glm::vec3(0.1f, 0.1f, 0.1f));

		//send to shader
		glm::mat4 normalTrans3 = glm::transpose(glm::inverse(duckObj));
		glUniformMatrix4fv(normalTransformLoc, 1, GL_FALSE, glm::value_ptr(normalTrans3));
		glUniformMatrix4fv(modelTransformLoc, 1, GL_FALSE, glm::value_ptr(duckObj));


		glActiveTexture(GL_TEXTURE0);
		GLuint duckTexture = duck.textures[duck.materials[0].diffuse_texname];
		glBindTexture(GL_TEXTURE_2D, duckTexture);
		//drawbackpack
		glDrawElements(GL_TRIANGLES, duck.numFaces, GL_UNSIGNED_INT, (void*)0);


		//https://free3d.com/3d-model/bird-v1--282209.html//

		//draw Parrot
		glUniform1f(modelIdLoc, 1.0f);
		glBindVertexArray(parrot.vaoId);

		// transforms
		parrotObj = glm::mat4(1.0f); // identity
		parrotObj = glm::translate(parrotObj, glm::vec3(0.0f, 6.0f, 1.0f)); // matrix * translate_matrix
		parrotObj = glm::rotate(parrotObj, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 1.0f));
		parrotObj = glm::scale(parrotObj, glm::vec3(0.15f, 0.15f, 0.15f));

		//send to shader
		glm::mat4 normalTrans4 = glm::transpose(glm::inverse(parrotObj));
		glUniformMatrix4fv(normalTransformLoc, 1, GL_FALSE, glm::value_ptr(normalTrans4));
		glUniformMatrix4fv(modelTransformLoc, 1, GL_FALSE, glm::value_ptr(parrotObj));


		glActiveTexture(GL_TEXTURE0);
		GLuint parrotTexture = parrot.textures[parrot.materials[0].diffuse_texname];
		glBindTexture(GL_TEXTURE_2D, parrotTexture);
		//drawbackpack
		glDrawElements(GL_TRIANGLES, parrot.numFaces, GL_UNSIGNED_INT, (void*)0);

		//https://free3d.com/3d-model/parrot-v1--172385.html

		//--- stop drawing here ---
#pragma endregion

		glfwSwapBuffers(window);
		//listen for glfw input events
		glfwPollEvents();
	}
	return 0;
}
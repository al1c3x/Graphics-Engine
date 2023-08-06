#include "CameraController.h"


CameraController::CameraController()
{

}

void CameraController::initialize()
{
	sharedInstance = new CameraController();
}

CameraController* CameraController::sharedInstance = NULL;

CameraController* CameraController::getInstance()
{
	return sharedInstance;
}

void CameraController::moveCam(GLFWwindow* window, float deltaTime)
{
	float cameraSpeed = deltaTime * 10;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		CameraController::getInstance()->cameraPos += cameraSpeed * CameraController::getInstance()->cameraFront;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		CameraController::getInstance()->cameraPos -= cameraSpeed * CameraController::getInstance()->cameraFront;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		CameraController::getInstance()->cameraPos -= glm::normalize(
			glm::cross(CameraController::getInstance()->cameraFront,
				CameraController::getInstance()->cameraUp)) * cameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		CameraController::getInstance()->cameraPos += glm::normalize(glm::cross(
			CameraController::getInstance()->cameraFront,
			CameraController::getInstance()->cameraUp)) * cameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		CameraController::getInstance()->cameraPos += cameraSpeed * CameraController::getInstance()->cameraUp;
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		CameraController::getInstance()->cameraPos -= cameraSpeed * CameraController::getInstance()->cameraUp;
	}
	CameraController::getInstance()->clampMovement();
}

void CameraController::clampMovement()
{
	//std::cout << "X: " << cameraMovement::getInstance()->cameraPos.x << std::endl;
	//std::cout << "Y: " << cameraMovement::getInstance()->cameraPos.y << std::endl;
	//std::cout << "z: " << cameraMovement::getInstance()->cameraPos.z << std::endl;

	if (CameraController::getInstance()->cameraPos.x < this->clampX[0])
	{
		CameraController::getInstance()->cameraPos.x = this->clampX[0];
	}
	if (CameraController::getInstance()->cameraPos.x > this->clampX[1])
	{
		CameraController::getInstance()->cameraPos.x = this->clampX[1];
	}
	if (CameraController::getInstance()->cameraPos.y < this->clampY[0])
	{
		CameraController::getInstance()->cameraPos.y = this->clampY[0];
	}
	if (CameraController::getInstance()->cameraPos.y > this->clampY[1])
	{
		CameraController::getInstance()->cameraPos.y = this->clampY[1];
	}
	if (CameraController::getInstance()->cameraPos.z < this->clampZ[0])
	{
		CameraController::getInstance()->cameraPos.z = this->clampZ[0];
	}
	if (CameraController::getInstance()->cameraPos.z > this->clampZ[1])
	{
		CameraController::getInstance()->cameraPos.z = this->clampZ[1];
	}
}

//properties
glm::vec3 CameraController::cameraPos = glm::vec3(0.0f, 0.0f, 10.0f);
glm::vec3 CameraController::cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 CameraController::cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
float CameraController::yaw = -90.0f;
float CameraController::pitch = 0.0f;
float CameraController::fov = 45.0f;

//mouse state
bool CameraController::firstMouse = true;
float CameraController::lastX = 1024 / 2.0;
float CameraController::lastY = 768 / 2.0;

void CameraController::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float mouseOffsetX = xpos - lastX;
	float mouseOffsetY = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f;
	mouseOffsetX *= sensitivity;
	mouseOffsetY *= sensitivity;

	yaw += mouseOffsetX;
	pitch += mouseOffsetY;

	if (pitch > 90.0f)
	{
		pitch = 89.0f;
	}
	if (pitch < -90.0f)
	{
		pitch = -89.0f;
	}

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
}
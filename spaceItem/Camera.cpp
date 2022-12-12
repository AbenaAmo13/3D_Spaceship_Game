#include "Camera.h"

float Camera::getPitch()
{
	return 0.0f;
}

float Camera::getYawAngle()
{
	return yawAngle;
}

float Camera::getRollAngle()
{
	return rollAngle;
}

void Camera::DecreaseZoom(float zoom)
{

	distanceFromPlayer += zoom;
}


void Camera::IncreaseZoom(float zoom)
{

	distanceFromPlayer -= zoom;
}

void Camera::CalculatePitch(float pitchChange) {
	pitchAngle -= pitchChange;

}

void Camera::calculateAngleAroundThePlayer(float angleChange) {
	angleAroundThePlayer -= angleChange;
}

float Camera::calculateHorizontalDistance()
{
	return distanceFromPlayer * cos(glm::radians(pitchAngle));
}

float Camera::calculateVerticalDistance()
{
	return distanceFromPlayer * sin(glm::radians(pitchAngle));
}

glm::vec3 Camera::CalculateCameraPosition(float horizontalDistance, float verticalDistance, SpaceShip *spaceShip)
{

	float theta = spaceShip->getSpaceShipObjectRotation()[1][1] + angleAroundThePlayer;
	float offsetX = horizontalDistance * sin(glm::radians(theta));
	float offsetZ = horizontalDistance * cos(glm::radians(theta));
	cameraPosition.x = spaceShip->getSpaceShipPos().x - offsetX;
	cameraPosition.z= spaceShip->getSpaceShipPos().z - offsetZ;
	cameraPosition.y = spaceShip->getSpaceShipPos().y + verticalDistance;

	return cameraPosition;


}


void Camera::move(SpaceShip spaceShip) {

	//calculateAngleAroundThePlayer(angleChange);
	//pitchAngle(pitchChange);
	//IncreaseZoom( zoom);
	//DecreaseZoom( zoom);

	//float verticalDistance = calculateVerticalDistance();
	//float horizontalDistance = calculateHorizontalDistance();
	//CalculateCameraPosition(horizontalDistance, verticalDistance, spaceShip);
	//yawAngle = 180 - spaceShip.getSpaceShipObjectRotation()[1][1] + angleAroundThePlayer;
	

}





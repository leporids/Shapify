#include "Camera.h"

const float Camera::MINIMUM_FOV_ANGLE = 5.0f;
const float Camera::DEFAULT_FOV_ANGLE = 45.0f;
const float Camera::MAXIMUM_FOV_ANGLE = 85.0f;
const int Camera::DEFAULT_WINDOW_WIDTH = 400;
const int Camera::DEFAULT_WINDOW_HEIGHT = 400;
const float Camera::DEFAULT_Z_NEAR = 0.1f;
const float Camera::DEFAULT_Z_FAR = 1000.0f;
const float Camera::DEFAULT_DISTANCE = 30.0f;

Camera::Camera() :
	position(Vec3f()),
	lookAtVector(Vec3f()),
	upVector(Vec3f()),
	fovAngle(Camera::DEFAULT_FOV_ANGLE),
	windowWidth(Camera::DEFAULT_WINDOW_WIDTH),
	windowHeight(Camera::DEFAULT_WINDOW_HEIGHT),
	zNear(Camera::DEFAULT_Z_NEAR),
	zFar(Camera::DEFAULT_Z_FAR),
	projectionMatrix(),
	viewMatrix(),
	currentDistance(Camera::ANTERIOR_DISTANCE),
	distances{Camera::DEFAULT_DISTANCE, Camera::DEFAULT_DISTANCE, Camera::DEFAULT_DISTANCE, Camera::DEFAULT_DISTANCE, Camera::DEFAULT_DISTANCE, Camera::DEFAULT_DISTANCE}
{
	this->loadAnterior();
}

float Camera::getFovAngle() const{
	return this->fovAngle;
}

int Camera::getWindowWidth() const{
	return this->windowWidth;
}

int Camera::getWindowHeight() const{
	return this->windowHeight;
}

float Camera::getRatio() const{
	return (float)this->windowWidth / (float)this->windowHeight;
}

float Camera::getZNear() const{
	return this->zNear;
}

float Camera::getZFar() const{
	return this->zFar;
}

const Mat4f* Camera::getProjectionMatrix() const{
	return &this->projectionMatrix;
}

const Vec3f* Camera::getPosition() const{
	return &this->position;
}

const Vec3f* Camera::getLookAtVector() const{
	return &this->lookAtVector;
}

const Vec3f* Camera::getUpVector() const{
	return &this->upVector;
}

const Mat4f* Camera::getViewMatrix() const{
	return &this->viewMatrix;
}

void Camera::setPosition(const Vec3f* const position){
	this->position.setComponents(position);
	this->position.normalize();
	this->updateViewMatrix();
}

void Camera::setLookAtVector(const Vec3f* const lookAtVector){
	this->lookAtVector.setComponents(lookAtVector);
	this->updateViewMatrix();
}

void Camera::setUpVector(const Vec3f* const upVector){
	this->upVector.setComponents(upVector);
	this->upVector.normalize();
	this->updateViewMatrix();
}

void Camera::reshape(const int windowWidth, const int windowHeight){
	this->windowWidth = windowWidth;
	this->windowHeight = windowHeight;
	this->updateProjectionMatrix();
}

void Camera::moveForwards(const float amount){
	Vec3f forwards(&this->lookAtVector);
	forwards.normalize();
	forwards.multiply(amount);
	this->position.add(&forwards);
	this->distances[this->currentDistance] -= amount;
	this->updateViewMatrix();
}

void Camera::moveRight(const float amount){
	Vec3f cross(&this->lookAtVector);
	cross.cross(&this->upVector);
	cross.normalize();
	cross.multiply(amount);
	this->position.add(&cross);
	this->updateViewMatrix();
}

void Camera::moveUp(const float amount){
	Vec3f up(&this->upVector);
	up.multiply(amount);
	this->position.add(&up);
	this->updateViewMatrix();
}

void Camera::zoomIn(const float amount){
	this->fovAngle -= amount;
	if(this->fovAngle < Camera::MINIMUM_FOV_ANGLE){
		this->fovAngle = Camera::MINIMUM_FOV_ANGLE;
	}else if(this->fovAngle > Camera::MAXIMUM_FOV_ANGLE){
		this->fovAngle = Camera::MAXIMUM_FOV_ANGLE;
	}
	this->updateProjectionMatrix();
}

void Camera::loadAnterior(){
	this->loadAnterior(this->distances[Camera::ANTERIOR_DISTANCE]);
}

void Camera::loadDorsal(){
	this->loadDorsal(this->distances[Camera::DORSAL_DISTANCE]);
}

void Camera::loadLateralLeft(){
	this->loadLateralLeft(this->distances[Camera::LATERAL_LEFT_DISTANCE]);
}

void Camera::loadLateralRight(){
	this->loadLateralRight(this->distances[Camera::LATERAL_RIGHT_DISTANCE]);
}

void Camera::loadPosterior(){
	this->loadPosterior(this->distances[Camera::POSTERIOR_DISTANCE]);
}

void Camera::loadVentral(){
	this->loadVentral(this->distances[Camera::VENTRAL_DISTANCE]);
}

void Camera::loadAnterior(const float distance){
	this->currentDistance = Camera::ANTERIOR_DISTANCE;
	this->position.setComponents(0.0f, 0.0f, distance);
	this->lookAtVector.setComponents(0.0f, 0.0f, -distance);
	this->upVector.setComponents(0.0f, 1.0f, 0.0f);
	this->updateViewMatrix();
}

void Camera::loadDorsal(const float distance){
	this->currentDistance = Camera::DORSAL_DISTANCE;
	this->position.setComponents(0.0f, distance, 0.0f);
	this->lookAtVector.setComponents(0.0f, -distance, 0.0f);
	this->upVector.setComponents(0.0f, 0.0f, -1.0f);
	this->updateViewMatrix();
}

void Camera::loadLateralLeft(const float distance){
	this->currentDistance = Camera::LATERAL_LEFT_DISTANCE;
	this->position.setComponents(distance, 0.0f, 0.0f);
	this->lookAtVector.setComponents(-distance, 0.0f, 0.0f);
	this->upVector.setComponents(0.0f, 1.0f, 0.0f);
	this->updateViewMatrix();
}

void Camera::loadLateralRight(const float distance){
	this->currentDistance = Camera::LATERAL_RIGHT_DISTANCE;
	this->position.setComponents(-distance, 0.0f, 0.0f);
	this->lookAtVector.setComponents(distance, 0.0f, 0.0f);
	this->upVector.setComponents(0.0f, 1.0f, 0.0f);
	this->updateViewMatrix();
}

void Camera::loadPosterior(const float distance){
	this->currentDistance = Camera::POSTERIOR_DISTANCE;
	this->position.setComponents(0.0f, 0.0f, -distance);
	this->lookAtVector.setComponents(0.0f, 0.0f, distance);
	this->upVector.setComponents(0.0f, 1.0f, 0.0f);
	this->updateViewMatrix();
}

void Camera::loadVentral(const float distance){
	this->currentDistance = Camera::VENTRAL_DISTANCE;
	this->position.setComponents(0.0f, -distance, 0.0f);
	this->lookAtVector.setComponents(0.0f, distance, 0.0f);
	this->upVector.setComponents(0.0f, 0.0f, 1.0f);
	this->updateViewMatrix();
}

void Camera::loadMatrix() const{
	Vec3f lookAtPoint(&this->position);
	lookAtPoint.add(&this->lookAtVector);
	const float* const position = this->position.getComponents();
	const float* const lookAt = lookAtPoint.getComponents();
	const float* const up = this->upVector.getComponents();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(position[0], position[1], position[2], lookAt[0], lookAt[1], lookAt[2], up[0], up[1], up[2]);
}

void Camera::updateProjectionMatrix(){
	this->projectionMatrix.perspective(this->fovAngle, this->getRatio(), this->zNear, this->zFar);
}

void Camera::updateViewMatrix(){
	this->viewMatrix.camera(&this->position, &this->lookAtVector, &this->upVector);
}

Camera::~Camera(){}

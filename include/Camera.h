#ifndef CAMERA_H
#define CAMERA_H
#include <GL/glew.h>
#include <GL/glut.h>
#include "vectors.h"
#include "matrices.h"
class Camera {
	public:
		Camera();

		float getFovAngle() const;
		int getWindowWidth() const;
		int getWindowHeight() const;
		float getRatio() const;
		float getZNear() const;
		float getZFar() const;
		const Mat4f* getProjectionMatrix() const;

		const Vec3f* getPosition() const;
		const Vec3f* getLookAtVector() const;
		const Vec3f* getUpVector() const;
		const Mat4f* getViewMatrix() const;

		void setPosition(const Vec3f* const position);
		void setLookAtVector(const Vec3f* const lookAtVector);
		void setUpVector(const Vec3f* const upVector);
		void reshape(const int windowWidth, const int windowHeight);

		void moveForwards(const float amount);
		void moveRight(const float amount);
		void moveUp(const float amount);
		void zoomIn(const float amount);
		static const float MINIMUM_FOV_ANGLE;
		static const float DEFAULT_FOV_ANGLE;
		static const float MAXIMUM_FOV_ANGLE;
		static const int DEFAULT_WINDOW_WIDTH;
		static const int DEFAULT_WINDOW_HEIGHT;
		static const float DEFAULT_Z_NEAR;
		static const float DEFAULT_Z_FAR;
		static const float DEFAULT_DISTANCE;

		void loadAnterior();
		void loadDorsal();
		void loadLateralLeft();
		void loadLateralRight();
		void loadPosterior();
		void loadVentral();

		void loadAnterior(const float distance);
		void loadDorsal(const float distance);
		void loadLateralLeft(const float distance);
		void loadLateralRight(const float distance);
		void loadPosterior(const float distance);
		void loadVentral(const float distance);

		void loadMatrix() const;
		void updateProjectionMatrix();
		void updateViewMatrix();

		~Camera();
	private:
		Vec3f position;
		Vec3f lookAtVector;
		Vec3f upVector;
		float fovAngle;
		int windowWidth;
		int windowHeight;
		float zNear;
		float zFar;
		Mat4f projectionMatrix;
		Mat4f viewMatrix;
		int currentDistance;
		float distances[6];
		static const int ANTERIOR_DISTANCE = 0;
		static const int DORSAL_DISTANCE = 1;
		static const int LATERAL_LEFT_DISTANCE = 2;
		static const int LATERAL_RIGHT_DISTANCE = 3;
		static const int POSTERIOR_DISTANCE = 4;
		static const int VENTRAL_DISTANCE = 5;
};
#endif // CAMERA_H

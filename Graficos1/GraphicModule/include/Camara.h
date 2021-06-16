#pragma once
#include "Vector.h"
#if defined(OGL)
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#endif

namespace GraphicsModule
{
class Camara
{
public:
	Camara();
	Camara(Vector eye, Vector lookAt, Vector WorldUp, float ViewWidth, float ViewHeight, float NearZ, float FarZ, bool isPerspective = false, float FovAngleY = 0);
	~Camara();

	void Init(Vector eye, Vector lookAt, Vector WorldUp, float ViewWidth, float ViewHeight, float NearZ, float FarZ, bool isPerspective = false, float FovAngleY = 0);

	void setEyePos(Vector eye);
	Vector getEyePos();

	void setLookAt(Vector lookAt);
	Vector getLookAt();

	void setWorldUpVector(Vector up);
	Vector getUpVector();

	void setIsPerspective(bool isPerspective) { m_isPersective = isPerspective; m_resized = true; }
	bool getIsPerspective() { return m_isPersective; }

	void setFovAngleY(float fovAngleY) { m_fovAngleY = fovAngleY;  m_resized = true; }
	float getFovAngleY() { return m_fovAngleY; }

	void setViewWidth(float viewWidth) { m_viewWidth = viewWidth; m_resized = true; }
	float getViewWidth() { return m_viewWidth; }

	void setViewHeight(float viewHeight) { m_viewHeight = viewHeight; m_resized = true; }
	float getViewHeight() { return m_viewHeight; }

	void setNearZ(float nearZ) { m_nearZ = nearZ; m_resized = true; }
	float getNearZ() { return m_nearZ; }

	void setFarZ(float farZ) { m_farZ = farZ;  m_resized = true; }
	float getFarZ() { return m_farZ; }


	void move(Vector moved);
	void rotate(Vector rotation);
	void Update();


	float* getViewMatrix();
	float* getProjectionMatrix();

	static float* getViewMatrix(Vector eye, Vector lookAt, Vector up);
	static float* getOrtograficMatrix(float ViewWidth, float ViewHeight, float NearZ, float FarZ);
	static float* getPerspectiveMatrix(float FovAngleY, float AspectRatio, float NearZ, float FarZ);

	Vector GetUpVector() { return m_up; }
	Vector GetRightVector() { return m_right; }
	Vector GetFrontVector() { return m_front; }

private:
	void calculateRight();
	void calculateFront();
	void calculateUp();

	float* getOrtograficMatrix();
	float* getPerspectiveMatrix();

private:
	Vector m_eye;
	Vector m_lookAt;
	Vector m_Worldup;
	
	Vector m_up;
	Vector m_right;
	Vector m_front;

	bool m_isPersective;

	float m_fovAngleY;
	float m_viewWidth;
	float m_viewHeight;
	float m_nearZ; 
	float m_farZ;

	float *m_ortoMatrix;
	float *m_perspectiveMatrix;
	float *m_viewMatrix;

	bool m_resized = true;

	float m_velocity = .3f;
};
}
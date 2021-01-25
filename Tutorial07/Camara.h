#pragma once
#include "Vector.h"
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

	void setIsPerspective(bool isPerspective) { m_isPersective = isPerspective; }
	bool getIsPerspective() { return m_isPersective; }

	void setFovAngleY(float fovAngleY) { m_fovAngleY = fovAngleY; }
	float getFovAngleY() { return m_fovAngleY; }

	void setViewWidth(float viewWidth) { m_viewWidth = viewWidth; }
	float getViewWidth() { return m_viewWidth; }

	void setViewHeight(bool viewHeight) { m_viewHeight = viewHeight; }
	bool getViewHeight() { return m_viewHeight; }

	void setNearZ(float nearZ) { m_nearZ = nearZ; }
	float getNearZ() { return m_nearZ; }

	void setFarZ(float farZ) { m_farZ = farZ; }
	float getFarZ() { return m_farZ; }


	void move(Vector moved);
	void rotate(Vector rotation);
	void Update();


	float* getViewMatrix();
	float* getProjectionMatrix();

	static float* getViewMatrix(Vector eye, Vector lookAt, Vector up);
	static float* getOrtograficMatrix(float ViewWidth, float ViewHeight, float NearZ, float FarZ);
	static float* getPerspectiveMatrix(float FovAngleY, float AspectRatio, float NearZ, float FarZ);

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
};
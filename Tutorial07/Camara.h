#pragma once
#include "Vector.h"
class Camara
{
public:
	Camara();
	Camara(Vector eye, Vector lookAt, Vector up);
	~Camara();

	void setEyePos(Vector eye);
	Vector getEyePos();

	void setLookAt(Vector lookAt);
	Vector getLookAt();

	void setWorldUpVector(Vector up);
	Vector getUpVector();

	float* getViewMatrix();

	static float* getViewMatrix(Vector eye, Vector lookAt, Vector up);
	static float* getOrtograficMatrix(float ViewWidth, float ViewHeight, float NearZ, float FarZ);
	static float* getPerspectiveMatrix(float FovAngleY, float AspectRatio, float NearZ, float FarZ);

	void move(Vector moved);

private:
	void calculateRight();
	void calculateFront();
	void calculateUp();

private:
	Vector m_eye;
	Vector m_lookAt;
	Vector m_Worldup;
	
	Vector m_up;
	Vector m_right;
	Vector m_front;
};
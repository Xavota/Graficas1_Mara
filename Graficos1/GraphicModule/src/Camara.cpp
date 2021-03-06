#include "Camara.h"
#include "Mouse.h"
#include <math.h>

namespace GraphicsModule
{
Camara::Camara()
{
    m_eye = { 0,0,0 };
    m_lookAt = { 0,0,1 };
    m_Worldup = { 0,1,0 };

	calculateFront();
	calculateRight();
	calculateUp();

	m_viewWidth = 640;
	m_viewHeight = 480;
	m_nearZ = .01f;
	m_farZ = 100.f;

	m_fovAngleY = 0;

	m_isPersective = false;
}

Camara::Camara(Vector eye, Vector lookAt, Vector WorldUp, float ViewWidth, float ViewHeight, float NearZ, float FarZ, bool isPerspective, float FovAngleY)
{
    Init(eye, lookAt, WorldUp, ViewWidth, ViewHeight, NearZ, FarZ, isPerspective, FovAngleY);
}

Camara::~Camara()
{
}

void Camara::Init(Vector eye, Vector lookAt, Vector WorldUp, float ViewWidth, float ViewHeight, float NearZ, float FarZ, bool isPerspective, float FovAngleY)
{
	m_eye = eye;
	m_lookAt = lookAt;
	m_Worldup = WorldUp;

	calculateFront();
	calculateRight();
	calculateUp();

	m_viewWidth = ViewWidth;
	m_viewHeight = ViewHeight;
	m_nearZ = NearZ;
	m_farZ = FarZ;

	m_fovAngleY = FovAngleY;

	m_isPersective = isPerspective;

    m_ortoMatrix = new float[16];
    m_perspectiveMatrix = new float[16];
    m_viewMatrix = new float[16];
}

void Camara::setEyePos(Vector eye)
{
    m_eye = eye;
    calculateFront();
    calculateRight();
    calculateUp();
}

Vector Camara::getEyePos()
{
    return m_eye;
}

void Camara::setLookAt(Vector lookAt)
{
    m_lookAt = lookAt;
    calculateFront();
    calculateRight();
    calculateUp();
}

Vector Camara::getLookAt()
{
    return m_lookAt;
}

void Camara::setWorldUpVector(Vector Worldup)
{
    m_Worldup = Worldup;
    calculateFront();
    calculateRight();
    calculateUp();
}

Vector Camara::getUpVector()
{
    return m_up;
}

float* Camara::getViewMatrix()
{
//#if defined(DX11)
	m_viewMatrix[0 * 4 + 0] = m_right.x();
	m_viewMatrix[0 * 4 + 1] = m_up.x();
	m_viewMatrix[0 * 4 + 2] = m_front.x();
	m_viewMatrix[0 * 4 + 3] = 0;
	m_viewMatrix[1 * 4 + 0] = m_right.y();
	m_viewMatrix[1 * 4 + 1] = m_up.y();
	m_viewMatrix[1 * 4 + 2] = m_front.y();
	m_viewMatrix[1 * 4 + 3] = 0;
	m_viewMatrix[2 * 4 + 0] = m_right.z();
	m_viewMatrix[2 * 4 + 1] = m_up.z();
	m_viewMatrix[2 * 4 + 2] = m_front.z();
	m_viewMatrix[2 * 4 + 3] = 0;
	m_viewMatrix[3 * 4 + 0] = -m_right.Dot(m_eye);
	m_viewMatrix[3 * 4 + 1] = -m_up.Dot(m_eye);
	m_viewMatrix[3 * 4 + 2] = -m_front.Dot(m_eye);
	m_viewMatrix[3 * 4 + 3] = 1;
/*#elif defined(OGL)
	glm::mat4 view = glm::lookAt(glm::vec3(m_eye.x(), m_eye.y(), m_eye.z()), glm::vec3(m_lookAt.x(), m_lookAt.y(), m_lookAt.z()), glm::vec3(m_up.x(), m_up.y(), m_up.z()));
	
	m_viewMatrix[0 * 4 + 0] = view[0][0];
	m_viewMatrix[0 * 4 + 1] = view[0][1];
	m_viewMatrix[0 * 4 + 2] = view[0][2];
	m_viewMatrix[0 * 4 + 3] = view[0][3];
	m_viewMatrix[1 * 4 + 0] = view[1][0];
	m_viewMatrix[1 * 4 + 1] = view[1][1];
	m_viewMatrix[1 * 4 + 2] = view[1][2];
	m_viewMatrix[1 * 4 + 3] = view[1][3];
	m_viewMatrix[2 * 4 + 0] = view[2][0];
	m_viewMatrix[2 * 4 + 1] = view[2][1];
	m_viewMatrix[2 * 4 + 2] = view[2][2];
	m_viewMatrix[2 * 4 + 3] = view[2][3];
	m_viewMatrix[3 * 4 + 0] = view[3][0];
	m_viewMatrix[3 * 4 + 1] = view[3][1];
	m_viewMatrix[3 * 4 + 2] = view[3][2];
	m_viewMatrix[3 * 4 + 3] = view[3][3];

#endif*/

    return m_viewMatrix;
}

float* Camara::getProjectionMatrix()
{
    return m_isPersective ? getPerspectiveMatrix() : getOrtograficMatrix();
}

float* Camara::getOrtograficMatrix()
{
	float r = m_viewWidth / 150;
	float l = -m_viewWidth / 150;
	float t = m_viewHeight / 150;
	float b = -m_viewHeight / 150;
	float f = (m_farZ - m_nearZ) / 2;
	float n = -(m_farZ - m_nearZ) / 2;

    m_ortoMatrix[0 * 4 + 0] = 2 / (r - l);
    m_ortoMatrix[0 * 4 + 1] = 0;
	m_ortoMatrix[0 * 4 + 2] = 0;
	m_ortoMatrix[0 * 4 + 3] = -((r + l) / (r - l));
	m_ortoMatrix[1 * 4 + 0] = 0;
	m_ortoMatrix[1 * 4 + 1] = 2 / (t - b);
	m_ortoMatrix[1 * 4 + 2] = 0;
	m_ortoMatrix[1 * 4 + 3] = -((t + b) / (t - b));
	m_ortoMatrix[2 * 4 + 0] = 0;
	m_ortoMatrix[2 * 4 + 1] = 0;
	m_ortoMatrix[2 * 4 + 2] = 2 / (f - n);
	m_ortoMatrix[2 * 4 + 3] = -((f + n) / (f - n));
	m_ortoMatrix[3 * 4 + 0] = 0;
	m_ortoMatrix[3 * 4 + 1] = 0;
	m_ortoMatrix[3 * 4 + 2] = 0;
	m_ortoMatrix[3 * 4 + 3] = 1;

    return m_ortoMatrix;
}

float* Camara::getPerspectiveMatrix()
{
	if (m_resized)
	{
		float AspectRatio = m_viewWidth / m_viewHeight;
//#if defined(DX11)
		float fovCos = cosf(m_fovAngleY * .5f), fovSin = sinf(m_fovAngleY * .5f);
		float height = fovCos / fovSin;
		float width = AspectRatio * height;

		m_perspectiveMatrix[0 * 4 + 0] = height / AspectRatio;
		m_perspectiveMatrix[0 * 4 + 1] = 0;
		m_perspectiveMatrix[0 * 4 + 2] = 0;
		m_perspectiveMatrix[0 * 4 + 3] = 0;
		m_perspectiveMatrix[1 * 4 + 0] = 0;
		m_perspectiveMatrix[1 * 4 + 1] = height;
		m_perspectiveMatrix[1 * 4 + 2] = 0;
		m_perspectiveMatrix[1 * 4 + 3] = 0;
		m_perspectiveMatrix[2 * 4 + 0] = 0;
		m_perspectiveMatrix[2 * 4 + 1] = 0;
		m_perspectiveMatrix[2 * 4 + 2] = (m_farZ / (m_farZ - m_nearZ));
		m_perspectiveMatrix[2 * 4 + 3] = 1;
		m_perspectiveMatrix[3 * 4 + 0] = 0;
		m_perspectiveMatrix[3 * 4 + 1] = 0;
		m_perspectiveMatrix[3 * 4 + 2] = (-m_farZ * m_nearZ) / (m_farZ - m_nearZ) ;
		m_perspectiveMatrix[3 * 4 + 3] = 1;

/*#elif defined(OGL)
		glm::mat4 per = glm::perspective(m_fovAngleY, AspectRatio, m_nearZ, m_farZ);

		m_perspectiveMatrix[0 * 4 + 0] = per[0][0];
		m_perspectiveMatrix[0 * 4 + 1] = per[0][1];
		m_perspectiveMatrix[0 * 4 + 2] = per[0][2];
		m_perspectiveMatrix[0 * 4 + 3] = per[0][3];
		m_perspectiveMatrix[1 * 4 + 0] = per[1][0];
		m_perspectiveMatrix[1 * 4 + 1] = per[1][1];
		m_perspectiveMatrix[1 * 4 + 2] = per[1][2];
		m_perspectiveMatrix[1 * 4 + 3] = per[1][3];
		m_perspectiveMatrix[2 * 4 + 0] = per[2][0];
		m_perspectiveMatrix[2 * 4 + 1] = per[2][1];
		m_perspectiveMatrix[2 * 4 + 2] = per[2][2];
		m_perspectiveMatrix[2 * 4 + 3] = per[2][3];
		m_perspectiveMatrix[3 * 4 + 0] = per[3][0];
		m_perspectiveMatrix[3 * 4 + 1] = per[3][1];
		m_perspectiveMatrix[3 * 4 + 2] = per[3][2];
		m_perspectiveMatrix[3 * 4 + 3] = per[3][3];
#endif*/
		m_resized = false;
	}

	return m_perspectiveMatrix;	
}

float* Camara::getViewMatrix(Vector eye, Vector lookAt, Vector Worldup)
{
    Vector z = lookAt - eye;
    z.Normalize();

    Vector x = Worldup.Cross(z);
    x.Normalize();

    Vector y = z.Cross(x);

    return new float[16]{ x.x(),         y.x(),       z.x(), 0,
                          x.y(),         y.y(),       z.y(), 0,
                          x.z(),         y.z(),       z.z(), 0,
                  -x.Dot(eye), -y.Dot(eye), -z.Dot(eye), 1};
}

float* Camara::getOrtograficMatrix(float ViewWidth, float ViewHeight, float NearZ, float FarZ)
{
    float r = ViewWidth / 2;
    float l = -ViewWidth / 2;
    float t = ViewHeight / 2;
    float b = -ViewHeight / 2;
    float f = (FarZ - NearZ) / 2;
    float n = -(FarZ - NearZ) / 2;

    return new float[16]{
        2 / (r - l), 0, 0, -((r + l) / (r - l)),
        0, 2 / (t - b), 0, -((t + b) / (t - b)),
        0, 0, 2 / (f - n), -((f + n) / (f - n)),
        0, 0, 0, 1
    };
}

float* Camara::getPerspectiveMatrix(float FovAngleY, float AspectRatio, float NearZ, float FarZ)
{
    float fovCos = cosf(FovAngleY * .5f), fovSin = sinf(FovAngleY * .5f);
    float height = fovCos / fovSin;
    float width = AspectRatio * height;

    return new float[16]{
        height / AspectRatio, 0, 0, 0,
        0, height, 0, 0,
        0, 0, (FarZ / (FarZ - NearZ)), 1,
        0, 0, -FarZ / (FarZ - NearZ) * NearZ, 0
    };
}

void Camara::move(Vector moved)
{
    Vector realMove = Vector(0, 0, 0);
    realMove += m_right * moved.x() * m_velocity;
    realMove += m_front * moved.y() * m_velocity;
    realMove += m_up * moved.z() * m_velocity;
    setEyePos(getEyePos() + realMove);
    setLookAt(getLookAt() + realMove);
}

void Camara::rotate(Vector rotation)
{
	float rad = (m_lookAt - m_eye).Lenght();
	//float yaw = atan(rotation.x() / rad);
	//float pitch = atan(rotation.y() / rad);
	//Vector dir;
	//dir.setVector(cos(yaw) * cos(pitch), sin(pitch), sin(yaw) * cos(pitch));
    //m_lookAt += rotation / 100;
	m_lookAt = m_front + (m_right * rotation.x()) * .003f + m_up * rotation.y() * .003f;
	m_lookAt.Normalize();
	m_lookAt = m_eye + m_lookAt;
	//m_lookAt = dir.getNormalized() * rad + m_eye;
	calculateFront();
	calculateRight();
	calculateUp();
}

void Camara::Update()
{
	if (Mouse::isPressed())
		rotate(Mouse::getMouseMovement());
}

void Camara::calculateFront()
{
	m_front = m_lookAt - m_eye;
	m_front.Normalize();
}

void Camara::calculateRight()
{
    m_right = m_Worldup.Cross(m_front);
    m_right.Normalize();
}

void Camara::calculateUp()
{
    m_up = m_front.Cross(m_right);
    m_up.Normalize();
}
}
#include "Vector.h"
#include <iostream>
#include <string>

Vector::Vector()
{
    /*m_coords = new float[3];
    std::memset(m_coords, 0, 3 * sizeof(float));*/
}

Vector::Vector(float x, float y, float z)
{
    /*m_coords = new float[3];
    std::memset(m_coords, 0, 3 * sizeof(float));*/
    setVector(x, y, z);
}

Vector::~Vector()
{
    /*if (m_coords != nullptr) {
        delete m_coords;
        m_coords = nullptr;
    }*/
}

void Vector::setVector(float x, float y, float z)
{
    /*m_coords[0] = x;
    m_coords[1] = y;
    m_coords[2] = z;*/
    m_x = x;
    m_y = y;
    m_z = z;
}

float Vector::Dot(Vector other)
{
    //return m_coords[0] * other.m_coords[0] + m_coords[1] * other.m_coords[1] + m_coords[2] * other.m_coords[2];
    float x = 0, y = 0, z = 0;
    x = m_x * other.m_x;
    y = m_y * other.m_y;
    z = m_z * other.m_z;

    return x+y+z;
}

Vector Vector::Cross(Vector other)
{
    /*float newX = m_coords[1] * other.m_coords[2] - m_coords[2] * other.m_coords[1];
    float newY = m_coords[2] * other.m_coords[0] - m_coords[0] * other.m_coords[2];
    float newZ = m_coords[0] * other.m_coords[1] - m_coords[1] * other.m_coords[0];*/

	float newX = m_y * other.m_z - m_z * other.m_y;
	float newY = m_z * other.m_x - m_x * other.m_z;
	float newZ = m_x * other.m_y - m_y * other.m_x;

    return Vector(newX, newY, newZ);
}

void Vector::Normalize()
{
    *this = getNormalized();
}

Vector Vector::getNormalized()
{
    return *this / Lenght();
}

float Vector::Lenght()
{
	//return pow(pow(m_coords[0], 2) + pow(m_coords[1], 2) + pow(m_coords[2], 2), .5f);
    return pow(pow(m_x, 2) + pow(m_y, 2) + pow(m_z, 2), .5f);
}

Vector Vector::operator-(Vector other)
{
    //return Vector(m_coords[0] - other.m_coords[0], m_coords[1] - other.m_coords[1], m_coords[2] - other.m_coords[2]);
    return Vector(m_x - other.m_x, m_y - other.m_y, m_z - other.m_z);
}


Vector Vector::operator+(Vector other)
{
    //return Vector(m_coords[0] + other.m_coords[0], m_coords[1] + other.m_coords[1], m_coords[2] + other.m_coords[2]);
    return Vector(m_x + other.m_x, m_y + other.m_y, m_z + other.m_z);
}

Vector Vector::operator*(float constant)
{
    //return Vector(m_coords[0] * constant, m_coords[1] * constant, m_coords[2] * constant);
    return Vector(m_x * constant, m_y * constant, m_z * constant);
}

Vector Vector::operator/(float constant)
{
    //return Vector(m_coords[0] / constant, m_coords[1] / constant, m_coords[2] / constant);
    return Vector(m_x / constant, m_y / constant, m_z / constant);
}

Vector Vector::operator=(Vector other)
{
    //memcpy(m_coords, other.m_coords, 3 * sizeof(float));
	m_x = other.m_x;
	m_y = other.m_y;
	m_z = other.m_z;
    return *this;
}

Vector Vector::operator-=(Vector other)
{
    *this = *this - other;
    return *this;
}

Vector Vector::operator+=(Vector other)
{
    *this = *this + other;
    return *this;
}

Vector Vector::operator*=(float constant)
{
    *this = *this * constant;
    return *this;
}

Vector Vector::operator/=(float constant)
{
    *this = *this / constant;
    return *this;
}

bool Vector::operator==(Vector other)
{
	//return !memcmp(m_coords, other.m_coords, 3 * sizeof(float));
	return m_x == other.m_x && m_x == other.m_y && m_z == other.m_z;
}

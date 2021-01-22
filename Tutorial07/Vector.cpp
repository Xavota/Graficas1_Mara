#include "Vector.h"
#include <iostream>

Vector::Vector()
{
    m_coords = new float[3];
    std::memset(m_coords, 0, 3 * sizeof(float));
}

Vector::Vector(float x, float y, float z)
{
    m_coords = new float[3];
    std::memset(m_coords, 0, 3 * sizeof(float));
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
    m_coords[0] = x;
    m_coords[1] = y;
    m_coords[2] = z;
}

float Vector::Point(Vector other)
{
    return m_coords[0] * other.m_coords[0] + m_coords[1] * other.m_coords[1] + m_coords[2] * other.m_coords[2];
}

Vector Vector::Cross(Vector other)
{
    float newX = m_coords[1] * other.m_coords[2] - m_coords[2] * other.m_coords[1];
    float newY = m_coords[2] * other.m_coords[0] - m_coords[0] * other.m_coords[2];
    float newZ = m_coords[0] * other.m_coords[1] - m_coords[1] * other.m_coords[0];
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
    return pow(pow(m_coords[0], 2) + pow(m_coords[1], 2) + pow(m_coords[2], 2), .5f);
}

Vector Vector::operator-(Vector other)
{
    return Vector(m_coords[0] - other.m_coords[0], m_coords[1] - other.m_coords[1], m_coords[2] - other.m_coords[2]);
}

Vector Vector::operator+(Vector other)
{
    return Vector(m_coords[0] + other.m_coords[0], m_coords[1] + other.m_coords[1], m_coords[2] + other.m_coords[2]);
}

Vector Vector::operator*(float constant)
{
    return Vector(m_coords[0] * constant, m_coords[1] * constant, m_coords[2] * constant);
}

Vector Vector::operator/(float constant)
{
    return Vector(m_coords[0] / constant, m_coords[1] / constant, m_coords[2] / constant);
}

Vector Vector::operator=(Vector other)
{
    memcpy(m_coords, other.m_coords, 3 * sizeof(float));
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
    return !memcmp(m_coords, other.m_coords, 3 * sizeof(float));
}

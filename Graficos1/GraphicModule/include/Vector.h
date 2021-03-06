#pragma once


namespace GraphicsModule
{
class Vector
{
public:
	Vector();
	Vector(float x, float y, float z);
	~Vector();
	void setVector(float x, float y, float z);
	//float* getVectorFloats() { return m_coords; }
	float Dot(Vector other);
	Vector Cross(Vector other);
	void Normalize();
	Vector getNormalized();
	float Lenght();
	float x() { return m_x; }
	float y() { return m_y; }
	float z() { return m_z; }

	Vector operator-(Vector other);
	Vector operator+(Vector other);
	Vector operator*(float constant);
	Vector operator/(float constant);

	Vector operator=(Vector other);
	Vector operator-=(Vector other);
	Vector operator+=(Vector other);
	Vector operator*=(float constant);
	Vector operator/=(float constant);

	bool operator==(Vector other);

private:
	float m_x = 0;
	float m_y = 0;
	float m_z = 0;
};
}

#pragma once
class Vector
{
public:
	Vector();
	Vector(float x, float y, float z);
	~Vector();
	void setVector(float x, float y, float z);
	float* getVectorFloats() { return m_coords; }
	float Point(Vector other);
	Vector Cross(Vector other);
	void Normalize();
	Vector getNormalized();
	float Lenght();
	float x() { return m_coords[0]; }
	float y() { return m_coords[1]; }
	float z() { return m_coords[2]; }

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
	float* m_coords;
};


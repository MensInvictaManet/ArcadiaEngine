#pragma once

#include <SDL_opengl.h>
#include "Vector3.h"

struct BasicPrimativeIcohedron
{
	BasicPrimativeIcohedron(float size = 10.0f, bool showLines = true) :
		m_HalfSize(size / 2.0f),
		m_ShowLines(showLines),
		m_LineColor(0.0f, 0.0f, 0.0f),
		m_SurfaceColor(1.0f, 1.0f, 1.0f)
	{}

	inline void SetValues(float size = 10.0f, bool showLines = true, float rotationSpeed = 0.0f) {
		SetSize(size);
		SetShowLines(showLines);
		SetRotationSpeed(rotationSpeed);
	}

	inline void SetSize(float size) {
		m_HalfSize = size / 2.0f;
	}

	inline void SetShowLines(bool showLines) {
		m_ShowLines = showLines;
	}

	inline void SetRotationSpeed(const float rotationSpeed) {
		m_RotationSpeed = rotationSpeed;
		m_RotationStartTime = gameSecondsF;
	}

	inline void SetLineColor(Vector3<float>& lineColor) {
		m_LineColor = lineColor;
	}

	inline void SetSurfaceColor(Vector3<float>& surfaceColor) {
		m_SurfaceColor = surfaceColor;
	}

	inline float getRotation() {
		return (gameSecondsF - m_RotationStartTime) * m_RotationSpeed;
	}

	void Render(Vector3<float>& position, GLuint shaderProgram = 0)
	{
		glPushMatrix();
			//  Move to the given position
			glTranslatef(position.x, position.y, position.z);
			glRotatef(getRotation(), 0.0f, 1.0f, 0.0f);

			//  Basic 3D cube in triangles
			glColor3f(m_SurfaceColor.x, m_SurfaceColor.y, m_SurfaceColor.z);
			glBegin(GL_TRIANGLE_STRIP);
				// 20
				glVertex3f(0.0f, m_HalfSize / 2.0f, -m_HalfSize);
				glVertex3f(m_HalfSize / 2.0f, m_HalfSize, 0.0f);
				glVertex3f(-m_HalfSize / 2.0f, m_HalfSize, 0.0f);
				// 8
				glVertex3f(0.0f, m_HalfSize / 2.0f, m_HalfSize);
				// 10
				glVertex3f(-m_HalfSize, 0.0f, m_HalfSize / 2.0f);
				// 17
				glVertex3f(0.0f, -m_HalfSize / 2.0f, m_HalfSize);
				// 7
				glVertex3f(-m_HalfSize / 2.0f, -m_HalfSize, 0.0f);
				// 1
				glVertex3f(m_HalfSize / 2.0f, -m_HalfSize, 0.0f);
				// 13
				glVertex3f(0.0f, -m_HalfSize / 2.0f, -m_HalfSize);
				// 11
				glVertex3f(m_HalfSize, 0.0f, -m_HalfSize / 2.0f);
				// 4
				glVertex3f(0.0f, m_HalfSize / 2.0f, -m_HalfSize);
				// 14
				glVertex3f(m_HalfSize / 2.0f, m_HalfSize, 0.0f);
				// Waste
				glVertex3f(m_HalfSize / 2.0f, m_HalfSize, 0.0f);
				glVertex3f(0.0f, m_HalfSize / 2.0f, m_HalfSize);
				// 16
				glVertex3f(m_HalfSize, 0.0f, m_HalfSize / 2.0f);
				// 3
				glVertex3f(0.0f, -m_HalfSize / 2.0f, m_HalfSize);
				// 19
				glVertex3f(m_HalfSize / 2.0f, -m_HalfSize, 0.0f);
				// Waste
				glVertex3f(m_HalfSize / 2.0f, -m_HalfSize, 0.0f);
				glVertex3f(m_HalfSize, 0.0f, m_HalfSize / 2.0f);
				// 9
				glVertex3f(m_HalfSize, 0.0f, -m_HalfSize / 2.0f);
				// 6
				glVertex3f(m_HalfSize / 2.0f, m_HalfSize, 0.0f);
				// Waste
				glVertex3f(m_HalfSize / 2.0f, m_HalfSize, 0.0f);
				glVertex3f(0.0f, m_HalfSize / 2.0f, -m_HalfSize);
				glVertex3f(0.0f, m_HalfSize / 2.0f, -m_HalfSize);
				glVertex3f(-m_HalfSize / 2.0f , m_HalfSize, 0.0f);
				// 2
				glVertex3f(-m_HalfSize, 0.0f, -m_HalfSize / 2.0f);
				// 12
				glVertex3f(-m_HalfSize, 0.0f, m_HalfSize / 2.0f);
				// 15
				glVertex3f(-m_HalfSize / 2.0f , -m_HalfSize, 0.0f);
				// Waste
				glVertex3f(-m_HalfSize / 2.0f, -m_HalfSize, 0.0f);
				glVertex3f(-m_HalfSize, 0.0f, -m_HalfSize / 2.0f);
				// 5
				glVertex3f(0.0f, -m_HalfSize / 2.0f, -m_HalfSize);
				// 18
				glVertex3f(0.0f, m_HalfSize / 2.0f, -m_HalfSize);
			glEnd();
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

			//  Basic 3D cube in lines, to outline
			glColor4f(m_LineColor.x, m_LineColor.y, m_LineColor.z, 0.2f);
			glBegin(GL_LINE_STRIP);
				// 20
				glVertex3f(0.0f, m_HalfSize / 2.0f, -m_HalfSize);
				glVertex3f(m_HalfSize / 2.0f, m_HalfSize, 0.0f);
				glVertex3f(-m_HalfSize / 2.0f, m_HalfSize, 0.0f);
				glVertex3f(0.0f, m_HalfSize / 2.0f, -m_HalfSize);
				// 2
				glVertex3f(-m_HalfSize, 0.0f, -m_HalfSize / 2.0f);
				glVertex3f(-m_HalfSize / 2.0f, m_HalfSize, 0.0f);
				// 8
				glVertex3f(0.0f, m_HalfSize / 2.0f, m_HalfSize);
				glVertex3f(m_HalfSize / 2.0f, m_HalfSize, 0.0f);
				// 14
				glVertex3f(m_HalfSize, 0.0f, -m_HalfSize / 2.0f);
				glVertex3f(0.0f, m_HalfSize / 2.0f, -m_HalfSize);
				// 18
				glVertex3f(0.0f, -m_HalfSize / 2.0f, -m_HalfSize);
				glVertex3f(-m_HalfSize, 0.0f, -m_HalfSize / 2.0f);
				// 5
				glVertex3f(-m_HalfSize / 2.0f, -m_HalfSize, 0.0f);
				glVertex3f(0.0f, -m_HalfSize / 2.0f, -m_HalfSize);
				// 4
				glVertex3f(m_HalfSize, 0.0f, -m_HalfSize / 2.0f);
				// 6
				glVertex3f(m_HalfSize, 0.0f, m_HalfSize / 2.0f);
				glVertex3f(m_HalfSize / 2.0f, m_HalfSize, 0.0f);
				// 16
				glVertex3f(m_HalfSize, 0.0f, m_HalfSize / 2.0f);
				glVertex3f(0.0f, m_HalfSize / 2.0f, m_HalfSize);
				// 10
				glVertex3f(-m_HalfSize, 0.0f, m_HalfSize / 2.0f);
				glVertex3f(-m_HalfSize / 2.0f, m_HalfSize, 0.0f);
				// 12
				glVertex3f(-m_HalfSize, 0.0f, m_HalfSize / 2.0f);
				glVertex3f(-m_HalfSize, 0.0f, -m_HalfSize / 2.0f);
				// 15
				glVertex3f(-m_HalfSize, 0.0f, m_HalfSize / 2.0f);
				glVertex3f(-m_HalfSize / 2.0f, -m_HalfSize, 0.0f);
				// 7
				glVertex3f(0.0f, -m_HalfSize / 2.0f, m_HalfSize);
				glVertex3f(-m_HalfSize, 0.0f, m_HalfSize / 2.0f);
				// 17
				glVertex3f(0.0f, -m_HalfSize / 2.0f, m_HalfSize);
				glVertex3f(0.0f, m_HalfSize / 2.0f, m_HalfSize);
				// 3
				glVertex3f(m_HalfSize, 0.0f, m_HalfSize / 2.0f);
				glVertex3f(0.0f, -m_HalfSize / 2.0f, m_HalfSize);
				// 19
				glVertex3f(m_HalfSize / 2.0f, -m_HalfSize, 0.0f);
				glVertex3f(0.0f, -m_HalfSize / 2.0f, m_HalfSize);
				// 1
				glVertex3f(-m_HalfSize / 2.0f, -m_HalfSize, 0.0f);
				glVertex3f(m_HalfSize / 2.0f, -m_HalfSize, 0.0f);
				// 13
				glVertex3f(0.0f, -m_HalfSize / 2.0f, -m_HalfSize);
				glVertex3f(-m_HalfSize / 2.0f, -m_HalfSize, 0.0f);
				glVertex3f(0.0f, -m_HalfSize / 2.0f, -m_HalfSize);
				// 11
				glVertex3f(m_HalfSize, 0.0f, -m_HalfSize / 2.0f);
				glVertex3f(m_HalfSize / 2.0f, -m_HalfSize, 0.0f);
				//  9
				glVertex3f(m_HalfSize, 0.0f, m_HalfSize / 2.0f);
			glEnd();
		glPopMatrix();
	}

private:
	float m_HalfSize;
	bool m_ShowLines;
	float m_RotationSpeed = 0.0f;
	float m_RotationStartTime = 0.0f;
	Vector3<float> m_LineColor;
	Vector3<float> m_SurfaceColor;
};
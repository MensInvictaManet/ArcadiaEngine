#pragma once

#include <SDL_opengl.h>
#include "Vector3.h"

struct BasicPrimativeCube
{
private:
	float m_HalfWidth;
	float m_HalfHeight;
	float m_HalfDepth;
	bool m_ShowLines;
	float m_RotationSpeed = 0.0f;
	float m_RotationStartTime = 0.0f;
	Vector3<float> m_LineColor;
	Vector3<float> m_SurfaceColor;

public:
	BasicPrimativeCube(float width = 10.0f, float height = 10.0f, float depth = 10.0f, bool showLines = true) :
		m_ShowLines(showLines),
		m_LineColor(0.0f, 0.0f, 0.0f),
		m_SurfaceColor(1.0f, 1.0f, 1.0f)
	{
		SetWidth(width);
		SetHeight(height);
		SetDepth(depth);
		SetShowLines(showLines);
		SetRotationSpeed(0.0f);
	}

	inline void SetValues(float width = 10.0f, float height = 10.0f, float depth = 10.0f, bool showLines = true, float rotationSpeed = 0.0f) {
		SetWidth(width);
		SetHeight(height);
		SetDepth(depth);
		SetShowLines(showLines);
		SetRotationSpeed(rotationSpeed);
	}

	inline void SetWidth(float width) {
		m_HalfWidth = width / 2.0f;
	}

	inline void SetHeight(float height) {
		m_HalfHeight = height / 2.0f;
	}

	inline void SetDepth(float depth) {
		m_HalfDepth = depth / 2.0f;
	}

	inline void SetShowLines(bool showLines) {
		m_ShowLines = showLines;
	}

	inline void SetLineColor(Vector3<float>& lineColor) {
		m_LineColor = lineColor;
	}

	inline void SetRotationSpeed(const float rotationSpeed) {
		m_RotationSpeed = rotationSpeed;
		m_RotationStartTime = gameSecondsF;
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
				glVertex3f(-m_HalfWidth, -m_HalfHeight, -m_HalfDepth);
				glVertex3f( m_HalfWidth, -m_HalfHeight, -m_HalfDepth);
				glVertex3f(-m_HalfWidth,  m_HalfHeight, -m_HalfDepth);
				glVertex3f( m_HalfWidth,  m_HalfHeight, -m_HalfDepth);
				glVertex3f(-m_HalfWidth,  m_HalfHeight,  m_HalfDepth);
				glVertex3f( m_HalfWidth,  m_HalfHeight,  m_HalfDepth);
				glVertex3f(-m_HalfWidth, -m_HalfHeight,  m_HalfDepth);
				glVertex3f( m_HalfWidth, -m_HalfHeight,  m_HalfDepth);
				glVertex3f( m_HalfWidth, -m_HalfHeight,  m_HalfDepth);
				glVertex3f( m_HalfWidth, -m_HalfHeight, -m_HalfDepth);
				glVertex3f( m_HalfWidth, -m_HalfHeight,  m_HalfDepth);
				glVertex3f( m_HalfWidth,  m_HalfHeight, -m_HalfDepth);
				glVertex3f( m_HalfWidth,  m_HalfHeight,  m_HalfDepth);
				glVertex3f(-m_HalfWidth,  m_HalfHeight, -m_HalfDepth);
				glVertex3f(-m_HalfWidth,  m_HalfHeight,  m_HalfDepth);
				glVertex3f(-m_HalfWidth, -m_HalfHeight, -m_HalfDepth);
				glVertex3f(-m_HalfWidth, -m_HalfHeight,  m_HalfDepth);
			glEnd();

			//  Basic 3D cube in lines, to outline
			glColor4f(m_LineColor.x, m_LineColor.y, m_LineColor.z, 0.2f);
			glBegin(GL_LINE_STRIP);
				glVertex3f(-m_HalfWidth,  m_HalfHeight, -m_HalfDepth);
				glVertex3f(-m_HalfWidth, -m_HalfHeight, -m_HalfDepth);
				glVertex3f( m_HalfWidth, -m_HalfHeight, -m_HalfDepth);
				glVertex3f( m_HalfWidth,  m_HalfHeight, -m_HalfDepth);
				glVertex3f(-m_HalfWidth,  m_HalfHeight, -m_HalfDepth);
				glVertex3f(-m_HalfWidth,  m_HalfHeight,  m_HalfDepth);
				glVertex3f( m_HalfWidth,  m_HalfHeight,  m_HalfDepth);
				glVertex3f( m_HalfWidth,  m_HalfHeight, -m_HalfDepth);
				glVertex3f( m_HalfWidth, -m_HalfHeight, -m_HalfDepth);
				glVertex3f( m_HalfWidth, -m_HalfHeight,  m_HalfDepth);
				glVertex3f( m_HalfWidth,  m_HalfHeight,  m_HalfDepth);
				glVertex3f(-m_HalfWidth,  m_HalfHeight,  m_HalfDepth);
				glVertex3f(-m_HalfWidth, -m_HalfHeight,  m_HalfDepth);
				glVertex3f(-m_HalfWidth, -m_HalfHeight,  m_HalfDepth);
				glVertex3f(-m_HalfWidth, -m_HalfHeight, -m_HalfDepth);
				glVertex3f(-m_HalfWidth, -m_HalfHeight,  m_HalfDepth);
				glVertex3f( m_HalfWidth, -m_HalfHeight,  m_HalfDepth);
			glEnd();
		glPopMatrix();
	}
};
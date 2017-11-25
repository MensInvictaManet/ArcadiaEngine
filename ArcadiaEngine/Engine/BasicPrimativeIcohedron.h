#pragma once

#include "SDL2\SDL_opengl.h"
#include "Vector3.h"

struct BasicPrimativeIcohedron
{
	BasicPrimativeIcohedron(float size = 10.0f, bool showLines = true) :
		m_HalfSize(size / 2.0f),
		m_ShowLines(showLines),
		m_LineColor(0.0f, 0.0f, 0.0f),
		m_SurfaceColor(1.0f, 1.0f, 1.0f)
	{}

	inline void SetValues(float size = 10.0f, bool showLines = true) {
		m_HalfSize = size / 2.0f;
		m_ShowLines = showLines;
	}

	inline void SetSize(float size) {
		m_HalfSize = size / 2.0f;
	}

	inline void SetShowLines(bool showLines) {
		m_ShowLines = showLines;
	}

	inline void SetLineColor(Vector3<float>& lineColor) {
		m_LineColor = lineColor;
	}

	inline void SetSurfaceColor(Vector3<float>& surfaceColor) {
		m_SurfaceColor = surfaceColor;
	}

	void Render(Vector3<float>& position)
	{
		auto x = position.x;
		auto y = position.y;
		auto z = position.z;

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
		glColor3f(m_LineColor.x, m_LineColor.y, m_LineColor.z);
		glLineWidth(3);
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
	}

private:
	float m_HalfSize;
	bool m_ShowLines;
	Vector3<float> m_LineColor;
	Vector3<float> m_SurfaceColor;
};
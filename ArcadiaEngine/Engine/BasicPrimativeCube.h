#pragma once

#include "SDL2\SDL_opengl.h"
#include "Vector3.h"

struct BasicPrimativeCube
{
private:
	float m_HalfWidth;
	float m_HalfHeight;
	float m_HalfDepth;
	bool m_ShowLines;
	Vector3<float> m_LineColor;
	Vector3<float> m_SurfaceColor;

public:
	BasicPrimativeCube(float width = 10.0f, float height = 10.0f, float depth = 10.0f, bool showLines = true) :
		m_HalfWidth(width / 2.0f),
		m_HalfHeight(height / 2.0f),
		m_HalfDepth(depth / 2.0f),
		m_ShowLines(showLines),
		m_LineColor(0.0f, 0.0f, 0.0f),
		m_SurfaceColor(1.0f, 1.0f, 1.0f)
	{}

	inline void SetValues(float width = 10.0f, float height = 10.0f, float depth = 10.0f, bool showLines = true) {
		m_HalfWidth = width / 2.0f;
		m_HalfHeight = height / 2.0f;
		m_HalfDepth = depth / 2.0f;
		m_ShowLines = showLines;
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
			glVertex3f(x + -m_HalfWidth, y + -m_HalfHeight, z + -m_HalfDepth);
			glVertex3f(x +  m_HalfWidth, y + -m_HalfHeight, z + -m_HalfDepth);
			glVertex3f(x + -m_HalfWidth, y +  m_HalfHeight, z + -m_HalfDepth);
			glVertex3f(x +  m_HalfWidth, y +  m_HalfHeight, z + -m_HalfDepth);
			glVertex3f(x + -m_HalfWidth, y +  m_HalfHeight, z +  m_HalfDepth);
			glVertex3f(x +  m_HalfWidth, y +  m_HalfHeight, z +  m_HalfDepth);
			glVertex3f(x + -m_HalfWidth, y + -m_HalfHeight, z +  m_HalfDepth);
			glVertex3f(x +  m_HalfWidth, y + -m_HalfHeight, z +  m_HalfDepth);
			glVertex3f(x +  m_HalfWidth, y + -m_HalfHeight, z +  m_HalfDepth);
			glVertex3f(x +  m_HalfWidth, y + -m_HalfHeight, z + -m_HalfDepth);
			glVertex3f(x +  m_HalfWidth, y + -m_HalfHeight, z +  m_HalfDepth);
			glVertex3f(x +  m_HalfWidth, y +  m_HalfHeight, z + -m_HalfDepth);
			glVertex3f(x +  m_HalfWidth, y +  m_HalfHeight, z +  m_HalfDepth);
			glVertex3f(x + -m_HalfWidth, y +  m_HalfHeight, z + -m_HalfDepth);
			glVertex3f(x + -m_HalfWidth, y +  m_HalfHeight, z +  m_HalfDepth);
			glVertex3f(x + -m_HalfWidth, y + -m_HalfHeight, z + -m_HalfDepth);
			glVertex3f(x + -m_HalfWidth, y + -m_HalfHeight, z +  m_HalfDepth);
		glEnd();

		//  Basic 3D cube in lines, to outline
		glColor3f(m_LineColor.x, m_LineColor.y, m_LineColor.z);
		glLineWidth(3);
		glBegin(GL_LINE_STRIP);
			glVertex3f(x + -m_HalfWidth, y +  m_HalfHeight, z + -m_HalfDepth);
			glVertex3f(x + -m_HalfWidth, y + -m_HalfHeight, z + -m_HalfDepth);
			glVertex3f(x +  m_HalfWidth, y + -m_HalfHeight, z + -m_HalfDepth);
			glVertex3f(x +  m_HalfWidth, y +  m_HalfHeight, z + -m_HalfDepth);
			glVertex3f(x + -m_HalfWidth, y +  m_HalfHeight, z + -m_HalfDepth);
			glVertex3f(x + -m_HalfWidth, y +  m_HalfHeight, z +  m_HalfDepth);
			glVertex3f(x +  m_HalfWidth, y +  m_HalfHeight, z +  m_HalfDepth);
			glVertex3f(x +  m_HalfWidth, y +  m_HalfHeight, z + -m_HalfDepth);
			glVertex3f(x +  m_HalfWidth, y + -m_HalfHeight, z + -m_HalfDepth);
			glVertex3f(x +  m_HalfWidth, y + -m_HalfHeight, z +  m_HalfDepth);
			glVertex3f(x +  m_HalfWidth, y +  m_HalfHeight, z +  m_HalfDepth);
			glVertex3f(x + -m_HalfWidth, y +  m_HalfHeight, z +  m_HalfDepth);
			glVertex3f(x + -m_HalfWidth, y + -m_HalfHeight, z +  m_HalfDepth);
			glVertex3f(x + -m_HalfWidth, y + -m_HalfHeight, z +  m_HalfDepth);
			glVertex3f(x + -m_HalfWidth, y + -m_HalfHeight, z + -m_HalfDepth);
			glVertex3f(x + -m_HalfWidth, y + -m_HalfHeight, z +  m_HalfDepth);
			glVertex3f(x +  m_HalfWidth, y + -m_HalfHeight, z +  m_HalfDepth);
		glEnd();
	}
};
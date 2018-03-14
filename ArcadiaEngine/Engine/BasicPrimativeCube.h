#pragma once

#include "BasicRenderable3D.h"


struct BasicPrimativeCube : public BasicRenderable3D
{
private:
	float m_HalfWidth;
	float m_HalfHeight;
	float m_HalfDepth;
	bool m_ShowLines;
	Color m_LineColor;

	GLuint m_VAO[2];
	GLuint m_VBO[2];

public:
	BasicPrimativeCube(float width = 10.0f, float height = 10.0f, float depth = 10.0f, bool showLines = true) :
		m_ShowLines(showLines),
		m_LineColor(0.0f, 0.0f, 0.0f, 0.2f)
	{
		SetWidth(width);
		SetHeight(height);
		SetDepth(depth);
		SetShowLines(showLines);

		BasicRenderable3D::SetRotationSpeed(0.0f);

		SetupVAO();
	}

	inline void SetValues(float width = 10.0f, float height = 10.0f, float depth = 10.0f, bool showLines = true, float rotationSpeed = 0.0f, tdogl::Program* shaderProgram = nullptr) {
		SetWidth(width);
		SetHeight(height);
		SetDepth(depth);

		SetShowLines(showLines);

		BasicRenderable3D::SetRotationSpeed(rotationSpeed);
		BasicRenderable3D::SetShaderProgram(shaderProgram);

		SetupVAO();
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

	inline void SetLineColor(const Color& lineColor) {
		m_LineColor = lineColor;
	}

	void SetupVAO()
	{
		const unsigned int floatsPerVertex = 5;
		const unsigned int verticesPerShape = 4;
		const unsigned int shapesPerObject = 6;
		const unsigned int floatsForPosition = 3;
		const unsigned int floatsForTextureMap = 2;

		float vertices[floatsPerVertex * verticesPerShape * shapesPerObject] =
		{
			-m_HalfWidth, -m_HalfHeight, -m_HalfDepth,	0.000f, 0.125f,
			m_HalfWidth, -m_HalfHeight, -m_HalfDepth,	1.000f, 0.125f,
			m_HalfWidth, m_HalfHeight, -m_HalfDepth,	1.000f, 0.001f,
			-m_HalfWidth, m_HalfHeight, -m_HalfDepth,	0.000f, 0.001f,

			m_HalfWidth, -m_HalfHeight, -m_HalfDepth,	0.000f, 0.625f,
			m_HalfWidth, -m_HalfHeight, m_HalfDepth,	1.000f, 0.625f,
			m_HalfWidth, m_HalfHeight, m_HalfDepth,		1.000f, 0.500f,
			m_HalfWidth, m_HalfHeight, -m_HalfDepth,	0.000f, 0.500f,

			m_HalfWidth, -m_HalfHeight, m_HalfDepth,	0.000f, 0.250f,
			-m_HalfWidth, -m_HalfHeight, m_HalfDepth,	1.000f, 0.250f,
			-m_HalfWidth, m_HalfHeight, m_HalfDepth,	1.000f, 0.125f,
			m_HalfWidth, m_HalfHeight, m_HalfDepth,		0.000f, 0.125f,

			-m_HalfWidth, -m_HalfHeight, m_HalfDepth,	0.000f, 0.750f,
			-m_HalfWidth, -m_HalfHeight, -m_HalfDepth,	1.000f, 0.750f,
			-m_HalfWidth, m_HalfHeight, -m_HalfDepth,	1.000f, 0.625f,
			-m_HalfWidth, m_HalfHeight, m_HalfDepth,	0.000f, 0.625f,

			-m_HalfWidth, m_HalfHeight, -m_HalfDepth,	0.000f, 0.375f,
			m_HalfWidth, m_HalfHeight, -m_HalfDepth,	1.000f, 0.375f,
			m_HalfWidth, m_HalfHeight, m_HalfDepth,		1.000f, 0.250f,
			-m_HalfWidth, m_HalfHeight, m_HalfDepth,	0.000f, 0.250f,

			-m_HalfWidth, -m_HalfHeight, m_HalfDepth,	0.000f, 0.500f,
			m_HalfWidth, -m_HalfHeight, m_HalfDepth,	1.000f, 0.500f,
			m_HalfWidth, -m_HalfHeight, -m_HalfDepth,	1.000f, 0.375f,
			-m_HalfWidth, -m_HalfHeight, -m_HalfDepth,	0.000f, 0.375f
		};

		BasicRenderable3D::SetupVAO(vertices, floatsPerVertex, verticesPerShape, shapesPerObject, floatsForPosition, floatsForTextureMap, GL_QUADS);
	}

	void Render(Vector3<float>& position, Camera& camera)
	{
		glPushMatrix();
			//  Render the object at the position relative to the camera
			BasicRenderable3D::RenderGeometry(position, camera);

			// Show the lines directly, regardless of the shader
			if (m_ShowLines)
			{
				glColor4f(m_LineColor.R, m_LineColor.G, m_LineColor.B, m_LineColor.A);
				glBegin(GL_LINE_STRIP);
					glVertex3f(-m_HalfWidth, m_HalfHeight, -m_HalfDepth);
					glVertex3f(-m_HalfWidth, -m_HalfHeight, -m_HalfDepth);
					glVertex3f(m_HalfWidth, -m_HalfHeight, -m_HalfDepth);
					glVertex3f(m_HalfWidth, m_HalfHeight, -m_HalfDepth);
					glVertex3f(-m_HalfWidth, m_HalfHeight, -m_HalfDepth);
					glVertex3f(-m_HalfWidth, m_HalfHeight, m_HalfDepth);
					glVertex3f(m_HalfWidth, m_HalfHeight, m_HalfDepth);
					glVertex3f(m_HalfWidth, m_HalfHeight, -m_HalfDepth);
					glVertex3f(m_HalfWidth, -m_HalfHeight, -m_HalfDepth);
					glVertex3f(m_HalfWidth, -m_HalfHeight, m_HalfDepth);
					glVertex3f(m_HalfWidth, m_HalfHeight, m_HalfDepth);
					glVertex3f(-m_HalfWidth, m_HalfHeight, m_HalfDepth);
					glVertex3f(-m_HalfWidth, -m_HalfHeight, m_HalfDepth);
					glVertex3f(-m_HalfWidth, -m_HalfHeight, m_HalfDepth);
					glVertex3f(-m_HalfWidth, -m_HalfHeight, -m_HalfDepth);
					glVertex3f(-m_HalfWidth, -m_HalfHeight, m_HalfDepth);
					glVertex3f(m_HalfWidth, -m_HalfHeight, m_HalfDepth);
				glEnd();
			}
		glPopMatrix();
	}
};
#pragma once

#include "BasicRenderable3D.h"

struct BasicPrimativeIcosahedron : public BasicRenderable3D
{
private:
	float m_HalfSize;
	Color m_LineColor;

public:
	BasicPrimativeIcosahedron(float size = 10.0f, bool showLines = true) :
		m_HalfSize(size / 2.0f),
		m_LineColor(0.0f, 0.0f, 0.0f, 0.2f)
	{}

	inline void SetValues(float size = 10.0f, bool showLines = true, float rotationSpeed = 0.0f, tdogl::Program* shaderProgram = nullptr) {
		SetSize(size);

		SetShowLines(showLines);

		BasicRenderable3D::SetRotationSpeed(rotationSpeed);
		BasicRenderable3D::SetShaderProgram(shaderProgram);

		SetupVAO();
	}

	inline void SetSize(float size) {
		m_HalfSize = size / 2.0f;
	}

	inline void SetLineColor(Color& lineColor) {
		m_LineColor = lineColor;
	}

	void SetupVAO()
	{
		float vertices[5 * 3 * 20] =
		{
			-m_HalfSize / 2.0f, m_HalfSize, 0.0f,	0.0000000f, 1.0000000f,
			m_HalfSize / 2.0f, m_HalfSize, 0.0f,	0.0458984f, 1.0000000f,
			0.0f, m_HalfSize / 2.0f, -m_HalfSize,	0.0234375f, 0.0000000f,

			m_HalfSize / 2.0f, m_HalfSize, 0.0f,	0.0458984f, 1.0000000f,
			-m_HalfSize / 2.0f, m_HalfSize, 0.0f,	0.0917968f, 1.0000000f,
			0.0f, m_HalfSize / 2.0f, m_HalfSize,	0.0693359f, 0.0000000f,

			0.0f, m_HalfSize / 2.0f, m_HalfSize,	0.0917968f, 1.0000000f,
			-m_HalfSize / 2.0f, m_HalfSize, 0.0f,	0.1376953f, 1.0000000f,
			-m_HalfSize, 0.0f, m_HalfSize / 2.0f,	0.1152343f, 0.0000000f,

			-m_HalfSize, 0.0f, m_HalfSize / 2.0f,	0.1376953f, 1.0000000f,
			0.0f, -m_HalfSize / 2.0f, m_HalfSize,	0.1835937f, 1.0000000f,
			0.0f, m_HalfSize / 2.0f, m_HalfSize,	0.1611328f, 0.0000000f,

			-m_HalfSize / 2.0f, -m_HalfSize, 0.0f,	0.1835937f, 1.0000000f,
			0.0f, -m_HalfSize / 2.0f, m_HalfSize,	0.2294921f, 1.0000000f,
			-m_HalfSize, 0.0f, m_HalfSize / 2.0f,	0.2070312f, 0.0000000f,

			-m_HalfSize / 2.0f, -m_HalfSize, 0.0f,	0.2294921f, 1.0000000f,
			m_HalfSize / 2.0f, -m_HalfSize, 0.0f,	0.2753906f, 1.0000000f,
			0.0f, -m_HalfSize / 2.0f, m_HalfSize,	0.2529296f, 0.0000000f,

			m_HalfSize / 2.0f, -m_HalfSize, 0.0f,	0.2753906f, 1.0000000f,
			-m_HalfSize / 2.0f, -m_HalfSize, 0.0f,	0.3212890f, 1.0000000f,
			0.0f, -m_HalfSize / 2.0f, -m_HalfSize,	0.2988281f, 0.0000000f,

			m_HalfSize / 2.0f, -m_HalfSize, 0.0f,	0.3212890f, 1.0000000f,
			0.0f, -m_HalfSize / 2.0f, -m_HalfSize,	0.3671875f, 1.0000000f,
			m_HalfSize, 0.0f, -m_HalfSize / 2.0f,	0.3447265f, 0.0000000f,

			0.0f, m_HalfSize / 2.0f, -m_HalfSize,	0.3671875f, 1.0000000f,
			m_HalfSize, 0.0f, -m_HalfSize / 2.0f,	0.4130859f, 1.0000000f,
			0.0f, -m_HalfSize / 2.0f, -m_HalfSize,	0.3906250f, 0.0000000f,

			0.0f, m_HalfSize / 2.0f, -m_HalfSize,	0.4130859f, 1.0000000f,
			m_HalfSize / 2.0f, m_HalfSize, 0.0f,	0.4589843f, 1.0000000f,
			m_HalfSize, 0.0f, -m_HalfSize / 2.0f,	0.4365234f, 0.0000000f,

			m_HalfSize / 2.0f, m_HalfSize, 0.0f,	0.4589843f, 1.0000000f,
			0.0f, m_HalfSize / 2.0f, m_HalfSize,	0.5048828f, 1.0000000f,
			m_HalfSize, 0.0f, m_HalfSize / 2.0f,	0.4824218f, 0.0000000f,

			0.0f, -m_HalfSize / 2.0f, m_HalfSize,	0.5048828f, 1.0000000f,
			m_HalfSize, 0.0f, m_HalfSize / 2.0f,	0.5507812f, 1.0000000f,
			0.0f, m_HalfSize / 2.0f, m_HalfSize,	0.5283203f, 0.0000000f,

			0.0f, -m_HalfSize / 2.0f, m_HalfSize,	0.5507812f, 1.0000000f,
			m_HalfSize / 2.0f, -m_HalfSize, 0.0f,	0.5966796f, 1.0000000f,
			m_HalfSize, 0.0f, m_HalfSize / 2.0f,	0.5742187f, 0.0000000f,

			m_HalfSize, 0.0f, m_HalfSize / 2.0f,	0.5966796f, 1.0000000f,
			m_HalfSize / 2.0f, -m_HalfSize, 0.0f,	0.6425781f, 1.0000000f,
			m_HalfSize, 0.0f, -m_HalfSize / 2.0f,	0.6201171f, 0.0000000f,

			m_HalfSize, 0.0f, -m_HalfSize / 2.0f,	0.6425781f, 1.0000000f,
			m_HalfSize / 2.0f, m_HalfSize, 0.0f,	0.6884765f, 1.0000000f,
			m_HalfSize, 0.0f, m_HalfSize / 2.0f,	0.6660156f, 0.0000000f,

			-m_HalfSize / 2.0f, m_HalfSize, 0.0f,	0.6884765f, 1.0000000f,
			0.0f, m_HalfSize / 2.0f, -m_HalfSize,	0.7343750f, 1.0000000f,
			-m_HalfSize, 0.0f, -m_HalfSize / 2.0f,	0.7119140f, 0.0000000f,

			-m_HalfSize / 2.0f, m_HalfSize, 0.0f,	0.7343750f, 1.0000000f,
			-m_HalfSize, 0.0f, -m_HalfSize / 2.0f,	0.7802734f, 1.0000000f,
			-m_HalfSize, 0.0f, m_HalfSize / 2.0f,	0.7578125f, 0.0000000f,

			-m_HalfSize / 2.0f, -m_HalfSize, 0.0f,	0.7802734f, 1.0000000f,
			-m_HalfSize, 0.0f, m_HalfSize / 2.0f,	0.8261718f, 1.0000000f,
			-m_HalfSize, 0.0f, -m_HalfSize / 2.0f,	0.8037109f, 0.0000000f,

			0.0f, -m_HalfSize / 2.0f, -m_HalfSize,	0.8261718f, 1.0000000f,
			-m_HalfSize / 2.0f, -m_HalfSize, 0.0f,	0.8720703f, 1.0000000f,
			-m_HalfSize, 0.0f, -m_HalfSize / 2.0f,	0.8496093f, 0.0000000f,

			-m_HalfSize, 0.0f, -m_HalfSize / 2.0f,	0.8720703f, 1.0000000f,
			0.0f, m_HalfSize / 2.0f, -m_HalfSize,	0.9179687f, 1.0000000f,
			0.0f, -m_HalfSize / 2.0f, -m_HalfSize,	0.8955078f, 0.0000000f,
		};

		//  Set up the Geometry VAO in the Basic Renderable 3D subclass
		BasicRenderable3D::SetupGeometryVAO(vertices, 5, 3, 20, 3, 2, GL_TRIANGLES);
	}

	void Render(Vector3<float>& position, Camera& camera)
	{
		glPushMatrix();
			//  Render the object at the position relative to the camera
			BasicRenderable3D::RenderGeometry(position, camera);

			// Show the lines directly, regardless of the shader
			if (BasicRenderable3D::GetShowLines())
			{
				glColor4f(m_LineColor.R, m_LineColor.G, m_LineColor.B, m_LineColor.A);
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
		glPopMatrix();
	}
};
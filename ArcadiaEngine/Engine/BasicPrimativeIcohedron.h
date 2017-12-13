#pragma once

#include <SDL_opengl.h>
#include "Vector3.h"

struct BasicPrimativeIcohedron
{
private:
	float m_HalfSize;
	bool m_ShowLines;
	float m_RotationSpeed = 0.0f;
	float m_RotationStartTime = 0.0f;
	Vector3<float> m_LineColor;
	Vector3<float> m_SurfaceColor;

	tdogl::Program* m_ShaderProgram = nullptr;
	GLuint m_VAO[2];
	GLuint m_VBO[2];

public:
	BasicPrimativeIcohedron(float size = 10.0f, bool showLines = true) :
		m_HalfSize(size / 2.0f),
		m_ShowLines(showLines),
		m_LineColor(0.0f, 0.0f, 0.0f),
		m_SurfaceColor(1.0f, 1.0f, 1.0f)
	{}

	inline void SetValues(float size = 10.0f, bool showLines = true, float rotationSpeed = 0.0f, tdogl::Program* shaderProgram = nullptr) {
		SetSize(size);

		SetShowLines(showLines);
		SetRotationSpeed(rotationSpeed);

		SetShaderProgram(shaderProgram);
		SetupVAO();
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

	inline void SetShaderProgram(tdogl::Program* program) {
		m_ShaderProgram = program;
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

		//  Generate and Bind the geometry vertex array
		glGenVertexArrays(1, &m_VAO[0]);
		glBindVertexArray(m_VAO[0]);

		//  Generate the OpenGL vertex buffer for geometry, and bind it
		glGenBuffers(1, &m_VBO[0]);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO[0]);

		//  Set the vertex buffer data information and the vertex attribute pointer within
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 5 * 3 * 20, vertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(GLuint(0), 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), NULL);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(GLuint(1), 2, GL_FLOAT, GL_TRUE, 5 * sizeof(float), (const GLvoid*)(3 * sizeof(float)));

		glBindVertexArray(0);
	}

	void Render(Vector3<float>& position, Camera& camera)
	{
		glPushMatrix();
			//  Move to the given position
			glTranslatef(position.x, position.y, position.z);
			glRotatef(getRotation(), 0.0f, 1.0f, 0.0f);

			//  Set the base color (in case we aren't overriding with a shader program)
			glColor3f(m_SurfaceColor.x, m_SurfaceColor.y, m_SurfaceColor.z);

			//  Set up and activate any currently set shader program
			if (m_ShaderProgram != nullptr)
			{
				m_ShaderProgram->use();
				m_ShaderProgram->setUniform("camera", camera.matrix());
				m_ShaderProgram->setUniform("model", glm::rotate(glm::translate(glm::mat4(), glm::vec3(position.x, position.y, position.z)), glm::radians(getRotation()), glm::vec3(0, 1, 0)));
				m_ShaderProgram->setUniform("tex", 0);
			}

			//  Draw the geometry (set into the video card using VAO and VBO
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glBindVertexArray(m_VAO[0]);
			glDrawArrays(GL_TRIANGLES, 0, 3 * 20);
			glBindVertexArray(0);

			if (m_ShaderProgram != nullptr) m_ShaderProgram->stopUsing();

			// Show the lines directly, regardless of the shader
			if (m_ShowLines)
			{
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
			}
		glPopMatrix();
	}
};
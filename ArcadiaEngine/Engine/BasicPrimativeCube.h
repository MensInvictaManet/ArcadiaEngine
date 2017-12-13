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

	tdogl::Program* m_ShaderProgram = nullptr;
	GLuint m_VAO[2];
	GLuint m_VBO[2];

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
		SetupVAO();
	}

	inline void SetValues(float width = 10.0f, float height = 10.0f, float depth = 10.0f, bool showLines = true, float rotationSpeed = 0.0f, tdogl::Program* shaderProgram = nullptr) {
		SetWidth(width);
		SetHeight(height);
		SetDepth(depth);

		SetShowLines(showLines);
		SetRotationSpeed(rotationSpeed);

		SetShaderProgram(shaderProgram);
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

	inline void SetShaderProgram(tdogl::Program* program) {
		m_ShaderProgram = program;
	}

	void SetupVAO()
	{
		float vertices[5 * 4 * 6] =
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

		//  Generate and Bind the geometry vertex array
		glGenVertexArrays(1, &m_VAO[0]);
		glBindVertexArray(m_VAO[0]);

		//  Generate the OpenGL vertex buffer for geometry, and bind it
		glGenBuffers(1, &m_VBO[0]);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO[0]);

		//  Set the vertex buffer data information and the vertex attribute pointer within
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 5 * 4 * 6, vertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(GLuint(0), 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), NULL);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(GLuint(1), 2, GL_FLOAT, GL_TRUE, 5 * sizeof(float), (const GLvoid*)(3 * sizeof(float)));

		glBindVertexArray(0);
	}

	void Render(Vector3<float>& position, Camera& camera)
	{
		glPushMatrix();
			//  Move to the given position and rotate the object based on it's rotation speed and rotation start time
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
			glDrawArrays(GL_QUADS, 0, 4 * 6);
			glBindVertexArray(0);

			if (m_ShaderProgram != nullptr) m_ShaderProgram->stopUsing();

			// Show the lines directly, regardless of the shader
			if (m_ShowLines)
			{
				glColor4f(m_LineColor.x, m_LineColor.y, m_LineColor.z, 0.2f);
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
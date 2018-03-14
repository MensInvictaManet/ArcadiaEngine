#pragma once

#include <SDL_opengl.h>
#include <GL/glew.h>
#include "Program.h"
#include "Vector3.h"
#include "TimeSlice.h"
#include "Color.h"

struct BasicRenderable3D
{
private:
	GLuint m_VAO[2];
	GLuint m_VBO[2];
	tdogl::Program* m_ShaderProgram = nullptr;
	float m_RotationSpeed = 0.0f;
	float m_RotationStartTime = 0.0f;
	Color m_RenderColor;
	unsigned int m_RenderType = GL_QUADS;

	unsigned int m_VerticesTotal = 0;

public:
	BasicRenderable3D() : m_RenderColor(1.0f, 1.0f, 1.0f, 1.0f) {}

	inline void SetShaderProgram(tdogl::Program* program) {
		m_ShaderProgram = program;
	}

	inline void SetRotationSpeed(const float rotationSpeed) {
		m_RotationSpeed = rotationSpeed;
		m_RotationStartTime = gameSecondsF;
	}

	inline float getRotation() {
		return (gameSecondsF - m_RotationStartTime) * m_RotationSpeed;
	}

	inline void SetRenderColor(const Color& renderColor) {
		m_RenderColor = renderColor;
	}

	void SetupVAO(float* vertices, unsigned int floatsPerVertex, unsigned int verticesPerShape, unsigned int shapesPerObject, unsigned int floatsForPosition, unsigned int floatsForTextureMap, unsigned int renderType)
	{
		m_VerticesTotal = verticesPerShape * shapesPerObject;
		m_RenderType = renderType;

		//  Generate and Bind the geometry vertex array
		glGenVertexArrays(1, &m_VAO[0]);
		glBindVertexArray(m_VAO[0]);

		//  Generate the OpenGL vertex buffer for geometry, and bind it
		glGenBuffers(1, &m_VBO[0]);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO[0]);

		//  Set the vertex buffer data information and the vertex attribute pointer within
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * floatsPerVertex * verticesPerShape * shapesPerObject, vertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(GLuint(0), floatsForPosition, GL_FLOAT, GL_FALSE, floatsPerVertex * sizeof(float), NULL);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(GLuint(1), floatsForTextureMap, GL_FLOAT, GL_TRUE, floatsPerVertex * sizeof(float), (const GLvoid*)(floatsForPosition * sizeof(float)));

		glBindVertexArray(0);
	}

	void RenderGeometry(Vector3<float>& position, Camera& camera)
	{
		if (m_VerticesTotal == 0) return;

		//  Set the base color (in case we aren't overriding with a shader program)
		glColor4f(m_RenderColor.R, m_RenderColor.G, m_RenderColor.B, m_RenderColor.A);

		//  Move to the given position and rotate the object based on it's rotation speed and rotation start time
		glTranslatef(position.x, position.y, position.z);
		glRotatef(getRotation(), 0.0f, 1.0f, 0.0f);

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
		glDrawArrays(m_RenderType, 0, m_VerticesTotal);
		glBindVertexArray(0);

		if (m_ShaderProgram != nullptr) m_ShaderProgram->stopUsing();
	}
};
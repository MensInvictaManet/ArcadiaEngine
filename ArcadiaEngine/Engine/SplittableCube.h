#pragma once

#include "SDL2\SDL_opengl.h"
#include "Vector3.h"
#include <tuple>
#include <algorithm>
#include "ShapeSplitPoints.h"

struct SplittableCube
{
protected:
	typedef std::tuple<float, float, float, float, float, float, float, float> SC_Point;
	typedef std::tuple<SC_Point, SC_Point, SC_Point, SC_Point> SC_Square;

	static SC_Point MakePoint(float x, float y, float z) { return SC_Point(x, y, z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f); }
	static SC_Point MakePoint(Vector3<float> v) { return SC_Point(v.x, v.y, v.z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f); }
	static SC_Point AddPoints(SC_Point& a, SC_Point& b) {
		return SC_Point(std::get<0>(a) + std::get<0>(b), std::get<1>(a) + std::get<1>(b), std::get<2>(a) + std::get<2>(b), std::get<3>(a) + std::get<3>(b), std::get<4>(a) + std::get<4>(b), std::get<5>(a) + std::get<5>(b), std::get<6>(a) + std::get<6>(b), std::get<7>(a) + std::get<7>(b));
	}
	static SC_Point MultiplyPoint(SC_Point& p, float f) {
		return SC_Point(std::get<0>(p) * f, std::get<1>(p) * f, std::get<2>(p) * f, std::get<3>(p) * f, std::get<4>(p) * f, std::get<5>(p) * f, std::get<6>(p) * f, std::get<7>(p) * f);
	}

	static SC_Point& NormalizePointDistance(SC_Point& p) {
		Vector3<float> distance(std::get<0>(p), std::get<1>(p), std::get<2>(p));
		distance.Normalize();
		std::get<0>(p) = distance.x;
		std::get<1>(p) = distance.y;
		std::get<2>(p) = distance.z;
		return p;
	}

	struct SC_SurfaceSquare
	{
		typedef std::tuple<SC_SurfaceSquare, SC_SurfaceSquare, SC_SurfaceSquare, SC_SurfaceSquare> SC_SplitSquare;

		SC_Square m_Square;
		SC_SplitSquare* m_SplitSquares;

		SC_SurfaceSquare() :
			m_Square(MakePoint(0, 0, 0), MakePoint(0, 0, 0), MakePoint(0, 0, 0), MakePoint(0, 0, 0)),
			m_SplitSquares(nullptr)
		{}

		SC_SurfaceSquare(SC_Square square, SC_SplitSquare* split = nullptr) :
			m_Square(square),
			m_SplitSquares(split)
		{}

		~SC_SurfaceSquare()
		{
			if (m_SplitSquares != nullptr) delete m_SplitSquares;
		}

		void Split(int splitCount, float pointDistance)
		{
			if (splitCount != 0 && m_SplitSquares != nullptr)
			{
				std::get<0>(*m_SplitSquares).Split(splitCount - 1, pointDistance);
				std::get<1>(*m_SplitSquares).Split(splitCount - 1, pointDistance);
				std::get<2>(*m_SplitSquares).Split(splitCount - 1, pointDistance);
				std::get<3>(*m_SplitSquares).Split(splitCount - 1, pointDistance);
				return;
			}

			if (splitCount == 0 && m_SplitSquares != nullptr)
			{
				delete m_SplitSquares;
				m_SplitSquares = nullptr;
				return;
			}

			if (splitCount == 0) return;

			//  If we arrive here, splitCount is greater than 0 and we have no Split Triangles. Create the triangles, and send the split ahead
			SC_Point point1 = MakePoint(std::get<0>(std::get<0>(m_Square)), std::get<1>(std::get<0>(m_Square)), std::get<2>(std::get<0>(m_Square)));
			SC_Point point2 = MakePoint(std::get<0>(std::get<1>(m_Square)), std::get<1>(std::get<1>(m_Square)), std::get<2>(std::get<1>(m_Square)));
			SC_Point point3 = MakePoint(std::get<0>(std::get<2>(m_Square)), std::get<1>(std::get<2>(m_Square)), std::get<2>(std::get<2>(m_Square)));
			SC_Point point4 = MakePoint(std::get<0>(std::get<3>(m_Square)), std::get<1>(std::get<3>(m_Square)), std::get<2>(std::get<3>(m_Square)));
			auto midpoint12 = MultiplyPoint(NormalizePointDistance(MultiplyPoint(AddPoints(point1, point2), 0.5f)), pointDistance);
			auto midpoint23 = MultiplyPoint(NormalizePointDistance(MultiplyPoint(AddPoints(point2, point3), 0.5f)), pointDistance);
			auto midpoint34 = MultiplyPoint(NormalizePointDistance(MultiplyPoint(AddPoints(point3, point4), 0.5f)), pointDistance);
			auto midpoint41 = MultiplyPoint(NormalizePointDistance(MultiplyPoint(AddPoints(point4, point1), 0.5f)), pointDistance);
			auto midpoint = MultiplyPoint(NormalizePointDistance(MultiplyPoint(AddPoints(AddPoints(AddPoints(point1, point2), point3), point4), (1.0f / 4.0f))), pointDistance);
			m_SplitSquares = new SC_SplitSquare(SC_Square(point1, midpoint12, midpoint, midpoint41), SC_Square(point2, midpoint23, midpoint, midpoint12), SC_Square(point3, midpoint34, midpoint, midpoint23), SC_Square(point4, midpoint41, midpoint, midpoint34));
			std::get<0>(*m_SplitSquares).Split(splitCount - 1, pointDistance);
			std::get<1>(*m_SplitSquares).Split(splitCount - 1, pointDistance);
			std::get<2>(*m_SplitSquares).Split(splitCount - 1, pointDistance);
			std::get<3>(*m_SplitSquares).Split(splitCount - 1, pointDistance);
		}
	};

	SC_Square MakeSquare(SC_Point p1, SC_Point p2, SC_Point p3, SC_Point p4) const {
		return std::make_tuple(p1, p2, p3, p4);
	}

	SC_SurfaceSquare MakeSurfaceSquare(SC_Square square, SC_SurfaceSquare::SC_SplitSquare* split) const {
		return SC_SurfaceSquare(square, split);
	}

private:
	const static int SURFACE_POINTS = 8;
	const static int SURFACE_COUNT = 6;
	const static int SURFACE_EDGES = 4;
	const static int SURFACE_SPLIT_COUNT = 4;
	float m_HalfSize;
	float m_PointDistance;
	int m_SplitCount;
	bool m_ShowLines;
	float m_RotationSpeed = 0.0f;
	float m_RotationStartTime = 0.0f;
	Vector3<float> m_LineColor;
	Vector3<float> m_SurfaceColor;

	SC_Point m_PrimarySurfacePoints[SURFACE_POINTS];
	SC_SurfaceSquare m_Surfaces[SURFACE_COUNT];

	unsigned int m_PointCount;
	unsigned int m_LinePointCount;
	GLuint m_VAO[2];
	GLuint m_VBO[2];

public:
	SplittableCube(float size = 10.0f, int splitCount = 0, bool showLines = true, float rotationSpeed = 0.0f) :
		m_LineColor(0.0f, 0.0f, 0.0f),
		m_SurfaceColor(1.0f, 1.0f, 1.0f)
	{
		SetSize(size);

		LayoutSurfacePoints();
		DeterminePrimarySurfaces();

		SetSplitCount(splitCount);
		SetShowLines(showLines);
		SetRotationSpeed(rotationSpeed);

		SetupVAO();
	}

	inline void SetValues(float size = 10.0f, int splitCount = 0, bool showLines = true, float rotationSpeed = 0.0f) {
		SetSize(size);

		LayoutSurfacePoints();
		DeterminePrimarySurfaces();

		SetSplitCount(splitCount);
		SetShowLines(showLines);
		SetRotationSpeed(rotationSpeed);

		SetupVAO();
	}

	inline void SetSize(float size) {
		m_HalfSize = size / 2.0f;
		m_PointDistance = std::sqrt(m_HalfSize * m_HalfSize + m_HalfSize * m_HalfSize + m_HalfSize * m_HalfSize);
	}

	inline void SetSplitCount(int splitCount) {
		if (splitCount != m_SplitCount)
			for (int i = 0; i < SURFACE_COUNT; ++i)
				m_Surfaces[i].Split(splitCount, m_PointDistance);
		m_SplitCount = splitCount;
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

	void LayoutSurfacePoints()
	{
		m_PrimarySurfacePoints[0] = MakePoint(-m_HalfSize, -m_HalfSize, -m_HalfSize);
		m_PrimarySurfacePoints[1] = MakePoint(m_HalfSize, -m_HalfSize, -m_HalfSize);
		m_PrimarySurfacePoints[2] = MakePoint(m_HalfSize, m_HalfSize, -m_HalfSize);
		m_PrimarySurfacePoints[3] = MakePoint(-m_HalfSize, m_HalfSize, -m_HalfSize);
		m_PrimarySurfacePoints[4] = MakePoint(-m_HalfSize, -m_HalfSize, m_HalfSize);
		m_PrimarySurfacePoints[5] = MakePoint(m_HalfSize, -m_HalfSize, m_HalfSize);
		m_PrimarySurfacePoints[6] = MakePoint(m_HalfSize, m_HalfSize, m_HalfSize);
		m_PrimarySurfacePoints[7] = MakePoint(-m_HalfSize, m_HalfSize, m_HalfSize);
	}

	void DeterminePrimarySurfaces()
	{
		m_Surfaces[0] = MakeSurfaceSquare(MakeSquare(m_PrimarySurfacePoints[0], m_PrimarySurfacePoints[1], m_PrimarySurfacePoints[2], m_PrimarySurfacePoints[3]), nullptr);
		m_Surfaces[1] = MakeSurfaceSquare(MakeSquare(m_PrimarySurfacePoints[1], m_PrimarySurfacePoints[5], m_PrimarySurfacePoints[6], m_PrimarySurfacePoints[2]), nullptr);
		m_Surfaces[2] = MakeSurfaceSquare(MakeSquare(m_PrimarySurfacePoints[5], m_PrimarySurfacePoints[4], m_PrimarySurfacePoints[7], m_PrimarySurfacePoints[6]), nullptr);
		m_Surfaces[3] = MakeSurfaceSquare(MakeSquare(m_PrimarySurfacePoints[4], m_PrimarySurfacePoints[0], m_PrimarySurfacePoints[3], m_PrimarySurfacePoints[7]), nullptr);
		m_Surfaces[4] = MakeSurfaceSquare(MakeSquare(m_PrimarySurfacePoints[3], m_PrimarySurfacePoints[2], m_PrimarySurfacePoints[6], m_PrimarySurfacePoints[7]), nullptr);
		m_Surfaces[5] = MakeSurfaceSquare(MakeSquare(m_PrimarySurfacePoints[4], m_PrimarySurfacePoints[5], m_PrimarySurfacePoints[1], m_PrimarySurfacePoints[0]), nullptr);
	}

	void SetupVAO()
	{
		//// NOTE: In the future, use the actual number of points instead of adding duplicate points in
		//auto pointCount = GetShapePointsAfterSplit(SURFACE_POINTS, SURFACE_COUNT, 4, 4, m_SplitCount);
		//float* vertices = new float[pointCount * 3];

		//  Set up a vertex array large enough to hold every point of every surface, even duplicates
		m_PointCount = (m_SplitCount > 0) ? (GetShapeSurfacesAfterSplit(SURFACE_POINTS, SURFACE_COUNT, SURFACE_SPLIT_COUNT, SURFACE_EDGES, m_SplitCount) * SURFACE_EDGES) : (SURFACE_COUNT * SURFACE_EDGES);
		float* vertices = new float[m_PointCount * 3];
		memset(vertices, 0, sizeof(float) * m_PointCount * 3);

		// Using each primary surface, add all geometry vertices
		for (int i = 0; i < SURFACE_COUNT; ++i) AddVerticesForGeometry(m_Surfaces[i], vertices, (m_PointCount * 3 / SURFACE_COUNT), i * (m_PointCount * 3 / SURFACE_COUNT));

		//  Generate and Bind the geometry vertex array
		glGenVertexArrays(1, &m_VAO[0]);
		glBindVertexArray(m_VAO[0]);

		//  Generate the OpenGL vertex buffer for geometry, and bind it
		glGenBuffers(1, &m_VBO[0]);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO[0]);

		//  Set the vertex buffer data information and the vertex attribute pointer within
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_PointCount * 3, vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(GLuint(0), 3, GL_FLOAT, GL_FALSE, 0, 0);

		glEnableVertexAttribArray(0);
		glBindVertexArray(0);

		delete[] vertices;

		m_LinePointCount = m_PointCount * 2;
		vertices = new float[m_LinePointCount * 3];
		memset(vertices, 0, sizeof(float) * m_LinePointCount * 3);

		// Using each primary surface, add all line vertices
		for (int i = 0; i < SURFACE_COUNT; ++i) AddVerticesForLines(m_Surfaces[i], vertices, (m_LinePointCount * 3 / SURFACE_COUNT), i * (m_LinePointCount * 3 / SURFACE_COUNT));

		//  Generate and Bind the line vertex array
		glGenVertexArrays(1, &m_VAO[1]);
		glBindVertexArray(m_VAO[1]);

		//  Generate the OpenGL vertex buffer for geometry, and bind it
		glGenBuffers(1, &m_VBO[1]);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO[1]);

		//  Set the vertex buffer data information and the vertex attribute pointer within
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_LinePointCount * 3, vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(GLuint(0), 3, GL_FLOAT, GL_FALSE, 0, 0);

		glEnableVertexAttribArray(0);
		glBindVertexArray(0);

		delete[] vertices;
	}

	void AddVerticesForGeometry(SC_SurfaceSquare& surfaceSquare, float* vertexArray, unsigned int memoryBlock, unsigned int memoryIndex)
	{
		//  vertexArray is size [m_PointCount * 3] because each point has three floats (x, y, z)
		//  memoryBlock is how many values we should be writing
		//  memoryIndex is where we begin to write, based on which surface we are

		if (surfaceSquare.m_SplitSquares == nullptr)
		{
			vertexArray[memoryIndex + 0] = std::get<0>(std::get<0>(surfaceSquare.m_Square));	vertexArray[memoryIndex +  1] = std::get<1>(std::get<0>(surfaceSquare.m_Square));	vertexArray[memoryIndex +  2] = std::get<2>(std::get<0>(surfaceSquare.m_Square));
			vertexArray[memoryIndex + 3] = std::get<0>(std::get<1>(surfaceSquare.m_Square));	vertexArray[memoryIndex +  4] = std::get<1>(std::get<1>(surfaceSquare.m_Square));	vertexArray[memoryIndex +  5] = std::get<2>(std::get<1>(surfaceSquare.m_Square));
			vertexArray[memoryIndex + 6] = std::get<0>(std::get<2>(surfaceSquare.m_Square));	vertexArray[memoryIndex +  7] = std::get<1>(std::get<2>(surfaceSquare.m_Square));	vertexArray[memoryIndex +  8] = std::get<2>(std::get<2>(surfaceSquare.m_Square));
			vertexArray[memoryIndex + 9] = std::get<0>(std::get<3>(surfaceSquare.m_Square));	vertexArray[memoryIndex + 10] = std::get<1>(std::get<3>(surfaceSquare.m_Square));	vertexArray[memoryIndex + 11] = std::get<2>(std::get<3>(surfaceSquare.m_Square));
		}
		else
		{
			auto newMemoryBlock = memoryBlock / 4;
			AddVerticesForGeometry(std::get<0>(*surfaceSquare.m_SplitSquares), vertexArray, newMemoryBlock, memoryIndex + newMemoryBlock * 0);
			AddVerticesForGeometry(std::get<1>(*surfaceSquare.m_SplitSquares), vertexArray, newMemoryBlock, memoryIndex + newMemoryBlock * 1);
			AddVerticesForGeometry(std::get<2>(*surfaceSquare.m_SplitSquares), vertexArray, newMemoryBlock, memoryIndex + newMemoryBlock * 2);
			AddVerticesForGeometry(std::get<3>(*surfaceSquare.m_SplitSquares), vertexArray, newMemoryBlock, memoryIndex + newMemoryBlock * 3);
		}
	}

	void AddVerticesForLines(SC_SurfaceSquare& surfaceSquare, float* vertexArray, unsigned int memoryBlock, unsigned int memoryIndex)
	{
		//  vertexArray is size [m_LinePointCount * 3] because each point has three floats (x, y, z)
		//  memoryBlock is how many values we should be writing
		//  memoryIndex is where we begin to write, based on which surface we are

		if (surfaceSquare.m_SplitSquares == nullptr)
		{
			vertexArray[memoryIndex +  0] = std::get<0>(std::get<0>(surfaceSquare.m_Square));	vertexArray[memoryIndex +  1] = std::get<1>(std::get<0>(surfaceSquare.m_Square));	vertexArray[memoryIndex +  2] = std::get<2>(std::get<0>(surfaceSquare.m_Square));
			vertexArray[memoryIndex +  3] = std::get<0>(std::get<1>(surfaceSquare.m_Square));	vertexArray[memoryIndex +  4] = std::get<1>(std::get<1>(surfaceSquare.m_Square));	vertexArray[memoryIndex +  5] = std::get<2>(std::get<1>(surfaceSquare.m_Square));
			vertexArray[memoryIndex +  6] = std::get<0>(std::get<1>(surfaceSquare.m_Square));	vertexArray[memoryIndex +  7] = std::get<1>(std::get<1>(surfaceSquare.m_Square));	vertexArray[memoryIndex +  8] = std::get<2>(std::get<1>(surfaceSquare.m_Square));
			vertexArray[memoryIndex +  9] = std::get<0>(std::get<2>(surfaceSquare.m_Square));	vertexArray[memoryIndex + 10] = std::get<1>(std::get<2>(surfaceSquare.m_Square));	vertexArray[memoryIndex + 11] = std::get<2>(std::get<2>(surfaceSquare.m_Square));
			vertexArray[memoryIndex + 12] = std::get<0>(std::get<2>(surfaceSquare.m_Square));	vertexArray[memoryIndex + 13] = std::get<1>(std::get<2>(surfaceSquare.m_Square));	vertexArray[memoryIndex + 14] = std::get<2>(std::get<2>(surfaceSquare.m_Square));
			vertexArray[memoryIndex + 15] = std::get<0>(std::get<3>(surfaceSquare.m_Square));	vertexArray[memoryIndex + 16] = std::get<1>(std::get<3>(surfaceSquare.m_Square));	vertexArray[memoryIndex + 17] = std::get<2>(std::get<3>(surfaceSquare.m_Square));
			vertexArray[memoryIndex + 18] = std::get<0>(std::get<3>(surfaceSquare.m_Square));	vertexArray[memoryIndex + 19] = std::get<1>(std::get<3>(surfaceSquare.m_Square));	vertexArray[memoryIndex + 20] = std::get<2>(std::get<3>(surfaceSquare.m_Square));
			vertexArray[memoryIndex + 21] = std::get<0>(std::get<0>(surfaceSquare.m_Square));	vertexArray[memoryIndex + 22] = std::get<1>(std::get<0>(surfaceSquare.m_Square));	vertexArray[memoryIndex + 23] = std::get<2>(std::get<0>(surfaceSquare.m_Square));
		}
		else
		{
			auto newMemoryBlock = memoryBlock / 4;
			AddVerticesForLines(std::get<0>(*surfaceSquare.m_SplitSquares), vertexArray, newMemoryBlock, memoryIndex + newMemoryBlock * 0);
			AddVerticesForLines(std::get<1>(*surfaceSquare.m_SplitSquares), vertexArray, newMemoryBlock, memoryIndex + newMemoryBlock * 1);
			AddVerticesForLines(std::get<2>(*surfaceSquare.m_SplitSquares), vertexArray, newMemoryBlock, memoryIndex + newMemoryBlock * 2);
			AddVerticesForLines(std::get<3>(*surfaceSquare.m_SplitSquares), vertexArray, newMemoryBlock, memoryIndex + newMemoryBlock * 3);
		}
	}

	void Render(Vector3<float>& position)
	{
		glPushMatrix();
			//  Move to the given position and rotate the object based on it's rotation speed and rotation start time
			glTranslatef(position.x, position.y, position.z);
			glRotatef((gameSecondsF - m_RotationStartTime) * m_RotationSpeed, 0.0f, 1.0f, 0.0f);

			//  Draw the geometry (set into the video card using VAO and VBO
			glColor3f(m_SurfaceColor.x, m_SurfaceColor.y, m_SurfaceColor.z);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glEnableVertexAttribArray(0);
			glBindVertexArray(m_VAO[0]);
			glDrawArrays(GL_QUADS, 0, m_PointCount);
			glBindVertexArray(0);

			if (m_ShowLines)
			{
				glColor4f(m_LineColor.x, m_LineColor.y, m_LineColor.z, 0.2f);
				glLineWidth(2);
				glEnableVertexAttribArray(0);
				glBindVertexArray(m_VAO[1]);
				glDrawArrays(GL_LINES, 0, m_LinePointCount);
				glBindVertexArray(0);
			}
		glPopMatrix();
	}
};
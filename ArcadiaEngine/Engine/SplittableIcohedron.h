#pragma once

#include "SDL2\SDL_opengl.h"
#include "Vector3.h"
#include <tuple>
#include <algorithm>
#include "ShapeSplitPoints.h"

struct SplittableIcohedron
{
protected:
	typedef std::tuple<float, float, float, float, float, float, float, float> SI_Point;
	typedef std::tuple<SI_Point, SI_Point, SI_Point> SI_Triangle;

	static SI_Point MakePoint(float x, float y, float z) { return SI_Point(x, y, z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f); }
	static SI_Point MakePoint(Vector3<float> v) { return SI_Point(v.x, v.y, v.z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f); }
	static SI_Point AddPoints(SI_Point& a, SI_Point& b) {
		return SI_Point(std::get<0>(a) + std::get<0>(b), std::get<1>(a) + std::get<1>(b), std::get<2>(a) + std::get<2>(b), std::get<3>(a) + std::get<3>(b), std::get<4>(a) + std::get<4>(b), std::get<5>(a) + std::get<5>(b), std::get<6>(a) + std::get<6>(b), std::get<7>(a) + std::get<7>(b));
	}
	static SI_Point MultiplyPoint(SI_Point& p, float f) {
		return SI_Point(std::get<0>(p) * f, std::get<1>(p) * f, std::get<2>(p) * f, std::get<3>(p) * f, std::get<4>(p) * f, std::get<5>(p) * f, std::get<6>(p) * f, std::get<7>(p) * f);
	}

	static SI_Point& NormalizePointDistance(SI_Point& p) {
		Vector3<float> distance(std::get<0>(p), std::get<1>(p), std::get<2>(p));
		distance.Normalize();
		std::get<0>(p) = distance.x;
		std::get<1>(p) = distance.y;
		std::get<2>(p) = distance.z;
		return p;
	}

	struct SI_SurfaceTriangle
	{
		typedef std::tuple<SI_SurfaceTriangle, SI_SurfaceTriangle, SI_SurfaceTriangle, SI_SurfaceTriangle> SI_SplitTriangle;

		SI_Triangle m_Triangle;
		SI_SplitTriangle* m_SplitTriangles;

		SI_SurfaceTriangle() :
			m_Triangle(MakePoint(0, 0, 0), MakePoint(0, 0, 0), MakePoint(0, 0, 0)),
			m_SplitTriangles(nullptr)
		{}

		SI_SurfaceTriangle(SI_Triangle triangle, SI_SplitTriangle* split = nullptr) :
			m_Triangle(triangle),
			m_SplitTriangles(split)
		{}

		~SI_SurfaceTriangle()
		{
			if (m_SplitTriangles != nullptr) delete m_SplitTriangles;
		}

		void Split(int splitCount, float pointDistance)
		{
			if (splitCount != 0 && m_SplitTriangles != nullptr)
			{
				std::get<0>(*m_SplitTriangles).Split(splitCount - 1, pointDistance);
				std::get<1>(*m_SplitTriangles).Split(splitCount - 1, pointDistance);
				std::get<2>(*m_SplitTriangles).Split(splitCount - 1, pointDistance);
				std::get<3>(*m_SplitTriangles).Split(splitCount - 1, pointDistance);
				return;
			}

			if (splitCount == 0 && m_SplitTriangles != nullptr)
			{
				delete m_SplitTriangles;
				m_SplitTriangles = nullptr;
				return;
			}

			if (splitCount == 0) return;

			//  If we arrive here, splitCount is greater than 0 and we have no Split Triangles. Create the triangles, and send the split ahead
			SI_Point point1 = MakePoint(std::get<0>(std::get<0>(m_Triangle)), std::get<1>(std::get<0>(m_Triangle)), std::get<2>(std::get<0>(m_Triangle)));
			SI_Point point2 = MakePoint(std::get<0>(std::get<1>(m_Triangle)), std::get<1>(std::get<1>(m_Triangle)), std::get<2>(std::get<1>(m_Triangle)));
			SI_Point point3 = MakePoint(std::get<0>(std::get<2>(m_Triangle)), std::get<1>(std::get<2>(m_Triangle)), std::get<2>(std::get<2>(m_Triangle)));
			auto midpoint12 = MultiplyPoint(NormalizePointDistance(MultiplyPoint(AddPoints(point1, point2), 0.5f)), pointDistance);
			auto midpoint23 = MultiplyPoint(NormalizePointDistance(MultiplyPoint(AddPoints(point2, point3), 0.5f)), pointDistance);
			auto midpoint31 = MultiplyPoint(NormalizePointDistance(MultiplyPoint(AddPoints(point3, point1), 0.5f)), pointDistance);
			auto midpoint = MultiplyPoint(NormalizePointDistance(MultiplyPoint(AddPoints(AddPoints(point1, point2), point3), (1.0f / 3.0f))), pointDistance);
			m_SplitTriangles = new SI_SplitTriangle(SI_Triangle(point1, midpoint12, midpoint31), SI_Triangle(point2, midpoint23, midpoint12), SI_Triangle(point3, midpoint31, midpoint23), SI_Triangle(midpoint12, midpoint23, midpoint31));
			std::get<0>(*m_SplitTriangles).Split(splitCount - 1, pointDistance);
			std::get<1>(*m_SplitTriangles).Split(splitCount - 1, pointDistance);
			std::get<2>(*m_SplitTriangles).Split(splitCount - 1, pointDistance);
			std::get<3>(*m_SplitTriangles).Split(splitCount - 1, pointDistance);
		}
	};

	SI_Triangle MakeTriangle(SI_Point p1, SI_Point p2, SI_Point p3) const {
		return std::make_tuple(p1, p2, p3);
	}

	SI_SurfaceTriangle MakeSurfaceTriangle(SI_Triangle triangle, SI_SurfaceTriangle::SI_SplitTriangle* split) const {
		return SI_SurfaceTriangle(triangle, split);
	}

private:
	const static int SURFACE_POINTS = 12;
	const static int SURFACE_COUNT = 20;
	const static int SURFACE_EDGES = 3;
	const static int SURFACE_SPLIT_COUNT = 4;
	float m_HalfSize;
	float m_PointDistance;
	int m_SplitCount;
	bool m_ShowLines;
	float m_RotationSpeed = 0.0f;
	float m_RotationStartTime = 0.0f;
	Vector3<float> m_LineColor;
	Vector3<float> m_SurfaceColor;

	SI_Point m_PrimarySurfacePoints[SURFACE_POINTS];
	SI_SurfaceTriangle m_Surfaces[SURFACE_COUNT];

	unsigned int m_PointCount;
	GLuint vaoID;
	GLuint vboID;

public:
	SplittableIcohedron(float size = 10.0f, int splitCount = 0, bool showLines = true, float rotationSpeed = 0.0f) :
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
		m_PointDistance = std::sqrt(m_HalfSize * m_HalfSize + (m_HalfSize / 2.0f) * (m_HalfSize / 2.0f));
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
		m_PrimarySurfacePoints[0] = MakePoint(0.0f, m_HalfSize / 2.0f, -m_HalfSize);
		m_PrimarySurfacePoints[1] = MakePoint(m_HalfSize / 2.0f, m_HalfSize, 0.0f);
		m_PrimarySurfacePoints[2] = MakePoint(-m_HalfSize / 2.0f, m_HalfSize, 0.0f);
		m_PrimarySurfacePoints[3] = MakePoint(0.0f, m_HalfSize / 2.0f, m_HalfSize);
		m_PrimarySurfacePoints[4] = MakePoint(-m_HalfSize, 0.0f, m_HalfSize / 2.0f);
		m_PrimarySurfacePoints[5] = MakePoint(0.0f, -m_HalfSize / 2.0f, m_HalfSize);
		m_PrimarySurfacePoints[6] = MakePoint(-m_HalfSize / 2.0f, -m_HalfSize, 0.0f);
		m_PrimarySurfacePoints[7] = MakePoint(m_HalfSize / 2.0f, -m_HalfSize, 0.0f);
		m_PrimarySurfacePoints[8] = MakePoint(0.0f, -m_HalfSize / 2.0f, -m_HalfSize);
		m_PrimarySurfacePoints[9] = MakePoint(m_HalfSize, 0.0f, -m_HalfSize / 2.0f);
		m_PrimarySurfacePoints[10] = MakePoint(m_HalfSize, 0.0f, m_HalfSize / 2.0f);
		m_PrimarySurfacePoints[11] = MakePoint(-m_HalfSize, 0.0f, -m_HalfSize / 2.0f);
	}

	void DeterminePrimarySurfaces()
	{
		SetSplitCount(0);
		m_Surfaces[0] = MakeSurfaceTriangle(MakeTriangle(m_PrimarySurfacePoints[0], m_PrimarySurfacePoints[1], m_PrimarySurfacePoints[2]), nullptr);	// 20
		m_Surfaces[1] = MakeSurfaceTriangle(MakeTriangle(m_PrimarySurfacePoints[1], m_PrimarySurfacePoints[2], m_PrimarySurfacePoints[3]), nullptr);	//  8
		m_Surfaces[2] = MakeSurfaceTriangle(MakeTriangle(m_PrimarySurfacePoints[2], m_PrimarySurfacePoints[3], m_PrimarySurfacePoints[4]), nullptr);	// 10
		m_Surfaces[3] = MakeSurfaceTriangle(MakeTriangle(m_PrimarySurfacePoints[3], m_PrimarySurfacePoints[4], m_PrimarySurfacePoints[5]), nullptr);	// 17
		m_Surfaces[4] = MakeSurfaceTriangle(MakeTriangle(m_PrimarySurfacePoints[4], m_PrimarySurfacePoints[5], m_PrimarySurfacePoints[6]), nullptr);	//  7
		m_Surfaces[5] = MakeSurfaceTriangle(MakeTriangle(m_PrimarySurfacePoints[5], m_PrimarySurfacePoints[6], m_PrimarySurfacePoints[7]), nullptr);	//  1
		m_Surfaces[6] = MakeSurfaceTriangle(MakeTriangle(m_PrimarySurfacePoints[6], m_PrimarySurfacePoints[7], m_PrimarySurfacePoints[8]), nullptr);	// 13
		m_Surfaces[7] = MakeSurfaceTriangle(MakeTriangle(m_PrimarySurfacePoints[7], m_PrimarySurfacePoints[8], m_PrimarySurfacePoints[9]), nullptr);	// 11
		m_Surfaces[8] = MakeSurfaceTriangle(MakeTriangle(m_PrimarySurfacePoints[8], m_PrimarySurfacePoints[9], m_PrimarySurfacePoints[0]), nullptr);	//  4
		m_Surfaces[9] = MakeSurfaceTriangle(MakeTriangle(m_PrimarySurfacePoints[9], m_PrimarySurfacePoints[0], m_PrimarySurfacePoints[1]), nullptr);	// 14
		m_Surfaces[10] = MakeSurfaceTriangle(MakeTriangle(m_PrimarySurfacePoints[1], m_PrimarySurfacePoints[3], m_PrimarySurfacePoints[10]), nullptr);	// 16
		m_Surfaces[11] = MakeSurfaceTriangle(MakeTriangle(m_PrimarySurfacePoints[3], m_PrimarySurfacePoints[10], m_PrimarySurfacePoints[5]), nullptr);	//  3
		m_Surfaces[12] = MakeSurfaceTriangle(MakeTriangle(m_PrimarySurfacePoints[10], m_PrimarySurfacePoints[5], m_PrimarySurfacePoints[7]), nullptr);	// 19
		m_Surfaces[13] = MakeSurfaceTriangle(MakeTriangle(m_PrimarySurfacePoints[7], m_PrimarySurfacePoints[10], m_PrimarySurfacePoints[9]), nullptr);	//  9
		m_Surfaces[14] = MakeSurfaceTriangle(MakeTriangle(m_PrimarySurfacePoints[10], m_PrimarySurfacePoints[9], m_PrimarySurfacePoints[1]), nullptr);	//  6
		m_Surfaces[15] = MakeSurfaceTriangle(MakeTriangle(m_PrimarySurfacePoints[0], m_PrimarySurfacePoints[2], m_PrimarySurfacePoints[11]), nullptr);	//  2
		m_Surfaces[16] = MakeSurfaceTriangle(MakeTriangle(m_PrimarySurfacePoints[2], m_PrimarySurfacePoints[11], m_PrimarySurfacePoints[4]), nullptr);	// 12
		m_Surfaces[17] = MakeSurfaceTriangle(MakeTriangle(m_PrimarySurfacePoints[11], m_PrimarySurfacePoints[4], m_PrimarySurfacePoints[6]), nullptr);	// 15
		m_Surfaces[18] = MakeSurfaceTriangle(MakeTriangle(m_PrimarySurfacePoints[6], m_PrimarySurfacePoints[11], m_PrimarySurfacePoints[8]), nullptr);	//  5
		m_Surfaces[19] = MakeSurfaceTriangle(MakeTriangle(m_PrimarySurfacePoints[11], m_PrimarySurfacePoints[8], m_PrimarySurfacePoints[0]), nullptr);	// 18
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

		// Using each primary surface, add ever vertex that exists in our shape
		for (int i = 0; i < SURFACE_COUNT; ++i) AddVerticesForSurface(m_Surfaces[i], vertices, (m_PointCount * 3 / SURFACE_COUNT), i * (m_PointCount * 3 / SURFACE_COUNT));

		//  Generate the OpenGL vertex array, and bind it
		glGenVertexArrays(1, &vaoID);
		glBindVertexArray(vaoID);

		//  Generate the OpenGL vertex buffer, and bind it
		glGenBuffers(1, &vboID);
		glBindBuffer(GL_ARRAY_BUFFER, vboID);

		//  Set the vertex buffer data information and the vertex attribute pointer within
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_PointCount * 3, vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(GLuint(0), 3, GL_FLOAT, GL_FALSE, 0, 0);

		glEnableVertexAttribArray(0);
		glBindVertexArray(0);

		delete[] vertices;
	}

	void AddVerticesForSurface(SI_SurfaceTriangle& surfaceSquare, float* vertexArray, unsigned int memoryBlock, unsigned int memoryIndex)
	{
		//  vertexArray is size [m_PointCount * 3] because each point has three floats (x, y, z)
		//  memoryBlock is how many values we should be writing
		//  memoryIndex is where we begin to write, based on which surface we are

		if (surfaceSquare.m_SplitTriangles == nullptr)
		{
			vertexArray[memoryIndex + 0] = std::get<0>(std::get<0>(surfaceSquare.m_Triangle));	vertexArray[memoryIndex + 1] = std::get<1>(std::get<0>(surfaceSquare.m_Triangle));	vertexArray[memoryIndex + 2] = std::get<2>(std::get<0>(surfaceSquare.m_Triangle));
			vertexArray[memoryIndex + 3] = std::get<0>(std::get<1>(surfaceSquare.m_Triangle));	vertexArray[memoryIndex + 4] = std::get<1>(std::get<1>(surfaceSquare.m_Triangle));	vertexArray[memoryIndex + 5] = std::get<2>(std::get<1>(surfaceSquare.m_Triangle));
			vertexArray[memoryIndex + 6] = std::get<0>(std::get<2>(surfaceSquare.m_Triangle));	vertexArray[memoryIndex + 7] = std::get<1>(std::get<2>(surfaceSquare.m_Triangle));	vertexArray[memoryIndex + 8] = std::get<2>(std::get<2>(surfaceSquare.m_Triangle));
		}
		else
		{
			auto newMemoryBlock = memoryBlock / 4;
			AddVerticesForSurface(std::get<0>(*surfaceSquare.m_SplitTriangles), vertexArray, newMemoryBlock, memoryIndex + newMemoryBlock * 0);
			AddVerticesForSurface(std::get<1>(*surfaceSquare.m_SplitTriangles), vertexArray, newMemoryBlock, memoryIndex + newMemoryBlock * 1);
			AddVerticesForSurface(std::get<2>(*surfaceSquare.m_SplitTriangles), vertexArray, newMemoryBlock, memoryIndex + newMemoryBlock * 2);
			AddVerticesForSurface(std::get<3>(*surfaceSquare.m_SplitTriangles), vertexArray, newMemoryBlock, memoryIndex + newMemoryBlock * 3);
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
			glBindVertexArray(vaoID);
			glDrawArrays(GL_TRIANGLES, 0, m_PointCount);
			glBindVertexArray(0);

			//  Basic 3D cube in lines, to outline
			if (m_ShowLines)
			{
				glColor4f(m_LineColor.x, m_LineColor.y, m_LineColor.z, 0.2f);
				glLineWidth(2);
				glBegin(GL_LINES);
				for (int i = 0; i < SURFACE_COUNT; ++i)
					SurfaceTriangleDrawCalls_Lines(&m_Surfaces[i]);
				glEnd();
			}
		glPopMatrix();
	}

	void SurfaceTriangleDrawCalls_Triangles(SI_SurfaceTriangle* triangle)
	{
		if (triangle->m_SplitTriangles != nullptr)
		{
			SurfaceTriangleDrawCalls_Triangles(&(std::get<0>(*triangle->m_SplitTriangles)));
			SurfaceTriangleDrawCalls_Triangles(&(std::get<1>(*triangle->m_SplitTriangles)));
			SurfaceTriangleDrawCalls_Triangles(&(std::get<2>(*triangle->m_SplitTriangles)));
			SurfaceTriangleDrawCalls_Triangles(&(std::get<3>(*triangle->m_SplitTriangles)));
			return;
		}

		glVertex3f(std::get<0>(std::get<0>(triangle->m_Triangle)), std::get<1>(std::get<0>(triangle->m_Triangle)), std::get<2>(std::get<0>(triangle->m_Triangle)));
		glVertex3f(std::get<0>(std::get<1>(triangle->m_Triangle)), std::get<1>(std::get<1>(triangle->m_Triangle)), std::get<2>(std::get<1>(triangle->m_Triangle)));
		glVertex3f(std::get<0>(std::get<2>(triangle->m_Triangle)), std::get<1>(std::get<2>(triangle->m_Triangle)), std::get<2>(std::get<2>(triangle->m_Triangle)));
	}

	void SurfaceTriangleDrawCalls_Lines(SI_SurfaceTriangle* triangle)
	{
		if (triangle->m_SplitTriangles != nullptr)
		{
			SurfaceTriangleDrawCalls_Lines(&(std::get<0>(*triangle->m_SplitTriangles)));
			SurfaceTriangleDrawCalls_Lines(&(std::get<1>(*triangle->m_SplitTriangles)));
			SurfaceTriangleDrawCalls_Lines(&(std::get<2>(*triangle->m_SplitTriangles)));
			SurfaceTriangleDrawCalls_Lines(&(std::get<3>(*triangle->m_SplitTriangles)));
			return;
		}

		glVertex3f(std::get<0>(std::get<0>(triangle->m_Triangle)), std::get<1>(std::get<0>(triangle->m_Triangle)), std::get<2>(std::get<0>(triangle->m_Triangle)));
		glVertex3f(std::get<0>(std::get<1>(triangle->m_Triangle)), std::get<1>(std::get<1>(triangle->m_Triangle)), std::get<2>(std::get<1>(triangle->m_Triangle)));
		glVertex3f(std::get<0>(std::get<1>(triangle->m_Triangle)), std::get<1>(std::get<1>(triangle->m_Triangle)), std::get<2>(std::get<1>(triangle->m_Triangle)));
		glVertex3f(std::get<0>(std::get<2>(triangle->m_Triangle)), std::get<1>(std::get<2>(triangle->m_Triangle)), std::get<2>(std::get<2>(triangle->m_Triangle)));
		glVertex3f(std::get<0>(std::get<2>(triangle->m_Triangle)), std::get<1>(std::get<2>(triangle->m_Triangle)), std::get<2>(std::get<2>(triangle->m_Triangle)));
		glVertex3f(std::get<0>(std::get<0>(triangle->m_Triangle)), std::get<1>(std::get<0>(triangle->m_Triangle)), std::get<2>(std::get<0>(triangle->m_Triangle)));
	}
};
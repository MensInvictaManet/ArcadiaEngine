#pragma once

#include <SDL_opengl.h>
#include <tuple>
#include <algorithm>
#include "Vector3.h"
#include "ShapeSplitPoints.h"

struct SplittableIcosahedron
{
protected:
	typedef std::tuple<float, float, float, float, float> SI_Point;
	typedef std::tuple<SI_Point, SI_Point, SI_Point> SI_Triangle;

	static SI_Point MakePoint(float x, float y, float z, float tx = 0.0f, float ty = 0.0f) { return SI_Point(x, y, z, tx, ty); }
	static SI_Point MakePoint(Vector3<float> v) { return SI_Point(v.x, v.y, v.z, 0.0f, 0.0f); }
	static SI_Point AddPoints(SI_Point& a, SI_Point& b) {
		return SI_Point(std::get<0>(a) + std::get<0>(b), std::get<1>(a) + std::get<1>(b), std::get<2>(a) + std::get<2>(b), std::get<3>(a) + std::get<3>(b), std::get<4>(a) + std::get<4>(b));
	}
	static SI_Point MultiplyPoint(SI_Point& p, float f, bool posOnly = false) {
		if (posOnly) return SI_Point(std::get<0>(p) * f, std::get<1>(p) * f, std::get<2>(p) * f, std::get<3>(p), std::get<4>(p));
		return SI_Point(std::get<0>(p) * f, std::get<1>(p) * f, std::get<2>(p) * f, std::get<3>(p) * f, std::get<4>(p) * f);
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
			SI_Point point1 = MakePoint(std::get<0>(std::get<0>(m_Triangle)), std::get<1>(std::get<0>(m_Triangle)), std::get<2>(std::get<0>(m_Triangle)), std::get<3>(std::get<0>(m_Triangle)), std::get<4>(std::get<0>(m_Triangle)));
			SI_Point point2 = MakePoint(std::get<0>(std::get<1>(m_Triangle)), std::get<1>(std::get<1>(m_Triangle)), std::get<2>(std::get<1>(m_Triangle)), std::get<3>(std::get<1>(m_Triangle)), std::get<4>(std::get<1>(m_Triangle)));
			SI_Point point3 = MakePoint(std::get<0>(std::get<2>(m_Triangle)), std::get<1>(std::get<2>(m_Triangle)), std::get<2>(std::get<2>(m_Triangle)), std::get<3>(std::get<2>(m_Triangle)), std::get<4>(std::get<2>(m_Triangle)));
			auto midpoint12 = MultiplyPoint(NormalizePointDistance(MultiplyPoint(AddPoints(point1, point2), 0.5f)), pointDistance, true);
			auto midpoint23 = MultiplyPoint(NormalizePointDistance(MultiplyPoint(AddPoints(point2, point3), 0.5f)), pointDistance, true);
			auto midpoint31 = MultiplyPoint(NormalizePointDistance(MultiplyPoint(AddPoints(point3, point1), 0.5f)), pointDistance, true);
			auto midpoint = MultiplyPoint(NormalizePointDistance(MultiplyPoint(AddPoints(AddPoints(point1, point2), point3), (1.0f / 3.0f))), pointDistance, true);
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
	tdogl::Program* m_ShaderProgram = nullptr;

	SI_Point m_PrimarySurfacePoints[SURFACE_POINTS];
	SI_SurfaceTriangle m_Surfaces[SURFACE_COUNT];

	unsigned int m_PointCount;
	unsigned int m_LinePointCount;
	GLuint m_VAO[2];
	GLuint m_VBO[2];

public:
	SplittableIcosahedron(float size = 10.0f, int splitCount = 0, bool showLines = true, float rotationSpeed = 0.0f) :
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

	inline void SetValues(float size = 10.0f, int splitCount = 0, bool showLines = true, float rotationSpeed = 0.0f, tdogl::Program* shaderProgram = nullptr) {
		SetSize(size);

		LayoutSurfacePoints();
		DeterminePrimarySurfaces();

		SetSplitCount(splitCount);
		SetShowLines(showLines);
		SetRotationSpeed(rotationSpeed);

		SetShaderProgram(shaderProgram);

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

	inline float getRotation() {
		return (gameSecondsF - m_RotationStartTime) * m_RotationSpeed;
	}

	inline void SetShaderProgram(tdogl::Program* program) {
		m_ShaderProgram = program;
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
		auto point00_0 = MakePoint(0.0f, 0.0f, 0.0f, 0.0000000f, 1.0000000f);
		auto point00_1 = MakePoint(0.0f, 0.0f, 0.0f, 0.0458984f, 1.0000000f);
		auto point00_2 = MakePoint(0.0f, 0.0f, 0.0f, 0.0234375f, 0.0000000f);

		auto point01_0 = MakePoint(0.0f, 0.0f, 0.0f, 0.0458984f, 1.0000000f);
		auto point01_1 = MakePoint(0.0f, 0.0f, 0.0f, 0.0917968f, 1.0000000f);
		auto point01_2 = MakePoint(0.0f, 0.0f, 0.0f, 0.0693359f, 0.0000000f);

		auto point02_0 = MakePoint(0.0f, 0.0f, 0.0f, 0.0917968f, 1.0000000f);
		auto point02_1 = MakePoint(0.0f, 0.0f, 0.0f, 0.1376953f, 1.0000000f);
		auto point02_2 = MakePoint(0.0f, 0.0f, 0.0f, 0.1152343f, 0.0000000f);

		auto point03_0 = MakePoint(0.0f, 0.0f, 0.0f, 0.1376953f, 1.0000000f);
		auto point03_1 = MakePoint(0.0f, 0.0f, 0.0f, 0.1835937f, 1.0000000f);
		auto point03_2 = MakePoint(0.0f, 0.0f, 0.0f, 0.1611328f, 0.0000000f);

		auto point04_0 = MakePoint(0.0f, 0.0f, 0.0f, 0.1835937f, 1.0000000f);
		auto point04_1 = MakePoint(0.0f, 0.0f, 0.0f, 0.2294921f, 1.0000000f);
		auto point04_2 = MakePoint(0.0f, 0.0f, 0.0f, 0.2070312f, 0.0000000f);

		auto point05_0 = MakePoint(0.0f, 0.0f, 0.0f, 0.2294921f, 1.0000000f);
		auto point05_1 = MakePoint(0.0f, 0.0f, 0.0f, 0.2753906f, 1.0000000f);
		auto point05_2 = MakePoint(0.0f, 0.0f, 0.0f, 0.2529296f, 0.0000000f);

		auto point06_0 = MakePoint(0.0f, 0.0f, 0.0f, 0.2753906f, 1.0000000f);
		auto point06_1 = MakePoint(0.0f, 0.0f, 0.0f, 0.3212890f, 1.0000000f);
		auto point06_2 = MakePoint(0.0f, 0.0f, 0.0f, 0.2988281f, 0.0000000f);

		auto point07_0 = MakePoint(0.0f, 0.0f, 0.0f, 0.3212890f, 1.0000000f);
		auto point07_1 = MakePoint(0.0f, 0.0f, 0.0f, 0.3671875f, 1.0000000f);
		auto point07_2 = MakePoint(0.0f, 0.0f, 0.0f, 0.3447265f, 0.0000000f);

		auto point08_0 = MakePoint(0.0f, 0.0f, 0.0f, 0.3671875f, 1.0000000f);
		auto point08_1 = MakePoint(0.0f, 0.0f, 0.0f, 0.4130859f, 1.0000000f);
		auto point08_2 = MakePoint(0.0f, 0.0f, 0.0f, 0.3906250f, 0.0000000f);

		auto point09_0 = MakePoint(0.0f, 0.0f, 0.0f, 0.4130859f, 1.0000000f);
		auto point09_1 = MakePoint(0.0f, 0.0f, 0.0f, 0.4589843f, 1.0000000f);
		auto point09_2 = MakePoint(0.0f, 0.0f, 0.0f, 0.4365234f, 0.0000000f);

		auto point10_0 = MakePoint(0.0f, 0.0f, 0.0f, 0.4589843f, 1.0000000f);
		auto point10_1 = MakePoint(0.0f, 0.0f, 0.0f, 0.5048828f, 1.0000000f);
		auto point10_2 = MakePoint(0.0f, 0.0f, 0.0f, 0.4824218f, 0.0000000f);

		auto point11_0 = MakePoint(0.0f, 0.0f, 0.0f, 0.5048828f, 1.0000000f);
		auto point11_1 = MakePoint(0.0f, 0.0f, 0.0f, 0.5507812f, 1.0000000f);
		auto point11_2 = MakePoint(0.0f, 0.0f, 0.0f, 0.5283203f, 0.0000000f);

		auto point12_0 = MakePoint(0.0f, 0.0f, 0.0f, 0.5507812f, 1.0000000f);
		auto point12_1 = MakePoint(0.0f, 0.0f, 0.0f, 0.5966796f, 1.0000000f);
		auto point12_2 = MakePoint(0.0f, 0.0f, 0.0f, 0.5742187f, 0.0000000f);

		auto point13_0 = MakePoint(0.0f, 0.0f, 0.0f, 0.5966796f, 1.0000000f);
		auto point13_1 = MakePoint(0.0f, 0.0f, 0.0f, 0.6425781f, 1.0000000f);
		auto point13_2 = MakePoint(0.0f, 0.0f, 0.0f, 0.6201171f, 0.0000000f);

		auto point14_0 = MakePoint(0.0f, 0.0f, 0.0f, 0.6425781f, 1.0000000f);
		auto point14_1 = MakePoint(0.0f, 0.0f, 0.0f, 0.6884765f, 1.0000000f);
		auto point14_2 = MakePoint(0.0f, 0.0f, 0.0f, 0.6660156f, 0.0000000f);

		auto point15_0 = MakePoint(0.0f, 0.0f, 0.0f, 0.6884765f, 1.0000000f);
		auto point15_1 = MakePoint(0.0f, 0.0f, 0.0f, 0.7343750f, 1.0000000f);
		auto point15_2 = MakePoint(0.0f, 0.0f, 0.0f, 0.7119140f, 0.0000000f);

		auto point16_0 = MakePoint(0.0f, 0.0f, 0.0f, 0.7343750f, 1.0000000f);
		auto point16_1 = MakePoint(0.0f, 0.0f, 0.0f, 0.7802734f, 1.0000000f);
		auto point16_2 = MakePoint(0.0f, 0.0f, 0.0f, 0.7578125f, 0.0000000f);

		auto point17_0 = MakePoint(0.0f, 0.0f, 0.0f, 0.7802734f, 1.0000000f);
		auto point17_1 = MakePoint(0.0f, 0.0f, 0.0f, 0.8261718f, 1.0000000f);
		auto point17_2 = MakePoint(0.0f, 0.0f, 0.0f, 0.8037109f, 0.0000000f);

		auto point18_0 = MakePoint(0.0f, 0.0f, 0.0f, 0.8261718f, 1.0000000f);
		auto point18_1 = MakePoint(0.0f, 0.0f, 0.0f, 0.8720703f, 1.0000000f);
		auto point18_2 = MakePoint(0.0f, 0.0f, 0.0f, 0.8496093f, 0.0000000f);

		auto point19_0 = MakePoint(0.0f, 0.0f, 0.0f, 0.8720703f, 1.0000000f);
		auto point19_1 = MakePoint(0.0f, 0.0f, 0.0f, 0.9179687f, 1.0000000f);
		auto point19_2 = MakePoint(0.0f, 0.0f, 0.0f, 0.8955078f, 0.0000000f);

		m_Surfaces[0]  = MakeSurfaceTriangle(MakeTriangle(AddPoints(m_PrimarySurfacePoints[2], point00_0), AddPoints(m_PrimarySurfacePoints[1], point00_1),  AddPoints(m_PrimarySurfacePoints[0], point00_2)), nullptr);	// 20
		m_Surfaces[1] = MakeSurfaceTriangle(MakeTriangle(AddPoints(m_PrimarySurfacePoints[1], point01_0), AddPoints(m_PrimarySurfacePoints[2], point01_1),   AddPoints(m_PrimarySurfacePoints[3], point01_2)), nullptr);	//  8
		m_Surfaces[2] = MakeSurfaceTriangle(MakeTriangle(AddPoints(m_PrimarySurfacePoints[3], point02_0), AddPoints(m_PrimarySurfacePoints[2], point02_1),   AddPoints(m_PrimarySurfacePoints[4], point02_2)), nullptr);	// 10
		m_Surfaces[3] = MakeSurfaceTriangle(MakeTriangle(AddPoints(m_PrimarySurfacePoints[4], point03_0), AddPoints(m_PrimarySurfacePoints[5], point03_1),   AddPoints(m_PrimarySurfacePoints[3], point03_2)), nullptr);	// 17
		m_Surfaces[4] = MakeSurfaceTriangle(MakeTriangle(AddPoints(m_PrimarySurfacePoints[6], point04_0), AddPoints(m_PrimarySurfacePoints[5], point04_1),   AddPoints(m_PrimarySurfacePoints[4], point04_2)), nullptr);	//  7
		m_Surfaces[5] = MakeSurfaceTriangle(MakeTriangle(AddPoints(m_PrimarySurfacePoints[6], point05_0), AddPoints(m_PrimarySurfacePoints[7], point05_1),   AddPoints(m_PrimarySurfacePoints[5], point05_2)), nullptr);	//  1
		m_Surfaces[6] = MakeSurfaceTriangle(MakeTriangle(AddPoints(m_PrimarySurfacePoints[7], point06_0), AddPoints(m_PrimarySurfacePoints[6], point06_1),   AddPoints(m_PrimarySurfacePoints[8], point06_2)), nullptr);	// 13
		m_Surfaces[7] = MakeSurfaceTriangle(MakeTriangle(AddPoints(m_PrimarySurfacePoints[7], point07_0), AddPoints(m_PrimarySurfacePoints[8], point07_1),   AddPoints(m_PrimarySurfacePoints[9], point07_2)), nullptr);	// 11
		m_Surfaces[8] = MakeSurfaceTriangle(MakeTriangle(AddPoints(m_PrimarySurfacePoints[0], point08_0), AddPoints(m_PrimarySurfacePoints[9], point08_1),   AddPoints(m_PrimarySurfacePoints[8], point08_2)), nullptr);	//  4
		m_Surfaces[9] = MakeSurfaceTriangle(MakeTriangle(AddPoints(m_PrimarySurfacePoints[0], point09_0), AddPoints(m_PrimarySurfacePoints[1], point09_1),   AddPoints(m_PrimarySurfacePoints[9], point09_2)), nullptr);	// 14
		m_Surfaces[10] = MakeSurfaceTriangle(MakeTriangle(AddPoints(m_PrimarySurfacePoints[1], point10_0), AddPoints(m_PrimarySurfacePoints[3], point10_1),  AddPoints(m_PrimarySurfacePoints[10], point10_2)), nullptr);	// 16
		m_Surfaces[11] = MakeSurfaceTriangle(MakeTriangle(AddPoints(m_PrimarySurfacePoints[5], point11_0), AddPoints(m_PrimarySurfacePoints[10], point11_1), AddPoints(m_PrimarySurfacePoints[3], point11_2)), nullptr);	//  3
		m_Surfaces[12] = MakeSurfaceTriangle(MakeTriangle(AddPoints(m_PrimarySurfacePoints[5], point12_0), AddPoints(m_PrimarySurfacePoints[7], point12_1), AddPoints(m_PrimarySurfacePoints[10], point12_2)), nullptr);	// 19
		m_Surfaces[13] = MakeSurfaceTriangle(MakeTriangle(AddPoints(m_PrimarySurfacePoints[10], point13_0), AddPoints(m_PrimarySurfacePoints[7], point13_1), AddPoints(m_PrimarySurfacePoints[9], point13_2)), nullptr);	//  9
		m_Surfaces[14] = MakeSurfaceTriangle(MakeTriangle(AddPoints(m_PrimarySurfacePoints[9], point14_0), AddPoints(m_PrimarySurfacePoints[1], point14_1), AddPoints(m_PrimarySurfacePoints[10], point14_2)), nullptr);	//  6
		m_Surfaces[15] = MakeSurfaceTriangle(MakeTriangle(AddPoints(m_PrimarySurfacePoints[2], point15_0), AddPoints(m_PrimarySurfacePoints[0], point15_1),  AddPoints(m_PrimarySurfacePoints[11], point15_2)), nullptr);	//  2
		m_Surfaces[16] = MakeSurfaceTriangle(MakeTriangle(AddPoints(m_PrimarySurfacePoints[2], point16_0), AddPoints(m_PrimarySurfacePoints[11], point16_1), AddPoints(m_PrimarySurfacePoints[4], point16_2)), nullptr);	// 12
		m_Surfaces[17] = MakeSurfaceTriangle(MakeTriangle(AddPoints(m_PrimarySurfacePoints[6], point17_0), AddPoints(m_PrimarySurfacePoints[4], point17_1), AddPoints(m_PrimarySurfacePoints[11], point17_2)), nullptr);	// 15
		m_Surfaces[18] = MakeSurfaceTriangle(MakeTriangle(AddPoints(m_PrimarySurfacePoints[8], point18_0), AddPoints(m_PrimarySurfacePoints[6], point18_1), AddPoints(m_PrimarySurfacePoints[11], point18_2)), nullptr);	//  5
		m_Surfaces[19] = MakeSurfaceTriangle(MakeTriangle(AddPoints(m_PrimarySurfacePoints[11], point19_0), AddPoints(m_PrimarySurfacePoints[0], point19_1), AddPoints(m_PrimarySurfacePoints[8], point19_2)), nullptr);	// 18
	}

	void SetupVAO()
	{
		//// NOTE: In the future, use the actual number of points instead of adding duplicate points in
		//auto pointCount = GetShapePointsAfterSplit(SURFACE_POINTS, SURFACE_COUNT, 4, 4, m_SplitCount);
		//float* vertices = new float[pointCount * 3];

		//  Set up a vertex array large enough to hold every point of every surface, even duplicates
		m_PointCount = (m_SplitCount > 0) ? (GetShapeSurfacesAfterSplit(SURFACE_POINTS, SURFACE_COUNT, SURFACE_SPLIT_COUNT, SURFACE_EDGES, m_SplitCount) * SURFACE_EDGES) : (SURFACE_COUNT * SURFACE_EDGES);
		float* vertices = new float[m_PointCount * 5];
		memset(vertices, 0, sizeof(float) * m_PointCount * 5);

		// Using each primary surface, add ever vertex that exists in our shape
		for (int i = 0; i < SURFACE_COUNT; ++i) AddVerticesForGeometry(m_Surfaces[i], vertices, (m_PointCount * 5 / SURFACE_COUNT), i * (m_PointCount * 5 / SURFACE_COUNT));

		//  Generate and Bind the geometry vertex array
		glGenVertexArrays(1, &m_VAO[0]);
		glBindVertexArray(m_VAO[0]);

		//  Generate the OpenGL vertex buffer for geometry, bind it, and set the vertex buffer data information 
		glGenBuffers(1, &m_VBO[0]);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO[0]);

		//  Set the vertex buffer data information and the vertex attribute pointer within
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_PointCount * 5, vertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(GLuint(0), 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(GLuint(1), 2, GL_FLOAT, GL_TRUE, 5 * sizeof(float), (const GLvoid*)(3 * sizeof(float)));

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

	void AddVerticesForGeometry(SI_SurfaceTriangle& surfaceSquare, float* vertexArray, unsigned int memoryBlock, unsigned int memoryIndex)
	{
		//  vertexArray is size [m_PointCount * 5] because each point has five floats (x, y, z, tx, ty)
		//  memoryBlock is how many values we should be writing
		//  memoryIndex is where we begin to write, based on which surface we are

		if (surfaceSquare.m_SplitTriangles == nullptr)
		{
			vertexArray[memoryIndex +  0] = std::get<0>(std::get<0>(surfaceSquare.m_Triangle));	vertexArray[memoryIndex +  1] = std::get<1>(std::get<0>(surfaceSquare.m_Triangle));	vertexArray[memoryIndex +  2] = std::get<2>(std::get<0>(surfaceSquare.m_Triangle));
			vertexArray[memoryIndex +  3] = std::get<3>(std::get<0>(surfaceSquare.m_Triangle));	vertexArray[memoryIndex +  4] = std::get<4>(std::get<0>(surfaceSquare.m_Triangle));
			vertexArray[memoryIndex +  5] = std::get<0>(std::get<1>(surfaceSquare.m_Triangle));	vertexArray[memoryIndex +  6] = std::get<1>(std::get<1>(surfaceSquare.m_Triangle));	vertexArray[memoryIndex +  7] = std::get<2>(std::get<1>(surfaceSquare.m_Triangle));
			vertexArray[memoryIndex +  8] = std::get<3>(std::get<1>(surfaceSquare.m_Triangle));	vertexArray[memoryIndex +  9] = std::get<4>(std::get<1>(surfaceSquare.m_Triangle));
			vertexArray[memoryIndex + 10] = std::get<0>(std::get<2>(surfaceSquare.m_Triangle));	vertexArray[memoryIndex + 11] = std::get<1>(std::get<2>(surfaceSquare.m_Triangle));	vertexArray[memoryIndex + 12] = std::get<2>(std::get<2>(surfaceSquare.m_Triangle));
			vertexArray[memoryIndex + 13] = std::get<3>(std::get<2>(surfaceSquare.m_Triangle));	vertexArray[memoryIndex + 14] = std::get<4>(std::get<2>(surfaceSquare.m_Triangle));
		}
		else
		{
			auto newMemoryBlock = memoryBlock / 4;
			AddVerticesForGeometry(std::get<0>(*surfaceSquare.m_SplitTriangles), vertexArray, newMemoryBlock, memoryIndex + newMemoryBlock * 0);
			AddVerticesForGeometry(std::get<1>(*surfaceSquare.m_SplitTriangles), vertexArray, newMemoryBlock, memoryIndex + newMemoryBlock * 1);
			AddVerticesForGeometry(std::get<2>(*surfaceSquare.m_SplitTriangles), vertexArray, newMemoryBlock, memoryIndex + newMemoryBlock * 2);
			AddVerticesForGeometry(std::get<3>(*surfaceSquare.m_SplitTriangles), vertexArray, newMemoryBlock, memoryIndex + newMemoryBlock * 3);
		}
	}

	void AddVerticesForLines(SI_SurfaceTriangle& surfaceTriangle, float* vertexArray, unsigned int memoryBlock, unsigned int memoryIndex)
	{
		//  vertexArray is size [m_LinePointCount * 3] because each point has three floats (x, y, z)
		//  memoryBlock is how many values we should be writing
		//  memoryIndex is where we begin to write, based on which surface we are

		if (surfaceTriangle.m_SplitTriangles == nullptr)
		{
			vertexArray[memoryIndex + 0] = std::get<0>(std::get<0>(surfaceTriangle.m_Triangle));	vertexArray[memoryIndex + 1] = std::get<1>(std::get<0>(surfaceTriangle.m_Triangle));	vertexArray[memoryIndex + 2] = std::get<2>(std::get<0>(surfaceTriangle.m_Triangle));
			vertexArray[memoryIndex + 3] = std::get<0>(std::get<1>(surfaceTriangle.m_Triangle));	vertexArray[memoryIndex + 4] = std::get<1>(std::get<1>(surfaceTriangle.m_Triangle));	vertexArray[memoryIndex + 5] = std::get<2>(std::get<1>(surfaceTriangle.m_Triangle));
			vertexArray[memoryIndex + 6] = std::get<0>(std::get<1>(surfaceTriangle.m_Triangle));	vertexArray[memoryIndex + 7] = std::get<1>(std::get<1>(surfaceTriangle.m_Triangle));	vertexArray[memoryIndex + 8] = std::get<2>(std::get<1>(surfaceTriangle.m_Triangle));
			vertexArray[memoryIndex + 9] = std::get<0>(std::get<2>(surfaceTriangle.m_Triangle));	vertexArray[memoryIndex + 10] = std::get<1>(std::get<2>(surfaceTriangle.m_Triangle));	vertexArray[memoryIndex + 11] = std::get<2>(std::get<2>(surfaceTriangle.m_Triangle));
			vertexArray[memoryIndex + 12] = std::get<0>(std::get<2>(surfaceTriangle.m_Triangle));	vertexArray[memoryIndex + 13] = std::get<1>(std::get<2>(surfaceTriangle.m_Triangle));	vertexArray[memoryIndex + 14] = std::get<2>(std::get<2>(surfaceTriangle.m_Triangle));
			vertexArray[memoryIndex + 15] = std::get<0>(std::get<0>(surfaceTriangle.m_Triangle));	vertexArray[memoryIndex + 16] = std::get<1>(std::get<0>(surfaceTriangle.m_Triangle));	vertexArray[memoryIndex + 17] = std::get<2>(std::get<0>(surfaceTriangle.m_Triangle));
		}
		else
		{
			auto newMemoryBlock = memoryBlock / 4;
			AddVerticesForLines(std::get<0>(*surfaceTriangle.m_SplitTriangles), vertexArray, newMemoryBlock, memoryIndex + newMemoryBlock * 0);
			AddVerticesForLines(std::get<1>(*surfaceTriangle.m_SplitTriangles), vertexArray, newMemoryBlock, memoryIndex + newMemoryBlock * 1);
			AddVerticesForLines(std::get<2>(*surfaceTriangle.m_SplitTriangles), vertexArray, newMemoryBlock, memoryIndex + newMemoryBlock * 2);
			AddVerticesForLines(std::get<3>(*surfaceTriangle.m_SplitTriangles), vertexArray, newMemoryBlock, memoryIndex + newMemoryBlock * 3);
		}
	}

	void Render(Vector3<float>& position, Camera& camera)
	{
		glPushMatrix();
			//  Move to the given position and rotate the object based on it's rotation speed and rotation start time
			glTranslatef(position.x, position.y, position.z);
			glRotatef(getRotation(), 0.0f, 1.0f, 0.0f);

			//  Set the base color (in case we aren't overriding with a shader program)
			glColor4f(m_SurfaceColor.x, m_SurfaceColor.y, m_SurfaceColor.z, 1.0f);

			//  Set up and activate any currently set shader program
			if (m_ShaderProgram != nullptr)
			{
				m_ShaderProgram->use();
				m_ShaderProgram->setUniform("camera", camera.matrix());
				m_ShaderProgram->setUniform("model", glm::rotate(glm::translate(glm::mat4(), glm::vec3(position.x, position.y, position.z)), glm::radians(getRotation()), glm::vec3(0, 1, 0)));
				m_ShaderProgram->setUniform("tex", 0);
			}

			//  Draw the geometry (set into the video card using VAO and VBO)
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glBindVertexArray(m_VAO[0]);
			glDrawArrays(GL_TRIANGLES, 0, m_PointCount);
			glBindVertexArray(0);

			if (m_ShaderProgram != nullptr) m_ShaderProgram->stopUsing();

			if (m_ShowLines)
			{
				glColor4f(m_LineColor.x, m_LineColor.y, m_LineColor.z, 0.2f);
				glLineWidth(2);
				glBindBuffer(GL_ARRAY_BUFFER, m_VBO[1]);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
				glDrawArrays(GL_LINES, 0, m_LinePointCount);
				glBindVertexArray(0);
			}
		glPopMatrix();
	}
};
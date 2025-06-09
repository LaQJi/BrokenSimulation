#pragma once

#include <iostream>
#include <cmath>

#include <vector>
#include <unordered_map>
#include <array>

namespace Geometry
{
	// ��ά����
	struct Vector3D
	{
		float x, y, z;

		Vector3D()
		{
			this->x = 0;
			this->y = 0;
			this->z = 0;
		}

		Vector3D(float x, float y, float z)
		{
			this->x = x;
			this->y = y;
			this->z = z;
		}
		// ���������
		Vector3D operator+(const Vector3D& other) const
		{
			return Vector3D(this->x + other.x, this->y + other.y, this->z + other.z);
		}

		Vector3D operator-(const Vector3D& other) const
		{
			return Vector3D(this->x - other.x, this->y - other.y, this->z - other.z);
		}

		Vector3D operator*(float scalar) const
		{
			return Vector3D(this->x * scalar, this->y * scalar, this->z * scalar);
		}

		bool operator<(const Vector3D& other) const
		{
			return (this->x < other.x) ||
				(this->x == other.x && this->y < other.y) ||
				(this->x == other.x && this->y == other.y && this->z < other.z);
		}

		bool operator==(const Vector3D& other) const
		{
			return this->x == other.x && this->y == other.y && this->z == other.z;
		}

		// ���
		float dot(const Vector3D& other) const
		{
			return this->x * other.x + this->y * other.y + this->z * other.z;
		}

		// ���
		Vector3D cross(const Vector3D& other) const
		{
			return Vector3D(
				this->y * other.z - this->z * other.y,
				this->z * other.x - this->x * other.z,
				this->x * other.y - this->y * other.x
			);
		}

		// ����ƽ��
		float lengthSquared() const
		{
			return this->dot(*this);
		}

		// ����
		float length() const
		{
			return std::sqrt(this->lengthSquared());
		}

		// ��λ��
		Vector3D normalize() const
		{
			float len = this->length();
			
			return len > 0 ? (*this) * (1.0f / len) : *this;
		}
	};

	// �߶�
	struct Edge
	{
		Vector3D start;
		Vector3D end;

		Edge(const Vector3D& start, const Vector3D& end)
		{
			this->start = start;
			this->end = end;
		}

		bool operator==(const Edge& other) const
		{
			return (this->start == other.start && this->end == other.end) ||
				(this->start == other.end && this->end == other.start);
		}
	};

	// ������
	struct Triangle
	{
		Vector3D vertices[3];

		Triangle(const Vector3D& v0, const Vector3D& v1, const Vector3D& v2)
		{
			this->vertices[0] = v0;
			this->vertices[1] = v1;
			this->vertices[2] = v2;
		}
	};

	// ������
	class Mesh
	{
	public:
		std::vector<Triangle> triangles;
		std::vector<Vector3D> vertices;

		void addTriangle(const Vector3D& v0, const Vector3D& v1, const Vector3D& v2);

		void calculateBounds(Vector3D& min, Vector3D& max) const;
	};

	// ģ��Ԥ������
	class ModelPreprocessor
	{
	public:
		// ����ģ��
		void loadModel(const std::string& filename);
		// ���߷��жϵ��Ƿ���ģ����
		bool isPointInsideModel(const Vector3D& point) const;

		// �ж������Ƿ����������ཻ  
		// rayOrigin: �������  
		// rayDirection: ���߷��򣨵�λ������  
		// triangle: ������  
		// intersectionDistance: �ཻ����  
		static bool rayIntersectsTriangle(const Vector3D& rayOrigin, const Vector3D& rayDirection,
			const Triangle& triangle, float& intersectionDistance);
		
		const Mesh& getModelMesh() const
		{
			return this->modelMesh;
		}

		const Vector3D& getBoundingBoxMin() const
		{
			return this->boundingBoxMin;
		}

		const Vector3D& getBoundingBoxMax() const
		{
			return this->boundingBoxMax;
		}

		const bool getClosed() const
		{
			return this->isClosed;
		}

	private:
		// ���ģ���Ƿ�պ�
		bool isModelClosed() const;
		// ��ȡģ�͵ı߽�
		void buildBoundingBox();

	private:
		Mesh modelMesh;
		Vector3D boundingBoxMin;
		Vector3D boundingBoxMax;
		bool isClosed;
	};

	// �����
	struct Sphere
	{
		Vector3D center; // ����
		float radius; // �뾶
		float radiusSquared; // �뾶ƽ��

		Sphere(const Vector3D& center = Vector3D(), float radius = 0)
			: center(center),
			radius(radius),
			radiusSquared(radius* radius){
		}
	};


	struct Tetrahedron
	{
		std::array<int, 4> vertices; // ��������
		std::array<int, 4> neighbors; // �ڽ�����������
		Sphere circumsphere; // �����
		bool isValid = true; // �Ƿ���Ч

		Tetrahedron(int v0, int v1, int v2, int v3)
		{
			vertices = { v0, v1, v2, v3 };
			neighbors.fill(-1); // ��ʼ���ڽ�����������Ϊ-1
		}

		std::array<int, 3> getFaceVertices(int faceIndex) const
		{
			switch (faceIndex)
			{
				case 0: return { vertices[1], vertices[2], vertices[3] };
				case 1: return { vertices[0], vertices[2], vertices[3] };
				case 2: return { vertices[0], vertices[1], vertices[3] };
				case 3: return { vertices[0], vertices[1], vertices[2] };
				default: throw std::out_of_range("Invalid face index");
			}
		}
	};


	class Delaunay3D
	{
	public:
		Delaunay3D(const std::vector<Vector3D>& points);

		// ����Delaunay�����ʷ�
		void triangulate();

		// ��ȡ��Ч��������
		std::vector<Tetrahedron> getValidTetrahedra() const;

	private:
		// ���ӵ�
		std::vector<Vector3D> points;
		// ������
		std::vector<Tetrahedron> tetrahedra;

		// �����������е�ĳ�������
		int createSuperTetrahedron();
		// �Ƴ��������������������
		void removeSuperTetrahedron(int superTetrahedronIndex);

		// ����µ㲢����������
		void insertPoint(const int pointIndex);

		// �ҵ��������������
		int findContainingTetrahedron(const Vector3D& point);

		bool isInfinity(int vertexIndex) const;

		Vector3D getVertex(int vertexIndex) const;
	};

	struct VoronoiCell
	{
		std::vector<Vector3D> vertices; // Voronoi��Ԫ�Ķ���
		std::vector<std::vector<int>> faces; // Voronoi��Ԫ����
		bool isOpen = false; // �Ƿ񿪷ţ����Ƿ��������棩
	};

	class Voronoi3D
	{
	public:
		Voronoi3D(const std::vector<Vector3D>& points);

		// ����Voronoiͼ
		void buildVoronoi(const Vector3D& center);
		// ��ȡVoronoi��Ԫ
		std::unordered_map<int, VoronoiCell> getVoronoiCells();

	private:
		std::vector<Vector3D> points; // ���ӵ�
		
		std::unordered_map<int, VoronoiCell> voronoiCells; // Voronoi��Ԫ

		Vector3D center; // ģ�����ĵ�

		// ����Voronoi��Ԫ
		void computeVoronoiCell(const std::vector<Tetrahedron>& tetrahedra);

		// ����Voronoi��Ԫ��͹��
		void computeVoronoiCellHull(VoronoiCell& cell);

		Vector3D computeFarPoint(const Vector3D& point);
	};

	struct AABB
	{
		// ��С��
		Vector3D min;
		// ����
		Vector3D max;
	};

	struct Point2D
	{
		float x, y;
	};


	// Voronoi��Ƭ��
	class VoronoiFracture
	{
	public:
		// ����Voronoi����
		static std::unordered_map<int, Mesh> generateVoronoiFracture(const ModelPreprocessor& inputMesh,
			const std::vector<Vector3D>& points, std::vector<Vector3D>& mappedPoints);

	private:
		// ��ȡ�����ڲ���һ����
		static Vector3D findPointInsideMesh(const ModelPreprocessor& mesh);

		// �����ӵ�ӳ�䵽������
		static Vector3D mapSeedToModel(const Vector3D& seed, const ModelPreprocessor& mesh);

		// �жϵ��Ƿ�����������
		static bool pointInTriangle(const Vector3D& point, const Triangle& triangle);

		// ��ȡ�����������ӵ�
		static int findClosedSeed(const Vector3D& point, const std::vector<Vector3D>& seeds);

		// ���������ε�����
		static Vector3D calculateCentroid(const Triangle& triangle);
	};

	// ��ȡ��������ڽ�������
	int getOppositeVertex(const Tetrahedron& tetrahedron, int faceIndex, const Tetrahedron& neighbor);

	// ����������������
	Sphere calculateCircumsphere(const Vector3D& a, const Vector3D& b, const Vector3D& c, const Vector3D& d);

	// �ж�������ķ���
	int orientation(const Vector3D& p, const Vector3D& q, const Vector3D& r, const Vector3D& s);

	// �жϵ��Ƿ����������
	bool circumsphereContainsPoint(const Tetrahedron& tet, const Vector3D& point);

	// ����3x3����ʽ
	float det3x3(const float a, const float b, const float c,
				const float d, const float e, const float f,
				const float g, const float h, const float i);

	// ����͹��
	std::vector<std::vector<int>> computeConvexHull(const std::vector<Vector3D>& points);

	// �ж��ĸ����Ƿ���
	bool arePointsCoplanar(const Vector3D& p1, const Vector3D& p2, const Vector3D& p3, const Vector3D& p4);

	// ���������εķ�����
	Vector3D calculateTriangleNormal(const Vector3D& p1, const Vector3D& p2, const Vector3D& p3);

	// ���������ΰ�Χ��
	AABB calculateTriangleAABB(const Triangle& triangle);
	// ����Voronoi��Ԫ�İ�Χ��
	AABB calculateVoronoiCellAABB(const VoronoiCell& cell);

	// �ж�AABB��Χ���Ƿ��ཻ
	bool aabbIntersect(const AABB& a, const AABB& b);

	// �жϵ��Ƿ���Voronoi��Ԫ��
	bool pointInVoronoiCell(const Vector3D& point, const VoronoiCell& cell);
	// �ж��������Ƿ���Voronoi��Ԫ�ཻ
	bool triangleIntersectVoronoiCell(const Triangle& triangle, const VoronoiCell& cell);

	// �ж��������Ƿ�����һ�������ཻ
	bool triangleIntersectTriangle(const Triangle& triangle, const Triangle& plane);

	// �жϱ��Ƿ��ཻ
	bool edgeIntersectEdge(const Edge& e1, const Edge& e2);
	// �жϵ��Ƿ�����������
	bool pointInTriangle(const Vector3D& point, const Triangle& triangle);

	// ���������
	bool satTest(const Triangle& triangle, const Triangle& plane);

	// ����Voronoi��Ԫ�����и����ཻ��������
	std::vector<Triangle> cutTrianglesByVoronoiCell(const Triangle& triangle, const VoronoiCell& cell, std::vector<int>& voronoiFaces);

	// �ж��߶����������Ƿ��ཻ
	bool lineIntersectTriangle(const Vector3D& lineStart, const Vector3D& lineEnd,
		const Triangle& triangle, Vector3D& intersectionPoint);

	// �����ά͹������
	std::vector<Point2D> calculateHull2D(const std::vector<Point2D>& points);
}

template<>
struct std::hash<Geometry::Vector3D>
{
	size_t operator()(const Geometry::Vector3D& v) const
	{
		size_t h1 = std::hash<float>()(v.x);
		size_t h2 = std::hash<float>()(v.y);
		size_t h3 = std::hash<float>()(v.z);

		// ��Ϲ�ϣֵ
		return h1 ^ (h2 << 1) ^ (h3 << 2);
	}
};

struct EdgeHash
{
	size_t operator()(const Geometry::Edge& e) const
	{
		size_t h1 = std::hash<Geometry::Vector3D>()(e.start);
		size_t h2 = std::hash<Geometry::Vector3D>()(e.end);
		// ��Ϲ�ϣֵ
		return h1 ^ h2;
	}
};

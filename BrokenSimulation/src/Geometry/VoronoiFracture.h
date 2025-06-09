#pragma once

#include <iostream>
#include <cmath>

#include <vector>
#include <unordered_map>
#include <array>

namespace Geometry
{
	// 三维向量
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
		// 重载运算符
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

		// 点乘
		float dot(const Vector3D& other) const
		{
			return this->x * other.x + this->y * other.y + this->z * other.z;
		}

		// 叉乘
		Vector3D cross(const Vector3D& other) const
		{
			return Vector3D(
				this->y * other.z - this->z * other.y,
				this->z * other.x - this->x * other.z,
				this->x * other.y - this->y * other.x
			);
		}

		// 长度平方
		float lengthSquared() const
		{
			return this->dot(*this);
		}

		// 长度
		float length() const
		{
			return std::sqrt(this->lengthSquared());
		}

		// 单位化
		Vector3D normalize() const
		{
			float len = this->length();
			
			return len > 0 ? (*this) * (1.0f / len) : *this;
		}
	};

	// 线段
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

	// 三角形
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

	// 网格类
	class Mesh
	{
	public:
		std::vector<Triangle> triangles;
		std::vector<Vector3D> vertices;

		void addTriangle(const Vector3D& v0, const Vector3D& v1, const Vector3D& v2);

		void calculateBounds(Vector3D& min, Vector3D& max) const;
	};

	// 模型预处理类
	class ModelPreprocessor
	{
	public:
		// 加载模型
		void loadModel(const std::string& filename);
		// 射线法判断点是否在模型内
		bool isPointInsideModel(const Vector3D& point) const;

		// 判断射线是否与三角形相交  
		// rayOrigin: 射线起点  
		// rayDirection: 射线方向（单位向量）  
		// triangle: 三角形  
		// intersectionDistance: 相交距离  
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
		// 检查模型是否闭合
		bool isModelClosed() const;
		// 获取模型的边界
		void buildBoundingBox();

	private:
		Mesh modelMesh;
		Vector3D boundingBoxMin;
		Vector3D boundingBoxMax;
		bool isClosed;
	};

	// 外接球
	struct Sphere
	{
		Vector3D center; // 球心
		float radius; // 半径
		float radiusSquared; // 半径平方

		Sphere(const Vector3D& center = Vector3D(), float radius = 0)
			: center(center),
			radius(radius),
			radiusSquared(radius* radius){
		}
	};


	struct Tetrahedron
	{
		std::array<int, 4> vertices; // 顶点索引
		std::array<int, 4> neighbors; // 邻接四面体索引
		Sphere circumsphere; // 外接球
		bool isValid = true; // 是否有效

		Tetrahedron(int v0, int v1, int v2, int v3)
		{
			vertices = { v0, v1, v2, v3 };
			neighbors.fill(-1); // 初始化邻接四面体索引为-1
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

		// 构建Delaunay三角剖分
		void triangulate();

		// 获取有效的四面体
		std::vector<Tetrahedron> getValidTetrahedra() const;

	private:
		// 种子点
		std::vector<Vector3D> points;
		// 四面体
		std::vector<Tetrahedron> tetrahedra;

		// 创建包含所有点的超四面体
		int createSuperTetrahedron();
		// 移除包含超四面体的四面体
		void removeSuperTetrahedron(int superTetrahedronIndex);

		// 添加新点并更新四面体
		void insertPoint(const int pointIndex);

		// 找到包含点的四面体
		int findContainingTetrahedron(const Vector3D& point);

		bool isInfinity(int vertexIndex) const;

		Vector3D getVertex(int vertexIndex) const;
	};

	struct VoronoiCell
	{
		std::vector<Vector3D> vertices; // Voronoi单元的顶点
		std::vector<std::vector<int>> faces; // Voronoi单元的面
		bool isOpen = false; // 是否开放（即是否有无限面）
	};

	class Voronoi3D
	{
	public:
		Voronoi3D(const std::vector<Vector3D>& points);

		// 构建Voronoi图
		void buildVoronoi(const Vector3D& center);
		// 获取Voronoi单元
		std::unordered_map<int, VoronoiCell> getVoronoiCells();

	private:
		std::vector<Vector3D> points; // 种子点
		
		std::unordered_map<int, VoronoiCell> voronoiCells; // Voronoi单元

		Vector3D center; // 模型中心点

		// 计算Voronoi单元
		void computeVoronoiCell(const std::vector<Tetrahedron>& tetrahedra);

		// 计算Voronoi单元的凸包
		void computeVoronoiCellHull(VoronoiCell& cell);

		Vector3D computeFarPoint(const Vector3D& point);
	};

	struct AABB
	{
		// 最小点
		Vector3D min;
		// 最大点
		Vector3D max;
	};

	struct Point2D
	{
		float x, y;
	};


	// Voronoi碎片化
	class VoronoiFracture
	{
	public:
		// 生成Voronoi裂纹
		static std::unordered_map<int, Mesh> generateVoronoiFracture(const ModelPreprocessor& inputMesh,
			const std::vector<Vector3D>& points, std::vector<Vector3D>& mappedPoints);

	private:
		// 获取网格内部的一个点
		static Vector3D findPointInsideMesh(const ModelPreprocessor& mesh);

		// 将种子点映射到网格上
		static Vector3D mapSeedToModel(const Vector3D& seed, const ModelPreprocessor& mesh);

		// 判断点是否在三角形内
		static bool pointInTriangle(const Vector3D& point, const Triangle& triangle);

		// 获取与点最近的种子点
		static int findClosedSeed(const Vector3D& point, const std::vector<Vector3D>& seeds);

		// 计算三角形的重心
		static Vector3D calculateCentroid(const Triangle& triangle);
	};

	// 获取四面体的邻接四面体
	int getOppositeVertex(const Tetrahedron& tetrahedron, int faceIndex, const Tetrahedron& neighbor);

	// 计算四面体的外接球
	Sphere calculateCircumsphere(const Vector3D& a, const Vector3D& b, const Vector3D& c, const Vector3D& d);

	// 判断四面体的方向
	int orientation(const Vector3D& p, const Vector3D& q, const Vector3D& r, const Vector3D& s);

	// 判断点是否在外接球内
	bool circumsphereContainsPoint(const Tetrahedron& tet, const Vector3D& point);

	// 计算3x3行列式
	float det3x3(const float a, const float b, const float c,
				const float d, const float e, const float f,
				const float g, const float h, const float i);

	// 计算凸包
	std::vector<std::vector<int>> computeConvexHull(const std::vector<Vector3D>& points);

	// 判断四个点是否共面
	bool arePointsCoplanar(const Vector3D& p1, const Vector3D& p2, const Vector3D& p3, const Vector3D& p4);

	// 计算三角形的法向量
	Vector3D calculateTriangleNormal(const Vector3D& p1, const Vector3D& p2, const Vector3D& p3);

	// 计算三角形包围盒
	AABB calculateTriangleAABB(const Triangle& triangle);
	// 计算Voronoi单元的包围盒
	AABB calculateVoronoiCellAABB(const VoronoiCell& cell);

	// 判断AABB包围盒是否相交
	bool aabbIntersect(const AABB& a, const AABB& b);

	// 判断点是否在Voronoi单元内
	bool pointInVoronoiCell(const Vector3D& point, const VoronoiCell& cell);
	// 判断三角面是否与Voronoi单元相交
	bool triangleIntersectVoronoiCell(const Triangle& triangle, const VoronoiCell& cell);

	// 判断三角面是否与另一三角面相交
	bool triangleIntersectTriangle(const Triangle& triangle, const Triangle& plane);

	// 判断边是否相交
	bool edgeIntersectEdge(const Edge& e1, const Edge& e2);
	// 判断点是否在三角形内
	bool pointInTriangle(const Vector3D& point, const Triangle& triangle);

	// 分离轴测试
	bool satTest(const Triangle& triangle, const Triangle& plane);

	// 根据Voronoi单元计算切割后的相交三角形面
	std::vector<Triangle> cutTrianglesByVoronoiCell(const Triangle& triangle, const VoronoiCell& cell, std::vector<int>& voronoiFaces);

	// 判断线段与三角形是否相交
	bool lineIntersectTriangle(const Vector3D& lineStart, const Vector3D& lineEnd,
		const Triangle& triangle, Vector3D& intersectionPoint);

	// 计算二维凸包顶点
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

		// 混合哈希值
		return h1 ^ (h2 << 1) ^ (h3 << 2);
	}
};

struct EdgeHash
{
	size_t operator()(const Geometry::Edge& e) const
	{
		size_t h1 = std::hash<Geometry::Vector3D>()(e.start);
		size_t h2 = std::hash<Geometry::Vector3D>()(e.end);
		// 混合哈希值
		return h1 ^ h2;
	}
};

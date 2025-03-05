#pragma once

#include <iostream>
#include <array>
#include <set>
#include <vector>
#include <tuple>
#include <map>
#include <algorithm>
#include <cmath>
#include <memory>
#include <cassert>

namespace Geometry
{
	// Forward declarations
	template <std::size_t N>
	class Point;

	template <std::size_t N>
	class Vector;

	template <std::size_t N>
	class Hyperplane;

	template <std::size_t N>
	class HyperplanePencil;

	template <std::size_t N>
	class Simplex;

	template <std::size_t N>
	class ConvexHull;


	// Point class
	template <std::size_t N>
	class Point
	{
	public:
		// 构造函数
		Point();
		Point(const std::array<double, N>& coordinates);

		template <std::size_t M>
		Point(const Point<M>& point);

		template <typename... Args>
		Point(Args... args);


		// 成员函数
		std::size_t getDimension() const;
		double getDistance(const Point<N>& point) const;


		// 重载运算符
		const double& operator[](std::size_t index) const;
		double& operator[](std::size_t index);
		Vector<N> operator-(const Point<N>& point) const;
		bool operator==(const Point<N>& point) const;
		bool operator!=(const Point<N>& point) const;
		bool operator<(const Point<N>& point) const;
		bool operator>(const Point<N>& point) const;
		bool operator<=(const Point<N>& point) const;
		bool operator>=(const Point<N>& point) const;
		friend std::ostream& operator<<(std::ostream& output, const Point<N>& point);

	private:
		std::array<double, N> coordinates;
	};


	// Vector class
	template <std::size_t N>
	class Vector
	{
	public:
		// 构造函数
		Vector();
		Vector(const Point<N>& point);
		Vector(const Vector<N>& vector);


		// 成员函数
		void normalize();
		std::size_t getDimension() const;
		double getMagnitude() const;
		Vector<N> getUnit() const;


		// 重载运算符
		const double& operator[](std::size_t index) const;
		double& operator[](std::size_t index);
		bool operator==(const Vector<N>& vector) const;
		bool operator!=(const Vector<N>& vector) const;
		Vector<N> operator+(const Vector<N>& vector) const;
		Vector<N> operator-(const Vector<N>& vector) const;
		Vector<N> operator*(double scalar) const;
		Vector<N> operator/(double scalar) const;
		double operator*(const Vector<N>& vector) const;
		Vector<N> operator^(const Vector<N>& vector) const;
		Vector<N> operator-() const;
		friend std::ostream& operator<<(std::ostream& output, const Vector<N>& vector);

	private:
		std::array<double, N> components;
	};


	// Hyperplane class
	template <std::size_t N>
	class Hyperplane
	{
	public:
		// 构造函数
		Hyperplane();
		Hyperplane(const std::array<Point<N>, N>& vertices);
		Hyperplane(const std::array<std::shared_ptr<Point<N>>, N>& vertices);
		Hyperplane(const Hyperplane<N>& hyperplane);


		// 成员函数
		std::size_t getDimension() const;
		Vector<N> getNormal() const;
		double getDistance(const Point<N>& point) const;
		void setNormalDirection(std::shared_ptr<Point<N>> pointBehind);
		bool isAbove(const Point<N>& point) const;
		bool isAdjacent(const Hyperplane<N>& hyperplane, std::size_t& index) const;
		void setNeighbor(std::shared_ptr<Hyperplane<N>> neighbor);
		void setNeighbor(std::size_t index, std::shared_ptr<Hyperplane<N>> neighbor);
		std::shared_ptr<Hyperplane<N>> getNeighbor(std::size_t index) const;
		std::array<std::shared_ptr<Hyperplane<N>>, N> getNeighbors() const;
		void removeNeighbor(std::shared_ptr<Hyperplane<N>> neighbor);
		bool addPointAbove(std::shared_ptr<Point<N>> point);
		std::vector<std::shared_ptr<Point<N>>> getPointsAbove() const;
		std::shared_ptr<Point<N>> getFurthestPointAbove() const;
		std::array<HyperplanePencil<N>, N> getPencils() const;
		std::size_t getVertexIndex(std::shared_ptr<Point<N>> vertex) const;


		// 重载运算符
		const std::shared_ptr<Point<N>>& operator[](std::size_t index) const;
		std::shared_ptr<Point<N>>& operator[](std::size_t index);
		bool operator==(const Hyperplane<N>& hyperplane) const;
		bool operator!=(const Hyperplane<N>& hyperplane) const;
		friend std::ostream& operator<<(std::ostream& output, const Hyperplane<N>& hyperplane);

	private:
		std::array<std::shared_ptr<Point<N>>, N> vertices;
		Vector<N> normal;
		std::array<std::shared_ptr<Hyperplane<N>>, N> neighbors;
		std::vector<std::shared_ptr<Point<N>>> pointsAbove;
	};


	// HyperplanePencil class
	template <std::size_t N>
	class HyperplanePencil
	{
	public:
		// 构造函数
		HyperplanePencil();
		HyperplanePencil(const std::array<std::shared_ptr<Point<N>>, N - 1>& vertices);
		HyperplanePencil(const HyperplanePencil<N>& hyperplanePencil);


		// 成员函数
		std::size_t getDimension() const;


		// 重载运算符
		const std::shared_ptr<Point<N>>& operator[](std::size_t index) const;
		std::shared_ptr<Point<N>>& operator[](std::size_t index);
		bool operator==(const HyperplanePencil<N>& hyperplanePencil) const;
		bool operator!=(const HyperplanePencil<N>& hyperplanePencil) const;
		bool operator<(const HyperplanePencil<N>& hyperplanePencil) const;
		friend std::ostream& operator<<(std::ostream& output, const HyperplanePencil<N>& hyperplanePencil);

	private:
		std::array<std::shared_ptr<Point<N>>, N - 1> vertices;
	};


	// Simplex class
	template <std::size_t N>
	class Simplex
	{
	public:
		// 构造函数
		Simplex();
		Simplex(const std::array<Point<N>, N + 1>& vertices);
		Simplex(const std::array<std::shared_ptr<Point<N>>, N + 1>& vertices);
		Simplex(const Simplex<N>& simplex);


		// 成员函数
		bool initializeFacets();
		std::size_t getDimension() const;
		std::shared_ptr<Point<N>> getCentroid() const;
		std::shared_ptr<Point<N>> getCircumcenter() const;
		std::shared_ptr<Hyperplane<N>> getFacet(std::size_t index) const;
		std::vector<std::shared_ptr<Hyperplane<N>>> getFacets() const;


		// 重载运算符
		const std::shared_ptr<Point<N>>& operator[](std::size_t index) const;
		std::shared_ptr<Point<N>>& operator[](std::size_t index);
		bool operator==(const Simplex<N>& simplex) const;
		bool operator!=(const Simplex<N>& simplex) const;
		friend std::ostream& operator<<(std::ostream& output, const Simplex<N>& simplex);

	private:
		std::array<std::shared_ptr<Point<N>>, N + 1> vertices;
		std::array<std::shared_ptr<Hyperplane<N>>, N + 1> facets;
	};


	// ConvexHull class
	template <std::size_t N>
	class ConvexHull
	{
	public:
		// 构造函数
		ConvexHull();
		ConvexHull(const std::vector<Point<N>>& points);
		ConvexHull(const std::vector<std::shared_ptr<Point<N>>>& points);
		ConvexHull(const ConvexHull<N>& convexHull);


		// 成员函数
		bool initialize();
		std::size_t getDimension() const;
		std::vector<std::shared_ptr<Hyperplane<N>>> getFacets() const;
		std::vector<std::shared_ptr<Point<N>>> getVertices() const;


		// 重载运算符
		bool operator==(const ConvexHull<N>& convexHull) const;
		bool operator!=(const ConvexHull<N>& convexHull) const;

	private:
		std::vector<std::shared_ptr<Point<N>>> vertices;
		std::vector<std::shared_ptr<Hyperplane<N>>> facets;
	};


	// Geometry functions
	template <std::size_t N>
	int calculateMatrixRank(std::vector<std::array<double, N>>& matrix);

	template <std::size_t N>
	bool isLinearlyIndependent(const std::vector<std::array<double, N>>& matrix, const Vector<N>& newVector);

	template <std::size_t N>
	Vector<N> calculateNormal(const std::array<std::shared_ptr<Point<N>>, N>& vertices);

	template <std::size_t N>
	std::shared_ptr<Point<N + 1>> projectOntoParaboloid(const std::shared_ptr<Point<N>>& point);
}

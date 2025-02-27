#pragma once

#include <iostream>
#include <set>
#include <vector>
#include <tuple>
#include <map>
#include <algorithm>
#include <cmath>
#include <memory>

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
	class Simplex;

	template <std::size_t N>
	class ConvexHull;


	// Point class
	template <std::size_t N>
	class Point
	{
		static_assert(N > 0, "Point dimension must be greater than 0");
		static_assert(N < 4, "Point dimension must be less than 4");

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
		static_assert(N > 0, "Vector dimension must be greater than 0");
		static_assert(N < 4, "Vector dimension must be less than 4");

	public:
		// 构造函数
		Vector();
		Vector(const Point<N>& point);
		Vector(const Vector<N>& vector);


		// 成员函数
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
		friend std::ostream& operator<<(std::ostream& output, const Vector<N>& vector);

	private:
		std::array<double, N> components;
	};


	// Hyperplane class
	template <std::size_t N>
	class Hyperplane
	{
		static_assert(N > 1, "Hyperplane dimension must be greater than 1");
		static_assert(N < 4, "Hyperplane dimension must be less than 4");

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

		//todo：反转方向、添加邻接等功能；simplex类构造函数初始化面的时候，需要考虑方向、且需计算hyperplane信息，包括邻接；hull扩展等功能；公共函数需初始化simplex


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
	};


	// Simplex class
	template <std::size_t N>
	class Simplex
	{
		static_assert(N > 0, "Simplex dimension must be greater than 0");
		static_assert(N < 4, "Simplex dimension must be less than 4");

	public:
		// 构造函数
		Simplex();
		Simplex(const std::array<Point<N>, N + 1>& vertices);
		Simplex(const std::array<std::shared_ptr<Point<N>>, N + 1>& vertices);
		Simplex(const Simplex<N>& simplex);


		// 成员函数
		std::size_t getDimension() const;
		std::shared_ptr<Point<N>> getCentroid() const;
		std::shared_ptr<Point<N>> getCircumcenter() const;
		std::vector<std::shared_ptr<Hyperplane<N>>> getFacets() const;


		// 重载运算符
		const std::shared_ptr<Point<N>>& operator[](std::size_t index) const;
		std::shared_ptr<Point<N>>& operator[](std::size_t index);
		bool operator==(const Simplex<N>& simplex) const;
		bool operator!=(const Simplex<N>& simplex) const;
		friend std::ostream& operator<<(std::ostream& output, const Simplex<N>& simplex);

	private:
		std::array<std::shared_ptr<Point<N>>, N + 1> vertices;
	};


	// ConvexHull class
	template <std::size_t N>
	class ConvexHull
	{
		static_assert(N > 0, "ConvexHull dimension must be greater than 0");
		static_assert(N < 4, "ConvexHull dimension must be less than 4");

	public:
		// 构造函数
		ConvexHull();
		ConvexHull(const std::vector<Point<N>>& points);
		ConvexHull(const ConvexHull<N>& convexHull);


		// 成员函数
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
	int calculateMatrixRank(std::array<std::array<double, N>, N>& matrix);

	template <std::size_t N>
	Vector<N> calculateNormal(const std::array<std::shared_ptr<Point<N>>, N>& vertices);
}
#include "Geometry.h"

namespace Geometry
{
	// Point class
	template <std::size_t N>
	Point<N>::Point()
	{
		this->coordinates.fill(0.0);
	}

	template <std::size_t N>
	Point<N>::Point(const std::array<double, N>& coordinates)
		: coordinates(coordinates)
	{
	}

	template <std::size_t N>
	template <std::size_t M>
	Point<N>::Point(const Point<M>& point)
	{
		if (N <= M)
		{
			for (std::size_t i = 0; i < N; i++)
			{
				this->coordinates[i] = point[i];
			}
		}
		else
		{
			for (std::size_t i = 0; i < M; i++)
			{
				this->coordinates[i] = point[i];
				projection += point[i] * point[i];
			}
			for (std::size_t i = M; i < N; i++)
			{
				this->coordinates[i] = 0.0;
			}
		}
	}

	template <std::size_t N>
	template <typename... Args>
	Point<N>::Point(Args... args)
	{
		if (sizeof...(args) < N)
		{
			std::size_t i = 0;
			for (auto arg : { args... })
			{
				this->coordinates[i] = arg;
				i++;
			}
			for (std::size_t j = i; j < N; j++)
			{
				this->coordinates[j] = 0.0;
			}
		}
		else
		{
			std::size_t i = 0;
			for (auto arg : { args... })
			{
				this->coordinates[i] = arg;
				i++;
				if (i == N)
				{
					break;
				}
			}
		}
	}

	template <std::size_t N>
	std::size_t Point<N>::getDimension() const
	{
		return N;
	}

	template <std::size_t N>
	double Point<N>::getDistance(const Point<N>& point) const
	{
		double distance = 0.0;
		for (std::size_t i = 0; i < N; i++)
		{
			distance += (this->coordinates[i] - point[i]) * (this->coordinates[i] - point[i]);
		}
		return std::sqrt(distance);
	}

	template <std::size_t N>
	const double& Point<N>::operator[](std::size_t index) const
	{
		static_assert(index < N, "Point index must be less than N");
		return this->coordinates[index];
	}

	template <std::size_t N>
	double& Point<N>::operator[](std::size_t index)
	{
		static_assert(index < N, "Point index must be less than N");
		return this->coordinates[index];
	}

	template <std::size_t N>
	bool Point<N>::operator==(const Point<N>& point) const
	{
		for (std::size_t i = 0; i < N; i++)
		{
			if (this->coordinates[i] != point[i])
			{
				return false;
			}
		}
		return true;
	}

	template <std::size_t N>
	bool Point<N>::operator!=(const Point<N>& point) const
	{
		return !(*this == point);
	}

	template <std::size_t N>
	bool Point<N>::operator<(const Point<N>& point) const
	{
		for (std::size_t i = 0; i < N; i++)
		{
			if (this->coordinates[i] < point[i])
			{
				return true;
			}
			else if (this->coordinates[i] > point[i])
			{
				return false;
			}
		}
		return false;
	}

	template <std::size_t N>
	bool Point<N>::operator>(const Point<N>& point) const
	{
		for (std::size_t i = 0; i < N; i++)
		{
			if (this->coordinates[i] > point[i])
			{
				return true;
			}
			else if (this->coordinates[i] < point[i])
			{
				return false;
			}
		}
		return false;
	}

	template <std::size_t N>
	bool Point<N>::operator<=(const Point<N>& point) const
	{
		return !(*this > point);
	}

	template <std::size_t N>
	bool Point<N>::operator>=(const Point<N>& point) const
	{
		return !(*this < point);
	}

	template <std::size_t N>
	std::ostream& operator<<(std::ostream& output, const Point<N>& point)
	{
		if (!point)
		{
			output << "Point is not initialized";
		}
		else
		{
			output << "(";
			for (std::size_t i = 0; i < N - 1; i++)
			{
				output << point[i] << ", ";
			}
			output << point[N - 1] << ")";
		}
		return output;
	}

	// Vector class
	template <std::size_t N>
	Vector<N>::Vector()
	{
		this->components.fill(0.0);
	}

	template <std::size_t N>
	Vector<N>::Vector(const Point<N>& point)
	{
		for (std::size_t i = 0; i < N; i++)
		{
			this->components[i] = point[i];
		}
	}

	template <std::size_t N>
	Vector<N>::Vector(const Vector<N>& vector)
	{
		this->components = vector.components;
	}

	template <std::size_t N>
	std::size_t Vector<N>::getDimension() const
	{
		return N;
	}

	template <std::size_t N>
	double Vector<N>::getMagnitude() const
	{
		double magnitude = 0.0;
		for (std::size_t i = 0; i < N; i++)
		{
			magnitude += this->components[i] * this->components[i];
		}
		return std::sqrt(magnitude);
	}

	template <std::size_t N>
	Vector<N> Vector<N>::getUnit() const
	{
		double magnitude = this->getMagnitude();
		if (magnitude == 0.0)
		{
			return Vector<N>();
		}
		Vector<N> unit;
		for (std::size_t i = 0; i < N; i++)
		{
			unit[i] = this->components[i] / magnitude;
		}
		return unit;
	}

	template<std::size_t N>
	const double& Vector<N>::operator[](std::size_t index) const
	{
		static_assert(index < N, "Vector index must be less than N");
		return this->components[index];
	}

	template<std::size_t N>
	double& Vector<N>::operator[](std::size_t index)
	{
		static_assert(index < N, "Vector index must be less than N");
		return this->components[index];
	}

	template<std::size_t N>
	bool Vector<N>::operator==(const Vector<N>& vector) const
	{
		for (std::size_t i = 0; i < N; i++)
		{
			if (this->components[i] != vector[i])
			{
				return false;
			}
		}
		return true;
	}

	template<std::size_t N>
	bool Vector<N>::operator!=(const Vector<N>& vector) const
	{
		return !(*this == vector);
	}

	template<std::size_t N>
	Vector<N> Vector<N>::operator+(const Vector<N>& vector) const
	{
		Vector<N> sum;
		for (std::size_t i = 0; i < N; i++)
		{
			sum[i] = this->components[i] + vector[i];
		}
		return sum;
	}

	template<std::size_t N>
	Vector<N> Vector<N>::operator-(const Vector<N>& vector) const
	{
		Vector<N> difference;
		for (std::size_t i = 0; i < N; i++)
		{
			difference[i] = this->components[i] - vector[i];
		}
		return difference;
	}

	template<std::size_t N>
	Vector<N> Vector<N>::operator*(double scalar) const
	{
		Vector<N> product;
		for (std::size_t i = 0; i < N; i++)
		{
			product[i] = this->components[i] * scalar;
		}
		return product;
	}

	template<std::size_t N>
	Vector<N> Vector<N>::operator/(double scalar) const
	{
		Vector<N> quotient;
		for (std::size_t i = 0; i < N; i++)
		{
			quotient[i] = this->components[i] / scalar;
		}
		return quotient;
	}

	template<std::size_t N>
	double Vector<N>::operator*(const Vector<N>& vector) const
	{
		double dotProduct = 0.0;
		for (std::size_t i = 0; i < N; i++)
		{
			dotProduct += this->components[i] * vector[i];
		}
		return dotProduct;
	}

	template<std::size_t N>
	Vector<N> Vector<N>::operator^(const Vector<N>& vector) const
	{
		static_assert(N == 3, "Cross product is only defined for 3D vectors");

		Vector<N> crossProduct;

		crossProduct[0] = this->components[1] * vector[2] - this->components[2] * vector[1];
		crossProduct[1] = this->components[2] * vector[0] - this->components[0] * vector[2];
		crossProduct[2] = this->components[0] * vector[1] - this->components[1] * vector[0];

		return crossProduct;
	}

	template<std::size_t N>
	std::ostream& operator<<(std::ostream& output, const Vector<N>& vector)
	{
		if (!vector)
		{
			output << "Vector is not initialized";
		}
		else
		{
			output << "(";
			for (std::size_t i = 0; i < N - 1; i++)
			{
				output << vector[i] << ", ";
			}
			output << vector[N - 1] << ")";
		}
		return output;
	}

	// Hyperplane class
	template <std::size_t N>
	Hyperplane<N>::Hyperplane()
	{
		this->vertices.fill(nullptr);
		this->normal = Vector<N>();
		this->neighbors.fill(nullptr);
	}

	template <std::size_t N>
	Hyperplane<N>::Hyperplane(const std::array<Point<N>, N>& vertices)
	{
		Vector<N> normal = calculateNormal(vertices);
		if (normal == Vector<N>())
		{
			this->vertices.fill(nullptr);
			this->normal = Vector<N>();
			this->neighbors.fill(nullptr);
		}
		else
		{
			std::sort(vertices.begin(), vertices.end());
			for (std::size_t i = 0; i < N; i++)
			{
				this->vertices[i] = std::make_shared<Point<N>>(vertices[i]);
			}
			this->normal = normal;
			this->neighbors.fill(nullptr);
		}
	}

	template <std::size_t N>
	Hyperplane<N>::Hyperplane(const std::array<std::shared_ptr<Point<N>>, N>& vertices)
		: vertices(vertices)
	{
		Vector<N> normal = calculateNormal(vertices);
		if (normal == Vector<N>())
		{
			this->vertices.fill(nullptr);
			this->normal = Vector<N>();
			this->neighbors.fill(nullptr);
		}
		else
		{
			std::sort(this->vertices.begin(), this->vertices.end());
			this->normal = normal;
			this->neighbors.fill(nullptr);
		}
	}

	template <std::size_t N>
	Hyperplane<N>::Hyperplane(const Hyperplane<N>& hyperplane)
		: vertices(hyperplane.vertices),
		normal(hyperplane.normal),
		neighbors(hyperplane.neighbors)
	{
	}

	template <std::size_t N>
	std::size_t Hyperplane<N>::getDimension() const
	{
		return N;
	}

	template <std::size_t N>
	Vector<N> Hyperplane<N>::getNormal() const
	{
		return normal;
	}

	template <std::size_t N>
	double Hyperplane<N>::getDistance(const Point<N>& point) const
	{
		double distance = 0.0;
		for (std::size_t i = 0; i < N; i++)
		{
			distance += normal[i] * (point[i] - (*this->vertices[0])[i]);
		}
		return distance / normal.getMagnitude();
	}

	template <std::size_t N>
	const std::shared_ptr<Point<N>>& Hyperplane<N>::operator[](std::size_t index) const
	{
		static_assert(index < N, "Hyperplane index must be less than N");
		return this->vertices[index];
	}

	template <std::size_t N>
	std::shared_ptr<Point<N>>& Hyperplane<N>::operator[](std::size_t index)
	{
		static_assert(index < N, "Hyperplane index must be less than N");
		return this->vertices[index];
	}

	template <std::size_t N>
	bool Hyperplane<N>::operator==(const Hyperplane<N>& hyperplane) const
	{
		for (std::size_t i = 0; i < N; i++)
		{
			if (this->vertices[i] != hyperplane[i])
			{
				return false;
			}
		}
		return true;
	}

	template <std::size_t N>
	bool Hyperplane<N>::operator!=(const Hyperplane<N>& hyperplane) const
	{
		return !(*this == hyperplane);
	}

	template <std::size_t N>
	std::ostream& operator<<(std::ostream& output, const Hyperplane<N>& hyperplane)
	{
		if (!hyperplane)
		{
			output << "Hyperplane is not initialized";
		}
		else
		{
			output << "{";
			for (std::size_t i = 0; i < N - 1; i++)
			{
				output << *hyperplane[i] << ", ";
			}
			output << *hyperplane[N - 1] << "}";
		}
		return output;
	}

	// Simplex class
	template <std::size_t N>
	Simplex<N>::Simplex()
	{
		this->vertices.fill(Point<N>());
	}

	template <std::size_t N>
	Simplex<N>::Simplex(const std::array<Point<N>, N + 1>& vertices)
	{
		std::sort(vertices.begin(), vertices.end());
		for (std::size_t i = 0; i < N + 1; i++)
		{
			this->vertices[i] = std::make_shared<Point<N>>(vertices[i]);
		}
	}

	template <std::size_t N>
	Simplex<N>::Simplex(const std::array<std::shared_ptr<Point<N>>, N + 1>& vertices)
		: vertices(vertices)
	{
		std::sort(this->vertices.begin(), this->vertices.end());
	}

	template <std::size_t N>
	Simplex<N>::Simplex(const Simplex<N>& simplex)
	{
		this->vertices = simplex.vertices;
		std::sort(this->vertices.begin(), this->vertices.end());
	}

	template<std::size_t N>
	std::size_t Simplex<N>::getDimension() const
	{
		return N;
	}

	template<std::size_t N>
	std::shared_ptr<Point<N>> Simplex<N>::getCentroid() const
	{
		Point<N> centroid;
		for (std::size_t i = 0; i < N + 1; i++)
		{
			for (std::size_t j = 0; j < N; j++)
			{
				centroid[j] += (*vertices[i])[j];
			}
		}
		for (std::size_t i = 0; i < N; i++)
		{
			centroid[i] /= (N + 1);
		}
		return centroid;
	}

	template<std::size_t N>
	std::shared_ptr<Point<N>> Simplex<N>::getCircumcenter() const
	{
		Point<N> circumcenter;
		std::array<std::array<double, N>, N> coefficients;
		for (std::size_t i = 0; i < N; i++)
		{
			for (std::size_t j = 0; j < N; j++)
			{
				coefficients[i][j] = 2.0 * ((*vertices[j])[i] - (*vertices[0])[i]);
			}
		}
		std::array<double, N> constants;
		for (std::size_t i = 0; i < N; i++)
		{
			constants[i] = 0.0;
			for (std::size_t j = 0; j < N; j++)
			{
				constants[i] += (*vertices[j])[i] * (*vertices[j])[i] - (*vertices[0])[i] * (*vertices[0])[i];
			}
		}
		for (std::size_t i = 1; i < N; i++)
		{
			circumcenter[i] = constants[i];
			for (std::size_t j = 1; j < i; j++)
			{
				circumcenter[i] -= coefficients[i][j] * circumcenter[j];
			}
			circumcenter[i] /= coefficients[i][i];
		}
		return circumcenter;
	}

	template<std::size_t N>
	std::vector<std::shared_ptr<Hyperplane<N>>> Simplex<N>::getFacets() const
	{
		std::vector<std::shared_ptr<Hyperplane<N>>> facets;
		for (std::size_t i = 0; i < N + 1; i++)
		{
			std::array<Point<N>, N> facetVertices;
			for (std::size_t j = 0; j < N; j++)
			{
				std::size_t index = 0;
				for (std::size_t k = 0; k < N + 1; k++)
				{
					if (k != i)
					{
						facetVertices[index] = vertices[k];
						index++;
					}
				}
			}
			facets.push_back(std::make_shared<Hyperplane<N>>(facetVertices));
		}
		return facets;
	}

	template<std::size_t N>
	const std::shared_ptr<Point<N>>& Simplex<N>::operator[](std::size_t index) const
	{
		static_assert(index < N + 1, "Simplex index must be less than N + 1");
		return this->vertices[index];
	}

	template<std::size_t N>
	std::shared_ptr<Point<N>>& Simplex<N>::operator[](std::size_t index)
	{
		static_assert(index < N + 1, "Simplex index must be less than N + 1");
		return this->vertices[index];
	}

	template<std::size_t N>
	bool Simplex<N>::operator==(const Simplex<N>& simplex) const
	{
		for (std::size_t i = 0; i < N + 1; i++)
		{
			if (this->vertices[i] != simplex[i])
			{
				return false;
			}
		}
		return true;
	}

	template<std::size_t N>
	bool Simplex<N>::operator!=(const Simplex<N>& simplex) const
	{
		return !(*this == simplex);
	}

	template<std::size_t N>
	std::ostream& operator<<(std::ostream& output, const Simplex<N>& simplex)
	{
		if (!simplex)
		{
			output << "Simplex is not initialized";
		}
		else
		{
			output << "{";
			for (std::size_t i = 0; i < N; i++)
			{
				output << *simplex[i] << ", ";
			}
			output << *simplex[N] << "}";
		}
		return output;
	}

	// ConvexHull class
	template <std::size_t N>
	ConvexHull<N>::ConvexHull()
	{
		this->vertices.fill(nullptr);
		this->facets.fill(nullptr);
	}

	template <std::size_t N>
	ConvexHull<N>::ConvexHull(const std::vector<Point<N>>& points)
	{
		std::vector<std::shared_ptr<Point<N>>> vertices;
		for (const Point<N>& point : points)
		{
			vertices.push_back(std::make_shared<Point<N>>(point));
		}
		this->vertices = vertices;
		this->facets.fill(nullptr);
	}

	template<std::size_t N>
	ConvexHull<N>::ConvexHull(const ConvexHull<N>& convexHull)
		: vertices(convexHull.vertices),
		facets(convexHull.facets)
	{
	}

	template<std::size_t N>
	std::size_t ConvexHull<N>::getDimension() const
	{
		return N;
	}

	template<std::size_t N>
	std::vector<std::shared_ptr<Hyperplane<N>>> ConvexHull<N>::getFacets() const
	{
		return this->facets;
	}

	template<std::size_t N>
	std::vector<std::shared_ptr<Point<N>>> ConvexHull<N>::getVertices() const
	{
		return this->vertices;
	}

	template<std::size_t N>
	bool ConvexHull<N>::operator==(const ConvexHull<N>& convexHull) const
	{
		for (std::size_t i = 0; i < this->vertices.size(); i++)
		{
			if (this->vertices[i] != convexHull.vertices[i])
			{
				return false;
			}
		}
		return true;
	}

	template<std::size_t N>
	bool ConvexHull<N>::operator!=(const ConvexHull<N>& convexHull) const
	{
		return !(*this == convexHull);
	}

	template<std::size_t N>
	std::ostream& operator<<(std::ostream& output, const ConvexHull<N>& convexHull)
	{
		if (!convexHull)
		{
			output << "ConvexHull is not initialized";
		}
		else
		{
			output << "{";
			for (std::size_t i = 0; i < convexHull.vertices.size() - 1; i++)
			{
				output << *convexHull.vertices[i] << ", ";
			}
			output << *convexHull.vertices[convexHull.vertices.size() - 1] << "}";
		}
		return output;
	}

	// Geometry functions
	template <std::size_t N>
	int calculateMatrixRank(std::array<std::array<double, N>, N>& matrix) 
	{
		int rank = 0;
		for (std::size_t i = 0; i < N; i++)
		{
			if (matrix[i][i] == 0)
			{
				bool swapped = false;
				for (std::size_t j = i + 1; j < N; j++)
				{
					if (matrix[j][i] != 0) 
					{
						std::swap(matrix[i], matrix[j]);
						swapped = true;
						break;
					}
				}
				if (!swapped) 
				{
					continue;
				}
			}
			for (std::size_t j = i + 1; j < N; j++) 
			{
				double factor = matrix[j][i] / matrix[i][i];
				for (std::size_t k = i; k < N; k++) 
				{
					matrix[j][k] -= factor * matrix[i][k];
				}
			}
		}
		for (std::size_t i = 0; i < N; i++) 
		{
			if (matrix[i][i] != 0.0) 
			{
				rank++;
			}
		}
		return rank;
	}

	template<std::size_t N>
	Vector<N> calculateNormal(const std::array<std::shared_ptr<Point<N>>, N>& vertices)
	{
		std::array<std::array<double, N>, N> matrix;
		for (std::size_t i = 0; i < N; i++)
		{
			for (std::size_t j = 0; j < N; j++)
			{
				matrix[i][j] = (*vertices[i])[j];
			}
		}
		if (calculateMatrixRank<N>(matrix) < N)
		{
			return Vector<N>();
		}
		Vector<N> normal;
		for (std::size_t i = 0; i < N; i++)
		{
			normal[i] = matrix[0][i];
		}
		return normal;
	}

}

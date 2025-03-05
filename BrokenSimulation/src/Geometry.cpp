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
		assert(index < N && "Point index must be less than N");
		return this->coordinates[index];
	}

	template <std::size_t N>
	double& Point<N>::operator[](std::size_t index)
	{
		assert(index < N && "Point index must be less than N");
		return this->coordinates[index];
	}

	template<std::size_t N>
	Vector<N> Point<N>::operator-(const Point<N>& point) const
	{
		Vector<N> vector;
		for (std::size_t i = 0; i < N; i++)
		{
			vector[i] = this->coordinates[i] - point[i];
		}
		return vector;
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

	// ��ʽʵ����
	template class Point<2>;
	template class Point<3>;
	template class Point<4>;

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

	template<std::size_t N>
	void Vector<N>::normalize()
	{
		double magnitude = this->getMagnitude();
		if (magnitude != 0.0)
		{
			for (std::size_t i = 0; i < N; i++)
			{
				this->components[i] /= magnitude;
			}
		}
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
		assert(index < N && "Vector index must be less than N");
		return this->components[index];
	}

	template<std::size_t N>
	double& Vector<N>::operator[](std::size_t index)
	{
		assert(index < N && "Vector index must be less than N");
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
		if (N == 3)
		{
			Vector<N> crossProduct;

			crossProduct[0] = this->components[1] * vector[2] - this->components[2] * vector[1];
			crossProduct[1] = this->components[2] * vector[0] - this->components[0] * vector[2];
			crossProduct[2] = this->components[0] * vector[1] - this->components[1] * vector[0];

			return crossProduct;
		}
		else
		{
			throw std::invalid_argument("Cross product is only defined for 3D vectors");
		}
    }

	template<std::size_t N>
	Vector<N> Vector<N>::operator-() const
	{
		return *this * -1.0;
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

	// ��ʽʵ����
	template class Vector<2>;
	template class Vector<3>;
	template class Vector<4>;

	// Hyperplane class
	template <std::size_t N>
	Hyperplane<N>::Hyperplane()
	{
		this->vertices.fill(nullptr);
		this->normal = Vector<N>();
		this->neighbors.fill(nullptr);
		this->pointsAbove.clear();
	}

	template <std::size_t N>
	Hyperplane<N>::Hyperplane(const std::array<Point<N>, N>& vertices)
	{
		std::array<std::shared_ptr<Point<N>>, N> sharedVertices;
		for (std::size_t i = 0; i < N; i++)
		{
			sharedVertices[i] = std::make_shared<Point<N>>(vertices[i]);
		}

		Vector<N> normal = calculateNormal(sharedVertices);
		if (normal == Vector<N>())
		{
			this->vertices.fill(nullptr);
			this->normal = Vector<N>();
			this->neighbors.fill(nullptr);
		}
		else
		{
			std::sort(sharedVertices.begin(), sharedVertices.end(), [](const std::shared_ptr<Point<N>>& point1, const std::shared_ptr<Point<N>>& point2)
				{
					return *point1 < *point2;
				});
			this->vertices = sharedVertices;
			this->normal = normal;
			this->neighbors.fill(nullptr);
		}

		this->pointsAbove.clear();
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
			std::sort(this->vertices.begin(), this->vertices.end(), [](const std::shared_ptr<Point<N>>& point1, const std::shared_ptr<Point<N>>& point2)
				{
					return *point1 < *point2;
				});
			this->normal = normal;
			this->neighbors.fill(nullptr);
		}
		this->pointsAbove.clear();
	}

	template <std::size_t N>
	Hyperplane<N>::Hyperplane(const Hyperplane<N>& hyperplane)
		: vertices(hyperplane.vertices),
		normal(hyperplane.normal),
		neighbors(hyperplane.neighbors),
		pointsAbove(hyperplane.pointsAbove)
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
		return normal * (point - *this->vertices[0]);
	}

	template<std::size_t N>
	void Hyperplane<N>::setNormalDirection(std::shared_ptr<Point<N>> pointBehind)
	{
		Vector<N> direction;
		for (std::size_t i = 0; i < N; i++)
		{
			direction[i] = (*pointBehind)[i] - (*this->vertices[0])[i];
		}
		if (normal* direction > 0.0)
		{
			normal = -normal;
		}
	}

	template<std::size_t N>
	bool Hyperplane<N>::isAbove(const Point<N>& point) const
	{
		return this->normal * (point - *this->vertices[0]) > 0.0;
	}

	template<std::size_t N>
	bool Hyperplane<N>::isAdjacent(const Hyperplane<N>& hyperplane, std::size_t& index) const
	{
		int count = 0;
		index = 0;
		for (std::size_t i = 0; i < N; i++)
		{
			bool flag = false;
			for (std::size_t j = 0; j < N; j++)
			{
				if (this->vertices[i] == hyperplane.vertices[j])
				{
					count++;
					flag = true;
					break;
				}
			}
			// ���hyperplane�в�����vertices[i]����index����Ϊi
			if (!flag)
			{
				index = i;
			}
		}
		return count == N - 1;
	}

	template<std::size_t N>
	void Hyperplane<N>::setNeighbor(std::shared_ptr<Hyperplane<N>> neighbor)
	{
		std::size_t index = 0;
		// �ж��Ƿ�Ϊ�ڽ�hyperplane
		if (isAdjacent(*neighbor, index))
		{
			this->neighbors[index] = neighbor;
		}
		else
		{
			throw std::invalid_argument("Hyperplanes are not adjacent");
		}
	}

	template<std::size_t N>
	void Hyperplane<N>::setNeighbor(std::size_t index, std::shared_ptr<Hyperplane<N>> neighbor)
	{
		assert(index < N && "Hyperplane neighbor index must be less than N");
		// tempΪneighbor�в������ĵ�
		std::shared_ptr<Point<N>> temp = this->vertices[index];
		// �ж�neighbor���Ƿ����temp
		auto iter = std::find(neighbor->vertices.begin(), neighbor->vertices.end(), temp);
		// ���neighbor�в�����temp����neighbor����Ϊ�ڽ�hyperplane
		if (iter == neighbor->vertices.end())
		{
			this->neighbors[index] = neighbor;
		}
		else
		{
			throw std::invalid_argument("Hyperplanes are not adjacent");
		}
	}

	template<std::size_t N>
	std::shared_ptr<Hyperplane<N>> Hyperplane<N>::getNeighbor(std::size_t index) const
	{
		return this->neighbors[index];
	}

	template<std::size_t N>
	std::array<std::shared_ptr<Hyperplane<N>>, N> Hyperplane<N>::getNeighbors() const
	{
		return this->neighbors;
	}

	template<std::size_t N>
	void Hyperplane<N>::removeNeighbor(std::shared_ptr<Hyperplane<N>> neighbor)
	{
		auto iter = std::find(this->neighbors.begin(), this->neighbors.end(), neighbor);
		if (iter != this->neighbors.end())
		{
			*iter = nullptr;
		}
	}

	template<std::size_t N>
	bool Hyperplane<N>::addPointAbove(std::shared_ptr<Point<N>> point)
	{
		if (point != nullptr)
		{
			Vector<N> vector = *point - *this->vertices[0];
			// ������ڳ�ƽ���Ϸ����򽫵����pointsAbove
			if (normal * vector > 0.0)
			{
				this->pointsAbove.push_back(point);
				return true;
			}
			else
			{
				return false;
			}
		}
	}

	template<std::size_t N>
	std::vector<std::shared_ptr<Point<N>>> Hyperplane<N>::getPointsAbove() const
	{
		return this->pointsAbove;
	}

	template<std::size_t N>
	std::shared_ptr<Point<N>> Hyperplane<N>::getFurthestPointAbove() const
	{
		std::shared_ptr<Point<N>> furthestPoint = nullptr;
		double maxDistance = 0.0;
		for (auto point : this->pointsAbove)
		{
			double distance = getDistance(*point);
			if (distance > maxDistance)
			{
				maxDistance = distance;
				furthestPoint = point;
			}
		}
		return furthestPoint;
	}

	template<std::size_t N>
	std::array<HyperplanePencil<N>, N> Hyperplane<N>::getPencils() const
	{
		std::array<HyperplanePencil<N>, N> pencils;
		for (std::size_t i = 0; i < N; i++)
		{
			std::array<std::shared_ptr<Point<N>>, N - 1> vertices;
			std::size_t index = 0;
			for (std::size_t j = 0; j < N; j++)
			{
				if (j != i)
				{
					vertices[index] = this->vertices[j];
					index++;
				}
			}
			pencils[i] = HyperplanePencil<N>(vertices);
		}
		return pencils;
	}

	template<std::size_t N>
	std::size_t Hyperplane<N>::getVertexIndex(std::shared_ptr<Point<N>> vertex) const
	{
		auto iter = std::find(this->vertices.begin(), this->vertices.end(), vertex);
		if (iter != this->vertices.end())
		{
			return std::distance(this->vertices.begin(), iter);
		}
		else
		{
			return N;
		}
	}

	template <std::size_t N>
	const std::shared_ptr<Point<N>>& Hyperplane<N>::operator[](std::size_t index) const
	{
		assert(index < N && "Hyperplane index must be less than N");
		return this->vertices[index];
	}

	template <std::size_t N>
	std::shared_ptr<Point<N>>& Hyperplane<N>::operator[](std::size_t index)
	{
		assert(index < N && "Hyperplane index must be less than N");
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

	template<std::size_t N>
	HyperplanePencil<N>::HyperplanePencil()
	{
		this->vertices.fill(nullptr);
	}

	// ��ʽʵ����
	template class Hyperplane<2>;
	template class Hyperplane<3>;
	template class Hyperplane<4>;

	// HyperplanePencil class
	template <std::size_t N>
	HyperplanePencil<N>::HyperplanePencil(const std::array<std::shared_ptr<Point<N>>, N - 1>& vertices)
		: vertices(vertices)
	{
		std::sort(this->vertices.begin(), this->vertices.end(), [](const std::shared_ptr<Point<N>>& point1, const std::shared_ptr<Point<N>>& point2)
			{
				return *point1 < *point2;
			});
	}

	template<std::size_t N>
	HyperplanePencil<N>::HyperplanePencil(const HyperplanePencil<N>& hyperplanePencil)
		: vertices(hyperplanePencil.vertices)
	{
	}

	template<std::size_t N>
	std::size_t HyperplanePencil<N>::getDimension() const
	{
		return N;
	}

	template <std::size_t N>
	const std::shared_ptr<Point<N>>& HyperplanePencil<N>::operator[](std::size_t index) const
	{
		assert(index < N - 1 && "HyperplanePencil index must be less than N - 1");
		return this->vertices[index];
	}

	template <std::size_t N>
	std::shared_ptr<Point<N>>& HyperplanePencil<N>::operator[](std::size_t index)
	{
		assert(index < N - 1 && "HyperplanePencil index must be less than N - 1");
		return this->vertices[index];
	}

	template <std::size_t N>
	bool HyperplanePencil<N>::operator==(const HyperplanePencil<N>& hyperplanePencil) const
	{
		for (std::size_t i = 0; i < N - 1; i++)
		{
			if (this->vertices[i] != hyperplanePencil[i])
			{
				return false;
			}
		}
		return true;
	}

	template <std::size_t N>
	bool HyperplanePencil<N>::operator!=(const HyperplanePencil<N>& hyperplanePencil) const
	{
		return !(*this == hyperplanePencil);
	}

	template<std::size_t N>
	bool HyperplanePencil<N>::operator<(const HyperplanePencil<N>& hyperplanePencil) const
	{
		for (std::size_t i = 0; i < N - 1; i++)
		{
			if (*this->vertices[i] < *hyperplanePencil[i])
			{
				return true;
			}
			else if (*this->vertices[i] > *hyperplanePencil[i])
			{
				return false;
			}
		}
		return false;
	}

	template <std::size_t N>
	std::ostream& operator<<(std::ostream& output, const HyperplanePencil<N>& hyperplanePencil)
	{
		if (!hyperplanePencil)
		{
			output << "HyperplanePencil is not initialized";
		}
		else
		{
			output << "{";
			for (std::size_t i = 0; i < N - 2; i++)
			{
				output << *hyperplanePencil[i] << ", ";
			}
			output << *hyperplanePencil[N - 2] << "}";
		}
		return output;
	}

	// ��ʽʵ����
	template class HyperplanePencil<2>;
	template class HyperplanePencil<3>;
	template class HyperplanePencil<4>;

	// Simplex class
	template <std::size_t N>
	Simplex<N>::Simplex()
	{
		this->vertices.fill(nullptr);
		this->facets.fill(nullptr);
	}

	template <std::size_t N>
	Simplex<N>::Simplex(const std::array<Point<N>, N + 1>& vertices)
	{
		std::vector<Point<N>> points(vertices.begin(), vertices.end());
		std::sort(points.begin(), points.end());
		for (std::size_t i = 0; i < N + 1; i++)
		{
			this->vertices[i] = std::make_shared<Point<N>>(points[i]);
		}
		if (!initializeFacets())
		{
			this->vertices.fill(nullptr);
			this->facets.fill(nullptr);
		}
	}

	template <std::size_t N>
	Simplex<N>::Simplex(const std::array<std::shared_ptr<Point<N>>, N + 1>& vertices)
		: vertices(vertices)
	{
		std::sort(this->vertices.begin(), this->vertices.end(), [](const std::shared_ptr<Point<N>>& point1, const std::shared_ptr<Point<N>>& point2)
			{
				return *point1 < *point2;
			});
		if (!initializeFacets())
		{
			this->vertices.fill(nullptr);
			this->facets.fill(nullptr);
		}
	}

	template <std::size_t N>
	Simplex<N>::Simplex(const Simplex<N>& simplex)
	{
		this->vertices = simplex.vertices;
		std::sort(this->vertices.begin(), this->vertices.end(), [](const std::shared_ptr<Point<N>>& point1, const std::shared_ptr<Point<N>>& point2)
			{
				return *point1 < *point2;
			});
		this->facets = simplex.facets;
	}

	template<std::size_t N>
	bool Simplex<N>::initializeFacets()
	{
		// ���㵥���ε����ģ�����ȷ�ϵ����ε���ķ���������
		std::shared_ptr<Point<N>> centroid = getCentroid();
		std::vector<std::shared_ptr<Hyperplane<N>>> facets;
		for (std::size_t i = 0; i < N + 1; i++)
		{
			std::array<std::shared_ptr<Point<N>>, N> facetVertices;
			std::size_t index = 0;
			for (std::size_t j = 0; j < N + 1; j++)
			{
				if (j != i)
				{
					facetVertices[index] = vertices[j];
					index++;
				}
			}
			facets.push_back(std::make_shared<Hyperplane<N>>(facetVertices));
			// �����ķ�����Ϊ�����������޷�����������
			if (facets[i]->getNormal() == Vector<N>())
			{
				this->facets = std::array<std::shared_ptr<Hyperplane<N>>, N + 1>();
				return false;
			}
			// �������ĵ�λ��������ķ���������
			facets[i]->setNormalDirection(centroid);
		}
		for (std::size_t i = 0; i < N + 1; i++)
		{
			std::size_t index = 0;
			for (std::size_t j = 0; j < N + 1; j++)
			{
				if (i != j)
				{
					facets[i]->setNeighbor(index, facets[j]);
					index++;
				}
			}
		}
		for (std::size_t i = 0; i < N + 1; i++)
		{
			this->facets[i] = facets[i];
		}
		return true;
	}

	template<std::size_t N>
	std::size_t Simplex<N>::getDimension() const
	{
		return N;
	}

	template<std::size_t N>
	std::shared_ptr<Point<N>> Simplex<N>::getCentroid() const
	{
		std::shared_ptr<Point<N>> centroid = std::make_shared<Point<N>>();
		for (std::size_t i = 0; i < N + 1; i++)
		{
			for (std::size_t j = 0; j < N; j++)
			{
				(*centroid)[j] += (*vertices[i])[j];
			}
		}
		for (std::size_t i = 0; i < N; i++)
		{
			(*centroid)[i] /= (N + 1);
		}
		return centroid;
	}

	template<std::size_t N>
	std::shared_ptr<Point<N>> Simplex<N>::getCircumcenter() const
	{
		std::shared_ptr<Point<N>> circumcenter = std::make_shared<Point<N>>();
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
			(*circumcenter)[i] = constants[i];
			for (std::size_t j = 1; j < i; j++)
			{
				(*circumcenter)[i] -= coefficients[i][j] * (*circumcenter)[j];
			}
			(*circumcenter)[i] /= coefficients[i][i];
		}
		return circumcenter;
	}

	template<std::size_t N>
	std::shared_ptr<Hyperplane<N>> Simplex<N>::getFacet(std::size_t index) const
	{
		return this->facets[index];
	}

	template<std::size_t N>
	std::vector<std::shared_ptr<Hyperplane<N>>> Simplex<N>::getFacets() const
	{
		std::vector<std::shared_ptr<Hyperplane<N>>> facets;
		for (std::size_t i = 0; i < N + 1; i++)
		{
			facets.push_back(this->facets[i]);
		}
		return facets;
	}

	template<std::size_t N>
	const std::shared_ptr<Point<N>>& Simplex<N>::operator[](std::size_t index) const
	{
		assert(index < N + 1 && "Simplex index must be less than N + 1");
		return this->vertices[index];
	}

	template<std::size_t N>
	std::shared_ptr<Point<N>>& Simplex<N>::operator[](std::size_t index)
	{
		assert(index < N + 1 && "Simplex index must be less than N + 1");
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

	// ��ʽʵ����
	template class Simplex<2>;
	template class Simplex<3>;
	template class Simplex<4>;

	// ConvexHull class
	template <std::size_t N>
	ConvexHull<N>::ConvexHull()
	{
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
		std::sort(this->vertices.begin(), this->vertices.end(), [](const std::shared_ptr<Point<N>>& point1, const std::shared_ptr<Point<N>>& point2)
			{
				return *point1 < *point2;
			});
	}

	template <std::size_t N>
	ConvexHull<N>::ConvexHull(const std::vector<std::shared_ptr<Point<N>>>& points)
		: vertices(points)
	{
		std::sort(this->vertices.begin(), this->vertices.end(), [](const std::shared_ptr<Point<N>>& point1, const std::shared_ptr<Point<N>>& point2)
			{
				return *point1 < *point2;
			});
	}

	template<std::size_t N>
	ConvexHull<N>::ConvexHull(const ConvexHull<N>& convexHull)
		: vertices(convexHull.vertices),
		facets(convexHull.facets)
	{
		std::sort(this->vertices.begin(), this->vertices.end(), [](const std::shared_ptr<Point<N>>& point1, const std::shared_ptr<Point<N>>& point2)
			{
				return *point1 < *point2;
			});
	}

	template<std::size_t N>
	bool ConvexHull<N>::initialize()
	{
		// ���Թ�����ʼ������
		std::vector<std::shared_ptr<Point<N>>> simplexVertices;
		// ��¼͹�����ڲ���
		std::shared_ptr<Point<N>> centroid = nullptr;
		{
			simplexVertices.push_back(this->vertices[0]);
			std::size_t index = 1;

			// matrix���ڴ洢���������εĵ�֮�������
			std::vector<std::array<double, N>> matrix;
			while (simplexVertices.size() < N + 1)
			{
				// ���vertices�еĵ�������أ����޷�������ʼ������
				if (index == this->vertices.size())
				{
					return false;
				}
				// directionΪvertices[index]��vertices[0]֮�������
				Vector<N> direction = *this->vertices[index] - *this->vertices[0];
				// ���direction��matrix�е����������޹أ���vertices[index]��ӵ�simplexVertices�У�����direction��ӵ�matrix��
				bool flag = isLinearlyIndependent(matrix, direction);
				if (flag)
				{
					simplexVertices.push_back(this->vertices[index]);
					std::array<double, N> temp;
					for (std::size_t i = 0; i < N; i++)
					{
						temp[i] = direction[i];
					}
					matrix.push_back(temp);
				}
				index++;
			}

			// ������ʼ������
			std::array<std::shared_ptr<Point<N>>, N + 1> tempVertices;
			for (std::size_t i = 0; i < N + 1; i++)
			{
				tempVertices[i] = simplexVertices[i];
			}
			Simplex<N> simplex(tempVertices);
			// ����޷�������ʼ�����Σ��򷵻�false
			bool flag = simplex.initializeFacets();
			if (!flag)
			{
				return false;
			}
			// ����ʼ�����εĶ�����渳ֵ��convexHull
			std::vector<std::shared_ptr<Hyperplane<N>>> facets = simplex.getFacets();

			for (std::size_t i = 0; i < N + 1; i++)
			{
				this->facets.push_back(facets[i]);
			}
			// ��¼��ʼ�����ε����ģ��õ�ʼ��Ϊ͹�����ڲ���
			centroid = simplex.getCentroid();
		}

		// ��convecHull�е��ڵ������ⲿ����ӵ�����Ϸ�������
		for (std::shared_ptr<Point<N>> point : this->vertices)
		{
			// ����㲻��simplexVertices�У��򽫵���ӵ�����ⲿ������
			auto iter = std::find(simplexVertices.begin(), simplexVertices.end(), point);
			if (iter == simplexVertices.end())
			{
				for (std::shared_ptr<Hyperplane<N>> facet : this->facets)
				{
					// �����������Ϸ����򽫵���ӵ�����ⲿ������
					if (facet->addPointAbove(point))
					{
						break;
					}
				}
			}
		}

		// �ݹ齫����ⲿ����ӵ�convexHull��
		{
			// ��¼���������
			std::vector<std::shared_ptr<Hyperplane<N>>> processFacets = this->facets;
			while (!processFacets.empty())
			{
				// ȡ�����������
				std::shared_ptr<Hyperplane<N>> facet = processFacets.back();
				processFacets.pop_back();
				// ȡ������ⲿ���о�������Զ�ĵ�
				std::shared_ptr<Point<N>> furthestPoint = facet->getFurthestPointAbove();
				if (furthestPoint != nullptr)
				{
					// ��¼�õ�Ŀɼ���
					std::vector<std::shared_ptr<Hyperplane<N>>> visibleFacets;
					// ����õ�Ŀɼ��棨�ɿ��ǳ�ȡΪ������
					{
						// ��¼�ѷ��ʵ���
						std::set<std::shared_ptr<Hyperplane<N>>> visitedFacets;
						// ��ȷ�ϵ���
						std::vector<std::shared_ptr<Hyperplane<N>>> facetsToCheck;
						facetsToCheck.push_back(facet);

						while (!facetsToCheck.empty())
						{
							std::shared_ptr<Hyperplane<N>> currentFacet = facetsToCheck.back();
							facetsToCheck.pop_back();

							if (visitedFacets.find(currentFacet) == visitedFacets.end())
							{
								visitedFacets.insert(currentFacet);

								if (currentFacet->isAbove(*furthestPoint))
								{
									visibleFacets.push_back(currentFacet);

									auto iter = std::find(processFacets.begin(), processFacets.end(), currentFacet);
									if (iter != processFacets.end())
									{
										processFacets.erase(iter);
									}

									// ����ǰ����ڽ�����ӵ���ȷ�ϵ�����
									for (auto& f : currentFacet->getNeighbors())
									{
										if (f != nullptr)
										{
											facetsToCheck.push_back(f);
										}
									}
								}
							}
						}
					}
					// �����µ�͹���棬�����ⲿ�����·��䵽�µ�����
					std::vector<std::shared_ptr<Hyperplane<N>>> newFacets;
					{
						// ��¼��ƽ������Ӧ���ڽ���
						std::map<HyperplanePencil<N>, std::shared_ptr<Hyperplane<N>>> pencilNeighbors;
						{
							// ͳ�Ƴ�ƽ�����ĳ��ִ���
							std::map<HyperplanePencil<N>, std::vector<std::shared_ptr<Point<N>>>> pencilMap;
							// ��¼��ƽ������Ӧ�ĳ�ƽ��
							std::map<HyperplanePencil<N>, std::shared_ptr<Hyperplane<N>>> pencilFacets;
							for (std::shared_ptr<Hyperplane<N>> visibleFacet : visibleFacets)
							{
								// ��ȡ�ɼ���ĳ�ƽ����
								std::array<HyperplanePencil<N>, N> pencils = visibleFacet->getPencils();
								std::size_t index = 0;
								for (HyperplanePencil<N> pencil : pencils)
								{
									pencilMap[pencil].push_back((*visibleFacet)[index]);
									pencilFacets[pencil] = visibleFacet;
									index++;
								}
							}
							// ��ȡ͹����ĳ�ƽ����
							for (auto& pair : pencilMap)
							{
								if (pair.second.size() == 1)
								{
									std::shared_ptr<Hyperplane<N>> hyperplane = pencilFacets[pair.first];
									std::size_t index = hyperplane->getVertexIndex(pair.second[0]);
									std::shared_ptr<Hyperplane<N>> neighbor = hyperplane->getNeighbor(index);
									pencilNeighbors[pair.first] = neighbor;
								}
							}
						}
						// ��¼�ⲿ��
						std::vector<std::shared_ptr<Point<N>>> pointsAbove;
						// ɾ��͹���еĿɼ���ƽ��
						for (std::shared_ptr<Hyperplane<N>> visibleFacet : visibleFacets)
						{
							auto iter = std::find(this->facets.begin(), this->facets.end(), visibleFacet);
							if (iter != this->facets.end())
							{
								this->facets.erase(iter);
								// ɾ���ڽӹ�ϵ
								std::array<std::shared_ptr<Hyperplane<N>>, N> neighbors = visibleFacet->getNeighbors();
								for (std::size_t i = 0; i < N; i++)
								{
									if (neighbors[i] != nullptr)
									{
										neighbors[i]->removeNeighbor(visibleFacet);
									}
								}
								std::vector<std::shared_ptr<Point<N>>> points = visibleFacet->getPointsAbove();
								pointsAbove.insert(pointsAbove.end(), points.begin(), points.end());
							}
						}
						// ����µ�͹����
						for (auto& pair : pencilNeighbors)
						{
							HyperplanePencil<N> pencil = pair.first;
							std::shared_ptr<Hyperplane<N>> neighbor = pair.second;
							std::array<std::shared_ptr<Point<N>>, N> vertices;
							for (std::size_t i = 0; i < N - 1; i++)
							{
								vertices[i] = pencil[i];
							}
							vertices[N - 1] = furthestPoint;
							std::shared_ptr<Hyperplane<N>> newFacet = std::make_shared<Hyperplane<N>>(vertices);
							newFacet->setNormalDirection(centroid);
							newFacet->setNeighbor(neighbor);
							neighbor->setNeighbor(newFacet);
							newFacets.push_back(newFacet);
						}
						// �����ڽӹ�ϵ
						for (std::shared_ptr<Hyperplane<N>> newFacet : newFacets)
						{
							for (std::shared_ptr<Hyperplane<N>> facet : newFacets)
							{
								if (newFacet != facet)
								{
									std::size_t index = 0;
									if (newFacet->isAdjacent(*facet, index))
									{
										newFacet->setNeighbor(index, facet);
									}
								}
							}
						}
						// ���ⲿ�����·��䵽�µ�͹������
						for (std::shared_ptr<Point<N>> p : pointsAbove)
						{
							if (p != furthestPoint)
							{
								for (std::shared_ptr<Hyperplane<N>> newFacet : newFacets)
								{
									if (newFacet->addPointAbove(p))
									{
										break;
									}
								}
							}
						}
					}
					// ���µ�͹������ӵ�convexHull��
					this->facets.insert(this->facets.end(), newFacets.begin(), newFacets.end());
					// ���µ�͹������ӵ������������
					processFacets.insert(processFacets.end(), newFacets.begin(), newFacets.end());
				}
			}
		}
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

	// ��ʽʵ����
	template class ConvexHull<2>;
	template class ConvexHull<3>;
	template class ConvexHull<4>;

	// Geometry functions
	template<std::size_t N>
	int calculateMatrixRank(std::vector<std::array<double, N>>& matrix)
	{
		std::size_t rows = matrix.size();
		std::size_t cols = N;
		int rank = 0;

		for (std::size_t col = 0; col < cols; col++)
		{
			std::size_t maxRow = rank;
			for (std::size_t row = rank + 1; row < rows; row++)
			{
				if (std::abs(matrix[row][col]) > std::abs(matrix[maxRow][col]))
				{
					maxRow = row;
				}
			}

			if (std::abs(matrix[maxRow][col]) < 1e-9)
			{
				continue;
			}

			if (maxRow != rank)
			{
				std::swap(matrix[rank], matrix[maxRow]);
			}

			for (std::size_t c = col + 1; c < cols; c++)
			{
				matrix[rank][c] /= matrix[rank][col];
			}
			matrix[rank][col] = 1.0;

			for (std::size_t row = rank + 1; row < rows; row++)
			{
				double factor = matrix[row][col];
				for (std::size_t c = col; c < cols; c++)
				{
					matrix[row][c] -= factor * matrix[rank][c];
				}
			}

			rank++;
		}

		return rank;
	}

	template<std::size_t N>
	bool isLinearlyIndependent(const std::vector<std::array<double, N>>& matrix, const Vector<N>& newVector)
	{
		int n = matrix.size();

		if (n == 0)
		{
			// ���matrixΪ�գ���newPoint��ԭ���������޹�
			return true;
		}

		if (n >= N)
		{
			// ���matrix�е������������ڵ���N�����޷������µ������޹�����
			return false;
		}

		std::vector<std::array<double, N>> tempMatrix = matrix;
		std::array<double, N> newVectorArray;
		for (std::size_t i = 0; i < N; i++)
		{
			newVectorArray[i] = newVector[i];
		}
		tempMatrix.push_back(newVectorArray);

		return calculateMatrixRank<N>(tempMatrix) == n + 1;
	}

	template<std::size_t N>
	Vector<N> calculateNormal(const std::array<std::shared_ptr<Point<N>>, N>& vertices)
	{
		std::vector<std::array<double, N>> matrix;
		for (std::size_t i = 1; i < N; i++)
		{
			std::array<double, N> row;
			for (std::size_t j = 0; j < N; j++)
			{
				row[j] = (*vertices[i])[j] - (*vertices[0])[j];
			}
			matrix.push_back(row);
		}

		Vector<N> normal;
		if constexpr (N == 3)
		{
			normal = Vector<N>(matrix[0]) ^ Vector<N>(matrix[1]);
		}
		else
		{
			normal = Vector<N>(matrix[0]);
			for (std::size_t i = 1; i < N - 1; i++)
			{
				double dotProduct = Vector<N>(matrix[i]) * Vector<N>(matrix[i + 1]);
				
				for (std::size_t j = 0; j < N; j++)
				{
					normal[j] -= dotProduct * matrix[i][j];
				}
			}
		}
		normal.normalize();
		return normal;
	}

	template<std::size_t N>
	std::shared_ptr<Point<N + 1>> projectOntoParaboloid(const std::shared_ptr<Point<N>>& point)
	{
		std::shared_ptr<Point<N + 1>> projected = std::make_shared<Point<N + 1>>();
		for (std::size_t i = 0; i < N; i++)
		{
			(*projected)[i] = (*point)[i];
		}
		(*projected)[N] = 0.0;
		for (std::size_t i = 0; i < N; i++)
		{
			(*projected)[N] += (*point)[i] * (*point)[i];
		}
		return projected;
	}
}

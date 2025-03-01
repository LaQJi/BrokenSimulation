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
		this->pointsAbove.clear();
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
			std::sort(this->vertices.begin(), this->vertices.end());
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
		double distance = 0.0;
		for (std::size_t i = 0; i < N; i++)
		{
			distance += normal[i] * (point[i] - (*this->vertices[0])[i]);
		}
		return distance / normal.getMagnitude();
	}

	template<std::size_t N>
	void Hyperplane<N>::setNormalDirection(std::shared_ptr<Point<N>> pointBehind)
	{
		Vector<N> direction;
		for (std::size_t i = 0; i < N; i++)
		{
			direction[i] = (*pointBehind)[i] - (*this->vertices[0])[i];
		}
		if (normal * direction < 0.0)
		{
			normal = -normal;
		}
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
			// 如果hyperplane中不包含vertices[i]，则将index设置为i
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
		// 判断是否为邻接hyperplane
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
		static_assert(index < N, "Hyperplane neighbor index must be less than N");
		// temp为neighbor中不包含的点
		std::shared_ptr<Point<N>> temp = this->vertices[index];
		// 判断neighbor中是否包含temp
		auto iter = std::find(neighbor->vertices.begin(), neighbor->vertices.end(), temp);
		// 如果neighbor中不包含temp，则将neighbor设置为邻接hyperplane
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
	bool Hyperplane<N>::addPointAbove(std::shared_ptr<Point<N>> point)
	{
		if (point != nullptr)
		{
			Vector<N> vector = *point - *this->vertices[0];
			// 如果点在超平面上方，则将点加入pointsAbove
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
		return std::vector<std::shared_ptr<Point<N>>>();
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
		this->vertices.fill(nullptr);
		this->facets.fill(nullptr);
	}

	template <std::size_t N>
	Simplex<N>::Simplex(const std::array<Point<N>, N + 1>& vertices)
	{
		std::sort(vertices.begin(), vertices.end());
		for (std::size_t i = 0; i < N + 1; i++)
		{
			this->vertices[i] = std::make_shared<Point<N>>(vertices[i]);
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
		std::sort(this->vertices.begin(), this->vertices.end());
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
		std::sort(this->vertices.begin(), this->vertices.end());
		this->facets = simplex.facets;
	}

	template<std::size_t N>
	bool Simplex<N>::initializeFacets()
	{
		// 计算单纯形的重心，用于确认单纯形的面的法向量方向
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
			// 如果面的法向量为零向量，则无法构建单纯形
			if (facets[i]->getNormal() == Vector<N>())
			{
				this->facets = std::array<std::shared_ptr<Hyperplane<N>>, N + 1>();
				return false;
			}
			// 根据重心的位置设置面的法向量方向
			facets[i]->setNormalDirection(centroid);
		}
		for (std::size_t i = 0; i < N + 1; i++)
		{
			std::size_t index = 0;
			for (std::size_t j = 0; j < N + 1; j++)
			{
				if (i != j)
				{
					facets[i]->setNeighbor(facets[j], index);
					index++;
				}
			}
		}
		this->facets = facets;
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
	std::shared_ptr<Hyperplane<N>> Simplex<N>::getFacet(std::size_t index) const
	{
		return this->facets[index];
	}

	template<std::size_t N>
	std::vector<std::shared_ptr<Hyperplane<N>>> Simplex<N>::getFacets() const
	{
		return this->facets;
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
		std::sort(this->vertices.begin(), this->vertices.end());
		this->facets.fill(nullptr);
	}

	template <std::size_t N>
	ConvexHull<N>::ConvexHull(const std::vector<std::shared_ptr<Point<N>>>& points)
		: vertices(points)
	{
		std::sort(this->vertices.begin(), this->vertices.end());
		this->facets.fill(nullptr);
	}

	template<std::size_t N>
	ConvexHull<N>::ConvexHull(const ConvexHull<N>& convexHull)
		: vertices(convexHull.vertices),
		facets(convexHull.facets)
	{
		std::sort(this->vertices.begin(), this->vertices.end());
	}

	template<std::size_t N>
	bool ConvexHull<N>::initialize()
	{
		// 尝试构建初始单纯形
		std::vector<std::shared_ptr<Point<N>>> simplexVertices;
		{
			simplexVertices.push_back(this->vertices[0]);
			std::size_t index = 1;

			// matrix用于存储构建单纯形的点之间的向量
			std::vector<std::array<double, N>> matrix;
			while (simplexVertices.size() < N + 1)
			{
				// 如果vertices中的点线性相关，则无法构建初始单纯形
				if (index == this->vertices.size())
				{
					return false;
				}
				// direction为vertices[index]与vertices[0]之间的向量
				Vector<N> direction = this->vertices[index] - this->vertices[0];
				// 如果direction与matrix中的向量线性无关，则将vertices[index]添加到simplexVertices中，并将direction添加到matrix中
				bool flag = isLinearlyIndependent(matrix, direction);
				if (flag)
				{
					simplexVertices.push_back(this->vertices[index]);
					matrix.push_back(direction);
				}
				index++;
			}

			// 构建初始单纯形
			Simplex<N> simplex(simplexVertices);
			// 如果无法构建初始单纯形，则返回false
			bool flag = simplex.initializeFacets();
			if (!flag)
			{
				return false;
			}
			// 将初始单纯形的顶点和面赋值给convexHull
			std::vector<std::shared_ptr<Hyperplane<N>>> facets = simplex.getFacets();

			for (std::size_t i = 0; i < N + 1; i++)
			{
				this->facets[i] = facets[i];
			}
		}

		// 将convecHull中的在单纯形外部点添加到面的上方集合中
		for (std::shared_ptr<Point<N>> point : this->vertices)
		{
			// 如果点不在simplexVertices中，则将点添加到面的外部集合中
			auto iter = std::find(simplexVertices.begin(), simplexVertices.end(), point);
			if (iter == simplexVertices.end())
			{
				for (std::shared_ptr<Hyperplane<N>> facet : this->facets)
				{
					// 如果点在面的上方，则将点添加到面的外部集合中
					if (facet->addPointAbove(point))
					{
						break;
					}
				}
			}
		}

		// TODO: 递归添加点
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
	int calculateSquareMatrixRank(std::array<std::array<double, N>, N>& matrix) 
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
	int calculateMatrixRank(std::vector<std::array<double, N>>& matrix)
	{
		std::size_t rows = matrix.size();
		std::size_t cols = N;
		int rank = 0;

		for (std::size_t i = 0; i < rows; i++)
		{
			if (matrix[i][rank] != 0)
			{
				for (std::size_t j = 0; j < cols; j++)
				{
					if (j != i)
					{
						double factor = matrix[j][rank] / matrix[i][rank];
						for (std::size_t k = 0; k < cols; k++)
						{
							matrix[j][k] -= factor * matrix[i][k];
						}
					}
				}
				rank++;
			}
		}
		return rank;
	}

	template<std::size_t N>
	bool isLinearlyIndependent(const std::vector<std::array<double, N>>& matrix, const Vector<N>& newVector)
	{
		int n = matrix.size();
		
		if (n == 0)
		{
			// 如果matrix为空，则newPoint与原向量线性无关
			return true;
		}

		if (n >= N)
		{
			// 如果matrix中的向量数量大于等于N，则无法构建新的线性无关向量
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
		std::array<std::array<double, N>, N> matrix;
		for (std::size_t i = 0; i < N; i++)
		{
			for (std::size_t j = 0; j < N; j++)
			{
				matrix[i][j] = (*vertices[i])[j];
			}
		}
		if (calculateSquareMatrixRank<N>(matrix) < N)
		{
			return Vector<N>();
		}
		Vector<N> normal;
		for (std::size_t i = 0; i < N; i++)
		{
			normal[i] = matrix[0][i];
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

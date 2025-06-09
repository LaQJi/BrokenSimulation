#include "VoronoiFracture.h"

#include <iostream>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <queue>
#include <algorithm>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Geometry
{
	// �������������
	void Mesh::addTriangle(const Vector3D& v0, const Vector3D& v1, const Vector3D& v2)
	{
		// ���������
		this->triangles.emplace_back(v0, v1, v2);

		// ��Ӷ���
		this->vertices.push_back(v0);
		this->vertices.push_back(v1);
		this->vertices.push_back(v2);
	}

	// ��������ı߽�
	void Mesh::calculateBounds(Vector3D& min, Vector3D& max) const
	{
		if (this->triangles.empty())
			return;
		min = max = this->triangles[0].vertices[0];
		for (const auto& triangle : this->triangles)
		{
			for (const auto& vertex : triangle.vertices)
			{
				min.x = std::min(min.x, vertex.x);
				min.y = std::min(min.y, vertex.y);
				min.z = std::min(min.z, vertex.z);
				max.x = std::max(max.x, vertex.x);
				max.y = std::max(max.y, vertex.y);
				max.z = std::max(max.z, vertex.z);
			}
		}
	}

	void ModelPreprocessor::loadModel(const std::string& filename)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(filename,
			aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);

		// ����Ƿ�ɹ�����ģ��
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
			return;
		}

		boundingBoxMin = Vector3D(FLT_MAX, FLT_MAX, FLT_MAX);
		boundingBoxMax = Vector3D(FLT_MIN, FLT_MIN, FLT_MIN);

		// ������������
		for (unsigned int i = 0; i < scene->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[i];
			for (unsigned int j = 0; j < mesh->mNumFaces; j++)
			{
				aiFace face = mesh->mFaces[j];
				if (face.mNumIndices == 3)
				{
					Vector3D v0(mesh->mVertices[face.mIndices[0]].x,
						mesh->mVertices[face.mIndices[0]].y,
						mesh->mVertices[face.mIndices[0]].z);

					Vector3D v1(mesh->mVertices[face.mIndices[1]].x,
						mesh->mVertices[face.mIndices[1]].y,
						mesh->mVertices[face.mIndices[1]].z); 

					Vector3D v2(mesh->mVertices[face.mIndices[2]].x,
						mesh->mVertices[face.mIndices[2]].y,
						mesh->mVertices[face.mIndices[2]].z);

					// ���±߽�
					boundingBoxMin.x = std::min({ boundingBoxMin.x, v0.x, v1.x, v2.x });
					boundingBoxMin.y = std::min({ boundingBoxMin.y, v0.y, v1.y, v2.y });
					boundingBoxMin.z = std::min({ boundingBoxMin.z, v0.z, v1.z, v2.z });

					boundingBoxMax.x = std::max({ boundingBoxMax.x, v0.x, v1.x, v2.x });
					boundingBoxMax.y = std::max({ boundingBoxMax.y, v0.y, v1.y, v2.y });
					boundingBoxMax.z = std::max({ boundingBoxMax.z, v0.z, v1.z, v2.z });

					// ��������ε�ģ������
					this->modelMesh.addTriangle(v0, v1, v2);
				}
			}
		}

		// ���ģ���Ƿ�պ�
		this->isClosed = isModelClosed();
	}

	bool ModelPreprocessor::isPointInsideModel(const Vector3D& point) const
	{
		// ʹ�����߷��жϵ��Ƿ���ģ����
		Vector3D rayDirection = Vector3D(2, 3, 5).normalize(); // ���߷���
		int intersectionCount = 0;

		for (const auto& triangle : this->modelMesh.triangles)
		{
			float intersectionDistance;
			if (rayIntersectsTriangle(point, rayDirection, triangle, intersectionDistance))
			{
				intersectionCount++;
			}
		}

		// ���������Ϊ�����������������
		return (intersectionCount % 2 == 1);
	}

	bool ModelPreprocessor::rayIntersectsTriangle(const Vector3D& rayOrigin, const Vector3D& rayDirection, const Triangle& triangle, float& intersectionDistance)
	{
		const float EPSILON = 1e-6f;
		// ʹ��Moller�CTrumbore�㷨�ж������������ε��ཻ

		// ���������εı�
		Vector3D edge1 = (triangle.vertices[1] - triangle.vertices[0]).normalize();
		Vector3D edge2 = (triangle.vertices[2] - triangle.vertices[0]).normalize();
		// ���㷨����
		Vector3D h = rayDirection.cross(edge2).normalize();
		float a = edge1.dot(h);

		// ����ƽ����������
		if (a > -EPSILON && a < EPSILON)
		{
			return false;
		}

		// ���������������εĽ���
		float f = 1.0f / a;
		Vector3D s = rayOrigin - triangle.vertices[0];

		float u = f * s.dot(h);
		if (u < 0.0f || u > 1.0f)
		{
			return false;
		}

		Vector3D q = s.cross(edge1);
		float v = f * rayDirection.dot(q);
		if (v < 0.0f || u + v > 1.0f)
		{
			return false;
		}

		// ���㽻�����
		intersectionDistance = f * edge2.dot(q);

		return (intersectionDistance > EPSILON);
	}

	bool ModelPreprocessor::isModelClosed() const
	{
		std::unordered_map<Edge, int, EdgeHash> edgeCounts;

		for (const auto& triangle : this->modelMesh.triangles)
		{
			for (int i = 0; i < 3; ++i)
			{
				Edge edge(triangle.vertices[i], triangle.vertices[(i + 1) % 3]);
				edgeCounts[edge]++;
			}
		}

		// ���ÿ���ߵļ����Ƿ�Ϊ2
		for (const auto& pair : edgeCounts)
		{
			if (pair.second != 2)
			{
				return false;
			}
		}

		return true;
	}

	void ModelPreprocessor::buildBoundingBox()
	{
		Vector3D minCorner, maxCorner;
		// ����ģ�͵ı߽�
		this->modelMesh.calculateBounds(minCorner, maxCorner);
		// ���ð�Χ��
		this->boundingBoxMin = minCorner;
		this->boundingBoxMax = maxCorner;
	}

	Delaunay3D::Delaunay3D(const std::vector<Vector3D>& points)
	{
		// ��ʼ���㼯
		// �������Ŷ������⹲���
		for (const auto& point : points)
		{
			Vector3D perturbedPoint = point;
			perturbedPoint.x += static_cast<float>(rand()) / RAND_MAX * 1e-6f;
			perturbedPoint.y += static_cast<float>(rand()) / RAND_MAX * 1e-6f;
			perturbedPoint.z += static_cast<float>(rand()) / RAND_MAX * 1e-6f;
			this->points.push_back(perturbedPoint);
		}
	}

	void Delaunay3D::triangulate()
	{
		// 1. ��ʼ��������
		if (points.size() < 4)
		{
			std::cerr << "Not enough points to form a tetrahedron." << std::endl;
			return;
		}

		// ��������������
		int superTetrahedronIndex = createSuperTetrahedron();

		// 2. ����㣨����������������ĸ����㣩
		for (int i = 0; i < static_cast<int>(points.size()) - 4; ++i)
		{
			insertPoint(i);
		}

		// 3. �Ƴ��������������嶥���������
		removeSuperTetrahedron(superTetrahedronIndex);
	}

	std::vector<Tetrahedron> Delaunay3D::getValidTetrahedra() const
	{
		std::vector<Tetrahedron> validTetrahedra;

		for (const auto& tetrahedron : tetrahedra)
		{
			if (tetrahedron.isValid)
			{
				validTetrahedra.push_back(tetrahedron);
			}
		}

		return validTetrahedra;
	}

	int Delaunay3D::createSuperTetrahedron()
	{
		// ����㼯�ı߽�
		float minX = std::numeric_limits<float>::max();
		float minY = std::numeric_limits<float>::max();
		float minZ = std::numeric_limits<float>::max();
		float maxX = std::numeric_limits<float>::lowest();
		float maxY = std::numeric_limits<float>::lowest();
		float maxZ = std::numeric_limits<float>::lowest();

		for (const auto& point : points)
		{
			minX = std::min(minX, point.x);
			minY = std::min(minY, point.y);
			minZ = std::min(minZ, point.z);
			maxX = std::max(maxX, point.x);
			maxY = std::max(maxY, point.y);
			maxZ = std::max(maxZ, point.z);
		}

		float dx = maxX - minX;
		float dy = maxY - minY;
		float dz = maxZ - minZ;
		float deltaMax = std::max({ dx, dy, dz });
		float midX = (minX + maxX) / 2;
		float midY = (minY + maxY) / 2;
		float midZ = (minZ + maxZ) / 2;

		// ��������������Ķ��㣨������㼯�У�
		int p0 = points.size();
		points.emplace_back(midX - 6 * deltaMax,	 midY - 6 * deltaMax,	 midZ - 6 * deltaMax);
		int p1 = points.size();
		points.emplace_back(midX + 6 * deltaMax,	 midY + 6 * deltaMax,	 midZ - 6 * deltaMax);
		int p2 = points.size();
		points.emplace_back(midX + 6 * deltaMax,	 midY - 6 * deltaMax,	 midZ + 6 * deltaMax);
		int p3 = points.size();
		points.emplace_back(midX - 6 * deltaMax,	 midY + 6 * deltaMax,	 midZ + 6 * deltaMax);

		// ��������������
		Tetrahedron superTetrahedron(p0, p1, p2, p3);
		superTetrahedron.circumsphere = calculateCircumsphere(
			points[p0], points[p1], points[p2], points[p3]);

		// ��������������ӵ��������б�
		tetrahedra.push_back(superTetrahedron);
		// ���س��������������
		return static_cast<int>(tetrahedra.size()) - 1;
	}

	void Delaunay3D::removeSuperTetrahedron(int superTetrahedronIndex)
	{
		const Tetrahedron& superTetrahedron = tetrahedra[superTetrahedronIndex];
		std::unordered_set<int> verticesToRemove(superTetrahedron.vertices.begin(), superTetrahedron.vertices.end());

		std::vector<int> tetrahedraToRemove;

		for (size_t i = 0; i < tetrahedra.size(); ++i)
		{
			const Tetrahedron& tetrahedron = tetrahedra[i];

			// ����������Ƿ���Ч
			if (!tetrahedron.isValid)
				continue;

			// ���������Ķ����Ƿ��ڳ���������Ķ��㼯����
			for (int vertex : tetrahedron.vertices)
			{
				if (verticesToRemove.find(vertex) != verticesToRemove.end())
				{
					tetrahedraToRemove.push_back(static_cast<int>(i));
					break;
				}
			}
		}

		// ��Ҫ�Ƴ�����������Ϊ��Ч
		for (int index : tetrahedraToRemove)
		{
			tetrahedra[index].isValid = false;
		}
	}

	void Delaunay3D::insertPoint(const int pointIndex)
	{
		const Vector3D& point = points[pointIndex];

		// 1. �ҵ��������������
		int tetrahedronIndex = findContainingTetrahedron(point);
		
		// ����Ƿ��ҵ��������������
		if (tetrahedronIndex == -1)
		{
			std::cerr << "No containing tetrahedron found for point " << pointIndex << std::endl;
			return;
		}
		
		// 2. ��ȡ�����������������
		std::vector<int> tetrahedraToProcess;
		std::queue<int> toProcess;
		std::unordered_set<int> processed;

		toProcess.push(tetrahedronIndex);
		processed.insert(tetrahedronIndex);

		while (!toProcess.empty())
		{
			int currentIndex = toProcess.front();
			toProcess.pop();

			const Tetrahedron& tetrahedron = tetrahedra[currentIndex];

			// ����������Ƿ���Ч
			if (!tetrahedron.isValid)
				continue;

			// �����Ƿ����������
			if (circumsphereContainsPoint(tetrahedron, point))
			{
				tetrahedraToProcess.push_back(currentIndex);

				// �����ڽ�������
				for (int neighborIndex : tetrahedron.neighbors)
				{
					if (neighborIndex != -1 && processed.find(neighborIndex) == processed.end())
					{
						toProcess.push(neighborIndex);
						processed.insert(neighborIndex);
					}
				}
			}
		}

		// 3. ��ȡ�߽�����
		std::map<std::array<int, 3>, std::vector<int>> faceMap;

		for (int tetrahedronIndex : tetrahedraToProcess)
		{
			const Tetrahedron& tetrahedron = tetrahedra[tetrahedronIndex];

			for (int faceIndex = 0; faceIndex < 4; ++faceIndex)
			{
				std::array<int, 3> faceVertices = tetrahedron.getFaceVertices(faceIndex);
				std::sort(faceVertices.begin(), faceVertices.end());
				faceMap[faceVertices].push_back(tetrahedronIndex);
			}
		}

		std::vector<std::array<int, 3>> boundaryFaces;

		for (const auto& pair : faceMap)
		{
			if (pair.second.size() == 1) // ����һ�������干�����
			{
				boundaryFaces.push_back(pair.first);
			}
		}

		// 4. �Ƴ����������������������
		for (int tetrahedronIndex : tetrahedraToProcess)
		{
			tetrahedra[tetrahedronIndex].isValid = false;
		}

		// 5. �����µ�������
		std::vector<int> newTetrahedra;

		for (const auto& face : boundaryFaces)
		{
			int v0 = face[0];
			int v1 = face[1];
			int v2 = face[2];
			// �����µ�������
			Tetrahedron newTetrahedron(v0, v1, v2, pointIndex);
			newTetrahedron.circumsphere = calculateCircumsphere(
				points[v0], points[v1], points[v2], points[pointIndex]);

			// ����������嵽�б�
			tetrahedra.push_back(newTetrahedron);
			newTetrahedra.push_back(static_cast<int>(tetrahedra.size()) - 1);
		}

		// 6. �����ڽӹ�ϵ
		for (size_t i = 0; i < newTetrahedra.size() - 1; i++)
		{
			int newTetrahedronIndex = newTetrahedra[i];
			const Tetrahedron& newTetrahedron = tetrahedra[newTetrahedronIndex];

			// ��ȡ�����������
			std::array<std::array<int, 3>, 3> faces = {
				newTetrahedron.getFaceVertices(0),
				newTetrahedron.getFaceVertices(1),
				newTetrahedron.getFaceVertices(2),
			};

			// ��ÿ�����������ڱȽ�
			for (auto& face : faces)
			{
				std::sort(face.begin(), face.end());
			}

			// �������������������ڽӹ�ϵ
			for (size_t j = i + 1; j < newTetrahedra.size(); j++)
			{
				int otherTetrahedronIndex = newTetrahedra[j];
				const Tetrahedron& otherTetrahedron = tetrahedra[otherTetrahedronIndex];

				// ��ȡ��ǰ���������
				std::array<std::array<int, 3>, 3> otherFaces = {
					otherTetrahedron.getFaceVertices(0),
					otherTetrahedron.getFaceVertices(1),
					otherTetrahedron.getFaceVertices(2),
				};

				// ��ÿ�����������ڱȽ�
				for (auto& otherFace : otherFaces)
				{
					std::sort(otherFace.begin(), otherFace.end());
				}

				// ����Ƿ�����ͬ����
				for (int f1 = 0; f1 < 3; ++f1)
				{
					for (int f2 = 0; f2 < 3; ++f2)
					{
						if (faces[f1] == otherFaces[f2])
						{
							// �����ڽӹ�ϵ
							tetrahedra[newTetrahedronIndex].neighbors[f1] = otherTetrahedronIndex;
							tetrahedra[otherTetrahedronIndex].neighbors[f2] = newTetrahedronIndex;
							break; // �ҵ�ƥ����棬����ѭ��
						}
					}
				}
			}

			// ��������������ڽӹ�ϵ
			{
				const auto& oldFace = newTetrahedron.getFaceVertices(3);

				// ���������������е�����
				int faceIndex = 3;

				// ��ȡ������������
				auto iter = faceMap.find(oldFace);

				if (iter == faceMap.end() || iter->second.size() != 1)
				{
					continue; // û���ҵ���Ӧ�ľ�������
				}

				int oldTetrahedronIndex = iter->second[0];
				const Tetrahedron& oldTetrahedron = tetrahedra[oldTetrahedronIndex];

				int oldFaceIndex = -1;
				for (int j = 0; j < 4; ++j)
				{
					auto oldFaceVertices = oldTetrahedron.getFaceVertices(j);
					std::sort(oldFaceVertices.begin(), oldFaceVertices.end());

					if (oldFaceVertices == oldFace)
					{
						oldFaceIndex = j;
						break;
					}
				}

				if (oldFaceIndex != -1)
				{
					// ��ȡ����������ڽ�����������
					int neighborIndex = oldTetrahedron.neighbors[oldFaceIndex];

					if (neighborIndex != -1)
					{
						const auto& neighborTetrahedron = tetrahedra[neighborIndex];

						if (!neighborTetrahedron.isValid)
						{
							continue; // �ڽ���������Ч
						}

						// ��ȡ����������ڽ�������ĸ��������
						int neighborFaceIndex = -1;
						for (int j = 0; j < 4; ++j)
						{
							if (neighborTetrahedron.neighbors[j] == oldTetrahedronIndex)
							{
								neighborFaceIndex = j;
								break;
							}
						}

						if (neighborFaceIndex != -1)
						{
							if (faceIndex != -1 && neighborFaceIndex != -1)
							{
								// �����ڽӹ�ϵ
								tetrahedra[newTetrahedronIndex].neighbors[faceIndex] = neighborIndex;
								tetrahedra[neighborIndex].neighbors[neighborFaceIndex] = newTetrahedronIndex;
							}
						}
					}
				}
			}
		}
	}

	int Delaunay3D::findContainingTetrahedron(const Vector3D& point)
	{
		// ʹ��BFS����������
		std::queue<int> toVisit;
		std::unordered_set<int> visited;

		// �ӵ�һ����Ч�����忪ʼ
		int startIndex = -1;
		for (size_t i = 0; i < tetrahedra.size(); ++i)
		{
			if (tetrahedra[i].isValid)
			{
				startIndex = static_cast<int>(i);
				break;
			}
		}

		toVisit.push(startIndex);
		visited.insert(startIndex);

		while (!toVisit.empty())
		{
			int currentIndex = toVisit.front();
			toVisit.pop();

			const Tetrahedron& tetrahedron = tetrahedra[currentIndex];

			// ����������Ƿ���Ч
			if (!tetrahedron.isValid)
				continue;
			
			// �����Ƿ��ڵ�ǰ��������������
			if (circumsphereContainsPoint(tetrahedron, point))
			{
				return currentIndex; // �ҵ��������������
			}

			// �����ڽ�������
			for (int neighborIndex : tetrahedron.neighbors)
			{
				if (neighborIndex != -1 && visited.find(neighborIndex) == visited.end())
				{
					toVisit.push(neighborIndex);
					visited.insert(neighborIndex);
				}
			}
		}

		// ���û���ҵ�������������壬����-1
		return -1;
	}

	bool Delaunay3D::isInfinity(int vertexIndex) const
	{
		return vertexIndex < 0 || vertexIndex >= static_cast<int>(points.size());
	}

	Vector3D Delaunay3D::getVertex(int vertexIndex) const
	{
		if (isInfinity(vertexIndex))
		{
			return Vector3D(INFINITY, INFINITY, INFINITY); // ���������
		}
		return points[vertexIndex];
	}



	Voronoi3D::Voronoi3D(const std::vector<Vector3D>& points)
		: points(points)
	{
	}

	void Voronoi3D::buildVoronoi(const Vector3D& center)
	{
		// ����Voronoi��Ԫ������
		this->center = center;

		Delaunay3D delaunay(points);
		delaunay.triangulate();
		const std::vector<Tetrahedron>& tetrahedra = delaunay.getValidTetrahedra();

		computeVoronoiCell(tetrahedra);
	}

	std::unordered_map<int, VoronoiCell> Voronoi3D::getVoronoiCells()
	{
		return voronoiCells;
	}

	void Voronoi3D::computeVoronoiCell(const std::vector<Tetrahedron>& tetrahedra)
	{
		// ��ǰ�������ӵ��͹���������ж��Ƿ񿪷�
		std::vector<std::vector<int>> convexHullFaces = computeConvexHull(points);
		std::unordered_set<int> convexHullVertices;
		for (const auto& face : convexHullFaces)
		{
			for (int vertex : face)
			{
				convexHullVertices.insert(vertex);
			}
		}

		// �������е�
		for (int index = 0; index < points.size(); ++index)
		{
			VoronoiCell cell;
			// ���ݵ�����ȷ��Voronoi��Ԫ�Ƿ񿪷�
			if (convexHullVertices.find(index) != convexHullVertices.end())
			{
				cell.isOpen = true;
			}
			else
			{
				cell.isOpen = false;
			}

			// ���������壬�ռ����а����õ��������
			for (const auto& tetrahedron : tetrahedra)
			{
				for (int i = 0; i < 4; ++i)
				{
					if (tetrahedron.vertices[i] == index)
					{
						cell.vertices.push_back(tetrahedron.circumsphere.center);
						break;
					}
				}
			}

			if (cell.isOpen)
			{
				// ����Զ��
				Vector3D farPoint = computeFarPoint(points[index]);
				cell.vertices.push_back(farPoint);
			}

			// ����Voronoi��Ԫ��͹��
			computeVoronoiCellHull(cell);
			// ��Voronoi��Ԫ��ӵ������
			voronoiCells[index] = cell;
		}
	}

	void Voronoi3D::computeVoronoiCellHull(VoronoiCell& cell)
	{
		std::vector<Vector3D> hullVertices;
		std::vector<std::vector<int>> hullFaces = computeConvexHull(cell.vertices);
		cell.faces = hullFaces;
	}

	Vector3D Voronoi3D::computeFarPoint(const Vector3D& point)
	{
		// ����Զ�㷽��
		Vector3D dir = point - center;

		// �����㹻���ϵ��
		const float SCALE = 1e6;

		// ����Զ��
		Vector3D farPoint = center + dir * SCALE;

		return farPoint;
	}



	std::unordered_map<int, Mesh> VoronoiFracture::generateVoronoiFracture(const ModelPreprocessor& inputMesh, const std::vector<Vector3D>& points, std::vector<Vector3D>& mappedPoints)
	{
		// 1. ������������Ƿ�պ�
		if (!inputMesh.getClosed())
		{
			std::cerr << "Input mesh is not closed." << std::endl;
			return std::unordered_map<int, Mesh>();
		}

		// 2. �����ӵ�ӳ�䵽ģ���ڲ�
		std::vector<Vector3D> mappedSeeds;
		for (const auto& seed : points)
		{
			Vector3D mappedSeed = mapSeedToModel(seed, inputMesh);
			mappedSeeds.push_back(mappedSeed);
			mappedPoints.push_back(mappedSeed);
		}

		// 3. Ϊÿ�����ӵ㴴��Voronoi��Ԫ
		Voronoi3D voronoi(mappedSeeds);
		Vector3D center = (inputMesh.getBoundingBoxMin() + inputMesh.getBoundingBoxMax()) * 0.5f;
		voronoi.buildVoronoi(center);
		// ��ȡVoronoi��Ԫ
		std::unordered_map<int, VoronoiCell> voronoiCells = voronoi.getVoronoiCells();

		// ʹ��AABB���ٲ���Ǳ�ڵ��ཻģ����
		// ����Voronoi��Ԫ��AABB
		std::unordered_map<int, AABB> voronoiCellAABBs;
		for (const auto& pair : voronoiCells)
		{
			voronoiCellAABBs[pair.first] = calculateVoronoiCellAABB(pair.second);
		}

		// ��������������������AABB
		std::vector<AABB> inputMeshAABBs;
		for (const auto& triangle : inputMesh.getModelMesh().triangles)
		{
			AABB aabb = calculateTriangleAABB(triangle);
			inputMeshAABBs.push_back(aabb);
		}

		// ����Voronoi��Ԫ��AABB�����������AABB�����ཻ����
		std::unordered_map<int, std::vector<int>> voronoiCellTriangles;
		for (const auto& pair : voronoiCellAABBs)
		{
			int cellIndex = pair.first;
			const AABB& cellAABB = pair.second;
			for (size_t i = 0; i < inputMeshAABBs.size(); ++i)
			{
				if (aabbIntersect(cellAABB, inputMeshAABBs[i]))
				{
					// ���Voronoi��Ԫ��AABB�����������AABB�ཻ������Ӹ�������
					voronoiCellTriangles[cellIndex].push_back(static_cast<int>(i));
				}
			}
		}

		// 4. ��ȷ��ȡVoronoi��Ԫ���ڲ�����ཻ��
		// �ڲ���: ��ȫ��Voronoi��Ԫ�ڲ����棬ֱ�ӱ���
		// �ཻ��: ��Voronoi��Ԫ�ཻ���棬�����и�

		// Voronoi��Ԫ���������б�
		std::unordered_map<int, std::vector<Triangle>> voronoiCellTrianglesMap;

		// ��Voronoi��Ԫ�ཻ���������б�
		std::unordered_map<int, std::vector<Triangle>> intersectingTrianglesMap;

		for (const auto& pair : voronoiCellTriangles)
		{
			int cellIndex = pair.first;
			const std::vector<int>& triangleIndices = pair.second;

			for (int triangleIndex : triangleIndices)
			{
				const Triangle& triangle = inputMesh.getModelMesh().triangles[triangleIndex];

				bool isAllInside = true;
				bool isAnyInside = false;

				// ����������Ƿ���ȫ��Voronoi��Ԫ�ڲ�
				for (const auto& vertex : triangle.vertices)
				{
					if (pointInVoronoiCell(vertex, voronoiCells[cellIndex]))
					{
						isAnyInside = true;
					}
					else
					{
						isAllInside = false;
					}
				}

				if (isAllInside)
				{
					// �����������ȫ��Voronoi��Ԫ�ڲ���ֱ�����
					voronoiCellTrianglesMap[cellIndex].push_back(triangle);
				}
				else if (isAnyInside || triangleIntersectVoronoiCell(triangle, voronoiCells[cellIndex]))
				{
					// �������ڲ����ཻ
					intersectingTrianglesMap[cellIndex].push_back(triangle);
				}
			}
		}

		// 5. �и��ཻ�������Σ����������ӵ�Voronoi��Ԫ���������б���
		// ͬʱ��¼��ģ���ཻ��Voronoi��Ԫ���������棬���ٺ�������
		std::unordered_map<int, std::set<int>> intersectingVoronoiTriangles;

		for (auto& pair : intersectingTrianglesMap)
		{
			int cellIndex = pair.first;
			const std::vector<Triangle>& intersectingTriangles = pair.second;

			for (const auto& triangle : intersectingTriangles)
			{
				// ��¼Voronoi��Ԫ����ģ���ཻ���������������
				std::vector<int> triangleIndices;
				// �и�������
				std::vector<Triangle> cutTriangles = cutTrianglesByVoronoiCell(triangle, voronoiCells[cellIndex], triangleIndices);
				// ���и�����������ӵ�Voronoi��Ԫ���������б���
				voronoiCellTrianglesMap[cellIndex].insert(voronoiCellTrianglesMap[cellIndex].end(), cutTriangles.begin(), cutTriangles.end());
				// ��¼��ģ���ཻ������������
				intersectingVoronoiTriangles[cellIndex].insert(triangleIndices.begin(), triangleIndices.end());
			}
		}

		// 6. ���Voronoi��Ԫ�������κϷ��ԣ����������������б�
		std::unordered_map<int, Mesh> resultMeshes;
		
		for (auto& pair : voronoiCellTrianglesMap)
		{
			int cellIndex = pair.first;
			VoronoiCell cell = voronoiCells[cellIndex];
			const std::vector<Triangle>& triangles = pair.second;

			Mesh mesh;

			if (triangles.empty())
			{
				for (auto& face : cell.faces)
				{
					// ���Voronoi��Ԫ����Ϊ�գ�˵��Voronoi��Ԫ��ȫ��ģ���ڲ�
					// Voronoi��Ԫ�������ֱ�����
					Vector3D v0 = cell.vertices[face[0]];
					Vector3D v1 = cell.vertices[face[1]];
					Vector3D v2 = cell.vertices[face[2]];

					mesh.addTriangle(v0, v1, v2);
				}
			}
			else
			{
				std::vector<int> vertexIndices;
				// ����ģ�͵����������Voronoi��Ԫ�Ķ�����л���
				for (int i = 0; i < cell.vertices.size(); ++i)
				{
					const auto& vertex = cell.vertices[i];
					bool isInside = true;
					for (const auto& triangle : triangles)
					{
						Vector3D normal = calculateTriangleNormal(
							triangle.vertices[0], triangle.vertices[1], triangle.vertices[2]);

						Vector3D planePoint = triangle.vertices[0];
						float distance = (vertex - planePoint).dot(normal);
						if (distance > 0)
						{
							isInside = false;
							break;
						}
					}
					if (isInside)
					{
						vertexIndices.push_back(i);
					}
				}

				std::vector<int> intersectingTriangles;
				for (int i = 0; i < cell.faces.size(); i++)
				{
					const auto& face = cell.faces[i];
					bool isInside = true;

					for (const auto& index : face)
					{
						if (std::find(vertexIndices.begin(), vertexIndices.end(), index) == vertexIndices.end())
						{
							isInside = false;
							break;
						}
					}

					if (isInside)
					{
						// ���Voronoi��Ԫ������ȫ��ģ���ڲ���ֱ��������������
						Vector3D v0 = cell.vertices[face[0]];
						Vector3D v1 = cell.vertices[face[1]];
						Vector3D v2 = cell.vertices[face[2]];
						mesh.addTriangle(v0, v1, v2);
					}
					else
					{
						intersectingTriangles.push_back(i);
					}
				}

				// ��¼Voronoi��Ԫ�Ķ�������
				int index = 0;

				std::unordered_map<Vector3D, int> vertexMap;
				for (const auto& triangle : triangles)
				{
					for (const auto& vertex : triangle.vertices)
					{
						if (vertexMap.find(vertex) == vertexMap.end())
						{
							vertexMap[vertex] = index++;
						}
					}
				}

				std::vector<Vector3D> verticesVec(vertexMap.size());
				for (const auto& pair : vertexMap)
				{
					verticesVec[pair.second] = pair.first;
				}

				// ����ģ�͵������棬���ұ߽�ߣ�����Voronoi��Ԫ�ཻ�ıߣ�
				std::unordered_map<int, std::set<int>> edgeMap;
				for (const auto& triangle : triangles)
				{
					for (int i = 0; i < 3; i++)
					{
						int v0 = vertexMap[triangle.vertices[i]];
						int v1 = vertexMap[triangle.vertices[(i + 1) % 3]];
						
						// ���������淨�������⣬������������ıߣ�������ı߷����෴
						if (edgeMap.find(v1) != edgeMap.end() &&
							edgeMap[v1].find(v0) != edgeMap[v1].end())
						{
							edgeMap[v1].erase(v0);
						}
						else
						{
							edgeMap[v0].insert(v1);
						}
					}
				}

				// ��ȡ���б߽�ߵĶ���
				std::set<Vector3D> boundaryVertices;
				for (const auto& pair : edgeMap)
				{
					int vertexIndex = pair.first;
					boundaryVertices.insert(verticesVec[vertexIndex]);

					for (const auto& neighborIndex : pair.second)
					{
						boundaryVertices.insert(verticesVec[neighborIndex]);
					}
				}

				// ��¼Voronoi��Ԫ��ģ���ཻ����������������Ӧ����
				std::unordered_map<int, std::vector<Vector3D>> verticesToTrianglesMap;
				for (const auto& vertex : boundaryVertices)
				{
					// ����Voronoi��Ԫ��������
					for (auto& trianleIndex : intersectingTriangles)
					{
						const auto& face = cell.faces[trianleIndex];
						Triangle triangle(
							cell.vertices[face[0]],
							cell.vertices[face[1]],
							cell.vertices[face[2]]);
						if (pointInTriangle(vertex, triangle))
						{
							// ��������������ڣ�����ӵ���Ӧ����������
							verticesToTrianglesMap[trianleIndex].push_back(vertex);
						}
					}
				}

				// �и�Voronoi���������
				for (auto& pair : verticesToTrianglesMap)
				{
					int triangleIndex = pair.first;
					const auto& face = cell.faces[triangleIndex];

					Vector3D v0 = cell.vertices[face[0]];
					Vector3D v1 = cell.vertices[face[1]];
					Vector3D v2 = cell.vertices[face[2]];

					Vector3D normal = calculateTriangleNormal(v0, v1, v2);

					auto& vertices = pair.second;

					// ȷ��Voronoi��Ԫ������Ķ����Ƿ���ģ���ڲ�
					// �������ڲ��Ķ�����ӵ���ƽ��Ķ����б���
					if (std::find(vertexIndices.begin(), vertexIndices.end(), face[0]) != vertexIndices.end())
					{
						vertices.push_back(v0);
					}
					if (std::find(vertexIndices.begin(), vertexIndices.end(), face[1]) != vertexIndices.end())
					{
						vertices.push_back(v1);
					}
					if (std::find(vertexIndices.begin(), vertexIndices.end(), face[2]) != vertexIndices.end())
					{
						vertices.push_back(v2);
					}

					// ���ݶ����б�������β��и�
					// ������ӳ����Voronoi��Ԫ��������ƽ��
					// ƽ�淽��: Ax + By + Cz + D = 0
					float A = normal.x;
					float B = normal.y;
					float C = normal.z;
					float D = -normal.dot(v0);

					// �����ֲ���ά����ϵ
					// ��v0Ϊԭ�㣬v1Ϊx��
					Vector3D xAxis = (v1 - v0).normalize();
					Vector3D yAxis = normal.cross(xAxis).normalize();

					std::vector<Point2D> projectedVertices;
					for (const auto& vertex : vertices)
					{
						Vector3D offset = vertex - v0;
						float x = offset.dot(xAxis);
						float y = offset.dot(yAxis);
						projectedVertices.push_back({ x, y });
					}

					std::vector<Point2D> hull2D = calculateHull2D(projectedVertices);

					// ӳ�����ά�ռ�
					std::vector<Vector3D> hullVertices;
					for (const auto& point : hull2D)
					{
						Vector3D mappedPoint = v0 +  xAxis * point.x + yAxis * point.y;
						hullVertices.push_back(mappedPoint);
					}

					// �жϷ�������ԭʼ�����η������ķ����Ƿ�һ��
					Vector3D newNormal = calculateTriangleNormal(
						hullVertices[0], hullVertices[1], hullVertices[2]);

					if (newNormal.dot(normal) < 0)
					{
						// �������������һ�£���ת����˳��
						std::reverse(hullVertices.begin(), hullVertices.end());
					}

					for (int i = 0; i < hullVertices.size() - 2; ++i)
					{
						// ���и�����������ӵ�������
						mesh.addTriangle(hullVertices[0], hullVertices[i + 1], hullVertices[i + 2]);
					}
				}

				// ��ģ�͵���������ӵ�������
				for (const auto& triangle : triangles)
				{
					Vector3D v0 = triangle.vertices[0];
					Vector3D v1 = triangle.vertices[1];
					Vector3D v2 = triangle.vertices[2];
					mesh.addTriangle(v0, v1, v2);
				}
			}

			// ��������ӵ������
			resultMeshes[cellIndex] = mesh;
		}

		// 7. ���ؽ��
		return resultMeshes;
	}

	Vector3D VoronoiFracture::findPointInsideMesh(const ModelPreprocessor& model)
	{
		// ʹ�����ж����ƽ��ֵ��Ϊ�ڲ����ѡ
		Vector3D centroid(0, 0, 0);

		const Mesh& mesh = model.getModelMesh();
		
		for (const auto& v : mesh.vertices)
		{
			centroid = centroid + v;
		}

		centroid = centroid * (1.0f / mesh.vertices.size());

		if (model.isPointInsideModel(centroid))
		{
			return centroid;
		}

		// ���ƽ��ֵ���������ڲ����ضԽ��߷�������
		Vector3D minCorner = model.getBoundingBoxMin();
		Vector3D maxCorner = model.getBoundingBoxMax();

		Vector3D searchDirection = maxCorner - minCorner;
		float stepSize = 0.1f; // ����

		for (float t = 0; t < 1.0f; t += stepSize)
		{
			Vector3D testPoint = minCorner + searchDirection * t;
			if (model.isPointInsideModel(testPoint))
			{
				return testPoint;
			}
		}

		// ���û���ҵ����ʵĵ㣬���ص�һ�������ε�����
		if (!mesh.triangles.empty())
		{
			return calculateCentroid(mesh.triangles[0]);
		}

		// ���û�������Σ�����ԭ��
		return Vector3D(0, 0, 0);
	}

	Vector3D VoronoiFracture::mapSeedToModel(const Vector3D& seed, const ModelPreprocessor& model)
	{
		Vector3D minCorner = model.getBoundingBoxMin();
		Vector3D maxCorner = model.getBoundingBoxMax();

		Vector3D size = maxCorner - minCorner;

		// ��ʼ��ӳ������Χ��
		Vector3D mappedSeed = Vector3D(
			seed.x * size.x + minCorner.x,
			seed.y * size.y + minCorner.y,
			seed.z * size.z + minCorner.z
		);

		// ��ӳ����������ڲ����򷵻�ӳ���
		if (model.isPointInsideModel(mappedSeed))
		{
			return mappedSeed;
		}

		// �����������ڲ����ƶ�ֱ������ģ���ڲ�
		Vector3D innerPoint = findPointInsideMesh(model);
		Vector3D direction = (innerPoint - mappedSeed).normalize();

		float stepSize = size.length() * 0.01f; // С����
		// ������100��
		for (int i = 0; i < 100; ++i)
		{
			mappedSeed = mappedSeed + direction * stepSize;
			if (model.isPointInsideModel(mappedSeed))
			{
				return mappedSeed;
			}
		}

		// �����Ȼ���������ڲ��������ڲ���
		return innerPoint;
	}

	bool VoronoiFracture::pointInTriangle(const Vector3D& point, const Triangle& triangle)
	{
		// ʹ���������귨�жϵ��Ƿ�����������
		Vector3D v0 = triangle.vertices[1] - triangle.vertices[0];
		Vector3D v1 = triangle.vertices[2] - triangle.vertices[0];
		Vector3D v2 = point - triangle.vertices[0];

		// ������������ε���������
		float dot00 = v0.dot(v0);
		float dot01 = v0.dot(v1);
		float dot02 = v0.dot(v2);
		float dot11 = v1.dot(v1);
		float dot12 = v1.dot(v2);

		// ���������ε����
		float D = dot00 * dot11 - dot01 * dot01;

		if (D < 1e-6f) // ����������ӽ�0
			return false;

		float invDenom = 1.0f / D;
		float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
		float v = (dot00 * dot12 - dot01 * dot02) * invDenom;

		return (u >= 0) && (v >= 0) && (u + v <= 1);
	}

	int VoronoiFracture::findClosedSeed(const Vector3D& point, const std::vector<Vector3D>& seeds)
	{
		// �ҵ������������������ӵ�
		int closedIdx = 0;
		float minDist = std::numeric_limits<float>::max();

		for (size_t i = 0; i < seeds.size(); ++i)
		{
			// ���㵱ǰ���ӵ��������ľ����ƽ��
			float dist = (point - seeds[i]).lengthSquared();
			if (dist < minDist)
			{
				minDist = dist;
				closedIdx = static_cast<int>(i);
			}
		}

		// ������������ӵ�����
		return closedIdx;
	}

	Vector3D VoronoiFracture::calculateCentroid(const Triangle& triangle)
	{
		return Vector3D(
			(triangle.vertices[0].x + triangle.vertices[1].x + triangle.vertices[2].x) / 3.0f,
			(triangle.vertices[0].y + triangle.vertices[1].y + triangle.vertices[2].y) / 3.0f,
			(triangle.vertices[0].z + triangle.vertices[1].z + triangle.vertices[2].z) / 3.0f
		);
	}

	int getOppositeVertex(const Tetrahedron& tetrahedron, int faceIndex, const Tetrahedron& neighbor)
	{
		// ��ȡ��ǰ������Ĺ����涥��
		const auto& sharedFaceVertices = tetrahedron.getFaceVertices(faceIndex);

		for (int i = 0; i < 4; ++i)
		{
			int vertex = neighbor.vertices[i];
			bool isShared = false;

			// ��鵱ǰ�����Ƿ��ڹ����涥����
			for (const auto sharedVertex : sharedFaceVertices)
			{
				if (vertex == sharedVertex)
				{
					isShared = true;
					break;
				}
			}

			if (!isShared)
			{
				// ���ص�ǰ����
				return vertex;
			}
		}

		// ���û���ҵ�������-1
		return -1;
	}

	Sphere calculateCircumsphere(const Vector3D& a, const Vector3D& b, const Vector3D& c, const Vector3D& d)
	{
		// ʹ�ÿ���ķ���������������ĺͰ뾶
		Vector3D ab = b - a;
		Vector3D ac = c - a;
		Vector3D ad = d - a;

		float volume = ab.dot(ac.cross(ad));

		// �ĵ㹲��ʱ��������Ч�������
		if (std::abs(volume) < std::numeric_limits<float>::epsilon())
		{
			return Sphere(Vector3D(0, 0, 0), 0); // ������Ч�������
		}

		float D = det3x3(
			ab.x, ab.y, ab.z,
			ac.x, ac.y, ac.z,
			ad.x, ad.y, ad.z);

		float P = (b.lengthSquared() - a.lengthSquared()) / 2.0f;
		float Q = (c.lengthSquared() - a.lengthSquared()) / 2.0f;
		float R = (d.lengthSquared() - a.lengthSquared()) / 2.0f;

		// �ֱ������������ĵ�X, Y, Zֵ
		float centerX = det3x3(
			P, ab.y, ab.z,
			Q, ac.y, ac.z,
			R, ad.y, ad.z) / D;
		float centerY = det3x3(
			ab.x, P, ab.z,
			ac.x, Q, ac.z,
			ad.x, R, ad.z) / D;
		float centerZ = det3x3(
			ab.x, ab.y, P,
			ac.x, ac.y, Q,
			ad.x, ad.y, R) / D;

		Vector3D center(centerX, centerY, centerZ);

		// ���������İ뾶
		float radiusSquared = (center - a).lengthSquared();
		float radius = std::sqrt(radiusSquared);

		return Sphere(center, radius);
	}

	// �ж�������ķ���
	int orientation(const Vector3D& p, const Vector3D& q, const Vector3D& r, const Vector3D& s)
	{
		// �����Ľ�����ʽ
		float det = p.x * (q.y * (r.z - s.z) - r.y * (q.z - s.z) + s.y * (q.z - r.z)) - 
					p.y * (q.x * (r.z - s.z) - r.x * (q.z - s.z) + s.x * (q.z - r.z)) +
					p.z * (q.x * (r.y - s.y) - r.x * (q.y - s.y) + s.x * (q.y - r.y)) -
					(q.x * (r.y * s.z - r.z * s.y) - r.x * (q.y * s.z - q.z * s.y) + s.x * (q.y * r.z - q.z * r.y));

		constexpr float EPSILON = std::numeric_limits<float>::epsilon() * 1e4;

		if (det > EPSILON)
		{
			return 1; // ˳ʱ��
		}
		else if (det < -EPSILON)
		{
			return -1; // ��ʱ��
		}
		else
		{
			return 0; // ƽ��
		}
	}

	bool circumsphereContainsPoint(const Tetrahedron& tet, const Vector3D& point)
	{
		// �������������ĺͰ뾶
		Vector3D center = tet.circumsphere.center;
		float radiusSquared = tet.circumsphere.radiusSquared;
		// ����㵽��������ĵľ���ƽ��
		float distanceSquared = (point - center).lengthSquared();

		return distanceSquared <= radiusSquared;
	}

	float det3x3(const float a, const float b, const float c,
				const float d, const float e, const float f,
				const float g, const float h, const float i)
	{
		return a * (e * i - f * h)
			- b * (d * i - f * g)
			+ c * (d * h - e * g);
	}

	std::vector<std::vector<int>> computeConvexHull(const std::vector<Vector3D>& points)
	{
		std::vector<std::vector<int>> faces;

		// ���㼯�Ƿ�Ϊ��
		if (points.empty())
		{
			std::cerr << "Point set is empty." << std::endl;
			return faces;
		}

		// ���㼯�Ƿ��㹻����һ��͹��
		if (points.size() < 4)
		{
			std::cerr << "Not enough points to form a convex hull." << std::endl;
			return faces;
		}

		// ʹ��QuickHull�㷨����͹��
		// 1. ���ҳ�ʼ�����壨ǰ4��������ĵ㣩
		std::array<int, 4> initialTetrahedron;
		bool found = false;
		for (int i = 0; i < points.size() && !found; ++i)
		{
			for (int j = i + 1; j < points.size() && !found; ++j)
			{
				for (int k = j + 1; k < points.size() && !found; ++k)
				{
					for (int l = k + 1; l < points.size() && !found; ++l)
					{
						if (!arePointsCoplanar(points[i], points[j], points[k], points[l]))
						{
							initialTetrahedron = { i, j, k, l };
							found = true;
						}
					}
				}
			}
		}
		
		// ����Ƿ��ҵ���ʼ������
		if (!found)
		{
			std::cerr << "No valid tetrahedron found." << std::endl;
			return faces;
		}

		// 2. ��ʼ��͹���棬��ȷ����Ϊ������
		std::vector<int> face1 = { initialTetrahedron[0], initialTetrahedron[1], initialTetrahedron[2] };
		std::vector<int> face2 = { initialTetrahedron[0], initialTetrahedron[1], initialTetrahedron[3] };
		std::vector<int> face3 = { initialTetrahedron[0], initialTetrahedron[2], initialTetrahedron[3] };
		std::vector<int> face4 = { initialTetrahedron[1], initialTetrahedron[2], initialTetrahedron[3] };

		// ����������
		Vector3D centroid = (points[initialTetrahedron[0]] +
			points[initialTetrahedron[1]] +
			points[initialTetrahedron[2]] +
			points[initialTetrahedron[3]]) * (1.0f / 4.0f);

		// ���㷨��
		Vector3D normal1 = calculateTriangleNormal(
			points[face1[0]], points[face1[1]], points[face1[2]]);
		Vector3D normal2 = calculateTriangleNormal(
			points[face2[0]], points[face2[1]], points[face2[2]]);
		Vector3D normal3 = calculateTriangleNormal(
			points[face3[0]], points[face3[1]], points[face3[2]]);
		Vector3D normal4 = calculateTriangleNormal(
			points[face4[0]], points[face4[1]], points[face4[2]]);

		// ȷ�����߳���(��������)
		if (normal1.dot(centroid - points[face1[0]]) > 0)
		{
			std::reverse(face1.begin(), face1.end());
		}

		if (normal2.dot(centroid - points[face2[0]]) > 0)
		{
			std::reverse(face2.begin(), face2.end());
		}

		if (normal3.dot(centroid - points[face3[0]]) > 0)
		{
			std::reverse(face3.begin(), face3.end());
		}

		if (normal4.dot(centroid - points[face4[0]]) > 0)
		{
			std::reverse(face4.begin(), face4.end());
		}

		// ��ӳ�ʼ���������
		faces.push_back(face1);
		faces.push_back(face2);
		faces.push_back(face3);
		faces.push_back(face4);


		// 3. ����ʽ��ӵ�
		for (int p = 0; p < points.size(); p++)
		{
			// ������ʼ������ĵ�
			if (std::find(initialTetrahedron.begin(), initialTetrahedron.end(), p) != initialTetrahedron.end())
			{
				continue;
			}

			// ���ҿɼ���
			std::vector<int> visibleFaces;
			for (int f = 0; f < faces.size(); ++f)
			{
				const auto& face = faces[f];

				Vector3D normal = calculateTriangleNormal(
					points[face[0]], points[face[1]], points[face[2]]);

				Vector3D vec = points[p] - points[face[0]];

				if (normal.dot(vec) > 1e-6f) // ��������
				{
					// ����ɼ�
					visibleFaces.push_back(f);
				}
			}

			// �ϲ��ɼ���
			std::vector<std::vector<int>> newFaces;
			std::unordered_map<int, std::unordered_set<int>> edges;

			for (int f : visibleFaces)
			{
				const auto& face = faces[f];

				for (int i = 0; i < 3; i++)
				{
					int start = face[i];
					int end = face[(i + 1) % 3];
					
					if (edges.find(end) != edges.end())
					{
						if (edges[end].find(start) != edges[end].end())
						{
							// ���Ѵ��ڣ�ɾ��
							edges[end].erase(start);
						}
						else
						{
							// ��ӱ�
							edges[start].insert(end);
						}
					}
					else
					{
						// ��ӱ�
						edges[start].insert(end);
					}
				}
			}

			// ��������
			for (const auto& edge : edges)
			{
				int start = edge.first;

				for (int end : edge.second)
				{
					std::vector<int> newFace = { start, end, p };
					newFaces.push_back(newFace);
				}
			}

			// 4. ɾ���ɼ��棬���������
			// Ϊ�˱����ڵ������޸�������ʹ��һ����ʱ�����洢���º����
			std::vector<std::vector<int>> updatedFaces;
			for (int f = 0; f < faces.size(); ++f)
			{
				if (std::find(visibleFaces.begin(), visibleFaces.end(), f) == visibleFaces.end())
				{
					updatedFaces.push_back(faces[f]);
				}
			}

			// �������
			updatedFaces.insert(updatedFaces.end(), newFaces.begin(), newFaces.end());
			faces = std::move(updatedFaces);
		}

		return faces;
	}

	bool arePointsCoplanar(const Vector3D& p1, const Vector3D& p2, const Vector3D& p3, const Vector3D& p4)
	{
		Vector3D v1 = p2 - p1;
		Vector3D v2 = p3 - p1;
		Vector3D v3 = p4 - p1;

		// ��������ʽ
		float det = det3x3(
			v1.x, v1.y, v1.z,
			v2.x, v2.y, v2.z,
			v3.x, v3.y, v3.z);

		constexpr float EPSILON = 1e-6f;

		return std::abs(det) < EPSILON; // �ж�����ʽ�Ƿ�ӽ�0
	}

	Vector3D calculateTriangleNormal(const Vector3D& p1, const Vector3D& p2, const Vector3D& p3)
	{
		Vector3D v1 = p2 - p1;
		Vector3D v2 = p3 - p1;

		Vector3D normal = v1.cross(v2).normalize();

		return normal;
	}

	AABB calculateTriangleAABB(const Triangle& triangle)
	{
		// ������Сֵ
		Vector3D min(
			std::min({ triangle.vertices[0].x, triangle.vertices[1].x, triangle.vertices[2].x }),
			std::min({ triangle.vertices[0].y, triangle.vertices[1].y, triangle.vertices[2].y }),
			std::min({ triangle.vertices[0].z, triangle.vertices[1].z, triangle.vertices[2].z })
		);
		// �������ֵ
		Vector3D max(
			std::max({ triangle.vertices[0].x, triangle.vertices[1].x, triangle.vertices[2].x }),
			std::max({ triangle.vertices[0].y, triangle.vertices[1].y, triangle.vertices[2].y }),
			std::max({ triangle.vertices[0].z, triangle.vertices[1].z, triangle.vertices[2].z })
		);

		return { min, max };
	}

	AABB calculateVoronoiCellAABB(const VoronoiCell& cell)
	{
		Vector3D min(
			std::numeric_limits<float>::max(),
			std::numeric_limits<float>::max(),
			std::numeric_limits<float>::max()
		);
		Vector3D max(
			std::numeric_limits<float>::lowest(),
			std::numeric_limits<float>::lowest(),
			std::numeric_limits<float>::lowest()
		);

		// ������Сֵ�����ֵ
		for (auto& vertex : cell.vertices)
		{
			min.x = std::min(min.x, vertex.x);
			min.y = std::min(min.y, vertex.y);
			min.z = std::min(min.z, vertex.z);

			max.x = std::max(max.x, vertex.x);
			max.y = std::max(max.y, vertex.y);
			max.z = std::max(max.z, vertex.z);
		}
		
		return { min, max };
	}

	bool aabbIntersect(const AABB& a, const AABB& b)
	{
		return (a.max.x >= b.min.x && a.min.x <= b.max.x) &&
			(a.max.y >= b.min.y && a.min.y <= b.max.y) &&
			(a.max.z >= b.min.z && a.min.z <= b.max.z);
	}

	bool pointInVoronoiCell(const Vector3D& point, const VoronoiCell& cell)
	{
		// ����Voronoi��Ԫ��͹������
		for (const auto& face : cell.faces)
		{
			Vector3D normal = calculateTriangleNormal(
				cell.vertices[face[0]], cell.vertices[face[1]], cell.vertices[face[2]]);
			Vector3D v0 = cell.vertices[face[0]].lengthSquared() < cell.vertices[face[1]].lengthSquared() ?
				cell.vertices[face[0]] : cell.vertices[face[1]];
			
			Vector3D vec = (point - v0).normalize();

			// �жϵ��Ƿ�������
			if (normal.dot(vec) > 1e-6f)
			{
				return false;
			}
		}

		// ����Voronoi��Ԫ��
		return true;
	}

	bool triangleIntersectVoronoiCell(const Triangle& triangle, const VoronoiCell& cell)
	{
		for (int i = 0; i < cell.faces.size(); i++)
		{
			const auto& face = cell.faces[i];

			Triangle plane(
				cell.vertices[face[0]],
				cell.vertices[face[1]],
				cell.vertices[face[2]]);

			// �����������Voronoi��Ԫ�����ཻ
			if (triangleIntersectTriangle(triangle, plane))
			{
				// �ཻ
				return true;
			}
		}

		// ���ཻ
		return false;
	}

	bool triangleIntersectTriangle(const Triangle& triangle, const Triangle& plane)
	{
		// ���������εķ���
		Vector3D n1 = calculateTriangleNormal(
			triangle.vertices[0], triangle.vertices[1], triangle.vertices[2]);

		// ����ƽ��ķ���
		Vector3D n2 = calculateTriangleNormal(
			plane.vertices[0], plane.vertices[1], plane.vertices[2]);

		// ��鷨���Ƿ�ƽ��
		if (n1.cross(n2).lengthSquared() < 1e-6f)
		{
			// ����Ƿ���ͬһƽ����
			if (std::abs(n1.dot(plane.vertices[0] - triangle.vertices[0])) < 1e-6f)
			{
				// ��ͬһƽ����
				// ������б�-���ཻ
				for (int i = 0; i < 3; ++i)
				{
					for (int j = 0; j < 3; ++j)
					{
						// ����-���ཻ
						if (edgeIntersectEdge(
							Edge(triangle.vertices[i], triangle.vertices[(i + 1) % 3]),
							Edge(plane.vertices[j], plane.vertices[(j + 1) % 3])))
						{
							return true; // �ཻ
						}
					}
				}

				// ��������ϵ
				return pointInTriangle(plane.vertices[0], triangle) ||
					pointInTriangle(triangle.vertices[0], plane);
			}
			else
			{
				// ����ͬһƽ����
				return false;
			}
		}
		else
		{
			// ���㽻��
			Vector3D lineDir = n1.cross(n2);
			
			// ���������������ڽ����ϵ�ͶӰ
			float min1 = std::numeric_limits<float>::max();
			float max1 = std::numeric_limits<float>::lowest();
			float min2 = std::numeric_limits<float>::max();
			float max2 = std::numeric_limits<float>::lowest();

			for (const auto& vertex : triangle.vertices)
			{
				float projection = lineDir.dot(vertex);
				min1 = std::min(min1, projection);
				max1 = std::max(max1, projection);
			}

			for (const auto& vertex : plane.vertices)
			{
				float projection = lineDir.dot(vertex);
				min2 = std::min(min2, projection);
				max2 = std::max(max2, projection);
			}

			// ���ͶӰ�Ƿ��ཻ
			if (min1 > max2 || min2 > max1)
			{
				// ���ཻ
				return false;
			}
			else
			{
				// ���������
				return satTest(triangle, plane);
			}
		}
	}

	bool edgeIntersectEdge(const Edge& e1, const Edge& e2)
	{
		Vector3D dir1 = e1.end - e1.start;
		Vector3D dir2 = e2.end - e2.start;

		Vector3D r = e1.start - e2.start;

		double a = dir1.dot(dir1);
		double b = dir1.dot(dir2);
		double c = dir2.dot(dir2);
		double d = dir1.dot(r);
		double e = dir2.dot(r);

		double det = a * c - b * b;
		if (std::abs(det) < 1e-6f)
		{
			// ƽ��
			return false;
		}

		double s = (b * e - c * d) / det;
		double t = (a * e - b * d) / det;

		return (s >= 0 && s <= 1 && t >= 0 && t <= 1);
	}

	bool pointInTriangle(const Vector3D& point, const Triangle& triangle)
	{
		Vector3D v0 = triangle.vertices[1] - triangle.vertices[0];
		Vector3D v1 = triangle.vertices[2] - triangle.vertices[0];
		Vector3D v2 = point - triangle.vertices[0];

		double dot00 = v0.dot(v0);
		double dot01 = v0.dot(v1);
		double dot02 = v0.dot(v2);
		double dot11 = v1.dot(v1);
		double dot12 = v1.dot(v2);

		double invDenom = 1.0 / (dot00 * dot11 - dot01 * dot01);
		double u = (dot11 * dot02 - dot01 * dot12) * invDenom;
		double v = (dot00 * dot12 - dot01 * dot02) * invDenom;

		return (u >= 0) && (v >= 0) && (u + v <= 1);
	}

	bool satTest(const Triangle& triangle, const Triangle& plane)
	{
		// ������������������εķ��ߺͱߵĲ��
		std::vector<Vector3D> axes;
		axes.push_back(calculateTriangleNormal(
			triangle.vertices[0], triangle.vertices[1], triangle.vertices[2]));
		axes.push_back(calculateTriangleNormal(
			plane.vertices[0], plane.vertices[1], plane.vertices[2]));
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				Vector3D edge1 = triangle.vertices[(i + 1) % 3] - triangle.vertices[i];
				Vector3D edge2 = plane.vertices[(j + 1) % 3] - plane.vertices[j];
				Vector3D axis = edge1.cross(edge2);
				if (axis.lengthSquared() > 1e-6f)
				{
					axes.push_back(axis);
				}
			}
		}

		// ��ÿ�������ͶӰ
		for (const auto& axis : axes)
		{
			float min1 = std::numeric_limits<float>::max();
			float max1 = std::numeric_limits<float>::lowest();
			float min2 = std::numeric_limits<float>::max();
			float max2 = std::numeric_limits<float>::lowest();
			for (const auto& vertex : triangle.vertices)
			{
				float projection = axis.dot(vertex);
				min1 = std::min(min1, projection);
				max1 = std::max(max1, projection);
			}
			for (const auto& vertex : plane.vertices)
			{
				float projection = axis.dot(vertex);
				min2 = std::min(min2, projection);
				max2 = std::max(max2, projection);
			}
			if (min1 > max2 || min2 > max1)
			{
				return false; // ����
			}
		}

		return true; // �ཻ
	}

	std::vector<Triangle> cutTrianglesByVoronoiCell(const Triangle& triangle, const VoronoiCell& cell, std::vector<int>& voronoiFaces)
	{
		std::vector<std::vector<int>> intersectingTriangles;

		int index = 0;

		// �����������Voronoi��Ԫ���ཻ
		for (const auto& face : cell.faces)
		{
			Triangle planeTriangle(
				cell.vertices[face[0]],
				cell.vertices[face[1]],
				cell.vertices[face[2]]
			);
			if (triangleIntersectTriangle(triangle, planeTriangle))
			{
				intersectingTriangles.push_back(face);
				voronoiFaces.push_back(index);
			}
			index++;
		}

		// �����ཻ�棬���ؿ�
		if (intersectingTriangles.empty())
		{
			return {};
		}

		// �������н���
		struct IntersectionPoint
		{
			// ��������
			Vector3D point;
			// �ཻ��ģ�������α�����
			// -1: ��ʾ������������
			// 0: 0-1
			// 1: 1-2
			// 2: 2-0
			int edgeIndex;
			// �������ڵ�Voronoi��Ԫ������
			int faceIndex;
		};
		// ��ϣ����
		struct IntersctionPointHash
		{
			size_t operator()(const IntersectionPoint& point) const
			{
				size_t h1 = std::hash<float>()(point.point.x);
				size_t h2 = std::hash<float>()(point.point.y);
				size_t h3 = std::hash<float>()(point.point.z);
				size_t h4 = std::hash<int>()(point.edgeIndex);
				size_t h5 = std::hash<int>()(point.faceIndex);

				return h1 ^ (h2 << 1) ^ (h3 << 2) ^ (h4 << 3) ^ (h5 << 4);
			}
		};
		// ����ж�
		struct IntersectionPointEqual
		{
			bool operator()(const IntersectionPoint& lhs, const IntersectionPoint& rhs) const
			{
				const float EPSILON = 1e-6f;
				bool positionEqual = (lhs.point - rhs.point).lengthSquared() < EPSILON * EPSILON;
				bool edgeEqual = (lhs.edgeIndex == rhs.edgeIndex);
				bool faceEqual = (lhs.faceIndex == rhs.faceIndex);

				return positionEqual && edgeEqual && faceEqual;
			}
		};

		std::vector<IntersectionPoint> intersectionPoints;

		for (int faceIndex = 0; faceIndex < intersectingTriangles.size(); ++faceIndex)
		{
			const auto& face = intersectingTriangles[faceIndex];
			Triangle planeTriangle(
				cell.vertices[face[0]],
				cell.vertices[face[1]],
				cell.vertices[face[2]]
			);

			// ��������ε�ÿ������Voronoi��Ԫ��Ľ���
			for (int edgeIndex = 0; edgeIndex < 3; ++edgeIndex)
			{
				Vector3D start = triangle.vertices[edgeIndex];
				Vector3D end = triangle.vertices[(edgeIndex + 1) % 3];
				// ���㽻��
				Vector3D intersectionPoint;

				// ������ƽ��Ľ���
				if (lineIntersectTriangle(start, end, planeTriangle, intersectionPoint))
				{
					intersectionPoints.push_back({ intersectionPoint, edgeIndex, faceIndex });
				}
			}

			// ���Voronoi��Ԫ���ÿ�����������εĽ���
			for (int edgeIndex = 0; edgeIndex < 3; ++edgeIndex)
			{
				Vector3D start = planeTriangle.vertices[edgeIndex];
				Vector3D end = planeTriangle.vertices[(edgeIndex + 1) % 3];
				// ���㽻��
				Vector3D intersectionPoint;
				// �����������εĽ���
				if (lineIntersectTriangle(start, end, triangle, intersectionPoint))
				{
					intersectionPoints.push_back({ intersectionPoint, -1, faceIndex });
				}
			}
		}

		// ȥ�ؽ���
		std::unordered_set<IntersectionPoint, IntersctionPointHash, IntersectionPointEqual> uniquePoints;
		for (const auto& point : intersectionPoints)
		{
			uniquePoints.insert(point);
		}

		// ���㽻������Voronoi��Ԫ�ڵĶ����͹��
		std::vector<Vector3D> allPoints;
		// �������������Voronoi��Ԫ�ڵĶ���
		for (const auto& vertex : triangle.vertices)
		{
			// ��鶥���Ƿ���Voronoi��Ԫ��
			if (pointInVoronoiCell(vertex, cell))
			{
				allPoints.push_back(vertex);
			}
		}
		for (const auto& point : uniquePoints)
		{
			// ����ض���Voronoi��Ԫ�ڣ�ʼ�����
			allPoints.push_back(point.point);
		}

		// ��������Voronoi��Ԫ�ڵĶ���Ͷ����������ƽ��

		// ƽ�淽��: Ax + By + Cz + D = 0
		// ����ƽ�淨��(A, B, C)
		Vector3D normal = calculateTriangleNormal(
			triangle.vertices[0], triangle.vertices[1], triangle.vertices[2]);
		// ����ƽ���ϵ�D
		float d = - normal.dot(triangle.vertices[0]);

		// �����ֲ���ά����ϵ
		// �������εĵ�һ������Ϊԭ��
		// �Ե�һ�����㵽�ڶ������������ΪX��
		Vector3D xAxis = (triangle.vertices[1] - triangle.vertices[0]).normalize();

		Vector3D yAxis = normal.cross(xAxis).normalize();

		std::vector<Point2D> projectedPoints;
		for (const auto& point : allPoints)
		{
			Vector3D direction = point - triangle.vertices[0];

			// ������ھֲ�����ϵ�е�����
			float x = direction.dot(xAxis);
			float y = direction.dot(yAxis);

			// ��ӵ�ͶӰ���б�
			projectedPoints.push_back({ x, y });
		}

		// ����͹��
		std::vector<Point2D> hull2D = calculateHull2D(projectedPoints);

		// Todo: ���������в���hull2d�㷨�Ƿ���ȷ����

		// ӳ�����ά�ռ�
		std::vector<Vector3D> hull3D;
		for (const auto& point : hull2D)
		{
			// ���������ά�ռ��е�����
			Vector3D projectedPoint = triangle.vertices[0] + xAxis * point.x + yAxis * point.y;
			hull3D.push_back(projectedPoint);
		}

		// ���ݷ������ж϶���εĶ���˳��
		Vector3D hullNormal = calculateTriangleNormal(
			hull3D[0], hull3D[1], hull3D[2]);

		bool isClockwise = (hullNormal.dot(normal) > 0);

		if (!isClockwise)
		{
			std::reverse(hull3D.begin(), hull3D.end());
		}

		// �и�������
		std::vector<Triangle> cutTriangles;
		for (int i = 1; i < hull3D.size() - 1; ++i)
		{
			// �����и������εĶ���
			Vector3D v1 = hull3D[i];
			Vector3D v2 = hull3D[(i + 1)];

			// �����и�������
			Triangle cutTriangle(v1, v2, triangle.vertices[0]);

			// ��ӵ��и��������б�
			cutTriangles.push_back(cutTriangle);
		}

		// �����и���������
		return cutTriangles;
	}

	bool lineIntersectTriangle(const Vector3D& lineStart, const Vector3D& lineEnd, const Triangle& triangle, Vector3D& intersectionPoint)
	{
		Vector3D edge1 = triangle.vertices[1] - triangle.vertices[0];
		Vector3D edge2 = triangle.vertices[2] - triangle.vertices[0];
		Vector3D normal = edge1.cross(edge2);

		// ��������������������ƽ��Ľ���
		Vector3D lineDir = lineEnd - lineStart;
		float d = normal.dot(lineDir);

		if (std::abs(d) < 1e-6f)
		{
			// ƽ��
			return false;
		}

		float t = normal.dot(triangle.vertices[0] - lineStart) / d;

		if (t < 0 || t > 1)
		{
			// �����߶���
			return false;
		}

		intersectionPoint = lineStart + lineDir * t;

		// ��齻���Ƿ�����������
		Vector3D v0 = triangle.vertices[1] - triangle.vertices[0];
		Vector3D v1 = triangle.vertices[2] - triangle.vertices[0];
		Vector3D v2 = intersectionPoint - triangle.vertices[0];

		float dot00 = v0.dot(v0);
		float dot01 = v0.dot(v1);
		float dot02 = v0.dot(v2);
		float dot11 = v1.dot(v1);
		float dot12 = v1.dot(v2);

		float invDenom = 1.0f / (dot00 * dot11 - dot01 * dot01);
		float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
		float v = (dot00 * dot12 - dot01 * dot02) * invDenom;

		if (u < 0 || v < 0 || u + v > 1)
		{
			// ������������
			return false;
		}

		// ��������������
		return true;
	}

	std::vector<Point2D> calculateHull2D(const std::vector<Point2D>& points)
	{
		if (points.size() <= 1)
		{
			return points;
		}
		else
		{
			std::vector<Point2D> pointsCopy = points;
			std::sort(pointsCopy.begin(), pointsCopy.end(), [](const Point2D& a, const Point2D& b) {
				return a.x < b.x || (a.x == b.x && a.y < b.y);
				});

			std::vector<Point2D> hull;

			// ����Andrew's Monotone Chain��
			// �����°벿��
			for (const auto& point : pointsCopy)
			{
				while (hull.size() >= 2)
				{
					const Point2D& p1 = hull[hull.size() - 1];
					const Point2D& p2 = hull[hull.size() - 2];
					
					double crossProduct = (p1.x - p2.x) * (point.y - p2.y) - (p1.y - p2.y) * (point.x - p2.x);

					if (crossProduct > 0)
					{
						break; // �γ���ת
					}
					else if (crossProduct == 0)
					{
						// ���ߣ�������Զ�ĵ�
						hull.pop_back();
					}
					else
					{
						hull.pop_back(); // �γ���ת
					}
				}
				hull.push_back(point);
			}

			// �����ϰ벿��
			size_t lowerSize = hull.size();
			for (int i = pointsCopy.size() - 2; i >= 0; --i)
			{
				const auto& point = pointsCopy[i];
				while (hull.size() >= lowerSize + 1)
				{
					const Point2D& p1 = hull[hull.size() - 1];
					const Point2D& p2 = hull[hull.size() - 2];

					double crossProduct = (p1.x - p2.x) * (point.y - p2.y) - (p1.y - p2.y) * (point.x - p2.x);

					if (crossProduct > 0)
					{
						break; // �γ���ת
					}
					else if (crossProduct == 0)
					{
						// ���ߣ�������Զ�ĵ�
						hull.pop_back();
					}
					else
					{
						hull.pop_back(); // �γ���ת
					}
				}
				hull.push_back(point);
			}

			// ȥ�����һ����
			hull.resize(hull.size() - 1);

			return hull;
		}
	}
}

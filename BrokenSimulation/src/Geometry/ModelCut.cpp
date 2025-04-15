#include "bspch.h"
#include "Geometry/ModelCut.h"

#include "ECS/ModelComponent.h"
#include "ECS/VoronoiComponent.h"
#include "Core/Log.h"

#include <glm/gtc/epsilon.hpp>

// ����std::hash����ģ����ػ�
template<>
struct std::hash<std::pair<int, int>>
{
	size_t operator()(const std::pair<int, int>& pair) const
	{
		return std::hash<int>()(pair.first) ^ (std::hash<int>()(pair.second) << 1);
	}
};

template<>
struct std::hash<std::tuple<int, int, int>>
{
	size_t operator()(const std::tuple<int, int, int>& tuple) const
	{
		return std::hash<int>()(std::get<0>(tuple)) ^ (std::hash<int>()(std::get<1>(tuple)) << 1) ^ (std::hash<int>()(std::get<2>(tuple)) << 2);
	}
};

// �Զ����ϣ��������ȱȽ���
struct Vec3Hash {
	size_t operator()(const glm::vec3& vec) const
	{
		return std::hash<float>()(vec.x) ^
			(std::hash<float>()(vec.y) << 1) ^
			(std::hash<float>()(vec.z) << 2);
	}
};

struct Vec3Equal {
	bool operator()(const glm::vec3& lhs, const glm::vec3& rhs) const
	{
		return glm::all(glm::epsilonEqual(lhs, rhs, 0.0001f));
	}
};

struct Vec2Hash {
	size_t operator()(const glm::vec2& vec) const
	{
		return std::hash<float>()(vec.x) ^
			(std::hash<float>()(vec.y) << 1);
	}
};

struct Vec2Equal {
	bool operator()(const glm::vec2& lhs, const glm::vec2& rhs) const
	{
		return glm::all(glm::epsilonEqual(lhs, rhs, 0.0001f));
	}
};

namespace BrokenSim
{
	namespace Geometry
	{
		std::vector<Meshes> cutModel(const ModelComponent& model, const VoronoiComponent& voronoi, const std::string& exportPath)
		{
			// ��ȡ����voronoi���ӵ�
			std::vector<glm::vec3> points = voronoi.GetPoints();

			// ��ȡģ�͵Ķ��㣬����������ɵĶ���
			std::vector<Vertex> vertices = model.GetVertices();

			// ����һ��ӳ������ڴ洢ÿ�������Ӧ��voronoi���ӵ�
			std::unordered_map<int, int> vertexMap;

			for (unsigned int i = 0; i < vertices.size(); i++)
			{
				// ��ȡ��ǰ���������
				glm::vec3 vertex = vertices[i].Position;

				// ��鵱ǰ�����Ƿ���voronoi���ӵ�ķ�Χ��
				float minDistance = std::numeric_limits<float>::max();
				int closestPointIndex = -1;

				for (unsigned int j = 0; j < points.size(); j++)
				{
					// ���㵱ǰ���㵽voronoi���ӵ�ľ���
					float distance = glm::length(vertex - points[j]);
					// �������С�ڵ�ǰ��С���룬�������С�������������ӵ�����
					if (distance < minDistance)
					{
						minDistance = distance;
						closestPointIndex = j;
					}
				}
				// ����ǰ����ӳ�䵽�����voronoi���ӵ�
				vertexMap[i] = closestPointIndex;
			}

			using triMesh = std::array<unsigned int, 3>;

			// ��¼������Ƭ���������ͬһ�����ӵ��������Ƭ����
			std::unordered_map<int, std::vector<triMesh>> pureMeshes;
			// ��¼������Ƭ����������������ӵ��������Ƭ����
			std::unordered_map<std::pair<int, int>, std::vector<triMesh>> biSeedMeshes;
			// ��¼������Ƭ����������������ӵ��������Ƭ����
			std::unordered_map<std::tuple<int, int, int>, std::vector<triMesh>> triSeedMeshes;

			{
				// ��ȡģ�͵�����������ÿ��������Ƭ���������
				std::vector<unsigned int> indices = model.GetIndices();

				for (int i = 0; i < indices.size(); i += 3)
				{
					// ��ȡ��ǰ������Ƭ��������������
					unsigned int index0 = indices[i];
					unsigned int index1 = indices[i + 1];
					unsigned int index2 = indices[i + 2];

					// ��ȡ��ǰ������Ƭ�����������Ӧ��voronoi���ӵ�����
					int seed0 = vertexMap[index0];
					int seed1 = vertexMap[index1];
					int seed2 = vertexMap[index2];

					// ����������㶼����ͬһ�����ӵ㣬������ӵ�����Ƭ�б���
					if (seed0 == seed1 && seed1 == seed2)
					{
						triMesh tri = { index0, index1, index2 };
						pureMeshes[seed0].push_back(tri);
					}
					// ���������������ͬһ�����ӵ㣬��һ���������ڲ�ͬ�����ӵ㣬������ӵ�˫������Ƭ�б���
					else if (seed0 == seed1)
					{
						// ����������Ƭ�������˳�򲻱�
						// ��ʼ�ձ�֤�������������ͬһ�������ӵ�Ϊpair�ĵ�һ��Ԫ��
						// ��һ����������ڲ�ͬ�����ӵ�Ϊpair�ĵڶ���Ԫ��
						triMesh tri = { index0, index1, index2 };
						biSeedMeshes[std::make_pair(seed0, seed2)].push_back(tri);
					}
					else if (seed1 == seed2)
					{
						// ����������Ƭ�������˳�򲻱�
						// ��ʼ�ձ�֤�������������ͬһ�������ӵ�Ϊpair�ĵ�һ��Ԫ��
						// ��һ����������ڲ�ͬ�����ӵ�Ϊpair�ĵڶ���Ԫ��
						triMesh tri = { index1, index2, index0 };
						biSeedMeshes[std::make_pair(seed1, seed0)].push_back(tri);
					}
					else if (seed0 == seed2)
					{
						// ����������Ƭ�������˳�򲻱�
						// ��ʼ�ձ�֤�������������ͬһ�������ӵ�Ϊpair�ĵ�һ��Ԫ��
						// ��һ����������ڲ�ͬ�����ӵ�Ϊpair�ĵڶ���Ԫ��
						triMesh tri = { index2, index0, index1 };
						biSeedMeshes[std::make_pair(seed2, seed1)].push_back(tri);
					}
					// ���򣬽�����ӵ���������Ƭ�б���
					else
					{
						triMesh tri = { index0, index1, index2 };
						triSeedMeshes[std::make_tuple(seed0, seed1, seed2)].push_back(tri);
					}
				}
			}

			// �������������ӵ����Ƭ�����и�
			// ���и����¶�����ӵ�ģ�͵Ķ����б���
			// ������ͬ����һ�����ӵ�������б�
			for (auto meshes : biSeedMeshes)
			{
				// ��ȡ��ǰ��Ƭ���������ӵ�
				int seed0 = meshes.first.first;
				int seed1 = meshes.first.second;

				glm::vec3 midPoint = (points[seed0] + points[seed1]) / 2.0f;

				glm::vec3 normal = glm::normalize(points[seed0] - points[seed1]);

				int midPointIndex = vertices.size();

				// �����������ӵ���е�
				// ���ڷ����������ӵ㣬�����÷����������෴��������
				Vertex midVertex0;
				midVertex0.Position = midPoint;
				midVertex0.Normal = -normal;
				midVertex0.TexCoords = glm::vec2(0.0f, 0.0f);

				Vertex midVertex1;
				midVertex1.Position = midPoint;
				midVertex1.Normal = normal;
				midVertex1.TexCoords = glm::vec2(0.0f, 0.0f);

				// ����е㵽ģ�͵Ķ����б���
				vertices.push_back(midVertex0);
				vertices.push_back(midVertex1);

				// ������ǰ��Ƭ�б�
				std::vector<triMesh>& triMeshes = meshes.second;

				for (auto mesh : triMeshes)
				{
					// ��ȡ��ǰ������Ƭ��������������
					unsigned int index0 = mesh[0];
					unsigned int index1 = mesh[1];
					unsigned int index2 = mesh[2];

					// ��ȡ��ǰ������Ƭ��������������
					glm::vec3 vertex0 = vertices[index0].Position;
					glm::vec3 vertex1 = vertices[index1].Position;
					glm::vec3 vertex2 = vertices[index2].Position;

					// vertex0��vertex1������seed0��vertex2������seed1
					glm::vec3 edge02 = vertex2 - vertex0;
					glm::vec3 edge12 = vertex2 - vertex1;

					// ����tֵ
					float denominator0 = 2.0f * glm::dot(edge02, points[seed1] - points[seed0]);
					float denominator1 = 2.0f * glm::dot(edge12, points[seed1] - points[seed0]);

					float numerator0 = glm::dot(points[seed1], points[seed1]) - glm::dot(points[seed0], points[seed0]) -
						2.0f * glm::dot(vertex0, points[seed1] - points[seed0]);
					float numerator1 = glm::dot(points[seed1], points[seed1]) - glm::dot(points[seed0], points[seed0]) -
						2.0f * glm::dot(vertex1, points[seed1] - points[seed0]);

					float t0 = numerator0 / denominator0;
					float t1 = numerator1 / denominator1;

					// �����¶�������
					glm::vec3 newVertex0 = vertex0 + t0 * edge02;
					glm::vec3 newVertex1 = vertex1 + t1 * edge12;

					// �����¶��㷨��
					glm::vec3 newNormal0 = vertices[index0].Normal * (1.0f - t0) + vertices[index2].Normal * t0;
					glm::vec3 newNormal1 = vertices[index1].Normal * (1.0f - t1) + vertices[index2].Normal * t1;

					// �����¶�����������
					glm::vec2 newTexCoord0 = vertices[index0].TexCoords * (1.0f - t0) + vertices[index2].TexCoords * t0;
					glm::vec2 newTexCoord1 = vertices[index1].TexCoords * (1.0f - t1) + vertices[index2].TexCoords * t1;

					// ����¶��㵽ģ�͵Ķ����б���
					Vertex newVertex0Data;
					newVertex0Data.Position = newVertex0;
					newVertex0Data.Normal = newNormal0;
					newVertex0Data.TexCoords = newTexCoord0;

					Vertex newVertex1Data;
					newVertex1Data.Position = newVertex1;
					newVertex1Data.Normal = newNormal1;
					newVertex1Data.TexCoords = newTexCoord1;

					Vertex newVertex0_N_Normal;
					newVertex0_N_Normal.Position = newVertex0;
					newVertex0_N_Normal.Normal = -normal;
					newVertex0_N_Normal.TexCoords = glm::vec2(0.0f, 0.0f);

					Vertex newVertex0_P_Normal;
					newVertex0_P_Normal.Position = newVertex0;
					newVertex0_P_Normal.Normal = normal;
					newVertex0_P_Normal.TexCoords = glm::vec2(0.0f, 0.0f);

					Vertex newVertex1_N_Normal;
					newVertex1_N_Normal.Position = newVertex1;
					newVertex1_N_Normal.Normal = -normal;
					newVertex1_N_Normal.TexCoords = glm::vec2(0.0f, 0.0f);

					Vertex newVertex1_P_Normal;
					newVertex1_P_Normal.Position = newVertex1;
					newVertex1_P_Normal.Normal = normal;
					newVertex1_P_Normal.TexCoords = glm::vec2(0.0f, 0.0f);

					int curIndex = vertices.size();

					vertices.push_back(newVertex0Data);
					vertices.push_back(newVertex1Data);

					vertices.push_back(newVertex0_N_Normal);
					vertices.push_back(newVertex0_P_Normal);
					vertices.push_back(newVertex1_N_Normal);
					vertices.push_back(newVertex1_P_Normal);

					// ���µ�ǰ������Ƭ������
					// mesh0��mesh1��mesh2Ϊ������seed0����Ƭ
					triMesh mesh0 = { index0, index1, curIndex };
					triMesh mesh1 = { curIndex,  index1, curIndex + 1 };
					triMesh mesh2 = { curIndex + 2, curIndex + 4, midPointIndex };

					// mesh3��mesh4Ϊ������seed1����Ƭ
					triMesh mesh3 = { curIndex, curIndex + 1, index2 };
					triMesh mesh4 = { curIndex + 5, curIndex + 3, midPointIndex + 1 };
					// ����ǰ������Ƭ��������ӵ�����Ƭ�б���
					pureMeshes[seed0].push_back(mesh0);
					pureMeshes[seed0].push_back(mesh1);
					pureMeshes[seed0].push_back(mesh2);

					pureMeshes[seed1].push_back(mesh3);
					pureMeshes[seed1].push_back(mesh4);
				}
			}

			// ���biSeedMeshes
			biSeedMeshes.clear();

			// �������������ӵ����Ƭ�����и�
			// ���и����¶�����ӵ�ģ�͵Ķ����б���
			// ������ͬ����һ�����ӵ�������б�
			for (auto meshes : triSeedMeshes)
			{
				// ��ȡ��ǰ��Ƭ���������ӵ�
				int seed0 = std::get<0>(meshes.first);
				int seed1 = std::get<1>(meshes.first);
				int seed2 = std::get<2>(meshes.first);

				// �������ӵ���е�
				glm::vec3 midPoint01 = (points[seed0] + points[seed1]) / 2.0f;
				glm::vec3 midPoint12 = (points[seed1] + points[seed2]) / 2.0f;
				glm::vec3 midPoint20 = (points[seed2] + points[seed0]) / 2.0f;

				glm::vec3 normal01 = glm::normalize(points[seed0] - points[seed1]);
				glm::vec3 normal12 = glm::normalize(points[seed1] - points[seed2]);
				glm::vec3 normal20 = glm::normalize(points[seed2] - points[seed0]);

				// �������ӵ�ƽ�淨����
				glm::vec3 normal = glm::normalize(glm::cross(points[seed1] - points[seed0], points[seed2] - points[seed0]));

				// �����������ӵ������
				glm::vec3 circumCenter = (points[seed0] + points[seed1] + points[seed2]) / 3.0f;
				{
					// ��������ͶӰ�����������������
					glm::vec3 edge01 = points[seed1] - points[seed0];
					glm::vec3 edge02 = points[seed2] - points[seed0];

					float edge01Length2 = glm::dot(edge01, edge01);
					float edge02Length2 = glm::dot(edge02, edge02);

					glm::vec3 d = glm::cross(edge01, edge02);
					glm::vec3 cross01 = glm::cross(edge01, normal);
					glm::vec3 cross02 = glm::cross(edge02, normal);

					float denom = 2.0f * glm::dot(d, d);
					circumCenter = points[seed0] + (edge01Length2 * cross02 - edge02Length2 * cross01) / denom;
				}

				int midPointIndex = vertices.size();

				// ���������ӵ���е㼰������ӵ�ģ�͵Ķ����б���
				// index Ϊ midPointIndex
				Vertex midVertex01_N;
				midVertex01_N.Position = midPoint01;
				midVertex01_N.Normal = -normal01;
				midVertex01_N.TexCoords = glm::vec2(0.0f, 0.0f);

				// index Ϊ midPointIndex + 1
				Vertex midVertex01_P;
				midVertex01_P.Position = midPoint01;
				midVertex01_P.Normal = normal01;
				midVertex01_P.TexCoords = glm::vec2(0.0f, 0.0f);

				// index Ϊ midPointIndex + 2
				Vertex midVertex12_N;
				midVertex12_N.Position = midPoint12;
				midVertex12_N.Normal = -normal12;
				midVertex12_N.TexCoords = glm::vec2(0.0f, 0.0f);

				// index Ϊ midPointIndex + 3
				Vertex midVertex12_P;
				midVertex12_P.Position = midPoint12;
				midVertex12_P.Normal = normal12;
				midVertex12_P.TexCoords = glm::vec2(0.0f, 0.0f);

				// index Ϊ midPointIndex + 4
				Vertex midVertex20_N;
				midVertex20_N.Position = midPoint20;
				midVertex20_N.Normal = -normal20;
				midVertex20_N.TexCoords = glm::vec2(0.0f, 0.0f);

				// index Ϊ midPointIndex + 5
				Vertex midVertex20_P;
				midVertex20_P.Position = midPoint20;
				midVertex20_P.Normal = normal20;
				midVertex20_P.TexCoords = glm::vec2(0.0f, 0.0f);

				// index Ϊ midPointIndex + 6
				Vertex midVertex_CircumCenter_N_N01;
				midVertex_CircumCenter_N_N01.Position = circumCenter;
				midVertex_CircumCenter_N_N01.Normal = -normal01;
				midVertex_CircumCenter_N_N01.TexCoords = glm::vec2(0.0f, 0.0f);

				// index Ϊ midPointIndex + 7
				Vertex midVertex_CircumCenter_P_N01;
				midVertex_CircumCenter_P_N01.Position = circumCenter;
				midVertex_CircumCenter_P_N01.Normal = normal01;
				midVertex_CircumCenter_P_N01.TexCoords = glm::vec2(0.0f, 0.0f);

				// index Ϊ midPointIndex + 8
				Vertex midVertex_CircumCenter_N_N12;
				midVertex_CircumCenter_N_N12.Position = circumCenter;
				midVertex_CircumCenter_N_N12.Normal = -normal12;
				midVertex_CircumCenter_N_N12.TexCoords = glm::vec2(0.0f, 0.0f);

				// index Ϊ midPointIndex + 9
				Vertex midVertex_CircumCenter_P_N12;
				midVertex_CircumCenter_P_N12.Position = circumCenter;
				midVertex_CircumCenter_P_N12.Normal = normal12;
				midVertex_CircumCenter_P_N12.TexCoords = glm::vec2(0.0f, 0.0f);

				// index Ϊ midPointIndex + 10
				Vertex midVertex_CircumCenter_N_N20;
				midVertex_CircumCenter_N_N20.Position = circumCenter;
				midVertex_CircumCenter_N_N20.Normal = -normal20;
				midVertex_CircumCenter_N_N20.TexCoords = glm::vec2(0.0f, 0.0f);

				// index Ϊ midPointIndex + 11
				Vertex midVertex_CircumCenter_P_N20;
				midVertex_CircumCenter_P_N20.Position = circumCenter;
				midVertex_CircumCenter_P_N20.Normal = normal20;
				midVertex_CircumCenter_P_N20.TexCoords = glm::vec2(0.0f, 0.0f);

				// ������ǰ��Ƭ�б�
				std::vector<triMesh>& triMeshes = meshes.second;

				for (auto mesh : triMeshes)
				{
					// ��ȡ��ǰ������Ƭ��������������
					unsigned int index0 = mesh[0];
					unsigned int index1 = mesh[1];
					unsigned int index2 = mesh[2];

					// ��ȡ��ǰ������Ƭ�����������Ӧ��voronoi���ӵ�����
					glm::vec3 vertex0 = vertices[index0].Position;
					glm::vec3 vertex1 = vertices[index1].Position;
					glm::vec3 vertex2 = vertices[index2].Position;

					glm::vec3 edge01 = vertex1 - vertex0;
					glm::vec3 edge12 = vertex2 - vertex1;
					glm::vec3 edge20 = vertex0 - vertex2;

					// ����tֵ
					glm::vec3 midToVertex0 = vertex0 - midPoint01;
					glm::vec3 midToVertex1 = vertex1 - midPoint12;
					glm::vec3 midToVertex2 = vertex2 - midPoint20;

					float t01 = -glm::dot(midToVertex0, normal01) / glm::dot(edge01, normal01);
					float t12 = -glm::dot(midToVertex1, normal12) / glm::dot(edge12, normal12);
					float t20 = -glm::dot(midToVertex2, normal20) / glm::dot(edge20, normal20);

					// �����¶�������
					glm::vec3 newVertex0 = vertex0 + t01 * edge01;
					glm::vec3 newVertex1 = vertex1 + t12 * edge12;
					glm::vec3 newVertex2 = vertex2 + t20 * edge20;

					// �����¶��㷨��
					glm::vec3 newNormal0 = vertices[index0].Normal * (1.0f - t01) + vertices[index1].Normal * t01;
					glm::vec3 newNormal1 = vertices[index1].Normal * (1.0f - t12) + vertices[index2].Normal * t12;
					glm::vec3 newNormal2 = vertices[index2].Normal * (1.0f - t20) + vertices[index0].Normal * t20;

					// �����¶�����������
					glm::vec2 newTexCoord0 = vertices[index0].TexCoords * (1.0f - t01) + vertices[index1].TexCoords * t01;
					glm::vec2 newTexCoord1 = vertices[index1].TexCoords * (1.0f - t12) + vertices[index2].TexCoords * t12;
					glm::vec2 newTexCoord2 = vertices[index2].TexCoords * (1.0f - t20) + vertices[index0].TexCoords * t20;

					// ʹ��Moller-Trumbore�㷨���㽻��
					glm::vec3 S = circumCenter - vertex0;
					glm::vec3 S1 = glm::cross(normal, -edge20);
					glm::vec3 S2 = glm::cross(S, edge01);

					float t = glm::dot(S2, -edge20) / glm::dot(S1, edge01);
					float u = glm::dot(S1, S) / glm::dot(S1, edge01);
					float v = glm::dot(S2, normal) / glm::dot(S1, edge01);

					if (u < 0 || v < 0 || u + v > 1)
					{
						int curIndex = vertices.size();

						// ����¶��㵽ģ�͵Ķ����б���
						// index Ϊ curIndex
						Vertex newVertex0Data;
						newVertex0Data.Position = newVertex0;
						newVertex0Data.Normal = newNormal0;
						newVertex0Data.TexCoords = newTexCoord0;

						// index Ϊ curIndex + 1
						Vertex newVertex1Data;
						newVertex1Data.Position = newVertex1;
						newVertex1Data.Normal = newNormal1;
						newVertex1Data.TexCoords = newTexCoord1;

						// index Ϊ curIndex + 2
						Vertex newVertex2Data;
						newVertex2Data.Position = newVertex2;
						newVertex2Data.Normal = newNormal2;
						newVertex2Data.TexCoords = newTexCoord2;

						// index Ϊ curIndex + 3
						Vertex newVertex0_N_Normal;
						newVertex0_N_Normal.Position = newVertex0;
						newVertex0_N_Normal.Normal = -normal01;
						newVertex0_N_Normal.TexCoords = glm::vec2(0.0f, 0.0f);

						// index Ϊ curIndex + 4
						Vertex newVertex0_P_Normal;
						newVertex0_P_Normal.Position = newVertex0;
						newVertex0_P_Normal.Normal = normal01;
						newVertex0_P_Normal.TexCoords = glm::vec2(0.0f, 0.0f);

						// index Ϊ curIndex + 5
						Vertex newVertex1_N_Normal;
						newVertex1_N_Normal.Position = newVertex1;
						newVertex1_N_Normal.Normal = -normal12;
						newVertex1_N_Normal.TexCoords = glm::vec2(0.0f, 0.0f);

						// index Ϊ curIndex + 6
						Vertex newVertex1_P_Normal;
						newVertex1_P_Normal.Position = newVertex1;
						newVertex1_P_Normal.Normal = normal12;
						newVertex1_P_Normal.TexCoords = glm::vec2(0.0f, 0.0f);

						// index Ϊ curIndex + 7
						Vertex newVertex2_N_Normal;
						newVertex2_N_Normal.Position = newVertex2;
						newVertex2_N_Normal.Normal = -normal20;
						newVertex2_N_Normal.TexCoords = glm::vec2(0.0f, 0.0f);

						// index Ϊ curIndex + 8
						Vertex newVertex2_P_Normal;
						newVertex2_P_Normal.Position = newVertex2;
						newVertex2_P_Normal.Normal = normal20;
						newVertex2_P_Normal.TexCoords = glm::vec2(0.0f, 0.0f);

						vertices.push_back(newVertex0Data);
						vertices.push_back(newVertex1Data);
						vertices.push_back(newVertex2Data);

						vertices.push_back(newVertex0_N_Normal);
						vertices.push_back(newVertex0_P_Normal);
						vertices.push_back(newVertex1_N_Normal);
						vertices.push_back(newVertex1_P_Normal);
						vertices.push_back(newVertex2_N_Normal);
						vertices.push_back(newVertex2_P_Normal);

						// ���µ�ǰ������Ƭ������
						// mesh0��mesh1��mesh2Ϊ������seed0����Ƭ
						triMesh mesh0 = { index0, curIndex, curIndex + 2 };
						triMesh mesh1 = { curIndex + 8, curIndex + 3, midPointIndex };
						triMesh mesh2 = { curIndex + 8, midPointIndex, midPointIndex + 5 };

						// mesh3��mesh4��mesh5Ϊ������seed1����Ƭ
						triMesh mesh3 = { curIndex, index1, curIndex + 1 };
						triMesh mesh4 = { curIndex + 4, curIndex + 5, midPointIndex + 1 };
						triMesh mesh5 = { midPointIndex + 1, curIndex + 5, midPointIndex + 2 };

						// mesh6��mesh7��mesh8Ϊ������seed2����Ƭ
						triMesh mesh6 = { curIndex + 2, curIndex + 1, index2 };
						triMesh mesh7 = { curIndex + 6, curIndex + 7, midPointIndex + 3 };
						triMesh mesh8 = { midPointIndex + 3, curIndex + 7, midPointIndex + 4 };

						// ����ǰ������Ƭ��������ӵ�����Ƭ�б���
						pureMeshes[seed0].push_back(mesh0);
						pureMeshes[seed0].push_back(mesh1);
						pureMeshes[seed0].push_back(mesh2);

						pureMeshes[seed1].push_back(mesh3);
						pureMeshes[seed1].push_back(mesh4);
						pureMeshes[seed1].push_back(mesh5);

						pureMeshes[seed2].push_back(mesh6);
						pureMeshes[seed2].push_back(mesh7);
						pureMeshes[seed2].push_back(mesh8);
					}
					else
					{
						// �����¶�������
						glm::vec3 newVertex3 = circumCenter + t * normal;

						// �����¶��㷨��
						glm::vec3 newNormal3 = vertices[index0].Normal * (1.0f - u - v) + vertices[index1].Normal * u + vertices[index2].Normal * v;

						// �����¶�����������
						glm::vec2 newTexCoord3 = vertices[index0].TexCoords * (1.0f - u - v) + vertices[index1].TexCoords * u + vertices[index2].TexCoords * v;
						
						int curIndex = vertices.size();

						// ����¶��㵽ģ�͵Ķ����б���
						// index Ϊ curIndex
						Vertex newVertex0Data;
						newVertex0Data.Position = newVertex0;
						newVertex0Data.Normal = newNormal0;
						newVertex0Data.TexCoords = newTexCoord0;

						// index Ϊ curIndex + 1
						Vertex newVertex1Data;
						newVertex1Data.Position = newVertex1;
						newVertex1Data.Normal = newNormal1;
						newVertex1Data.TexCoords = newTexCoord1;

						// index Ϊ curIndex + 2
						Vertex newVertex2Data;
						newVertex2Data.Position = newVertex2;
						newVertex2Data.Normal = newNormal2;
						newVertex2Data.TexCoords = newTexCoord2;

						// index Ϊ curIndex + 3
						Vertex newVertex3Data;
						newVertex3Data.Position = newVertex3;
						newVertex3Data.Normal = newNormal3;
						newVertex3Data.TexCoords = newTexCoord3;

						// index Ϊ curIndex + 4
						Vertex newVertex0_N_Normal;
						newVertex0_N_Normal.Position = newVertex0;
						newVertex0_N_Normal.Normal = -normal01;
						newVertex0_N_Normal.TexCoords = glm::vec2(0.0f, 0.0f);

						// index Ϊ curIndex + 5
						Vertex newVertex0_P_Normal;
						newVertex0_P_Normal.Position = newVertex0;
						newVertex0_P_Normal.Normal = normal01;
						newVertex0_P_Normal.TexCoords = glm::vec2(0.0f, 0.0f);

						// index Ϊ curIndex + 6
						Vertex newVertex1_N_Normal;
						newVertex1_N_Normal.Position = newVertex1;
						newVertex1_N_Normal.Normal = -normal12;
						newVertex1_N_Normal.TexCoords = glm::vec2(0.0f, 0.0f);

						// index Ϊ curIndex + 7
						Vertex newVertex1_P_Normal;
						newVertex1_P_Normal.Position = newVertex1;
						newVertex1_P_Normal.Normal = normal12;
						newVertex1_P_Normal.TexCoords = glm::vec2(0.0f, 0.0f);

						// index Ϊ curIndex + 8
						Vertex newVertex2_N_Normal;
						newVertex2_N_Normal.Position = newVertex2;
						newVertex2_N_Normal.Normal = -normal20;
						newVertex2_N_Normal.TexCoords = glm::vec2(0.0f, 0.0f);

						// index Ϊ curIndex + 9
						Vertex newVertex2_P_Normal;
						newVertex2_P_Normal.Position = newVertex2;
						newVertex2_P_Normal.Normal = normal20;
						newVertex2_P_Normal.TexCoords = glm::vec2(0.0f, 0.0f);

						// index Ϊ curIndex + 10
						Vertex newVertex3_N_Normal01;
						newVertex3_N_Normal01.Position = newVertex3;
						newVertex3_N_Normal01.Normal = -normal01;
						newVertex3_N_Normal01.TexCoords = glm::vec2(0.0f, 0.0f);

						// index Ϊ curIndex + 11
						Vertex newVertex3_P_Normal01;
						newVertex3_P_Normal01.Position = newVertex3;
						newVertex3_P_Normal01.Normal = normal01;
						newVertex3_P_Normal01.TexCoords = glm::vec2(0.0f, 0.0f);

						// index Ϊ curIndex + 12
						Vertex newVertex3_N_Normal12;
						newVertex3_N_Normal12.Position = newVertex3;
						newVertex3_N_Normal12.Normal = -normal12;
						newVertex3_N_Normal12.TexCoords = glm::vec2(0.0f, 0.0f);

						// index Ϊ curIndex + 13
						Vertex newVertex3_P_Normal12;
						newVertex3_P_Normal12.Position = newVertex3;
						newVertex3_P_Normal12.Normal = normal12;
						newVertex3_P_Normal12.TexCoords = glm::vec2(0.0f, 0.0f);

						// index Ϊ curIndex + 14
						Vertex newVertex3_N_Normal20;
						newVertex3_N_Normal20.Position = newVertex3;
						newVertex3_N_Normal20.Normal = -normal20;
						newVertex3_N_Normal20.TexCoords = glm::vec2(0.0f, 0.0f);

						// index Ϊ curIndex + 15
						Vertex newVertex3_P_Normal20;
						newVertex3_P_Normal20.Position = newVertex3;
						newVertex3_P_Normal20.Normal = normal20;
						newVertex3_P_Normal20.TexCoords = glm::vec2(0.0f, 0.0f);

						vertices.push_back(newVertex0Data);
						vertices.push_back(newVertex1Data);
						vertices.push_back(newVertex2Data);
						vertices.push_back(newVertex3Data);

						vertices.push_back(newVertex0_N_Normal);
						vertices.push_back(newVertex0_P_Normal);
						vertices.push_back(newVertex1_N_Normal);
						vertices.push_back(newVertex1_P_Normal);
						vertices.push_back(newVertex2_N_Normal);
						vertices.push_back(newVertex2_P_Normal);

						vertices.push_back(newVertex3_N_Normal01);
						vertices.push_back(newVertex3_P_Normal01);
						vertices.push_back(newVertex3_N_Normal12);
						vertices.push_back(newVertex3_P_Normal12);
						vertices.push_back(newVertex3_N_Normal20);
						vertices.push_back(newVertex3_P_Normal20);

						// ���µ�ǰ������Ƭ������
						// mesh0��mesh1��mesh2��mesh3��mesh4��mesh5Ϊ������seed0����Ƭ
						triMesh mesh0 = { index0, curIndex, curIndex + 3 };
						triMesh mesh1 = { index0, curIndex + 3, curIndex + 2 };
						triMesh mesh2 = { curIndex + 10, curIndex + 4, midPointIndex };
						triMesh mesh3 = { curIndex + 10, midPointIndex, midPointIndex + 6 };
						triMesh mesh4 = { curIndex + 15, midPointIndex + 11, midPointIndex + 5 };
						triMesh mesh5 = { curIndex + 15, midPointIndex + 5, curIndex + 9 };

						// mesh6��mesh7��mesh8��mesh9��mesh10��mesh11Ϊ������seed1����Ƭ
						triMesh mesh6 = { curIndex, index1, curIndex + 3 };
						triMesh mesh7 = { curIndex + 3, index1, curIndex + 1 };
						triMesh mesh8 = { curIndex + 11, midPointIndex + 1, curIndex + 5 };
						triMesh mesh9 = { curIndex + 11, midPointIndex + 7, midPointIndex + 1 };
						triMesh mesh10 = { curIndex + 12, midPointIndex + 2, midPointIndex + 8 };
						triMesh mesh11 = { curIndex + 12, curIndex + 6, midPointIndex + 2 };

						// mesh12��mesh13��mesh14��mesh15��mesh16��mesh17Ϊ������seed2����Ƭ
						triMesh mesh12 = { curIndex + 3, curIndex + 1, index2 };
						triMesh mesh13 = { curIndex + 2, curIndex + 3, index2 };
						triMesh mesh14 = { curIndex + 13, midPointIndex + 3, curIndex + 7 };
						triMesh mesh15 = { curIndex + 13, midPointIndex + 9, midPointIndex + 3 };
						triMesh mesh16 = { curIndex + 14, midPointIndex + 4, midPointIndex + 10 };
						triMesh mesh17 = { curIndex + 14, curIndex + 8, midPointIndex + 4 };

						// ����ǰ������Ƭ��������ӵ�����Ƭ�б���
						pureMeshes[seed0].push_back(mesh0);
						pureMeshes[seed0].push_back(mesh1);
						pureMeshes[seed0].push_back(mesh2);
						pureMeshes[seed0].push_back(mesh3);
						pureMeshes[seed0].push_back(mesh4);
						pureMeshes[seed0].push_back(mesh5);

						pureMeshes[seed1].push_back(mesh6);
						pureMeshes[seed1].push_back(mesh7);
						pureMeshes[seed1].push_back(mesh8);
						pureMeshes[seed1].push_back(mesh9);
						pureMeshes[seed1].push_back(mesh10);
						pureMeshes[seed1].push_back(mesh11);

						pureMeshes[seed2].push_back(mesh12);
						pureMeshes[seed2].push_back(mesh13);
						pureMeshes[seed2].push_back(mesh14);
						pureMeshes[seed2].push_back(mesh15);
						pureMeshes[seed2].push_back(mesh16);
						pureMeshes[seed2].push_back(mesh17);
					}
				}
			}

			// ���triSeedMeshes
			triSeedMeshes.clear();

			std::string modelName = model.GetPath();

			// �������й�����ͬһ���ӵ����Ƭ
			// ������Ϊһ����Ƭģ��
			for (auto meshes : pureMeshes)
			{
				// �����ɵ���Ƭģ�͵������ļ�
				std::string fileName = modelName + "_frag_" + std::to_string(meshes.first) + ".obj";

				std::ofstream outFile(exportPath + "/" + fileName, std::ios::out | std::ios::binary);

				if (!outFile)
				{
					BS_CORE_ERROR("Failed to open file: {0}", fileName);
					continue;
				}

				outFile << "# OBJ file\n";

				//outFile << "mtllib " << fileName << ".mtl\n";
				//outFile << "usemtl material_0\n";
				//outFile << "g group_0\n";

				outFile << "o frag_" << meshes.first << "\n";

				// �����ɵ���Ƭģ�ͼ�¼����
				Meshes newMeshes;

				// ��¼��ǰ������������
				std::unordered_map<glm::vec3, int, Vec3Hash, Vec3Equal> vertexMap;
				int vIndex = 1;

				// ��¼��ǰ����������������
				std::unordered_map<glm::vec2, int, Vec2Hash, Vec2Equal> texCoordMap;
				int vtIndex = 1;

				// ��¼��ǰ���㷨������
				std::unordered_map<glm::vec3, int, Vec3Hash, Vec3Equal> normalMap;
				int vnIndex = 1;

				for (auto mesh : meshes.second)
				{
					for (int i = 0; i < 3; i++)
					{
						unsigned int index = mesh[i];

						// �ֱ���ǰ������Ƭ��������������
						glm::vec3 vertex = vertices[index].Position;
						glm::vec2 texCoord = vertices[index].TexCoords;
						glm::vec3 normal = vertices[index].Normal;

						// ����������
						if (vertexMap.find(vertex) == vertexMap.end())
						{
							vertexMap[vertex] = vIndex++;
						}

						// ������������
						if (texCoordMap.find(texCoord) == texCoordMap.end())
						{
							texCoordMap[texCoord] = vtIndex++;
						}

						// ����������
						if (normalMap.find(normal) == normalMap.end())
						{
							normalMap[normal] = vnIndex++;
						}
					}
				}

				// ��ӡ��������
				for (auto vertex : vertexMap)
				{
					glm::vec3 v = vertex.first;
					outFile << "v " << v.x << " " << v.y << " " << v.z << "\n";
				}

				// ��ӡ��������
				for (auto texCoord : texCoordMap)
				{
					glm::vec2 vt = texCoord.first;
					outFile << "vt " << vt.x << " " << vt.y << "\n";
				}

				// ��ӡ��������
				for (auto normal : normalMap)
				{
					glm::vec3 vn = normal.first;
					outFile << "vn " << vn.x << " " << vn.y << " " << vn.z << "\n";
				}

				// ��ӡ��Ƭ����
				for (auto mesh : meshes.second)
				{
					outFile << "f ";

					for (int i = 0; i < 3; i++)
					{
						// ԭ����
						unsigned int index = mesh[i];

						// ��Ӧ�Ķ������ꡢ�������ꡢ��������
						glm::vec3 vertex = vertices[index].Position;
						glm::vec2 texCoord = vertices[index].TexCoords;
						glm::vec3 normal = vertices[index].Normal;

						// ��ȡ��ǰ�������ꡢ�������ꡢ�������������
						int vIndex = vertexMap[vertex];
						int vtIndex = texCoordMap[texCoord];
						int vnIndex = normalMap[normal];

						outFile << vIndex << "/" << vtIndex << "/" << vnIndex << " ";
					}

					outFile << "\n";
				}
			}

			return std::vector<Meshes>();
		}
	}
}

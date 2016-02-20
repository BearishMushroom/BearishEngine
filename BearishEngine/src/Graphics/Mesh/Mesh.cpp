#include "../RenderingEngine.h"
#include "../../Core/Model.h"
#include "../../Math/vec3.h"
#include "../../Math/mat4.h"
#include "Mesh.h"

using namespace Bearish;
using namespace Graphics;
using namespace Math;

Mesh* Mesh::CreateQuad(vec4 texCoords) {
	std::vector<Vertex> vertices {
		Vertex(vec3(-0.5f, -0.5f, 0), vec2(texCoords.x, texCoords.y)),
		Vertex(vec3(0.5f, -0.5f, 0), vec2(texCoords.z, texCoords.y)),
		Vertex(vec3(0.5f, 0.5f, 0), vec2(texCoords.z, texCoords.w)),
		Vertex(vec3(-0.5f, 0.5f, 0), vec2(texCoords.x, texCoords.w)),
	};

	static std::vector<u32> indices{
		0, 1, 2,
		0, 2, 3
	};

	return new Mesh(vertices, indices);
}


Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<u32>& indices, bool calculateNormals) {
	if (calculateNormals) {
		CalculateNormals(vertices, indices);
	}

	SetupBuffers();

	std::vector<vec3> positions;
	std::vector<vec2> texCoords;
	std::vector<vec3> normals;
	std::vector<vec3> tangents;
	std::vector<vec4i> boneIDs;
	std::vector<vec4> boneWeights;

	for (u32 i = 0; i < vertices.size(); i++) {
		positions.push_back(vertices[i].position);
		texCoords.push_back(vertices[i].texCoord);
		normals.push_back(vertices[i].normal);
		tangents.push_back(vertices[i].tangent);
		boneIDs.push_back(vertices[i].boneIDs);
		boneWeights.push_back(vertices[i].boneWeights);
	}

	SetPositionData(&positions[0], sizeof(vec3) * positions.size());
	SetTexCoordData(&texCoords[0], sizeof(vec2) * texCoords.size());
	SetNormalData(&normals[0], sizeof(vec3) * normals.size());
	SetTangentData(&tangents[0], sizeof(vec3) * tangents.size());
	SetBoneIDData(&boneIDs[0], sizeof(vec4i) * boneIDs.size());
	SetBoneWeightData(&boneWeights[0], sizeof(vec4) * boneWeights.size());

	SetIndexData(&indices[0], indices.size());
	
	SetupInstanceData();

	_vao->Unbind();
	_firstAnim = true;
}

Mesh::Mesh(u32 numVertices, Math::vec3* positions, Math::vec2* texCoords, Math::vec3* normals, Math::vec3* tangents, Math::vec4i* boneids, Math::vec4* boneweights,
		   u32 numIndices, u32* indices) {
	SetupBuffers();

	SetPositionData(positions, sizeof(vec3) * numVertices);
	SetTexCoordData(texCoords, sizeof(vec2) * numVertices);
	SetNormalData(normals, sizeof(vec3) * numVertices);
	SetTangentData(tangents, sizeof(vec3) * numVertices);

	if (boneids == 0) {
		boneids = new vec4i[numVertices];
		memset(boneids, -1, sizeof(vec4i) * numVertices);

		boneweights = new vec4[numVertices];
		memset(boneweights, -1, sizeof(vec4) * numVertices);
	}

	SetBoneIDData(boneids, sizeof(vec4i) * numVertices);
	SetBoneWeightData(boneweights, sizeof(vec4) * numVertices);

	SetIndexData(indices, numIndices);

	SetupInstanceData();

	_vao->Unbind();
	_firstAnim = true;
}

Mesh::Mesh(Core::Model model) {
	*this = model.ToMesh();
}

Mesh::~Mesh() {}


void Mesh::SetupBuffers() {
	_vao = new VAO;
	_vao->Bind();

	_positions = new VBO;
	_texCoords = new VBO;
	_normals = new VBO;
	_tangents = new VBO;
	_boneIDs = new VBO;
	_boneWeights = new VBO;
	_indices = new IBO;
}

void Mesh::SetupInstanceData() {
	_worlds = new VBO;
	_mvps = new VBO;

	_worlds->Bind();
	for (i32 i = 0; i < 4; i++) {
		Renderer::EnableAttribArray(Renderer::WORLD_ATTRIBUTE + i);
		Renderer::SetAttribPointer(Renderer::WORLD_ATTRIBUTE + i, 4, sizeof(mat4), sizeof(f32) * i * 4);
		Renderer::SetAttribDivisor(Renderer::WORLD_ATTRIBUTE + i, 1);
	}

	_mvps->Bind();
	for (i32 i = 0; i < 4; i++) {
		Renderer::EnableAttribArray(Renderer::MVP_ATTRIBUTE + i);
		Renderer::SetAttribPointer(Renderer::MVP_ATTRIBUTE + i, 4, sizeof(mat4), sizeof(f32) * i * 4);
		Renderer::SetAttribDivisor(Renderer::MVP_ATTRIBUTE + i, 1);
	}
}

void Mesh::SetPositionData(vec3* data, u32 size) {
	_positions->SetData(data, size);
	Renderer::EnableAttribArray(Renderer::POSITION_ATTRIBUTE);
	Renderer::SetAttribPointer(Renderer::POSITION_ATTRIBUTE, sizeof(vec3) / sizeof(f32), sizeof(vec3), 0);
}

void Mesh::SetTexCoordData(vec2* data, u32 size) {
	_texCoords->SetData(data, size);
	Renderer::EnableAttribArray(Renderer::TEXCOORD_ATTRIBUTE);
	Renderer::SetAttribPointer(Renderer::TEXCOORD_ATTRIBUTE, sizeof(vec2) / sizeof(f32), sizeof(vec2), 0);
}

void Mesh::SetNormalData(vec3* data, u32 size) {
	_normals->SetData(data, size);
	Renderer::EnableAttribArray(Renderer::NORMAL_ATTRIBUTE);
	Renderer::SetAttribPointer(Renderer::NORMAL_ATTRIBUTE, sizeof(vec3) / sizeof(f32), sizeof(vec3), 0);
}

void Mesh::SetTangentData(vec3* data, u32 size) {
	_tangents->SetData(data, size);
	Renderer::EnableAttribArray(Renderer::TANGENT_ATTRIBUTE);
	Renderer::SetAttribPointer(Renderer::TANGENT_ATTRIBUTE, sizeof(vec3) / sizeof(f32), sizeof(vec3), 0);
}

void Mesh::SetBoneIDData(vec4i* data, u32 size) {
	_boneIDs->SetData(data, size);
	Renderer::EnableAttribArray(Renderer::BONEID_ATTRIBUTE);
	Renderer::SetAttribPointer(Renderer::BONEID_ATTRIBUTE, sizeof(vec4i) / sizeof(i32), sizeof(vec4i), 0, AttributeType::Int32);
}

void Mesh::SetBoneWeightData(vec4* data, u32 size) {
	_boneWeights->SetData(data, size);
	Renderer::EnableAttribArray(Renderer::BONEWEIGHT_ATTRIBUTE);
	Renderer::SetAttribPointer(Renderer::BONEWEIGHT_ATTRIBUTE, sizeof(vec4) / sizeof(f32), sizeof(vec4), 0);
}

void Mesh::SetIndexData(u32* data, u32 size) {
	_indices->SetData(data, size);
}

void Mesh::Submit(const mat4& world, const mat4& mvp) {
	_worldMatrices.push_back(world.Transpose());
	_mvpMatrices.push_back(mvp.Transpose());
}

void Mesh::Flush() {
	_vao->Bind();

	_worlds->SetData(&_worldMatrices[0], sizeof(mat4) * _worldMatrices.size());
	_mvps->SetData(&_mvpMatrices[0], sizeof(mat4) * _mvpMatrices.size());

	glDrawElementsInstanced(GL_TRIANGLES, _indices->GetSize(), GL_UNSIGNED_INT, 0, _worldMatrices.size());
	_vao->Unbind();

	_worldMatrices.clear();
	_mvpMatrices.clear();
}

void Mesh::CalculateNormals(std::vector<Vertex>& vertices, std::vector<u32>& indices) const {
	for (u32 i = 0; i < indices.size(); i += 3) {
		u32 i0 = indices[i];
		u32 i1 = indices[i + 1];
		u32 i2 = indices[i + 2];

		vec3 v1 = vertices[i1].position - vertices[i0].position;
		vec3 v2 = vertices[i2].position - vertices[i0].position;

		vec3 normal = v1.Cross(v2);
		normal.Normalize();

		vertices[i0].normal += normal;
		vertices[i1].normal += normal;
		vertices[i2].normal += normal;
	}

	for (u32 i = 0; i < vertices.size(); i++) {
		vertices[i].normal.Normalize();
	}
}

void Mesh::Animate(f32 deltaTime) {
	_boneTransforms.resize(_skeleton->GetNumBones());

	for (i32 i = 0; i < (i32)_animations.size(); i++) {
		bool didAnimate = _animations.at(i).Update(deltaTime, _rootNode);

		if (didAnimate) {
			for (i32 j = 0; j < (i32)_skeleton->GetNumBones(); j++) {
				_boneTransforms.at(j) = _animations.at(i).GetTransform(j);
			}
		}
	}
}

MeshAnimation* Mesh::GetAnimation(string name) {
	for (auto& it : _animations) {
		if (it.GetName() == name) {
			return &it;
		}
	}

	return 0;
}

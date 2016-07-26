#include "../RenderingEngine.h"
#include "../../Core/Model.h"
#include "../../Math/vec3.h"
#include "../../Math/mat4.h"
#include "Mesh.h"
#include "../Shader.h"
#include "../Frustum.h"
#include "../Camera.h"

using namespace Bearish;
using namespace Graphics;
using namespace Math;
using namespace Core;

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

	//std::vector<vec3> positions;
	//std::vector<vec2> texCoords;
	//std::vector<vec3> normals;
	//std::vector<vec3> tangents;
	//std::vector<vec4i> boneIDs;
	//std::vector<vec4> boneWeights;
	//
	//for (u32 i = 0; i < vertices.size(); i++) {
	//	positions.push_back(vertices[i].position);
	//	texCoords.push_back(vertices[i].texCoord);
	//	normals.push_back(vertices[i].normal);
	//	tangents.push_back(vertices[i].tangent);
	//	boneIDs.push_back(vertices[i].boneIDs);
	//	boneWeights.push_back(vertices[i].boneWeights);
	//}
	//
	//SetPositionData(&positions[0], sizeof(vec3) * positions.size());
	//SetTexCoordData(&texCoords[0], sizeof(vec2) * texCoords.size());
	//SetNormalData(&normals[0], sizeof(vec3) * normals.size());
	//SetTangentData(&tangents[0], sizeof(vec3) * tangents.size());
	//SetBoneIDData(&boneIDs[0], sizeof(vec4i) * boneIDs.size());
	//SetBoneWeightData(&boneWeights[0], sizeof(vec4) * boneWeights.size());

	SetVertexData(&vertices[0], sizeof(Vertex) * vertices.size());

	SetIndexData(&indices[0], indices.size());

	_vertexState->Unbind();
	_firstAnim = true;
}

Mesh::Mesh(u32 numVertices, Math::vec3* positions, Math::vec2* texCoords, Math::vec3* normals, Math::vec3* tangents, Math::vec4i* boneids, Math::vec4* boneweights,
		   u32 numIndices, u32* indices) {
	SetupBuffers();

	_extremes = vec3(0, 0, 0);
	_min = vec3(0, 0, 0);
	_max = vec3(0, 0, 0);

	if (boneids == 0) {
		std::vector<Vertex> vertices;
		for (i32 i = 0; i < (i32)numVertices; i++) {
			vertices.push_back(Vertex(positions[i], texCoords[i], normals[i], tangents[i]));
		}
		
		SetVertexData(&vertices[0], sizeof(Vertex) * vertices.size());
	}
	else {
		std::vector<SkinnedVertex> vertices;
		for (i32 i = 0; i < (i32)numVertices; i++) {
			vertices.push_back(SkinnedVertex(positions[i], texCoords[i], normals[i], tangents[i], 
				boneids ? boneids[i] : vec4i(-1, -1, -1, -1), boneids ? boneweights[i] : vec4(0, 0, 0, 0)));
		}

		SetVertexData(&vertices[0], sizeof(SkinnedVertex) * vertices.size());
	}

	for (i32 i = 0; i < (i32)numVertices; i++) {
		vec3 pos = positions[i];

		if (abs(pos.x) > abs(_extremes.x)) _extremes.x = abs(pos.x);
		if (abs(pos.y) > abs(_extremes.y)) _extremes.y = abs(pos.y);
		if (abs(pos.z) > abs(_extremes.z)) _extremes.z = abs(pos.z);

		if (pos.x < _min.x) _min.x = pos.x;
		if (pos.y < _min.y) _min.y = pos.y;
		if (pos.z < _min.z) _min.z = pos.z;

		if (pos.x > _max.x) _max.x = pos.x;
		if (pos.y > _max.y) _max.y = pos.y;
		if (pos.z > _max.z) _max.z = pos.z;
	}

	SetIndexData(indices, numIndices);

	_vertexState->Unbind();
	_firstAnim = true;
}

Mesh::Mesh(Core::Model model) {
	*this = model.ToMesh();
}

Mesh::~Mesh() {
	//if(_uniformBuffer)     delete _uniformBuffer;
	//if(_vertexBuffer) delete _vertexBuffer;
	//if(_indexBuffer) delete _indexBuffer;
	//if (_vertexState)	 delete _vertexState;
}


void Mesh::SetupBuffers() {
	_vertexState = new VertexState;
	_vertexState->Bind();

	_vertexBuffer = new VertexBuffer;
	_indexBuffer = new IndexBuffer;
}

void Mesh::SetIndexData(u32* data, u32 size) {
	// This function assumes that _vertexState is already bound.
	_numFaces = size / 3;
	_indexBuffer->SetData(data, size);
}

/*void Mesh::SetVertexData(void* data, u32 size) {
	// This function assumes that _vertexState is already bound.
	_numVerts = size / sizeof(Vertex);
	_vertexBuffer->SetData(data, size);
	Renderer::EnableAttribArray(Renderer::POSITION_ATTRIBUTE);
	Renderer::SetAttribPointer(Renderer::POSITION_ATTRIBUTE, sizeof(vec3) / sizeof(f32), sizeof(Vertex), 0);

	Renderer::EnableAttribArray(Renderer::TEXCOORD_ATTRIBUTE);
	Renderer::SetAttribPointer(Renderer::TEXCOORD_ATTRIBUTE, sizeof(vec2) / sizeof(f32), sizeof(Vertex), offsetof(Vertex, texCoord));

	Renderer::EnableAttribArray(Renderer::NORMAL_ATTRIBUTE);
	Renderer::SetAttribPointer(Renderer::NORMAL_ATTRIBUTE, sizeof(vec3) / sizeof(f32), sizeof(Vertex), offsetof(Vertex, normal));

	Renderer::EnableAttribArray(Renderer::TANGENT_ATTRIBUTE);
	Renderer::SetAttribPointer(Renderer::TANGENT_ATTRIBUTE, sizeof(vec3) / sizeof(f32), sizeof(Vertex), offsetof(Vertex, tangent));

	Renderer::EnableAttribArray(Renderer::BONEID_ATTRIBUTE);
	Renderer::SetAttribPointer(Renderer::BONEID_ATTRIBUTE, sizeof(vec4i) / sizeof(i32), sizeof(Vertex), offsetof(Vertex, boneIDs));

	Renderer::EnableAttribArray(Renderer::BONEWEIGHT_ATTRIBUTE);
	Renderer::SetAttribPointer(Renderer::BONEWEIGHT_ATTRIBUTE, sizeof(vec4) / sizeof(f32), sizeof(Vertex), offsetof(Vertex, boneWeights));

	// This sets up our uniform buffer, so we can set the data without reallocating it.
	//_uniformBuffer = new UBO;
	//_uniformBuffer->SetData(0, sizeof(InstanceData));
	//_uniformBuffer->Unbind();
}*/

void Mesh::SetVertexData(Vertex* vertices, u32 size) {
	VertexLayout<Vertex> layout;
	// Vertex layout
	layout.PushComponent("POSITION", sizeof(Math::vec3));
	layout.PushComponent("TEXCOORD", sizeof(Math::vec2));
	layout.PushComponent("NORMAL", sizeof(Math::vec3));
	layout.PushComponent("TANGENT", sizeof(Math::vec3));

	// Constants for skinning
	layout.PushComponent(Math::vec4i(-1, -1, -1, -1));
	layout.PushComponent(Math::vec4(0, 0, 0, 0));

	SetVertexData<Vertex>(vertices, size, layout);
}

void Mesh::SetVertexData(SkinnedVertex* vertices, u32 size) {
	VertexLayout<SkinnedVertex> layout;
	// Vertex layout
	layout.PushComponent("POSITION", sizeof(Math::vec3));
	layout.PushComponent("TEXCOORD", sizeof(Math::vec2));
	layout.PushComponent("NORMAL", sizeof(Math::vec3));
	layout.PushComponent("TANGENT", sizeof(Math::vec3));
	layout.PushComponent("BONEID", sizeof(Math::vec4i));
	layout.PushComponent("BONEWEIGHT", sizeof(Math::vec4));

	SetVertexData<SkinnedVertex>(vertices, size, layout);
}

void Mesh::Submit(Transform* transform, const mat4& world, const Camera* camera) {
	if (transform != 0) {
		Frustum fr(camera->GetViewMatrix());

		if (!fr.SphereIntersects(transform->GetTranslation(), _extremes.Max() * (transform->GetScale().Max() * 1.3f))) {
			return;
		}
	}

	_worldMatrices.push_back(world);
	_mvpMatrices.push_back(camera->GetViewMatrix() * world);
}

void Mesh::Flush(Shader* shader) {
	_vertexState->Bind();

	i32 numInstances = _worldMatrices.size();
	mat4* worlds = &_worldMatrices[0];
	mat4* mvps = &_mvpMatrices[0];

	for (i32 i = 0; i < numInstances; i++) {
		//data.mvp = mvps[i];
		//data.world = worlds[i];
		//_uniformBuffer->UpdateData(data);
		//shader->SetUniformBlock("instance_data", _uniformBuffer);
		shader->SetUniform("world", worlds[i]);
		shader->SetUniform("MVP", mvps[i]);
		_vertexBuffer->Apply();
		glDrawElements((GLenum)Renderer::GetPrimitiveMode(), _indexBuffer->GetSize(), GL_UNSIGNED_INT, 0);
	}

	_vertexState->Unbind();

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

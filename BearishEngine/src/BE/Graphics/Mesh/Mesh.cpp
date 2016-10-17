#include <BE/Graphics/RenderingEngine.h>
#include <BE/Core/Model.h>
#include <BE/Math/vec3.h>
#include <BE/Math/mat4.h>
#include <BE\Graphics\Mesh\Mesh.h>
#include <BE/Graphics/Shader.h>
#include <BE/Graphics/Frustum.h>
#include <BE/Graphics/Camera.h>

using namespace Bearish;
using namespace Graphics;
using namespace Math;
using namespace Core;

Mesh* Mesh::CreateQuad(vec4 texCoords, f32 scale, bool winding) {
	std::vector<Vertex> vertices{
		Vertex(vec3(-0.5f * scale, -0.5f * scale, 0), vec2(texCoords.x, texCoords.y)),
		Vertex(vec3( 0.5f * scale, -0.5f * scale, 0), vec2(texCoords.z, texCoords.y)),
		Vertex(vec3( 0.5f * scale,  0.5f * scale, 0), vec2(texCoords.z, texCoords.w)),
		Vertex(vec3(-0.5f * scale,  0.5f * scale, 0), vec2(texCoords.x, texCoords.w)),
	};

	static std::vector<u32> indices{
		0, 1, 2,
		0, 2, 3
	};

	static std::vector<u32> rev_indices{
		3, 2, 0,
		2, 1, 0
	};

	return new Mesh(vertices, winding == true ? indices : rev_indices);
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

	SetVertexData(&vertices[0], sizeof(Vertex) * (u32)vertices.size());

	SetIndexData(&indices[0], (u32)indices.size());

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
		
		SetVertexData(&vertices[0], sizeof(Vertex) * (u32)vertices.size());
	}
	else {
		std::vector<SkinnedVertex> vertices;
		for (i32 i = 0; i < (i32)numVertices; i++) {
			vertices.push_back(SkinnedVertex(positions[i], texCoords[i], normals[i], tangents[i], 
				boneids ? boneids[i] : vec4i(-1, -1, -1, -1), boneids ? boneweights[i] : vec4(0, 0, 0, 0)));
		}

		SetVertexData(&vertices[0], sizeof(SkinnedVertex) * (u32)vertices.size());
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

Mesh::Mesh(Core::Model* model) {
	*this = model->ToMesh();
	_source = model;
}

Mesh::Mesh(Mesh& other) {
	_vertexState = other._vertexState;
	_vertexBuffer = other._vertexBuffer;
	_indexBuffer = other._indexBuffer;

	_uniformBuffer = other._uniformBuffer;

	_skeleton = other._skeleton;
	DecoupleSkeleton();

	_rootNode = other._rootNode;
	_animations = other._animations;
	_transform = other._transform;
	_boneTransforms = other._boneTransforms;
	_firstAnim = true;
	_rigged = other._rigged;

	_extremes = other._extremes;
	_min = other._min;
	_max = other._max;
	_numVerts = other._numVerts;
	_numFaces = other._numFaces;
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

void Mesh::SetVertexData(Vertex* vertices, u32 size) {
	SetVertexData<Vertex>(vertices, size, VertexLayout<Vertex>::Default);
}

void Mesh::SetVertexData(SkinnedVertex* vertices, u32 size) {
	SetVertexData<SkinnedVertex>(vertices, size, VertexLayout<SkinnedVertex>::Default);
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

	i32 numInstances = (i32)_worldMatrices.size();
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

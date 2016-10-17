#ifndef _BEARISH_GRAPHICS_MESH_H_
#define _BEARISH_GRAPHICS_MESH_H_

#include <BE/Graphics/Mesh/MeshAnimation.h>
#include <BE/Graphics/Mesh/MeshSkeleton.h>
#include <BE/Graphics/Mesh/MeshNode.h>

#include <BE/Graphics/Buffer/UniformBuffer.h>
#include <BE/Graphics/Buffer/VertexBuffer.h>
#include <BE/Graphics/Buffer/IndexBuffer.h>
#include <BE/Graphics/Buffer/VertexState.h>

#include <BE/Graphics/Renderer.h>

#include <BE/Core/IAllocatable.h>
#include <BE/Core/Transform.h>
#include <BE/Core/Shared.h>
#include <BE/Core/Model.h>

#include <BE/Math/mat4.h>
#include <BE/Math/vec2.h>
#include <BE/Math/vec3.h>
#include <BE/Math/vec4.h>
#include <BE/Math/quat.h>

/// TODO (14/03/16): Interfaceify Mesh.
/// Refractor mesh into multiple mesh classes.
/// StaticMesh: Uses AOS and no instancing <- 1 VBO, 1 IBO
/// DynamicMesh: Uses SOA and no instancing <- 6 VBOs, 1 IBO
/// InstancedMesh: Uses AOS and instancing <- 3 VBOs, 1 IBO

/// Add support for UBO instance data.
/// Also make materials use UBO's.

/// These all share some methods: Submit, Flush, and Animation stuff.

/// Meshes should have a VertexDescriptor which defines the vertex layout, if a user wishes to roll custom vertices.
/// If no descriptor is found, the engine uses a default that matches the Vertex struct.

namespace Bearish { 
	namespace Graphics {	
		class RenderingEngine;
		class Shader;
		class Camera;

		class BEARISH_API Mesh : public Core::IAllocatable<Mesh> {
		private:
			struct InstanceData {
				Math::mat4 world;
				Math::mat4 mvp;
			};
		public:
			static Mesh* CreateQuad(Math::vec4 texCoords = Math::vec4(0, 0, 1, 1), f32 scale = 1, bool winding = true);

			Mesh(std::vector<Vertex>& vertices, std::vector<u32>& indices, bool calculateNormals = false);
			Mesh(u32 numVertices, Math::vec3* positions, Math::vec2* texCoords, Math::vec3* normals, Math::vec3* tangents, Math::vec4i* boneids, 
				 Math::vec4* boneweights, u32 numIndices, u32* indices);
			Mesh(Core::Model* model);
			Mesh(Mesh& other);

			~Mesh();

			void SetBoneData(MeshSkeleton* skeleton, MeshNode* rootNode, std::vector<MeshAnimation> animations, const Math::mat4& transform) {
				_skeleton = skeleton;
				_rootNode = rootNode;
				_animations = animations;
				_transform = transform;
			}

			void Submit(Core::Transform* transform, const Math::mat4& world, const Camera* camera);

			void Flush(Shader* shader);
			void CalculateNormals(std::vector<Vertex>& vertices, std::vector<u32>& indices) const;

			u32 GetVAOID() { return _vertexState->GetID(); }

			MeshAnimation* GetAnimation(string name);

			void Animate(f32 deltaTime);
			void DecoupleSkeleton() {
				_skeleton = new MeshSkeleton(*_skeleton);
			}

			MeshSkeleton* GetSkeleton() { return _skeleton; }

			std::vector<Math::mat4>& GetBones() { if (_boneTransforms.size() == 0) Animate(0); return _boneTransforms; }
			
			void SetVertexData(Vertex* vertices, u32 size);
			void SetVertexData(SkinnedVertex* vertices, u32 size);
			void SetIndexData(u32* data, u32 size);

			template<typename T>
			void SetVertexData(T* vertices, u32 size, VertexLayout<T>& layout) {
				_numVerts = size / sizeof(Vertex);
				_vertexBuffer->SetData((void*)vertices, size);
				_vertexBuffer->SetLayout(layout);
			}

			Math::vec3 GetExtremes() const { return _extremes; }
			Math::vec3 GetMin() const { return _min; }
			Math::vec3 GetMax() const { return _max; }

			u32 GetNumVerts() const { return _numVerts; }
			u32 GetNumFaces() const { return _numFaces; }
			u32 GetQueued() const { return _mvpMatrices.size(); }

			void SetSourceModel(Core::Model* model) { _source = model; }
			Core::Model* const GetSourceModel() const { return _source; }
		private:
			void SetupBuffers();
			
			Core::Shared<VertexState>  _vertexState;
			Core::Shared<VertexBuffer> _vertexBuffer;
			Core::Shared<IndexBuffer>  _indexBuffer;

			Core::Shared<UniformBuffer> _uniformBuffer;

			std::vector<Math::mat4> _worldMatrices;
			std::vector<Math::mat4> _mvpMatrices;
			Core::Model* _source;

			MeshSkeleton* _skeleton;
			MeshNode* _rootNode;
			std::vector<MeshAnimation> _animations;
			Math::mat4 _transform;
			std::vector<Math::mat4> _boneTransforms;
			bool _firstAnim, _rigged;

			Math::vec3 _extremes, _min, _max;
			u32 _numVerts, _numFaces;
		};
} }

#endif
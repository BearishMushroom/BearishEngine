#ifndef _BEARISH_GRAPHICS_MESH_H_
#define _BEARISH_GRAPHICS_MESH_H_

#include "../../Core/IAllocatable.h"
#include "../../Core/Model.h"
#include "../Renderer.h"
#include "../Buffer/VBO.h"
#include "../Buffer/IBO.h"
#include "../Buffer/VAO.h"
#include "../Buffer/UBO.h"
#include "../../Math/mat4.h"
#include "../../Math/vec2.h"
#include "../../Math/vec3.h"
#include "../../Math/vec4.h"
#include "../../Math/quat.h"
#include "MeshAnimation.h"
#include "MeshSkeleton.h"
#include "MeshNode.h"

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

		class Mesh : public Core::IAllocatable<Mesh> {
		private:
			struct InstanceData {
				Math::mat4 world;
				Math::mat4 mvp;
				f32 rigged;
			};
		public:
			static Mesh* CreateQuad(Math::vec4 texCoords = Math::vec4(0, 0, 1, 1));

			Mesh(std::vector<Vertex>& vertices, std::vector<u32>& indices, bool calculateNormals = false);
			Mesh(u32 numVertices, Math::vec3* positions, Math::vec2* texCoords, Math::vec3* normals, Math::vec3* tangents, Math::vec4i* boneids, 
				 Math::vec4* boneweights, u32 numIndices, u32* indices);
			Mesh(Core::Model model);
			~Mesh();

			void SetBoneData(MeshSkeleton* skeleton, MeshNode* rootNode, std::vector<MeshAnimation> animations, Math::mat4 transform) {
				_skeleton = skeleton;
				_rootNode = rootNode;
				_animations = animations;
				_transform = transform;
			}

			void Submit(const Math::mat4& world, const Math::mat4& mvp);

			void Flush(Shader* shader);
			void CalculateNormals(std::vector<Vertex>& vertices, std::vector<u32>& indices) const;

			u32 GetVAOID() { return _vao->GetID(); }

			MeshAnimation* GetAnimation(string name);

			void Animate(f32 deltaTime);
			void DecoupleSkeleton() {
				_skeleton = new MeshSkeleton(*_skeleton);
			}

			MeshSkeleton* GetSkeleton() { return _skeleton; }

			std::vector<Math::mat4>& GetBones() { if (_boneTransforms.size() == 0) Animate(0); return _boneTransforms; }

			void SetVertexData(Vertex* vertices, u32 size);
			void SetIndexData(u32* data, u32 size);
		private:
			void SetupBuffers();
			
			struct {
				VBO* _attribs;
				IBO* _indices;
			};


			VAO* _vao;
			UBO* _ubo;

			std::vector<Math::mat4> _worldMatrices;
			std::vector<Math::mat4> _mvpMatrices;

			MeshSkeleton* _skeleton;
			MeshNode* _rootNode;
			std::vector<MeshAnimation> _animations;
			Math::mat4 _transform;
			std::vector<Math::mat4> _boneTransforms;
			bool _firstAnim, _rigged;
		};
} }

#endif
#ifndef _BEARISH_GRAPHICS_MESH_H_
#define _BEARISH_GRAPHICS_MESH_H_

#include "../../Core/IAllocatable.h"
#include "../../Core/Model.h"
#include "../Renderer.h"
#include "../Buffer/VBO.h"
#include "../Buffer/IBO.h"
#include "../Buffer/VAO.h"
#include "../../Math/mat4.h"
#include "../../Math/vec2.h"
#include "../../Math/vec3.h"
#include "../../Math/vec4.h"
#include "../../Math/quat.h"
#include "MeshAnimation.h"
#include "MeshSkeleton.h"
#include "MeshNode.h"

namespace Bearish { 
	namespace Graphics {		
		class RenderingEngine;

		class Mesh : public Core::IAllocatable<Mesh> {
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

			void Flush();
			void CalculateNormals(std::vector<Vertex>& vertices, std::vector<u32>& indices) const;

			u32 GetVAOID() { return _vao->GetID(); }

			MeshAnimation* GetAnimation(string name);

			void Animate(f32 deltaTime);
			void DecoupleSkeleton() {
				_skeleton = new MeshSkeleton(*_skeleton);
			}

			std::vector<Math::mat4>& GetBones() { if (_boneTransforms.size() == 0) Animate(0); return _boneTransforms; }

			void SetPositionData(Math::vec3* data, u32 size);
			void SetTexCoordData(Math::vec2* data, u32 size);
			void SetNormalData(Math::vec3* data, u32 size);
			void SetTangentData(Math::vec3* data, u32 size);
			void SetBoneIDData(Math::vec4i* data, u32 size);
			void SetBoneWeightData(Math::vec4* data, u32 size);
			void SetIndexData(u32* data, u32 size);
		private:
			void SetupBuffers();
			void SetupInstanceData();

			union {
				struct {
					VBO* _positions;
					VBO* _texCoords;
					VBO* _normals;
					VBO* _tangents;
					VBO* _boneIDs;
					VBO* _boneWeights;
					VBO* _mvps;
					VBO* _worlds;
					IBO* _indices;
				};

				struct {
					VBO* _attribs[6];
					VBO* _instances[2];
					IBO* _indices;
				};
			};

			VAO* _vao;

			std::vector<Math::mat4> _worldMatrices;
			std::vector<Math::mat4> _mvpMatrices;

			MeshSkeleton* _skeleton;
			MeshNode* _rootNode;
			std::vector<MeshAnimation> _animations;
			Math::mat4 _transform;
			std::vector<Math::mat4> _boneTransforms;
			bool _firstAnim;
		};
} }

#endif
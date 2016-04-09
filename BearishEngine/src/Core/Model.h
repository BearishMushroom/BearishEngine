#ifndef _BEARISH_CORE_MODEL_H_
#define _BEARISH_CORE_MODEL_H_

#include "../Math/mat4.h"
#include "BEMFile.h"
#include "../Graphics/Vertex.h"
#include "IAllocatable.h"
#include <assimp\scene.h>
#include "../Types.h"

namespace Bearish { 
	namespace Graphics {
		class Mesh;
		class MeshNode;
	}

	namespace Core {

	class Model : public IAllocatable<Model> {
	public:
		Model(const string& filename);
		~Model();

		Graphics::Mesh ToMesh();
		Graphics::MeshNode* BEMToBENode(BEMNode root);
		
		string name;
	private:
		BEMFile* _data;
		string _filename;
		static std::unordered_map<string, BEMFile*> _dataMap;
	};
} }

#endif
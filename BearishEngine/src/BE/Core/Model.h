#ifndef _BEARISH_CORE_MODEL_H_
#define _BEARISH_CORE_MODEL_H_

#include <BE/Math/mat4.h>
#include "BEMFile.h"
#include <BE/Graphics/Vertex.h>
#include <BE/Core/IAllocatable.h>
#include <BE/Types.h>

namespace Bearish { 
	namespace Graphics {
		class Mesh;
		class MeshNode;
	}

	namespace Core {
	class BEARISH_API Model : public IAllocatable<Model> {
	public:
		Model(const string& filename);
		~Model();

		Graphics::Mesh ToMesh();
		Graphics::MeshNode* BEMToBENode(const BEMNode& root);

		string GetAssetName() { return _assetname; }
		void SetAssetName(string name) { _assetname = name; }

		string name;
	private:
		BEMFile* _data;
		string _filename, _assetname;
		static std::unordered_map<string, BEMFile*> _dataMap;
	};
} }

#endif
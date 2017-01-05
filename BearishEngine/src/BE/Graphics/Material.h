#ifndef _BEARISH_GRAPHICS_MATERIAL_H_
#define _BEARISH_GRAPHICS_MATERIAL_H_

#include <BE/Core/IAllocatable.h>
#include "RenderingEngine.h"
#include <BE/Math/vec2.h>
#include <BE/Math/vec3.h>
#include <BE/Math/vec4.h>
#include <BE/Types.h>
#include "Texture/Texture.h"
#include "Shader.h"
#include <BE\Core\Resource.h>

#include <fstream>

namespace Bearish {namespace Graphics {
	class BEARISH_API Material : public Core::IAllocatable<Material> {
	public:
		Material(string filepath) {
			*this = Material::FromFile(filepath);
		}

		Material(string name, Shader* shader) : _shader(shader), _name(name) {
			_copies = 0;
			Set("UsingAlbedoMap", 0.f);
			Set("UsingSpecularMap", 0.f);
			Set("UsingGlossMap", 0.f);
		}

		~Material() {}

		Material(Material& other) {
			_name = other._name + std::to_string(other._copies++);
			_shader = other._shader;
			_floatMap = other._floatMap;
			_intMap = other._intMap;
			_vec2Map = other._vec2Map;
			_vec3Map = other._vec3Map;
			_vec4Map = other._vec4Map;
			_textureMap = other._textureMap;
			_assetname = other._assetname;
			_shadername = other._shadername;
		}

		void Set(string name, f32 value) { _floatMap[name] = value; }
		void Set(string name, i32 value) { _intMap[name] = value; }
		void Set(string name, Math::vec2 value) { _vec2Map[name] = value; }
		void Set(string name, Math::vec3 value) { _vec3Map[name] = value; }
		void Set(string name, Math::vec4 value) { _vec4Map[name] = value; }
		void Set(string name, Core::Resource<Texture> value) { _textureMap[name] = value; }

		void Bind(Shader* shader) {
			for (auto& i : _floatMap) {
				shader->SetUniform(i.first, i.second);
			}

			for (auto& i : _intMap) {
				shader->SetUniform(i.first, i.second);
			}

			for (auto& i : _vec2Map) {
				shader->SetUniform(i.first, i.second);
			}

			for (auto& i : _vec3Map) {
				shader->SetUniform(i.first, i.second);
			}

			for (auto& i : _vec4Map) {
				shader->SetUniform(i.first, i.second);
			}

			for (auto& i : _textureMap) {
				if (i.second.IsLoaded()) {
					shader->SetUniform(i.first, i.second.Get());
				}
			}
		}

		Shader* GetShader() {
			return _shader;
		}

		void SetShader(Shader* shader) {
			_shader = shader;
		}

		string GetName() {
			return _name;
		}

		bool operator==(const Material& other) {
			return _name == other._name;
		}

		static Material FromFile(string path) {
			if (!path.length()) return Material("", 0);

			std::ifstream stream(path);
			Material mat("");

			string attrName;
			string type;
			while (stream >> attrName) {
				if (attrName.at(0) == '@') continue;

				if (attrName.at(0) == '#') {
					mat._shader = Shader::Get(attrName.substr(1));
					mat._shadername = attrName.substr(1);
					continue;
				}

				stream >> type;
				std::transform(type.begin(), type.end(), type.begin(), tolower);

				if (type == "int") {
					i32 val;
					stream >> val;
					mat._intMap[attrName] = val;
				}
				else if (type == "float") {
					f32 val;
					stream >> val;
					mat._floatMap[attrName] = val;
				}
				else if (type == "vec2") {
					vec2 val;
					stream >> val.x >> val.y;
					mat._vec2Map[attrName] = val;
				}
				else if (type == "vec3") {
					vec3 val;
					stream >> val.x >> val.y >> val.z;
					mat._vec3Map[attrName] = val;
				}
				else if (type == "vec4") {
					vec4 val;
					stream >> val.x >> val.y >> val.z >> val.w;
					mat._vec4Map[attrName] = val;
				}
				else if (type == "texture") {
					string name;
					stream >> name;
					mat._textureMap[attrName] = Core::Resource<Texture>::Get(name);
				}
			}

			return mat;
		}

		void ToFile(string path) {
			std::ofstream stream(path);
			stream << "#" << _shadername << std::endl;
			
			for (auto& i : _floatMap) {
				stream << i.first << " float " << i.second << std::endl;
			}

			for (auto& i : _intMap) {
				stream << i.first << " int " << i.second << std::endl;
			}

			for (auto& i : _vec2Map) {
				stream << i.first << " vec2 " << i.second.x << " " << i.second.y << std::endl;
			}

			for (auto& i : _vec3Map) {
				stream << i.first << " vec3 " << i.second.x << " " << i.second.y << " " << i.second.z << std::endl;
			}

			for (auto& i : _vec4Map) {
				stream << i.first << " vec4 " << i.second.x << " " << i.second.y << " " << i.second.z << " " << i.second.w << std::endl;
			}

			for (auto& i : _textureMap) {
				stream << i.first << " texture " << i.second->GetAssetName() << std::endl;
			}
		}

		string GetAssetName() { return _assetname; }
		void SetAssetName(string name) { _assetname = name; }
	private:
		std::map<string, f32>        _floatMap;
		std::map<string, i32>        _intMap;
		std::map<string, Math::vec2> _vec2Map;
		std::map<string, Math::vec3> _vec3Map;
		std::map<string, Math::vec4> _vec4Map;
		std::map<string, Core::Resource<Texture>>   _textureMap;
		string _name, _assetname, _shadername;
		Shader* _shader;
		i32 _copies;
	};

} }

#endif // _BEARISH_GRAPHICS_MATERIAL_H_
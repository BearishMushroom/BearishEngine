#ifndef _BEARISH_GRAPHGICS_SHADER_H_
#define _BEARISH_GRAPHGICS_SHADER_H_

#include <BE/Core/IAllocatable.h>
#include <BE/Core/Logger.h>
#include <BE/Math/vec4.h>
#include <BE/Math/mat4.h>
#include "Lights.h"
#include "OpenGL.h"
#include "Vertex.h"
#include "Buffer\UniformBuffer.h"

namespace Bearish { namespace Graphics {
	class Shader;
	class Texture;

	enum class BEARISH_API ShaderType : u32 {
		Vertex = GL_VERTEX_SHADER,
		Fragment = GL_FRAGMENT_SHADER,
		Geometry = GL_GEOMETRY_SHADER,
	};

	struct BEARISH_API ShaderLoadInformation {
		std::vector<string> files;
		std::vector<ShaderType> types;
		std::vector<u32> timeStamps;
		Shader* shader;
	};


	class BEARISH_API ShaderUniform {
	public:
		i32 Position;
		ShaderUniform() {
			Position = -1;
		}

		ShaderUniform(i32 pos) {
			Position = pos;
		}

		~ShaderUniform() {}

		operator i32 () { return Position; }
	};

	class BEARISH_API Shader : public Core::IAllocatable<Shader> {
	public:
		Shader();
		Shader(const string& vsPath, const string& fsPath);
		~Shader();

		static string LoadShader(const string& filename);
		
		void AddShader(const string& source, const ShaderType type);
		void Compile();

		void RegisterUniform(const string& name);

		void SetAttribLocation(const c8* name, i32 location);
		void SetUniform(const string& name, i32 value);
		void SetUniform(const string& name, f32 value);
		void SetUniform(const string& name, const Math::vec2& value);
		void SetUniform(const string& name, const Math::vec3& value);
		void SetUniform(const string& name, std::vector<Math::vec3>& value);
		void SetUniform(const string& name, const Math::vec4& value);
		void SetUniform(const string& name, const Math::mat4& value);
		void SetUniform(const string& name, std::vector<Math::mat4>& value);
		void SetUniform(const string& name, const Light& value);
		void SetUniform(const string& name, const DirectionalLight& value);
		void SetUniform(const string& name, const Attenuation& value);
		void SetUniform(const string& name, const PointLight& value);
		void SetUniform(const string& name, const SpotLight& value);
		void SetUniform(const string& name, const Texture* const value);
		void SetUniformBlock(const string& name, UniformBuffer* const value);

		void Bind() const;
		void Unbind() const;

		void AddAllStructs(const string& source);
		void AddAllAttributes(const string& source);
		void AddAllUniforms(const string& source);
		void AddAllGSVaryings(const string& source);
		void AddUniform(const string& name, const string& type);

		void SetIsFeedback(bool feedback) { _isFeedback = feedback; }

		void SetName(string name) { _name = name; }
		string GetName() { return _name; }

		static void Set(string name, Shader* me) { _mapped[name] = me; }
		static Shader* Get(string name) { return _mapped.at(name); }

		static void ReloadChanged();
	private:
		// Private types.
		typedef std::pair<string, string>                           GLSLStructComponent;
		typedef std::pair<string, std::vector<GLSLStructComponent>> GLSLStruct;
		typedef std::map <string, std::vector<GLSLStructComponent>> GLSLStructList;

		static std::vector<ShaderLoadInformation> _loaded;
		static std::unordered_map<string, Shader*> _mapped;
		usize _loadID;

		// Temp variables used for pre-compilation data.
		GLSLStructList _structs;
		std::vector<string> _uniformsToAdd, _uniformBlocksToAdd;
		std::vector<std::pair<string, usize>> _attributesToAdd;
		std::vector<string> _gsVaryings;

		// Runtime variables.
		std::unordered_map<string, ShaderUniform> _uniformLocations, _uniformBlockLocations;
		std::unordered_map<string, i32> _samplerLocations;
		string _vsPath, _fsPath;
		string _name;
		u32 _programID;
		bool _isFeedback;
	};
}}

#endif
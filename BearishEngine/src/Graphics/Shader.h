#ifndef _BEARISH_GRAPHGICS_SHADER_H_
#define _BEARISH_GRAPHGICS_SHADER_H_

#include "../Core/IAllocatable.h"
#include "../Core/Logger.h"
#include "../Math/vec4.h"
#include "../Math/mat4.h"
#include "Lights.h"
#include "OpenGL.h"
#include "Vertex.h"

namespace Bearish { namespace Graphics {
	enum class ShaderType : u32 {
		Vertex = GL_VERTEX_SHADER,
		Fragment = GL_FRAGMENT_SHADER,
		Geometry = GL_GEOMETRY_SHADER,
	};

	class ShaderUniform {
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

	class Shader : public Core::IAllocatable<Shader> {
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
		void SetUniform(const string& name, const Math::vec4& value);
		void SetUniform(const string& name, Math::mat4 value);
		void SetUniform(const string& name, std::vector<Math::mat4>& value);
		void SetUniform(const string& name, const Light& value);
		void SetUniform(const string& name, const DirectionalLight& value);
		void SetUniform(const string& name, const Attenuation& value);
		void SetUniform(const string& name, const PointLight& value);
		void SetUniform(const string& name, const SpotLight& value);

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
	private:
		// Private types.
		typedef std::pair<string, string>                           GLSLStructComponent;
		typedef std::pair<string, std::vector<GLSLStructComponent>> GLSLStruct;
		typedef std::map <string, std::vector<GLSLStructComponent>> GLSLStructList;

		// Temp variables used for pre-compilation data.
		GLSLStructList _structs;
		std::vector<string> _uniformsToAdd;
		std::vector<std::pair<string, u32>> _attributesToAdd;
		std::vector<string> _gsVaryings;

		// Runtime variables.
		std::unordered_map<string, ShaderUniform> _uniformLocations;
		string _vsPath, _fsPath;
		string _name;
		u32 _programID;
		bool _isFeedback;
	};
}}

#endif
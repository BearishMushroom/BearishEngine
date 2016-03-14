#include "Shader.h"
#include "Texture.h"
#include "../Core/Timer.h"

using namespace Bearish;
using namespace Graphics;
using namespace Core;

std::vector<ShaderLoadInformation> Shader::_loaded;

void Shader::ReloadChanged() {
	for (auto&& it : _loaded) {
		for (i32 file = 0; file < it.files.size(); ++file) {
			struct stat st;
			stat(it.files.at(file).c_str(), &st);
			if (it.timeStamps.at(file) < st.st_mtime) {
				Logger::Info("Swapping shader %s.", it.files.at(file).c_str());
				Shader newSh = Shader();
				for (i32 i = 0; i < it.files.size(); i++) {
					stat(it.files.at(i).c_str(), &st);
					it.timeStamps.at(i) = st.st_mtime;
					newSh.AddShader(LoadShader(it.files.at(i)), it.types.at(i));
				}
				newSh.Compile();
				*(it.shader) = newSh;
				break;
			}
		}
	}
}

string Shader::LoadShader(const string& filename) { // Recursive function that looks through a shader's source and includes any other files specified.
  //TODO: Profiling.
	string content;
	std::ifstream fileStream(filename, std::ios::in);

	static std::string INCLUDE_DIRECTIVE = "#include";

	if (!fileStream.is_open()) {
		Logger::Error("Could not read shader at \"%s\". File does not exist.", filename);
		return "";
	}

	string line = "";
	while (!fileStream.eof()) {
		std::getline(fileStream, line);

		if (!line.compare(0, INCLUDE_DIRECTIVE.size(), INCLUDE_DIRECTIVE)) {
			const string includePath = line.substr(INCLUDE_DIRECTIVE.size() + 2, line.size() - INCLUDE_DIRECTIVE.size() - 3);

			// + 2 for space and opening quote.
			// - 3 for space and both quotes.
			// #include "res/Shadur.vs"
			//         ||             |

			const string includeSource = Shader::LoadShader(includePath.c_str());
			content.append(includeSource + "\n");
		}
		else {
			content.append(line + "\n");
		}
	}

	fileStream.close();

	return content;
}

Shader::Shader() {
	_programID = glCreateProgram();
}

Shader::Shader(const string& vsPath, const string& fsPath) {
	_programID = glCreateProgram();

	string vsSrc = LoadShader(vsPath.c_str());
	string fsSrc = LoadShader(fsPath.c_str());

	_loadID = _loaded.size();
	_loaded.push_back(ShaderLoadInformation{});
	_loaded.back().files.push_back(vsPath);
	_loaded.back().files.push_back(fsPath);

	struct stat st;
	stat(vsPath.c_str(), &st);
	_loaded.back().timeStamps.push_back(st.st_mtime);
	stat(fsPath.c_str(), &st);
	_loaded.back().timeStamps.push_back(st.st_mtime);

	_loaded.back().types.push_back(ShaderType::Vertex);
	_loaded.back().types.push_back(ShaderType::Fragment);

	_vsPath = vsPath;
	_fsPath = fsPath;

	AddShader(vsSrc, ShaderType::Vertex);
	AddShader(fsSrc, ShaderType::Fragment);
	Compile();
	_loaded.back().shader = this;
}

Shader::~Shader() {
	glDeleteShader(_programID);
}

void Shader::AddShader(const string& source, const ShaderType type) {
	u32 shader;
	shader = glCreateShader((GLenum)type);

	if (!shader) {
		Logger::Error("Shader creation failed, invalid memory location.");
	}

	const c8* sourceCString = source.c_str();

	glShaderSource(shader, 1, &sourceCString, NULL);
	glCompileShader(shader);

	i32 shaderStatus;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &shaderStatus);

	if (shaderStatus == GL_FALSE) {
		c8 log[1024];
		glGetShaderInfoLog(shader, 1024, NULL, log);
		Logger::Error("Shader failed to compile: %s", &log);
	}

	glAttachShader(_programID, shader);

	AddAllStructs(source);
	
	if (type == ShaderType::Vertex) {
		AddAllAttributes(source);
	}

	AddAllUniforms(source);

	if (type == ShaderType::Geometry && _isFeedback) {
		AddAllGSVaryings(source);
		const c8** cstrs = new const c8*[_gsVaryings.size()];
		for (i32 i = 0; i < (i32)_gsVaryings.size(); i++) {
			cstrs[i] = _gsVaryings[i].c_str();
		}

		glTransformFeedbackVaryings(_programID, _gsVaryings.size(), cstrs, GL_INTERLEAVED_ATTRIBS);
	}
}

void Shader::Compile() {
	Timer timer;
	timer.Start();
	glLinkProgram(_programID);

	i32 programStatus;
	glGetProgramiv(_programID, GL_LINK_STATUS, &programStatus);

	if (programStatus == GL_FALSE) {
		c8 log[1024];
		glGetProgramInfoLog(_programID, 1024, NULL, log);
		Logger::Error("Shader program failed to compile: %s", &log);
	}

	glValidateProgram(_programID);

	glGetProgramiv(_programID, GL_VALIDATE_STATUS, &programStatus);

	if (programStatus == GL_FALSE) {
		c8 log[1024];
		glGetProgramInfoLog(_programID, 1024, NULL, log);
		Logger::Error("Shader program failed to validate: %s", &log);
	}

	for (auto& str : _uniformsToAdd) {
		RegisterUniform(str);
	}
	_uniformsToAdd.clear();

	for (auto& str : _uniformBlocksToAdd) {
		_uniformBlockLocations.emplace(str, glGetUniformBlockIndex(_programID, str.c_str()));
		glUniformBlockBinding(_programID, _uniformBlockLocations.at(str), _uniformBlockLocations.at(str));
	}
	_uniformBlocksToAdd.clear();

	for (auto& attribute : _attributesToAdd) {
		SetAttribLocation(attribute.first.c_str(), attribute.second);
	}
	_attributesToAdd.clear();

	_structs.clear();
	Logger::Info("Shader compiled in %.3fms", timer.DeltaMS());
}

void Shader::RegisterUniform(const string& name) {
	_uniformLocations.insert(std::pair<string, ShaderUniform>(name, ShaderUniform(glGetUniformLocation(_programID, name.c_str()))));
}

void Shader::SetAttribLocation(const c8* name, i32 location) {
	glBindAttribLocation(_programID, location, name);
}

void Shader::SetUniform(const string& name, i32 value) {
	i32 pos = _uniformLocations[name];
	if (pos == -1) return;
	glUniform1i(pos, value);
}

void Shader::SetUniform(const string& name, f32 value) {
	i32 pos = _uniformLocations[name];
	if (pos == -1) return;
	glUniform1f(pos, value);
}

void Shader::SetUniform(const string& name, const Math::vec2& value) {
	i32 pos = _uniformLocations[name];
	if (pos == -1) return;
	glUniform2f(pos, value.x, value.y);
}

void Shader::SetUniform(const string& name, const Math::vec3& value) {
	i32 pos = _uniformLocations[name];
	if (pos == -1) return;
	glUniform3f(pos, value.x, value.y, value.z);
}

void Shader::SetUniform(const string& name, const Math::vec4& value) {
	i32 pos = _uniformLocations[name];
	if (pos == -1) return;
	glUniform4f(pos, value.x, value.y, value.z, value.w);
}

void Shader::SetUniform(const string& name, Math::mat4 value) {
	i32 pos = _uniformLocations[name];
	if (pos == -1) return;
	glUniformMatrix4fv(pos, 1, GL_TRUE, &(value[0][0]));
}

void Shader::SetUniform(const string& name, const Light& value) {
	SetUniform(name + ".color", value.GetColor());
	SetUniform(name + ".ambientIntensity", value.GetAmbientIntensity());
	SetUniform(name + ".diffuseIntensity", value.GetDiffuseIntensity());
}

void Shader::SetUniform(const string& name, const DirectionalLight& value) {
	SetUniform(name + ".base", (Light)value);
	SetUniform(name + ".direction", value.GetDirection().Forward());
}

void Shader::SetUniform(const string& name, const Attenuation& value) {
	SetUniform(name + ".constant", value.GetConstant());
	SetUniform(name + ".linear", value.GetLinear());
	SetUniform(name + ".exponent", value.GetExponent());
}

void Shader::SetUniform(const string& name, const PointLight& value) {
	SetUniform(name + ".base", (Light)value);
	SetUniform(name + ".attenuation", value.GetAttenuation());
	SetUniform(name + ".position", value.GetPosition());
}

void Shader::SetUniform(const string& name, const SpotLight& value) {
	SetUniform(name + ".base", (PointLight)value);
	SetUniform(name + ".direction", value.GetDirection().Forward());
	SetUniform(name + ".cutoff", value.GetCutoff());
}

void Shader::SetUniform(const string& name, std::vector<Math::mat4>& value) {
	glUniformMatrix4fv(_uniformLocations.at(name), value.size(), GL_TRUE, &((value[0])[0][0]));
}

void Shader::SetUniform(const string& name, const Texture* const value) {
	i32 loc = _samplerLocations[name];
	SetUniform(name, loc);
	value->Bind(loc);
}

void Shader::SetUniformBlock(const string& name, UBO* const value) {
	value->Bind(_uniformBlockLocations.at(name));
}

void Shader::Bind() const {
	glUseProgram(_programID);
}

void Shader::Unbind() const {
	glUseProgram(0);
}

void Shader::AddAllStructs(const string& source) {
	static string STRUCT_DIRECTIVE = "struct";
	u32 structStartLocation = source.find(STRUCT_DIRECTIVE, 0);

	while (structStartLocation != std::string::npos) {
		u32 nameBegin = structStartLocation + STRUCT_DIRECTIVE.size() + 1;
		u32 braceBegin = source.find("{", nameBegin);
		u32 braceEnd = source.find("}", braceBegin);

		string structName(source.begin() + nameBegin, source.begin() + braceBegin - 1);

		std::vector<GLSLStructComponent> structComponents;

		u32 componentSemicolonPosition = source.find(";", braceBegin);
		while (componentSemicolonPosition != string::npos && componentSemicolonPosition < braceEnd) {
			u32 componentNameStart = componentSemicolonPosition;

			while (!isspace(source[componentNameStart - 1])) {
				componentNameStart--;
			}

			u32 componentTypeEnd = componentNameStart - 1;
			u32 componentTypeStart = componentTypeEnd;

			while (!isspace(source[componentTypeStart - 1])) {
				componentTypeStart--;
			}

			string componentName(source.begin() + componentNameStart, source.begin() + componentSemicolonPosition);
			string componentType(source.begin() + componentTypeStart, source.begin() + componentTypeEnd);

			structComponents.push_back(GLSLStructComponent(componentName, componentType));

			componentSemicolonPosition = source.find(";", componentSemicolonPosition + 1);
		}

		_structs.insert(GLSLStruct(structName, structComponents));

		structStartLocation = source.find(STRUCT_DIRECTIVE, structStartLocation + STRUCT_DIRECTIVE.size());
	}
}

void Shader::AddAllUniforms(const string& source) {
	// Look for uniforms.
	// Format: uniform vec3...;
	static string UNIFORM_DIRECTIVE = "uniform";

	u32 offset = 0, position = 0;
	while ((position = source.find(UNIFORM_DIRECTIVE, offset)) != string::npos) {
		offset = position + UNIFORM_DIRECTIVE.length() + 1; // Add one to account for space "uniform sampler..."
		u32 nameStartPos = source.find(" ", offset) + 1; // Add one to account for space  |-------|
		u32 nameEndPos = source.find("=", offset) < source.find("\n", offset) ? source.find("=", offset) - 1 : source.find(";", offset);
		string name = source.substr(nameStartPos, nameEndPos - nameStartPos);
		string type = source.substr(offset, nameStartPos - offset - 1);

		if (*name.begin() == '{') {
			_uniformBlocksToAdd.push_back(type);
			continue;
		}

		AddUniform(name, type);
	}
}

void Shader::AddUniform(const string& name, const string& type) {
	bool shouldAdd = true;

	string nameToAdd(name);
	std::vector<GLSLStructComponent> structComponents;

	if (_structs.find(type) != _structs.end()) {
		structComponents = _structs.at(type);
		shouldAdd = false;

		for (i32 i = 0; i < (i32)structComponents.size(); i++) {
			AddUniform(nameToAdd + "." + structComponents.at(i).first, structComponents.at(i).second);
		}
	}
	else {
		if (type.find("sampler") != string::npos) {
			_samplerLocations.emplace(name, _samplerLocations.size());
		}
	}

	i32 arrNum = 0;
	i32 arrPlace = 0;
	if ((arrPlace = nameToAdd.find('[')) != string::npos) {
		i32 arrEnd = nameToAdd.find(']');
		arrPlace++;
		arrNum = std::stoi(nameToAdd.substr(arrPlace, arrEnd));
	}

	if (shouldAdd) {
		if (arrNum > 0) {
			for (i32 i = 0; i < arrNum; i++) {
				_uniformsToAdd.push_back(nameToAdd.substr(0, arrPlace) + std::to_string(i) + "]");
			}

			nameToAdd = nameToAdd.substr(0, arrPlace - 1);
		}
		_uniformsToAdd.push_back(nameToAdd); // Even if the uniform is an array, add the start of it for the glUniform*v functions.
	}
}

void Shader::AddAllAttributes(const string& source) {
	// Look for attributes.
	// Format: layout (location = 0) in vec2...;
	static string ATTRIBUTE_DIRECTIVE = "layout";
	u32 offset = 0, position = 0;

	while ((position = source.find(ATTRIBUTE_DIRECTIVE, offset)) != string::npos) {
		offset = position + ATTRIBUTE_DIRECTIVE.length() + 1; // Add one to account for space "uniform sampler..."
		u32 nameStartPos = source.find(" ", offset) + 1; // Add one to account for space  |-------|
		u32 nameEndPos = source.find(";", offset);
		string name = source.substr(nameStartPos, nameEndPos - nameStartPos);
		_gsVaryings.push_back(name);
	}
}

void Shader::AddAllGSVaryings(const string& source) {
	static string VARYING_DIRECTIVE = "out";
	u32 offset = 0, position = 0;
	while ((position = source.find(VARYING_DIRECTIVE, offset)) != string::npos) {
		offset = position + VARYING_DIRECTIVE.length() + 1;

		u32 positionStartPos = source.find("=", offset) + 2;
		u32 positionEndPos = source.find(")", offset);
		u32 attributePosition = std::stoi(source.substr(positionStartPos, positionEndPos - positionStartPos));

		u32 nameStartPos = source.find(" ", source.find("in", offset) + 3) + 1;
		u32 nameEndPos = source.find(";", offset);
		string name = source.substr(nameStartPos, nameEndPos - nameStartPos);

		_attributesToAdd.push_back(std::pair<string, u32>(name, attributePosition));
	}
}
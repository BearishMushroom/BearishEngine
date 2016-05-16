#include "OpenGL.h"
#include "../Core/Logger.h"
#include "../Utils.h"
#include <glfw3.h>
#include <glad.h>

void APIENTRY gl_debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, GLvoid *userParam) {
	switch (type) {
	case GL_DEBUG_TYPE_ERROR:
		Bearish::Core::Logger::Error("%d %s\n", source, message);
		break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		Bearish::Core::Logger::Warn("%d %s\n", source, message);
		break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		Bearish::Core::Logger::Warn("%d %s\n", source, message);
		break;
	case GL_DEBUG_TYPE_PORTABILITY:
		//BEARISH_INFO("%d %s\n", source, message);
		break;
	case GL_DEBUG_TYPE_PERFORMANCE:
		//BEARISH_INFO("%d %s\n", source, message);
		break;
	case GL_DEBUG_TYPE_OTHER:
		//BEARISH_INFO("%d %s\n", source, message);
		break;
	case GL_DEBUG_TYPE_MARKER:
		//BEARISH_INFO("%d %s\n", source, message);
		break;
	case GL_DEBUG_TYPE_PUSH_GROUP:
		//BEARISH_INFO("%d %s\n", source, message);
		break;
	case GL_DEBUG_TYPE_POP_GROUP:
		//BEARISH_INFO("%d %s\n", source, message);
		break;
	}
}

using namespace Bearish;
using namespace Graphics;
using namespace Core;

bool OpenGL::_isOGLLoaded = false;
std::vector<string> OpenGL::_extensions;

void* OpenGL::GetFn(const char* name) {
	void *p = (void *)wglGetProcAddress(name);
	if (p == 0 ||
		(p == (void*)0x1) || (p == (void*)0x2) || (p == (void*)0x3) ||
		(p == (void*)-1)) {
		HMODULE module = LoadLibraryA("opengl32.dll");
		p = (void *)GetProcAddress(module, name);
	}

	return p;
}

void OpenGL::Init() {
	if (!gladLoadGLLoader((GLADloadproc)&GetFn)) {
		Logger::Fatal("Failed to load OpenGL functions!");
	}

	Logger::Info("OpenGL loaded! Version %d.%d", GLVersion.major, GLVersion.minor);

	GetAllExtensions();
	SetDebugCallback();
	_isOGLLoaded = true;
}

bool OpenGL::HandleErrors() {
	i32 error; 
	bool isError = false;
	
	while ((error = glGetError()) != GL_NO_ERROR) {
		string errorStr = GetErrorString(error);
		Logger::Error(errorStr);
		isError = true;
	}

	return isError;
}

string OpenGL::GetErrorString(i32 error) {
	switch (error) {
		case GL_INVALID_ENUM                  : return "GL INVALID ENUM";                  break;
		case GL_INVALID_VALUE                 : return "GL INVALID VALUE";                 break;
		case GL_INVALID_OPERATION             : return "GL INVALID OPERATION";             break;
		case GL_INVALID_FRAMEBUFFER_OPERATION : return "GL INVALID FRAMEBUFFER OPERATION"; break;
		case GL_OUT_OF_MEMORY                 : return "GL OUT OF MEMORY";                 break;
		case GL_STACK_UNDERFLOW               : return "GL STACK UNDERFLOW";               break;
		case GL_STACK_OVERFLOW                : return "GL STACK OVERFLOW";                break;
	}

	return "";
}

bool OpenGL::IsExtensionSupported(string extension) {
	return std::find(_extensions.begin(), _extensions.end(), extension) != _extensions.end();
}

void OpenGL::SetDebugCallback() {
	if (IsExtensionSupported("GL_KHR_debug") || IsExtensionSupported("ARB_debug_output")) {
		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback((GLDEBUGPROC)&gl_debug_callback, NULL);
	}
	else {
		Logger::Warn("Failed to set up GL debugging.");
	}
}

void OpenGL::PrintSupportedExtensions() {
	string total = "";
	for (auto ext : _extensions) {
		total = total.append(ext);
		total = total.append(" ");
	}

	Logger::Info(total);
}

void OpenGL::GetAllExtensions() {
	string str = string(reinterpret_cast<const c8*>(glGetString(GL_EXTENSIONS)));
	if (!str.empty()) {
		std::vector<string> extensions = Util::SplitString(str, " ");
		Logger::Info("OpenGL extensions loaded: %d", extensions.size());
		for (auto& s : extensions) {
			s.erase(remove_if(s.begin(), s.end(), isspace), s.end());
		}
		_extensions = extensions;
	}
}
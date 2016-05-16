#include "Win32WindowGLViewport.h"
#include "../Graphics/OpenGL.h"


using namespace Bearish;
using namespace GUI;

void Win32WindowGLViewport::Register(void* parentHandle, i32 id) {
	_hwnd = CreateWindowEx(NULL, "STATIC", (LPSTR)NULL, WS_VISIBLE | WS_CLIPCHILDREN | WS_CHILD,
							_x, _y, _width, _height, (HWND)parentHandle, (HMENU)id, GetModuleHandle(NULL), NULL);
	//_hwnd = parentHandle;
	_hdc = GetDC((HWND)_hwnd);

	PIXELFORMATDESCRIPTOR pfd; // Create a new PIXELFORMATDESCRIPTOR (PFD)  
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR)); // Clear our  PFD  
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR); // Set the size of the PFD to the size of the class  
	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW; // Enable double buffering, opengl support and drawing to a window  
	pfd.iPixelType = PFD_TYPE_RGBA; // Set our application to use RGBA pixels  
	pfd.cColorBits = 32; // Give us 32 bits of color information (the higher, the more colors)  
	pfd.cDepthBits = 32; // Give us 32 bits of depth information (the higher, the more depth levels)  
	pfd.iLayerType = PFD_MAIN_PLANE; // Set the layer of the PFD  

	i32 nPixelFormat = ChoosePixelFormat((HDC)_hdc, &pfd); // Check if our PFD is valid and get a pixel format back  
	if (nPixelFormat == 0) // If it fails  
		return;

	bool bResult = SetPixelFormat((HDC)_hdc, nPixelFormat, &pfd);

	HGLRC tempOpenGLContext = wglCreateContext((HDC)_hdc); // Create an OpenGL 2.1 context for our device context  
	wglMakeCurrent((HDC)_hdc, tempOpenGLContext); // Make the OpenGL 2.1 context current and active  
	HMODULE dll = LoadLibraryA("opengl32.dll");

	Graphics::OpenGL::Init();

	/*i32 attributes[] = {
		0x2091, 3, // Set the MAJOR version of OpenGL to 3  
		0x2092, 3, // Set the MINOR version of OpenGL to 2  
		0x2094, 0x0002, // Set our OpenGL context to be forward compatible  
		0
	};

	_createProc = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
	_hglrc = _createProc((HDC)_hdc, NULL, attributes);

	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(tempOpenGLContext);
	wglMakeCurrent((HDC)_hdc, (HGLRC)_hglrc);*/
	glViewport(0, 0, 1280, 720);
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Win32WindowGLViewport::Update() {
	SwapBuffers((HDC)_hdc);
}

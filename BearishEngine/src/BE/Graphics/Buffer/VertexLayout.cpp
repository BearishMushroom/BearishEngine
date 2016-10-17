#include <BE/Graphics/Buffer/VertexLayout.h>
#include <BE/Graphics/Vertex.h>

using namespace Bearish;
using namespace Graphics;
using namespace Core;

VertexLayout<Vertex> VertexLayout<Vertex>::Default = VertexLayout<Vertex>([](VertexLayout<Vertex>& l) {
	l.PushComponent("POSITION", sizeof(Math::vec3));
	l.PushComponent("TEXCOORD", sizeof(Math::vec2));
	l.PushComponent("NORMAL", sizeof(Math::vec3));
	l.PushComponent("TANGENT", sizeof(Math::vec3));

	// Constants for skinning
	l.PushComponent(Math::vec4i(-1, -1, -1, -1));
	l.PushComponent(Math::vec4(0, 0, 0, 0));
});


VertexLayout<SkinnedVertex> VertexLayout<SkinnedVertex>::Default = VertexLayout<SkinnedVertex>([](VertexLayout<SkinnedVertex>& l) {
	l.PushComponent("POSITION", sizeof(Math::vec3));
	l.PushComponent("TEXCOORD", sizeof(Math::vec2));
	l.PushComponent("NORMAL", sizeof(Math::vec3));
	l.PushComponent("TANGENT", sizeof(Math::vec3));
	l.PushComponent("BONEID", sizeof(Math::vec4i));
	l.PushComponent("BONEWEIGHT", sizeof(Math::vec4));
});

template<typename T>
VertexLayout<T> VertexLayout<T>::Default = VertexLayout<T>();

#ifndef _BEARISH_COMPONENTS_UILABEL_H_
#define _BEARISH_COMPONENTS_UILABEL_H_

#include "../Core/IActorComponent.h"
#include "../Core/IAllocatable.h"
#include "../Graphics/Texture.h"
#include "../Graphics/Font.h"
#include "../Graphics/Mesh/Mesh.h"
#include "../Types.h"

namespace Bearish { namespace Components {
	class UILabel : public Core::IActorComponent, public Core::IAllocatable<UILabel> {
	public:
		UILabel(Graphics::Font* font, std::string text, f32 scale) : _font(font), _text(text), _mesh(font->GenerateMesh(text, scale)), _scale(scale) {}
		~UILabel() {}

		virtual void Draw2D(Graphics::RenderingEngine* engine, Graphics::Shader* shader, Graphics::Camera* camera) override {
			Math::vec3 offset = _actor->GetParent() ? _actor->GetParent()->GetTranslation() : Math::vec3(0);
			Core::Transform model = _actor->GetTransform();
			model.GetTranslation() += offset;

			_mesh->Submit(model.GetTransformation(), camera->GetViewMatrix() * model.GetTransformation());

			_font->GetTexture()->Bind(0);
			_mesh->Flush(shader);
			_font->GetTexture()->Unbind(0);
		}

		void SetText(std::string text, f32 scale) {
			_text = text;
			_scale = scale;
			delete _mesh;
			_mesh = _font->GenerateMesh(text, scale);
		}
	private:
		f32 _scale;
		Graphics::Mesh* _mesh;
		Graphics::Font* _font;
		std::string _text;
	};
} }

#endif // _BEARISH_COMPONENTS_UILABEL_H_
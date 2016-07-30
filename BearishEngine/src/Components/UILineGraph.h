#ifndef _BEARISH_COMPONENT_UILINEGRAPH_
#define _BEARISH_COMPONENT_UILINEGRAPH_

#include "../Types.h"
#include "../Core/IActorComponent.h"
#include "../Core/Actor.h"
#include "../Utils.h"
#include "../Graphics/Renderer.h"

namespace Bearish { namespace Components {
	class UILineGraph : public Core::IActorComponent {
	public:
		UILineGraph(f32 min = 0, f32 max = 1, i32 numDataPoints = 100, const Math::vec4& color = vec4(1, 1, 1, 1)) : _min(min), _max(max), _numDataPoints(numDataPoints) {
			_texture = new Graphics::Texture(color);
			_line = 0;

		}

		~UILineGraph() {
			delete _line;
		}

		void AddDataPoint(f32 data) {
			_dataPoints.push_back(data);

			if (_dataPoints.size() >= (u32)_numDataPoints) {
				_dataPoints.erase(_dataPoints.begin());
			}

			UpdateMesh();
		}

		void UpdateMesh() {
			Math::vec3 scale = _actor->GetScale();

			f32 minX = -scale.x / 2;
			f32 maxX = scale.x / 2;
		
			f32 minY = scale.y / 2;
			f32 maxY = -scale.y / 2;

			f32 xscale = (maxX - minX) / _numDataPoints;

			std::vector<Graphics::Vertex> vertices;
			std::vector<u32> indices;
			u32 current = 0;

			for (auto& point : _dataPoints) {
				indices.push_back(current);

				vertices.push_back(Graphics::Vertex(vec3(minX + current * xscale, 
														minY + Util::Clamp(point, _min, _max) / (_max - _min) * (maxY - minY), 0)));
			
				current++;
			}

			if(!_line) _line = new Graphics::Mesh(vertices, indices);
			else {
				_line->SetVertexData(&vertices[0], sizeof(Graphics::Vertex) * (u32)vertices.size());
				_line->SetIndexData(&indices[0], (i32)indices.size());
			}
		}

		virtual void Draw2D(Graphics::RenderingEngine* engine, Graphics::Shader* shader, Graphics::Camera* camera) override {
			if (_line) {
				Math::vec3 offset = _actor->GetParent() ? _actor->GetParent()->GetTranslation() : Math::vec3(0);
				Core::Transform model = _actor->GetTransform();
				model.GetTranslation() += offset;
				model.SetScale(vec3(1, 1, 1));

				Graphics::PrimitiveMode oldMode = Graphics::Renderer::GetPrimitiveMode();
				Graphics::Renderer::SetPrimitiveMode(Graphics::PrimitiveMode::LineStrip);

				_texture->Bind(0);

				_line->Submit(0, model.GetTransformation(), camera);
				_line->Flush(shader);

				_texture->Unbind(0);

				Graphics::Renderer::SetPrimitiveMode(oldMode);
			}
		}
	private:
		Graphics::Mesh* _line;
		Graphics::Texture* _texture;
		std::vector<f32> _dataPoints;

		f32 _min, _max;
		i32 _numDataPoints;
	};
} }

#endif // _BEARISH_COMPONENT_UILINEGRAPH_
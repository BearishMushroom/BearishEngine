#ifndef _BEARISH_SERILIZATION_OBJECT_H_
#define _BEARISH_SERILIZATION_OBJECT_H_

#include <BE\Serialization\Field.h>
#include <BE\Serialization\Type.h>

#include <vector>

namespace Bearish { namespace Serilization {
	class Object {
	public:
		Object() {}

		Object(string name) {
			_name = name;
		}


		Object(string name, std::vector<Field> fields) {
			_name = name;
			_fields = fields;
		}

		template<typename T>
		Object& Type() {
			_type = Serilization::Type<T>::name;
			return *this;
		}

		void AddField(Field f) { _fields.push_back(f); }

		string GetName() const { return _name; }
		string GetType() const { return _type; }
		const std::vector<Field>& GetFields() const { return _fields; }
	private:
		string _name, _type;
		std::vector<Field> _fields;
	};
} }

#endif // _BEARISH_SERILIZATION_OBJECT_H_
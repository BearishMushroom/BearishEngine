#ifndef _BEARISH_SERIALIZATION_FIELD_H_
#define _BEARISH_SERIALIZATION_FIELD_H_

#include <BE\Types.h>
#include <BE\Serialization\Type.h>

namespace Bearish { namespace Serilization {
	class Field {
	public:
		template<typename T>
		Field(T& data) {
			_elements = 1;
			_size = sizeof(T);
			_data = new T(data);
			_type = Type<T>::name;
		}
		
		template<typename T>
		Field(T* data) {
			_elements = 1;
			_size = sizeof(T);
			_data = new T(data);
			_type = Type<T>::name;
		}

		template<typename T>
		Field(string name, T& data) {
			_elements = 1;
			_size = sizeof(T);
			_name = name;
			_data = new T(data);
			_type = Type<T>::name;
		}

		template<typename T>
		Field(string name, T* data) {
			_elements = 1;
			_size = sizeof(T);
			_name = name;
			_data = new T(data);
			_type = Type<T>::name;
		}

		template<typename T>
		Field(T* data, i32 elements) {
			_size = sizeof(T);
			_data = new T[elements](data);
			_elements = elements;
			_type = Type<T>::name;
		}

		template<typename T>
		Field(std::vector<T>& data) {
			_size = sizeof(T);
			_data = new T[data.size()](&data[0]);
			_elements = data.size();
			_type = Type<T>::name;
		}

		template<typename T>
		Field(string name, T* data, i32 elements) {
			_size = sizeof(T);
			_name = name;
			_data = data;
			_elements = elements;
			_type = Type<T>::name;
		}

		template<typename T>
		Field(string name, std::vector<T>& data) {
			_size = sizeof(T);
			_name = name;
			_data = &data[0];
			_elements = data.size();
			_type = Type<T>::name;
		}

		~Field() {
			if (_elements > 1) {
				delete[] _data;
			}
			else {
				delete _data;
			}
		}

		template<typename T>
		T* Get() const { return (T)_data; }

		i32 GetOffset() const { return _offset; }
		void SetOffset(i32 offset) { _offset = offset; }

		string GetName() const { return _name; }
		string GetType() const { return _type; }

		i32 GetElements() const { return _elements; }
		i32 GetSize() const { return _size; }
	private:
		string _name, _type;
		i32 _offset, _elements, _size;
		void* _data;
	};
} } 

#endif // _BEARISH_SERIALIZATION_FIELD_H_

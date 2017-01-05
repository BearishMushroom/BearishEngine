#ifndef _BEARISH_SERILIZATION_DATABASE_H_
#define _BEARISH_SERILIZATION_DATABASE_H_

#include <BE\Serialization\Object.h>
#include <deque>

namespace Bearish { namespace Serilization {
	class Database;
	template<typename T, bool>
	class Saver {
		void Save(Database& db, T& data);
	};

	class Database {
	public:
		Database(string name = "") {
			_name = name;
			_mode = true;
		}
		 
		template<typename T>
		void Start() {
			_current.push_back(Object().Type<T>());
		}

		template<typename T>
		void Push(T& data) {
			Saver<T, std::is_fundamental<T>::value> s;
			s.Save(*this, data);
		}

		void End() {
			_objects.push_back(_current.back());
			_current.pop_back();
		}

		Object& GetCurrent() {
			return _current.back();
		}

		void Save() { _mode = true; }
		void Load() { _mode = false; }
	private:
		bool _mode;
		string _name;
		std::deque<Object> _current;
		std::vector<Object> _objects;
	};

	template<typename T>
	class Saver <T, false> {
	public:
		void Save(Database& db, T& data) {
			data.Save(db);
		}
	};

	template<>
	class Saver <string, false> {
	public:
		void Save(Database& db, string& data) {
			db.GetCurrent().AddField(Field(data));
		}
	};

	template<typename T>
	class Saver <T, true> {
	public:
		friend class Database;
		void Save(Database& db, T& data) {
			db.GetCurrent().AddField(Field(data));
		}
	};
} }

/*
New serialization structure:
	Definition structures:
		Field {
			string typename; // used in factory creation
			usize offset, size; // position in struct
		}

		Type {
			string typename;
			vector<Field> fields; // a type with 0 fields is primitive
			usize size;
		}
	Implementation structures:
		Object {
			string typename;
			Object* parent;
			usize offset, size; // if (parent), tell us where we are in (parent)
			void* data;
			vector<Object> children;
		}

	At start-time, define all types.
		#define BEARISH_SERIALIZE_BASE(t, base)
			Seriailization::Type basetype = Serialization::GetType(#base);
			t.AddType(basetype);

		#define BEARISH_SERIALIZE_FIELD(t, type, field, ftype)
			t.AddField<ftype>(#ftype, offsetof(&type::field));

		#define BEARISH_SERIALIZE_TYPE(type)
			Serialization::Type(#type);
			TypeFactory::Declare<type>(#type, []() -> type* { return new type(); });
			TypeLookup::Add(#type, typeid(type));
		
		#define BEARISH_SERIALIZE_PRIMITIVE(type)
			Serializetion::AddType(#type, sizeof(type));
			
		Serialization::AddType("f32", sizeof(f32));

		Serialization::Type vec2_ = BEARISH_SERIALIZE_TYPE(vec2);
			BEARISH_SERIALIZE_FIELD(vec2_, vec2, x, f32);
			BEARISH_SERIALIZE_FIELD(vec2_, vec2, y, f32);
		Serialization::AddType(vec2_);

	At run time, (de-)serialize.
		Serialization::Database db;
		vec2 x(20, 20);
		db.Serialize<vec2>(x);
-------------------------------------------------------
		Serialization::Database db = Serialization::BinaryArchive("scene.db").ToDatabase();
		vec2 x = db.Deserialize<vec2>();


*/

#endif // n_BEARISH_SERILIZATION_DATABASE_H_

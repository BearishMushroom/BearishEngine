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
	private:
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

	template<typename T>
	class Saver <T, true> {
	public:
		friend class Database;
		void Save(Database& db, T& data) {
			db.GetCurrent().AddField(Field(data));
		}
	};
} }

#endif // n_BEARISH_SERILIZATION_DATABASE_H_

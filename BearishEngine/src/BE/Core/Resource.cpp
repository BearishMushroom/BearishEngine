#include "Resource.h"

#include <BE\Core\Model.h>
#include <BE\Graphics\Mesh\Mesh.h>
#include <BE\Core\ResourceManager.h>
#include <BE\Graphics\Texture\Texture.h>
#include <BE\Graphics\Texture\TextureCube.h>

using namespace Bearish;
using namespace Core;
using namespace Graphics;

std::map<string, Shared<Model*>>       Resource<Model>::_loaded;
std::map<string, Shared<Mesh*>>        Resource<Mesh>::_loaded;
std::map<string, Shared<Texture*>>     Resource<Texture>::_loaded;
std::map<string, Shared<TextureCube*>> Resource<TextureCube>::_loaded;

template<>
Resource<Model> Resource<Model>::Get(string name, Action<Resource<Model>&> end) {
	if (_loaded.find(name) != _loaded.end()) {
		return Resource<Model>(name, _loaded.at(name));
	}
	
	string realname = "asset/" + name + ".bem";
	Resource<Model> res(name);
	Model** model = new Model*;

	ResourceManager::Queue([&, realname, model]() {
		*model = new Model(realname);
	}, [&, model, res, end, name]() mutable {
		res.IsLoaded(true);
		res.Set(*model);
		_loaded[name] = res._resource;
		end(res);
	});

	return res;
}

template<>
Resource<Mesh> Resource<Mesh>::Get(string name, Action<Resource<Mesh>&> end) {
	if (_loaded.find(name) != _loaded.end()) {
		return Resource<Mesh>(name, _loaded.at(name));
	}

	Resource<Mesh> res(name);
	Resource<Model> source = Resource<Model>::Get(name, [&, res, end, name](Resource<Model>& src) mutable {
		res.IsLoaded(true);
		res.Set(new Mesh(src.Get()));
		_loaded[name] = res._resource;
		end(res);
	});

	return res;
}

template<>
Resource<Texture> Resource<Texture>::Get(string name, Action<Resource<Texture>&> end) {
	if (_loaded.find(name) != _loaded.end()) {
		return Resource<Texture>(name, _loaded.at(name));
	}

	string realname = "asset/" + name + ".bet";
	Resource<Texture> res(name);
	BET::BETFile** file = new BET::BETFile*;

	ResourceManager::Queue([&, realname, file]() {
		*file = new BET::BETFile();
		(*file)->ReadFromFile(realname);
		(*file)->ConvertToRaw();
	}, [&, file, res, end, realname, name]() mutable {
		Logger::Info("Texture %s loaded.", realname.c_str());
		res.IsLoaded(true);
		res.Set(new Texture(*file));
		free((*file)->data);
		_loaded[name] = res._resource;
		end(res);
	});

	return res;
}

template<>
Resource<TextureCube> Resource<TextureCube>::Get(string name, Action<Resource<TextureCube>&> end) {
	if (_loaded.find(name) != _loaded.end()) {
		return Resource<TextureCube>(name, _loaded.at(name));
	}

	static std::vector<string> names = {
		"_right", "_left", "_top", "_bottom", "_front", "_back"
	};

	Resource<TextureCube> res(name);
	BET::BETFile*** file = new BET::BETFile**[6];
	for (i32 i = 0; i < 6; i++) {
		file[i] = new BET::BETFile*;
	}

	ResourceManager::Queue([&, name, file]() {
		for (i32 i = 0; i < 6; i++) {
			*file[i] = new BET::BETFile();
			(*file[i])->ReadFromFile("asset/" + name + names[i] + ".bet");
			(*file[i])->ConvertToRaw();
		}
	}, [&, file, res, end, name]() mutable {
		Logger::Info("Cubemap '%s' loaded.", name.c_str());
		res.IsLoaded(true);
		res.Set(new TextureCube(*file[0], *file[1], *file[2], *file[3], *file[4], *file[5]));

		for (i32 i = 0; i < 6; i++) {
			free((*file[i])->data);
			delete file[i];
		}
		delete[] file;

		_loaded[name] = res._resource;
		end(res);
	});

	return res;
}
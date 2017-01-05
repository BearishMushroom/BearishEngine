#include "ResourceManager.h"
#include <BE\Core\JobFactory.h>

using namespace Bearish;
using namespace Core;

bool ResourceManager::_isContinuous;
std::vector<ResourceJob> ResourceManager::_loadQueue;
std::vector<ResourceJob> ResourceManager::_unloadQueue;
std::vector<ResourceJob> ResourceManager::_progressQueue;

void ResourceManager::Queue(Action<> loader, Action<> end) {
	_loadQueue.push_back({ loader, end });
}

void ResourceManager::Update() {
	if (_loadQueue.size()) {
		JobFactory::AddJob(_loadQueue.back().loader, _loadQueue.back().onEnd);
		_loadQueue.pop_back();
	} else if (_unloadQueue.size()) {
		JobFactory::AddJob(_unloadQueue.back().loader, _unloadQueue.back().onEnd);
		_unloadQueue.pop_back();
	}
}

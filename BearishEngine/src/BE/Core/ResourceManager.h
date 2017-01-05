#ifndef _BEARISH_CORE_RESOURCEMANAGER_H_
#define _BEARISH_CORE_RESOURCEMANAGER_H_

#include <BE\Types.h>
#include <BE\Core\Callback.h>

namespace Bearish { namespace Core {
	struct ResourceJob {
		Action<> loader;
		Action<> onEnd;
	};

	class ResourceManager {
	public:
		static void Queue(Action<> loader, Action<> end = []() {});
		static void SetContinous(bool cont) { _isContinuous = cont; }
		static void Update();
	private:
		static bool _isContinuous;
		static std::vector<ResourceJob> _loadQueue;
		static std::vector<ResourceJob> _unloadQueue;
		static std::vector<ResourceJob> _progressQueue;
	};
} }

#endif // _BEARISH_CORE_RESOURCEMANAGER_H_

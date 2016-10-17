#ifndef _BEARISH_CORE_BLOCK_ALLOCATOR_H_
#define _BEARISH_CORE_BLOCK_ALLOCATOR_H_

#include <BE/Types.h>
#include <BE/Core/Logger.h>

namespace Bearish { namespace Core {
	template <usize CHUNK_SIZE>
	class BEARISH_API BlockAllocator {
	private:
		const u32 MAX_SIZE = 1024; // Allocate no more space than enough for 1024 objects at a time.

		union Node {
			c8 data[CHUNK_SIZE];
			Node* next;
		};

		Node* _free;
		std::vector<Node*> _pools;
		u32 _size = 1;

		void CreateNewPool() {
			if (_size < MAX_SIZE) {
				_size *= 2;
			}

			_free = static_cast<Node*>(malloc(sizeof(Node) * _size));

			while (!_free || _free == (Node*)1) {
				Logger::Instance().Error("BlockAllocator: Failed to get valid pointer, retrying...");
				_free = static_cast<Node*>(malloc(sizeof(Node) * _size));
			}

			_pools.push_back(_free);

			for (u32 i = 0; i < _size; ++i) {
				_free[i].next = &_free[i + 1];
			}

			_free[_size - 1].next = 0;
		}

		BlockAllocator() {
			CreateNewPool();
		}
	public:
		BlockAllocator(const BlockAllocator&) = delete;

		static BlockAllocator& Instance() {
			static BlockAllocator instance;
			return instance;
		}

		void* Allocate() {
			if (!_free) {
				CreateNewPool();
			}

			Node* result = _free; 
			_free = _free->next; 
			return static_cast<void*>(result);
		}

		void Free(void* elem) {
			Node* node = static_cast<Node*>(elem);

			node->next = _free;
			_free = node;
		}

		~BlockAllocator() {
			for (auto ptr : _pools) {
				free(ptr);
			}
		}
	};
}}
#endif
#pragma once

#include "Engine/Core/Core.h"

namespace Copper::AssetStorage {

	// Linked list storing all of Assets of type T
	template<typename T> struct AssetList {

		struct Node {

			T data;
			Node* next = nullptr;

			template<typename... Args> Node(Args&&... args) : data(args...) {}

		};

		template<typename... Args> T* Create(Args&&... args) {

			// List empty

			if (!m_head) {

				m_head = new Node(args...);
				m_tail = m_head;

				m_len = 1;

				return &m_head->data;

			}

			// List contains at least one element

			CU_ASSERT(m_tail, "AssetList has a head but no tail!");
			
			Node* node = new Node(args...);

			m_tail->next = node;
			m_tail = node;

			m_len++;

			return &node->data;

		}
		T* Get(uint32 index) {

			Node* node = GetNode(index);
			CU_ASSERT(node, "Invalid List nodes! AssetList::Get, index: ({})", index);
			if (!node) return nullptr;

			return &node->data;

		}
		void Remove(uint32 index) {

			Node* before = GetNode(index - 1);
			CU_ASSERT(before, "Invalid List nodes! AssetList::Remove, index: ({})", index);
			if (!before) return;

			Node* toRemove = before->next;
			before->next = toRemove->next;
			m_len--;

			delete toRemove;

		}

		Node* GetNode(uint32 index) {

			if (index >= m_len || index < 0) return nullptr;

			Node* node = m_head;
			for (uint32 i = 0; i < index; i++)
				node = node->next;
			CU_ASSERT(node, "Invalid List nodes! AssetList::GetNode, index: ({})", index);

			return node;

		}

	private:
		Node* m_head = nullptr;
		Node* m_tail = nullptr;

		uint32 m_len = 0;

	};

}
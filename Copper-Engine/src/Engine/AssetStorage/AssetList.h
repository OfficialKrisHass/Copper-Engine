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

	private:
		Node* m_head = nullptr;
		Node* m_tail = nullptr;

		uint32 m_len = 0;

	};

}
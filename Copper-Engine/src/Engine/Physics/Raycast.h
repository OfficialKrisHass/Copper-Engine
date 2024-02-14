#pragma once

#include "Engine/Core/Core.h"

namespace Copper {

	class Raycast {

	public:
		struct Data {

			bool hit = false;

			Vector3 position;
			Vector3 normal;
			float distance = 0.0f;

			class InternalEntity* entity = nullptr;

			inline operator bool() const { return hit; }

		};

		inline Raycast(const Vector3& origin, const Vector3& direction, float maxDistance = 1000.0f, bool fire = true) : m_origin(origin), m_direction(direction), m_dist(maxDistance) {

			CUP_FUNCTION();

			if (!fire) return;
			Fire();

		}

		static bool Fire(const Vector3& origin, const Vector3& direction, Data* data, float maxDistance = 1000.0f);
		bool Fire() { CUP_FUNCTION(); return Fire(m_origin, m_direction, &m_hitData, m_dist); }

		// Getters

		inline const Data& GetData() const { return m_hitData; }

		inline bool Hit() const { return m_hitData.hit; }
		
		inline float Distance() const { return m_hitData.distance; }
		inline const Vector3& Position() const { return m_hitData.position; }
		inline const Vector3& Normal() const { return m_hitData.normal; }
		
		inline InternalEntity* Entity() const { return m_hitData.entity; }

		// Operator overloading

		inline operator bool() const { return m_hitData; }



	private:
		Vector3 m_origin = Vector3::zero;
		Vector3 m_direction = Vector3(0.0f, 0.0f, -1.0f);
		float m_dist = 1000.0f;

		Data m_hitData;

	};

}
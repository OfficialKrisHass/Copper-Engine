#pragma once

#include "Engine/Core/Core.h"

namespace Copper {

	class Raycast {

	public:
		inline Raycast(const Vector3& origin, const Vector3& direction, float maxDistance, bool fire = true) : m_origin(origin), m_direction(direction), m_dist(maxDistance) {

			if (!fire) return;
			Fire();

		}

		bool Fire();

		struct Data {

			bool hit = false;

			float distance = 0.0f;
			Vector3 position;
			Vector3 normal;

			class InternalEntity* entity = nullptr;

		};

		inline const Data& GetData() const { return m_hitData; }
		
		inline float Distance() const { return m_hitData.distance; }
		inline const Vector3& Position() const { return m_hitData.position; }
		inline const Vector3& Normal() const { return m_hitData.normal; }
		
		inline InternalEntity* Entity() const { return m_hitData.entity; }

		inline operator bool() const { return m_hitData.hit; }



	private:
		Vector3 m_origin = Vector3::zero;
		Vector3 m_direction = Vector3(0.0f, 0.0f, -1.0f);
		float m_dist = 0.0f;

		Data m_hitData;

	};

}
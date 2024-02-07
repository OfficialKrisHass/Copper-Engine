#include "cupch.h"
#include "Raycast.h"

#include "Engine/Scene/CopperECS.h"

#include <PxPhysicsAPI.h>

#define PVec3(vec) *(PxVec3*) &vec
#define CVec3(vec) *(Vector3*) &vec

namespace Copper {

	using namespace physx;

	class FilterCallback : public PxQueryFilterCallback {

	public:
		virtual PxQueryHitType::Enum preFilter(const PxFilterData& filterData, const PxShape* shape, const PxRigidActor* actor, PxHitFlags& queryFlags) override {

			return PxQueryHitType::eBLOCK;

		}
		virtual PxQueryHitType::Enum postFilter(const PxFilterData& filterData, const PxQueryHit& hit) override {

			const PxRaycastHit& rayHit = (PxRaycastHit&) hit;
			if (rayHit.hadInitialOverlap())
				return PxQueryHitType::eNONE;

			return PxQueryHitType::eBLOCK;

		}

	private:
		//

	};
	FilterCallback filterCallback;

	bool Raycast::Fire() {

		Scene* scene = GetScene();
		if (!scene->m_runtimeRunning) return false;

		static const PxHitFlags hitFlags = PxHitFlag::eDEFAULT;
		PxQueryFilterData filterData = PxQueryFilterData(PxQueryFlag::eDYNAMIC | PxQueryFlag::eSTATIC | PxQueryFlag::ePOSTFILTER);

		PxRaycastBuffer out;
		if (!scene->m_physicsScene->raycast(PVec3(m_origin), PVec3(m_direction), m_dist, out, hitFlags, filterData, &filterCallback)) return false;

		m_hitData.hit = true;

		m_hitData.distance = out.block.distance;
		m_hitData.position = CVec3(out.block.position);
		m_hitData.normal = CVec3(out.block.normal);

		m_hitData.entity = (InternalEntity*) out.block.actor->userData;

		return true;

	}

}
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
		virtual PxQueryHitType::Enum preFilter(const PxFilterData& filterData, const PxShape* shape, const PxRigidActor* actor, PxHitFlags& queryFlags) override { return PxQueryHitType::eBLOCK; }
		virtual PxQueryHitType::Enum postFilter(const PxFilterData& filterData, const PxQueryHit& hit) override {

			CUP_FUNCTION();

			const PxRaycastHit& rayHit = (PxRaycastHit&) hit;
			if (rayHit.hadInitialOverlap())
				return PxQueryHitType::eNONE;

			return PxQueryHitType::eBLOCK;

		}

	};
	FilterCallback filterCallback;

	bool Raycast::Fire(const Vector3& origin, const Vector3& direction, Data* data, float maxDistance) {

		CUP_FUNCTION();

		Scene* scene = GetScene();
		if (!scene->m_runtimeRunning) return false;

		static const PxHitFlags hitFlags = PxHitFlag::eDEFAULT;
		PxQueryFilterData filterData = PxQueryFilterData(PxQueryFlag::eDYNAMIC | PxQueryFlag::eSTATIC | PxQueryFlag::ePOSTFILTER);

		PxRaycastBuffer out;
		if (!scene->m_physicsScene->raycast(PVec3(origin), PVec3(direction), maxDistance, out, hitFlags, filterData, &filterCallback)) return false;

		data->hit = true;

		data->position = CVec3(out.block.position);
		data->normal = CVec3(out.block.normal);
		data->distance = out.block.distance;

		data->entity = (InternalEntity*) out.block.actor->userData;

		return true;

	}

}
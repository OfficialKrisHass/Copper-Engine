#include "cupch.h"

#include <PxPhysicsAPI.h>

#define GROUP_SIZE 32

namespace Copper::PhysicsEngine {

    using namespace physx;

    PxFilterFlags DefaultSimulationShader(PxFilterObjectAttributes attributes0, PxFilterData filterData0,
                                          PxFilterObjectAttributes attributes1, PxFilterData filterData1,
                                          PxPairFlags& pairFlags, const void* constantBlock, uint32_t constantBlockSize) {

		CUP_FUNCTION();

        PX_UNUSED(constantBlock);
        PX_UNUSED(constantBlockSize);

        if (PxFilterObjectIsTrigger(attributes0) || PxFilterObjectIsTrigger(attributes1)) {

            pairFlags = PxPairFlag::eTRIGGER_DEFAULT;
            return PxFilterFlag::eDEFAULT;

        }

        pairFlags = PxPairFlag::eCONTACT_DEFAULT;
        pairFlags |= PxPairFlag::eNOTIFY_TOUCH_FOUND | PxPairFlag::eNOTIFY_TOUCH_PERSISTS | PxPairFlag::eNOTIFY_TOUCH_LOST;

        return PxFilterFlag::eDEFAULT;

    }

}
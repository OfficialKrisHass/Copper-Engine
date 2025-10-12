#include "cupch.h"

#include "Engine/Scene/Scene.h"

#include "Engine/Components/ScriptComponent.h"

#include <PxPhysicsAPI.h>

namespace Copper::PhysicsEngine {

    using namespace physx;

    class CollisionNotifier : public PxSimulationEventCallback {

    private:
        virtual void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, uint32 nbPairs) {

            CUP_FUNCTION();

            CU_ASSERT(!(pairHeader.flags & PxContactPairHeaderFlag::eREMOVED_ACTOR_0), "First actor out of a contact pair was removed.");
            CU_ASSERT(!(pairHeader.flags & PxContactPairHeaderFlag::eREMOVED_ACTOR_1), "Second actor out of a contact pair was removed.");

            uint32 id0 = static_cast<uint32>((uint64) pairHeader.actors[0]->userData);
            CU_ASSERT(GetEntityFromID(id0) != nullptr, "Could not retrieve the first contact entity from the ContactPairHeader. Retrieved ID: '{}'", id0);

            uint32 id1 = static_cast<uint32>((uint64) pairHeader.actors[1]->userData);
            CU_ASSERT(GetEntityFromID(id1) != nullptr, "Could not retrieve the second contact entity from the ContactPairHeader. Retrieved ID: '{}'", id1);

            ScriptComponent* script0 = GetEntityFromID(id0)->GetComponent<ScriptComponent>();
            ScriptComponent* script1 = GetEntityFromID(id1)->GetComponent<ScriptComponent>();

            if (script0 == nullptr && script1 == nullptr) return;
            
            for (uint32 i = 0; i < pairHeader.nbPairs; i++) {

                const PxContactPair& pair = pairs[i];

                if (pair.events & PxPairFlag::eNOTIFY_TOUCH_FOUND) {

                    if (script0 != nullptr)
                        script0->OnCollisionBegin(GetEntityFromID(id1));
                    if (script1 != nullptr)
                        script1->OnCollisionBegin(GetEntityFromID(id0));

                } else if (pair.events & PxPairFlag::eNOTIFY_TOUCH_PERSISTS) {

                    if (script0 != nullptr)
                        script0->OnCollisionPersist(GetEntityFromID(id1));
                    if (script1)
                        script1->OnCollisionPersist(GetEntityFromID(id0));

                } else if (pair.events & PxPairFlag::eNOTIFY_TOUCH_LOST) {

                    if (script0 != nullptr)
                        script0->OnCollisionEnd(GetEntityFromID(id1));
                    if (script1 != nullptr)
                        script1->OnCollisionEnd(GetEntityFromID(id0));


                }

            }

        }
        virtual void onTrigger(PxTriggerPair* pairs, uint32 count) {

            CUP_FUNCTION();

            for (uint32 i = 0; i < count; i++) {

                const PxTriggerPair& pair = pairs[i];

                CU_ASSERT(!(pair.flags & PxTriggerPairFlag::eREMOVED_SHAPE_TRIGGER), "Trigger shape of a trigger pair was removed.");
                CU_ASSERT(!(pair.flags & PxTriggerPairFlag::eREMOVED_SHAPE_OTHER), "Other shape of a trigger pair was removed.");

                uint32 triggerID = static_cast<uint32>((uint64) pair.triggerActor->userData);
                CU_ASSERT(GetEntityFromID(triggerID) != nullptr, "Could not retrieve trigger entity from a trigger pair. Retrieved ID: '{}'", triggerID);

                ScriptComponent* script = GetEntityFromID(triggerID)->GetComponent<ScriptComponent>();
                if (script == nullptr) continue;

                uint32 otherID = static_cast<uint32>((uint64) pair.otherActor->userData);
                CU_ASSERT(GetEntityFromID(otherID) != nullptr, "Could not retrieve other entity from a trigger pair. Retrieved ID: '{}'", otherID);

                if (pair.status & PxPairFlag::eNOTIFY_TOUCH_FOUND)
                    script->OnTriggerEnter(GetEntityFromID(otherID));
                else if (pair.status & PxPairFlag::eNOTIFY_TOUCH_LOST)
                    script->OnTriggerLeave(GetEntityFromID(otherID));

            }

        }

        virtual void onConstraintBreak(PxConstraintInfo*, PxU32) {}
        virtual void onWake(PxActor** , PxU32 ) {}
        virtual void onSleep(PxActor** , PxU32 ){}
        virtual void onAdvance(const PxRigidBody*const*, const PxTransform*, const PxU32) {}

    };

}

#include "cupch.h"

#include "Engine/Components/ScriptComponent.h"

#include "Engine/Scene/Scene.h"

#include <PxPhysicsAPI.h>

namespace Copper::PhysicsEngine {

    using namespace physx;

    class CollisionNotifier : public PxSimulationEventCallback {

    public:
        //

    private:
        virtual void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, uint32_t nbPairs);
        virtual void onTrigger(PxTriggerPair* pairs, uint32_t count);

        virtual void onConstraintBreak(PxConstraintInfo*, PxU32) {}
	    virtual void onWake(PxActor** , PxU32 ) {}
	    virtual void onSleep(PxActor** , PxU32 ){}
	    virtual void onAdvance(const PxRigidBody*const*, const PxTransform*, const PxU32) {}

    };

    void CollisionNotifier::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, uint32_t nbPairs) {

        for (int i = 0; i < nbPairs; i++) {

            const PxContactPair& pair = pairs[i];

            InternalEntity* entity0 = (InternalEntity*) pairHeader.actors[0]->userData;
            InternalEntity* entity1 = (InternalEntity*) pairHeader.actors[1]->userData;

            ScriptComponent* script0 = entity0->GetComponent<ScriptComponent>();
            ScriptComponent* script1 = entity1->GetComponent<ScriptComponent>();
            if (!script0 && !script1) continue;

            if (pair.events & PxPairFlag::eNOTIFY_TOUCH_FOUND) {

                if (script0) script0->InvokeOnCollisionBegin(entity1);
                if (script1) script1->InvokeOnCollisionBegin(entity0);

                continue;

            }
            if (pair.events & PxPairFlag::eNOTIFY_TOUCH_PERSISTS) {

                if (script0) script0->InvokeOnCollisionPersist(entity1);
                if (script1) script1->InvokeOnCollisionPersist(entity0);

                continue;

            }
            if (pair.events & PxPairFlag::eNOTIFY_TOUCH_LOST) {

                if (script0) script0->InvokeOnCollisionEnd(entity1);
                if (script1) script1->InvokeOnCollisionEnd(entity0);

                continue;

            }

        }

    }
    void CollisionNotifier::onTrigger(PxTriggerPair* pairs, uint32_t count) {

        for (int i = 0; i < count; i++) {

            const PxTriggerPair& pair = pairs[i];

            ScriptComponent* script = ((InternalEntity*) pair.triggerActor->userData)->GetComponent<ScriptComponent>();
            if (!script) continue;

            if (pair.status & PxPairFlag::eNOTIFY_TOUCH_FOUND) {

                script->InvokeOnTriggerEnter((InternalEntity*) pair.otherActor->userData);
                continue;

            }
            if (pair.status & PxPairFlag::eNOTIFY_TOUCH_LOST) {

                script->InvokeOnTriggerLeave((InternalEntity*) pair.otherActor->userData);
                continue;

            }

        } 

    }

}
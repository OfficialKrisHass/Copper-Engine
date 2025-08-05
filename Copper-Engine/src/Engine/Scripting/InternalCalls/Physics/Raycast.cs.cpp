#include "cupch.h"
#include "Raycast.cs.h"

#include "Engine/Scene/InternalEntity.h"

#include "Engine/Scripting/ManagedReferences.h"

namespace Copper::Scripting::Raycast {

    typedef ::Copper::Raycast Raycast;

    bool Fire(const Vector3& origin, const Vector3& direction, Raycast::Data* data, float maxDistance) {

        CUP_FUNCTION();

        bool ret = Raycast::Fire(origin, direction, data, maxDistance);
        if (ret)
            data->entity = (InternalEntity*) GetManagedReference((void*) (uint64) data->entity->GetID(), Class::Entity);

        return ret;

    }

}

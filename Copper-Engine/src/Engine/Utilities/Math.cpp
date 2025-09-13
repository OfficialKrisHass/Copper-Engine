#include "cupch.h"
#include "Math.h"

#define PI 3.14159265358979323846f
#define RAD2DEG (180.0f / PI)

namespace Copper::Math {

    void DecomposeTransform(const Matrix4& transform, Vector3& position, Quaternion& rotation, Vector3& scale) {

        CUP_FUNCTION();

        Vector3 col1 = transform[0];
        Vector3 col2 = transform[1];
        Vector3 col3 = transform[2];

        // Scale

        scale.x = col1.Length();
        scale.y = col2.Length();
        scale.z = col3.Length();

        // Rotation, requires us to re-orthogonalize the axes (Gram-Schmidt or whatever)

        if (scale.x != 0.0f && scale.y != 0.0f && scale.z != 0.0f) {

            col1 /= scale.x;
            col2 /= scale.y;
            col3 /= scale.z;

            Vector3 test;

            test.x = RAD2DEG * atan2f(col2.z, col3.z);
            test.y = RAD2DEG * atan2f(-col1.z, sqrtf(col2.z * col2.z + col3.z * col3.z));
            test.z = RAD2DEG * atan2f(col1.y, col1.x);

            rotation = test;

        } else
            rotation = Quaternion::identity;


        // Position, incredibly easy lol

        position = transform[3];

    }

}

using System;

namespace Copper {

    public class Camera : Component {

        public float fov {

            get { return InternalCalls.CameraGetFOV(eID); }
            set { InternalCalls.CameraSetFOV(eID, value); }

        }
        public float nearPlane {

            get { return InternalCalls.CameraGetNearPlane(eID); }
            set { InternalCalls.CameraSetNearPlane(eID, value); }

        }
        public float farPlane {

            get { return InternalCalls.CameraGetFarPlane(eID); }
            set { InternalCalls.CameraSetFarPlane(eID, value); }

        }

    }

}
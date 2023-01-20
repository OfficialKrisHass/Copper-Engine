using System;

namespace Copper {

    public class Camera : Component {

        public float fov {

            get { return InternalCalls.CameraGetFOV(objID); }
            set { InternalCalls.CameraSetFOV(objID, value); }

        }
        public float nearPlane {

            get { return InternalCalls.CameraGetNearPlane(objID); }
            set { InternalCalls.CameraSetNearPlane(objID, value); }

        }
        public float farPlane {

            get { return InternalCalls.CameraGetFarPlane(objID); }
            set { InternalCalls.CameraSetFarPlane(objID, value); }

        }

    }

}
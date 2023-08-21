using System;

namespace Copper {

    public struct Quaternion {
        
        public Quaternion(float w, float x, float y, float z) {

            this.w = w;
            this.x = x;
            this.y = y;
            this.z = z;

        }

        public float w;
        public float x;
        public float y;
        public float z;

    }

}
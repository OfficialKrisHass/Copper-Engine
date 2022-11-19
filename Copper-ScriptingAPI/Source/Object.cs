using System;

namespace Copper {

    public class Object {

        public readonly int ID;
        public string name {

            get {
                InternalFunctions.GetObjectName(ID, out string result);
                return result;
            }
            set {
                InternalFunctions.SetObjectName(ID, ref value);
            }

        }

    }

}
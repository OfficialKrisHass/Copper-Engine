using System;

namespace Copper {

    public class Component {

        private int objID;
        public Transform transform;

        protected Component() { }

        public Component(int id) {

            objID = id;
            transform = new Transform(id);

        }

        public Object obj {

            get {
                InternalFunctions.GetObject(objID, out Object result);
                return result;
            }

        }

    }

}
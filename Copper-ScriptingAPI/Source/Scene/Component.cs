using System;

namespace Copper {

    public class Component {

        protected int objID;

        protected Component() { }
        protected Component(int objID) {

            this.objID = objID;

        }

        public Object obj {

            get {

                return new Object(objID);

            }

        }
        public Transform transform {

            get {
                return new Transform(objID);
            }

        }

    }

}
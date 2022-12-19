﻿using System;

namespace Copper {

    public class Object {

        protected int objID;

        public Object(int objID) {

            this.objID = objID;

        }

        public string name {

            get {

                return InternalCalls.GetObjectName(objID);

            }
            set {

                InternalCalls.SetObjectName(objID, value);

            }

        }

        public Transform transform {

            get {

                return new Transform(objID);

            }

        }

    }

}
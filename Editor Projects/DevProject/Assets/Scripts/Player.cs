using System;

using Copper;

namespace Testing {

    public class FirstScript : Component {

        public float speed = 0.01f;

        void OnCreate() {

            Editor.Log("Test");

        }

        void OnUpdate() {

            transform.position += new Vector3(speed, 0.0f, 0.0f);

        }

    }

}
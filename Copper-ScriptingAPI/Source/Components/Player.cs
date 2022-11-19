using System;

namespace Copper {

    public class Player : Component {

        public float speed = 0.001f;

        void OnCreate() {

            Editor.Log("Start called!!!");

        }

        void OnUpdate() {

            transform.position += new Vector3(speed, 0.0f, 0.0f);

        }

    }

    public class Mesh : Component {

        void OnCreate() {

            Editor.Log("Start called! 1");

        }

        void OnUpdate() {

            Editor.Log("Update called! 1");

        }

    }

}
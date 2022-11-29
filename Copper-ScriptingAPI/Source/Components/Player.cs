using System;

namespace Copper {

    public class Player : Component {

        public float speed = 0.001f;
        public float test = 69.420f;
        public int id = -1;
        public uint unsigned = 6;

        void OnCreate() {

            Light light = GetComponent<Light>();

            Editor.Log("Light color: " + light.Color.ToString());

        }

        void OnUpdate() {

            transform.position += new Vector3(speed, 0.0f, 0.0f);

        }

    }

    public class Mesh : Component {

        public float color;
        public uint numOfVertices = 25;

    }

}
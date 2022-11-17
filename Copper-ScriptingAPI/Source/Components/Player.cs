using System;

namespace Copper {

    public class Player : Component {

        public Player() {

            Console.WriteLine("Constructor Called!");

        }

        void OnCreate() {

            Console.WriteLine("Start called!");

        }

        void OnUpdate() {

            Console.WriteLine("Update called!");

        }

    }

    public class Mesh : Component {

        public Mesh() {

            Console.WriteLine("Mesh-uh!");

        }

        void OnCreate() {

            Console.WriteLine("Start called! 1");

        }

        void OnUpdate() {

            Console.WriteLine("Update called! 1");

        }

    }

}
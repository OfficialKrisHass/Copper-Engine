using System;

using Copper;

namespace Testing {

    class Test : Component {

        void Create() {

            Editor.Log("Has Test: " + HasComponent<Test>().ToString());
            Editor.Log("Has Player: " + HasComponent<Player>().ToString());

        }

        void Update() {

            //

        }

    }

}
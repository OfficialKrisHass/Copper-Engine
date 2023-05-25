using System;

using Copper;

class Player : Component {

    public float speed = 1.0f;

    private void Create() {

        Editor.Log("Test from the Script World!");

    }

}
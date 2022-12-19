using System;

using Copper;

class Test : Component {

    void Create() {

        transform.position = new Vector3(1.0f, 0.0f, -0.5f);

    }

    void Update() {

        transform.rotation += new Vector3(0.0f, 0.1f, 0.0f);

    }

}
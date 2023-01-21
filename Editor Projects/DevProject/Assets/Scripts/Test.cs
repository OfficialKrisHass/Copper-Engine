using System;
using Copper;

class Test : Component {

    public CopperObject objects = new CopperObject(2);
    public Player player;
    public float speed = 1.0f;

    void Create() {

        Editor.Log(objects.name);

    }

    void Update() {

        objects.transform.position += new Vector3(speed / 100, 0.0f, 0.0f);

    }

}
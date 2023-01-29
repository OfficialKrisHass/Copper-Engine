using System;
using Copper;

class Test : Component {

    public CopperObject objects;
    public float speed = 0.1f;
    public Vector3 test;

    void Create() {

        test = Vector3.zero;

    }

    void Update() {

        if (Input.IsKeyDown(KeyCode.A)) Editor.Log("Pressed");
        if (Input.IsKey(KeyCode.A)) Editor.Log("Held");
        if (Input.IsKeyReleased(KeyCode.A)) Editor.Log("Released");

        //float ws = Input.GetAxis("Keys_WS") * speed;
        //float ad = Input.GetAxis("Keys_DA") * speed;

        //test = transform.forward * ws + transform.right * ad;
        //transform.position += test;

    }

}
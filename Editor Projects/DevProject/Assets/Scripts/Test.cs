using System;
using Copper;

class Test : Component {

    public CopperObject objects;
    public float speed = 0.1f;

    Vector3 test;

    void Create() {

        test = Vector3.zero;

    }

    void Update() {

        float ws = Input.GetAxis("Keys_WS") * speed;
        float ad = Input.GetAxis("Keys_DA") * speed;

        test = transform.forward * ws + transform.right * ad;
        transform.position += test;

    }

}
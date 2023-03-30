using System;
using Copper;

class Test : Component {

    public float speed = 0.1f;
    public float sensitivity = 1.0f;
    public Entity moveThis;

    private Vector3 test;

    void Create() {

        test = Vector3.zero;

        Input.SetCursorLocked(true);
        Input.SetCursorVisible(false);

    }

    void Update() {

        float ws = Input.GetAxis("Keys_WS") * speed;
        float ad = Input.GetAxis("Keys_DA") * speed;

        test = transform.forward * ws + transform.right * ad;
        transform.position += test;

        if(moveThis != null) {

            moveThis.transform.position += test;

        }

        float mouseX = Input.GetAxis("Mouse X") * sensitivity * 100.0f;
        float mouseY = Input.GetAxis("Mouse Y") * sensitivity * 100.0f;

        transform.rotation += new Vector3(mouseY, mouseX, 0.0f);

    }

}
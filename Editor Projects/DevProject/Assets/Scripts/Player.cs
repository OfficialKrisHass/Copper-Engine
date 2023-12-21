using System;

using Copper;

class Player : Component {

    public float speed = 1.0f;

    RigidBody rb;

    private void Create() {

        rb = GetComponent<RigidBody>();

        Input.SetCursorLocked(true);
        Input.SetCursorVisible(false);

    }
    private void Update() {

        float x = Input.GetAxis("Keys_WS") * speed;
        float z = Input.GetAxis("Keys_DA") * speed;

        rb.AddForce(transform.forward * x + transform.right * z, ForceMode.Force);

    }

}
using System;

using Copper;

class Player : Component {

    public float jumpForce = 10.0f;

    public RigidBody rb;

    private void Create() {

        rb = GetComponent<RigidBody>();

    }
    private void Update() {

        if (Input.IsKeyDown(KeyCode.Space)) {

            rb.AddForce(new Vector3(0.0f, jumpForce, 0.0f), ForceMode.Force);

        }

    }

}
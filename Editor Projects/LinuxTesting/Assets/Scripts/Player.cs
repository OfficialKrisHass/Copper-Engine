using System;

using Copper;

class Player : Component {

    public float speed = 1.0f;

    public RigidBody rb;

    private void Create() {
        
        rb = GetComponent<RigidBody>();

    }

    public void Update() {

        if (Input.IsKeyDown(KeyCode.Space)) {

            rb.gravity = !rb.gravity;

        }

    }

}
using System;
using Copper;

public class PlayerMovement : Component {

    [ShowInEditor] private float speed = 1.0f;

    private RigidBody rb = null;

    // Called once, at the beginning of the lifetime of the Entity this component is attached to. Use for initialization
    private void OnBegin() {

        rb = GetComponent<RigidBody>();

    }
    // Called every frame. Use for game logic
    private void OnUpdate() {

        float input = Input.GetAxis("Keys_AD");
        Vector3 force = new Vector3(input * speed, 0.0f, 0.0f);

        rb.AddForce(force);

    }

}

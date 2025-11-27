using System;
using Copper;

public class PlayerMovement : Component {

    [ShowInEditor] private float speed = 1.0f;
    [ShowInEditor] private float sensitivity = 1.0f;

    [ShowInEditor] private Transform camera = null;
    private RigidBody rb = null;

    float yaw = 0.0f;
    float pitch = 0.0f;

    // Called once, at the beginning of the lifetime of the Entity this component is attached to. Use for initialization
    private void OnBegin() {

        rb = GetComponent<RigidBody>();

    }
    // Called every frame. Use for game logic
    private void OnUpdate() {

        // Move

        float x = Input.GetAxis("Keys_AD");
        float y = Input.GetAxis("Keys_WS");

        Vector3 force = transform.right * x + transform.forward * y;
        force.Normalize();
        rb.AddForce(force * speed);

        // Look

        pitch -= Input.GetMouseAxis("Mouse X") * sensitivity * Game.deltaTime;
        yaw -= Input.GetMouseAxis("Mouse Y") * sensitivity * Game.deltaTime;

        transform.rotation = new Quaternion(0.0f, pitch, 0.0f);

        // If your player entity (the body) has a unfirom scale (all ones), you can use this version if the camera is a child
        // of the player entity, but you can also use the second one if it's not.
        // camera.rotation = new Quaternion(yaw, 0.0f, 0.0f);

        // If your player entity (the body) has a non uniform scale (not all ones), you have to use this version
        // Make sure the camera is NOT a child of the player entity.
        camera.rotation = new Quaternion(yaw, pitch, 0.0f);
        camera.position = new Vector3(transform.position.x, camera.position.y, transform.position.z);

    }

}

using System;

using Copper;

class Player : Component {

    public float speed = 1.0f;
    public float sensitivity = 1.0f;
    public float jumpHeight = 1.0f;

    public Transform camera;

    RigidBody rb;
    Vector2 mouseRot;

    private void Create() {

        rb = GetComponent<RigidBody>();

        Input.SetCursorLocked(true);
        Input.SetCursorVisible(false);

        if (rb == null) {

            Editor.LogError("Player has no RigidBody component!");

        }

    }
    private void Update() {

        if (camera == null) {

            Editor.LogError("Camera has not been assigned!");
            return;

        }

        HandleMovement();
        HandleMouse();

        if (!Input.IsKey(KeyCode.Q)) return;

        Raycast.Data hitData;
        if (!Raycast.Fire(camera.globalPosition, camera.forward, out hitData)) return;

        Editor.Log("Hit: " + hitData.entity.name);

    }

    private void HandleMovement() {

        float x = Input.GetAxis("Keys_WS") * speed;
        float y = Input.GetAxis("Keys_DA") * speed;

        Vector3 force = transform.forward * x + transform.right * y;

        rb.AddForce(force * Game.deltaTime * 10000.0f, ForceMode.Force);

        if (Input.IsKeyDown(KeyCode.Space))
            rb.AddForce(transform.up * jumpHeight, ForceMode.Force);

    }
    private void HandleMouse() {

        mouseRot.x -= Input.GetAxis("Mouse X") * sensitivity * Game.deltaTime * 1000.0f;
        mouseRot.y -= Input.GetAxis("Mouse Y") * sensitivity * Game.deltaTime * 1000.0f;

        transform.rotation = Quaternion.Euler(0.0f, mouseRot.x, 0.0f);
        camera.rotation = Quaternion.Euler(mouseRot.y, 0.0f, 0.0f);

    }

}
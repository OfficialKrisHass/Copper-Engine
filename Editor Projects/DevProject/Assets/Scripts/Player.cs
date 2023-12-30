using System;

using Copper;

class Player : Component {

    public float speed = 1.0f;
    public float sensitivity= 1.0f;

    public Transform camera;

    RigidBody rb;
    Vector2 mouseRot;

    private void Create() {

        rb = GetComponent<RigidBody>();

        Input.SetCursorLocked(true);
        Input.SetCursorVisible(false);

    }
    private void Update() {

        if (rb == null) {

            Editor.Log("Has no RigidBody!");
            return;

        }

        float x = Input.GetAxis("Keys_WS") * speed;
        float z = Input.GetAxis("Keys_DA") * speed;

        rb.AddForce(transform.forward * x + transform.right * z, ForceMode.Force);

        mouseRot.x -= Input.GetAxis("Mouse X") * sensitivity;
        mouseRot.y += Input.GetAxis("Mouse Y") * sensitivity;

        Editor.Log(mouseRot.y.ToString());

        transform.rotation = Quaternion.Euler(0.0f, mouseRot.x, 0.0f);
        camera.rotation = Quaternion.Euler(mouseRot.y, 0.0f, 0.0f);

    }

}
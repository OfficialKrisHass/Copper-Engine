using System;

using Copper;

class Player : Component {

    public float speed = 1.0f;
    public float sensitivity;

    public Entity camera;

    private void Create() {

        Input.SetCursorLocked(true);
        Input.SetCursorVisible(false);

    }
    private void Update() {

        float moveX = Input.GetAxis("Keys_DA") * speed;
        float moveZ = Input.GetAxis("Keys_WS") * speed;

        float mouseX = Input.GetAxis("Mouse X") * sensitivity;
        float mouseY = Input.GetAxis("Mouse Y") * sensitivity;

        transform.position += transform.right * moveX + transform.forward * moveZ;
        camera.transform.position += transform.right * moveX + transform.forward * moveZ;

        transform.rotation += new Vector3(0.0f, mouseX, 0.0f);
        camera.transform.rotation += new Vector3(mouseY, mouseX, 0.0f);

    }

}
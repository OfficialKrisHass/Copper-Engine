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

        transform.position += new Vector3(moveX, 0.0f, moveZ);
        //transform.rotation += new Vector3(mouseX, 0.0f, 0.0f);

        //camera.transform.rotation += new Vector3(mouseX, 0.0f, mouseY);

    }

}
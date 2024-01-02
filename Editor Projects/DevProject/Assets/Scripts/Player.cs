using System;

using Copper;

class Testing {

    public void Use() {

        Editor.Log("Test");

    }

}

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

        Testing testing = null;

        testing.Use();

    }

}
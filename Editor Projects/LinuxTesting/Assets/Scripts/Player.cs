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

    private void OnCollisionBegin(Entity other) {

        Editor.Log("Collision Begin: " + other.name);

    }
    private void OnCollisionPersist(Entity other) {

        Editor.Log("Collision Persist: " + other.name);

    }
    private void OnCollisionEnd(Entity other) {

        Editor.Log("Collision End: " + other.name);

    }

}
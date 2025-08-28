using System;
using Copper;

public class PlayerMovement : Component {

    [ShowInEditor] private float speed = 1.0f;

    public Transform test = null;

    // Called once, at the beginning of the lifetime of the Entity this component is attached to. Use for initialization
    private void OnBegin() {

        //

    }
    // Called every frame. Use for game logic
    private void OnUpdate() {

        float input = Input.GetAxis("Keys_AD") * speed * Game.deltaTime;
        transform.position += new Vector3(input, 0.0f, 0.0f);

        if (test != null)
            test.position += new Vector3(0.0f, speed * Game.deltaTime, 0.0f);

    }

}

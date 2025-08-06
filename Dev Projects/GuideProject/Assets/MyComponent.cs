using System;
using Copper;

public class MyComponent : Component {

    [ShowInEditor]
    private float speed = 1.0f;

    [ShowInEditor]
    private Transform another;

    // Called once, at the beginning of the lifetime of the Entity this component is attached to. Use for initialization
    private void OnBegin() {

        //

    }
    // Called every frame. Use for game logic
    private void OnUpdate() {

        Vector3 value = new Vector3(0.0f, speed * Game.deltaTime, 0.0f);

        transform.position += value;
        another.position += value;

    }

}

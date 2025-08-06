using System;
using Copper;

public class MyComponent : Component {

    // Called once, at the beginning of the lifetime of the Entity this component is attached to. Use for initialization
    private void OnBegin() {

        Editor.Log("Hello, world!");

    }
    // Called every frame. Use for game logic
    private void OnUpdate() {

        //

    }

}

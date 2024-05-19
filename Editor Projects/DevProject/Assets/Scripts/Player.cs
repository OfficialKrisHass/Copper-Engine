using System;
using Copper;

public class Player : Component {

    float y = 0.0f;
    
    void OnBegin() {

        Editor.Log("Begin has been called");
        Editor.Log(entity.name);

        entity.name += " UwU";

        transform.position = new Vector3(1.0f, 0.0f, 0.0f);

    }
    void OnUpdate() {

        transform.position = new Vector3(transform.position.x, y, 0.0f);
        y += transform.position.x * 0.01f;

    }

}

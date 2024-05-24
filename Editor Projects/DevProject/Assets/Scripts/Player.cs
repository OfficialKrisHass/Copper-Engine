using System;
using Copper;

public class Player : Component {

    float rot = 0.0f;

    void OnBegin() {

        Editor.Log("Begin has been called");
        Editor.Log(entity.name);

        entity.name += " UwU";

        Editor.Log(transform.rotation.ToString());
        Editor.Log(transform.eulerAngles.ToString());

    }
    void OnUpdate() {

        rot += 0.1f;
        transform.eulerAngles = new Vector3(0.0f, rot, 0.0f);

    }

}

using System;
using Copper;

public class Player : Component {

    float rot = 0.0f;

    void OnBegin() {

        Editor.Log("Begin has been called");
        Editor.Log(entity.name);

        entity.name += " UwU";

        Editor.Log(transform.position.ToString());
        Editor.Log(transform.globalPosition.ToString());

        Editor.Log(transform.rotation.ToString());
        Editor.Log(transform.globalRotation.ToString());

        Editor.Log(transform.eulerAngles.ToString());
        Editor.Log(transform.globalEulerAngles.ToString());

        Editor.Log(transform.scale.ToString());
        Editor.Log(transform.globalScale.ToString());

    }
    void OnUpdate() {

        rot += 0.1f;
        transform.eulerAngles = new Vector3(0.0f, rot, 0.0f);

    }

}

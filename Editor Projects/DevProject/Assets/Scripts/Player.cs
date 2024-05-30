using System;
using Copper;

public class Player : Component {
    
    [ShowInEditor] private float rotSpeed = 0.4f;
    [ShowInEditor] private Vector3 pos;
    [ShowInEditor] private Entity target;

    float rot = 0.0f;

    void OnBegin() {

        Editor.Log("Begin has been called");
        Editor.Log(entity.name);

        entity.name += " UwU";

        if (target == null) {

            Editor.LogError("Target has not been set!");
            return;

        }

        target.transform.position = pos;

    }
    void OnUpdate() {

        if (target == null) return;

        rot += rotSpeed;
        target.transform.eulerAngles = new Vector3(0.0f, rot, 0.0f);

    }

}

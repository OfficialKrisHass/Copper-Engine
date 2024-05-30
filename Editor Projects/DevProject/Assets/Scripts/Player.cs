using System;
using Copper;

public class Player : Component {
    
    [ShowInEditor] private float rotSpeed = 0.4f;
    [ShowInEditor] private Vector3 pos;
    [ShowInEditor] private Transform target;

    float rot = 0.0f;

    void OnBegin() {

        Editor.Log("Begin has been called");
        Editor.Log(entity.name);

        entity.name += " UwU";
        
        Editor.Log(target.entity.name);
        target.position = pos;

    }
    void OnUpdate() {

        if (target == null) return;

        rot += rotSpeed;
        target.eulerAngles = new Vector3(0.0f, rot, 0.0f);

    }

}

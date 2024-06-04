using System;
using Copper;

public class Player : Component {
    
    [ShowInEditor] private float rotSpeed = 0.4f;
    [ShowInEditor] private Vector3 pos;
    [ShowInEditor] private Transform target;
    [ShowInEditor] private Entity cam;

    float rot = 0.0f;

    void OnBegin() {

        Editor.Log("Begin has been called");
        Editor.Log(entity.name);

        entity.name += " UwU";
        
        Editor.Log(target.entity.name);
        target.position = pos;

        if (cam.HasComponent<Transform>())
            Editor.Log("Camera has Transform");
        if (cam.HasComponent<Camera>()) {
            
            Editor.Log("Camera has Camera");
            Editor.Log(cam.GetComponent<Camera>().fov.ToString());

        }
        if (cam.HasComponent<Light>())
            Editor.Log("Camera has Light");

    }
    void OnUpdate() {

        if (target == null) return;

        rot += rotSpeed;
        target.eulerAngles = new Vector3(0.0f, rot, 0.0f);

    }

}

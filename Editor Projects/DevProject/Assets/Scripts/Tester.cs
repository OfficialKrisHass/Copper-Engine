using System;
using Copper;

public class Tester : Component {

    [ShowInEditor] private Entity camera;

    private Raycast raycast;

    void OnBegin() {

        Cursor.visible = true;
        Cursor.locked = false;

        if (camera != null) {

          raycast = new Raycast(camera.transform.position, camera.transform.forward, 1000.0f, false);

        }

    }
    void OnUpdate() {

        if (camera != null && raycast.Fire())
          Editor.Log(raycast.m_data.entity.name);

    }

}

using System;
using Copper;

public class Tester : Component {

    public Entity camera;
    [ShowInEditor] private float cameraFov = 85;

    [ShowInEditor] private Transform target;
    [ShowInEditor] private Vector3 targetPosition;
    [ShowInEditor] private float rotationSpeed = 0.5f;

    [ShowInEditor] private Entity newLight;
    [ShowInEditor] private float lightIntensity;

    private float rot = 0.0f;

    void OnBegin() {

        TestEntity();
        TestTransform();
        TestEntityReference();
        TestTransformReference();
        TestAddGetComponent();
        TestRigidBody();

    }
    void OnUpdate() {

        if (target == null) return;

        target.eulerAngles = new Vector3(0.0f, rot, 0.0f);
        rot += rotationSpeed;

    }

    private void TestEntity() {

        Editor.Log("TEST ENTITY");

        Editor.Log("Entity: " + entity.name);
        entity.name += " UwU";

        Editor.Log("");

    }
    private void TestTransform() {

        Editor.Log("TEST TRANSFORM");

        Editor.Log("Transform entity: " + transform.entity.name);
        Editor.Log("Transform position: " + transform.position.ToString());
        Editor.Log("Transform rotation: " + transform.rotation.ToString());
        Editor.Log("Transform eulerAngles: " + transform.eulerAngles.ToString());
        Editor.Log("");

    }
    private void TestEntityReference() {

        Editor.Log("TEST ENTITY REFERENCE");

        if (camera == null) {

            Editor.LogError("Camera has not been assigned!");
            return;

        }

        Editor.Log("Camera name: " + camera.name);
        camera.name += " OwO";

        if (!camera.HasComponent<Camera>()) {

            Editor.Log("Camera does not have Camera component");
            return;

        }
        
        Editor.Log("Camera has Camera component");
        Camera cam = camera.GetComponent<Camera>();
        Editor.Log("Camera FOV: " + cam.fov.ToString());
        cam.fov = cameraFov;

        Editor.Log("");

    }
    private void TestTransformReference() {

        Editor.Log("TEST TRANSFORM REFERENCE");

        if (target == null) {

            Editor.LogError("Target has not been assigned!");
            return;

        }

        Editor.Log("Target position:" + target.position.ToString());
        target.position = targetPosition;

        Editor.Log("Target rotation: " + target.rotation.ToString());
        Editor.Log("Target eulerAngles: " + target.eulerAngles.ToString());
        target.rotation = Quaternion.identity;

        rot = target.eulerAngles.y;

        Editor.Log("");

    }
    private void TestAddGetComponent() {

        Editor.Log("TEST ADD & GET COMPONENT");

        if (newLight == null) {

            Editor.Log("New Light has not been assigned!");
            return;

        }

        Editor.Log("newLight entity: " + newLight.name);
        newLight.name += " Shiny!";

        Light l;
        if (!newLight.HasComponent<Light>()) {

            l = newLight.AddComponent<Light>();
            Editor.Log("Added Light component");

        } else
            l = newLight.GetComponent<Light>();

        Editor.Log("Light type: " + l.type.ToString());
        l.type = Light.Type.Directional;
        
        Editor.Log("Light color: " + l.color.ToString());
        l.color = Color.red;

        Editor.Log("Light intensity: " + l.intensity.ToString());
        l.intensity = lightIntensity;

        Editor.Log("");

    }
    private void TestRigidBody() {

        Editor.Log("TEST RIGIDBODY");

        RigidBody rb = GetComponent<RigidBody>();

        if (rb == null) {

            Editor.Log(entity.name + " does not have a Rigid Body component");
            return;

        }

        Editor.Log("rb entity: " + rb.entity.name);

        Editor.Log("rb isStatic: " + rb.isStatic.ToString());
        Editor.Log("rb gravity: " + rb.gravity.ToString());
        rb.gravity = false;

        Editor.Log("rb mass: " + rb.mass.ToString());
        rb.mass = 2.0f;

        rb.AddForce(transform.forward);

        Editor.Log("rb lockMask: " + rb.lockMask.ToString());
        rb.lockMask = 0;

        Editor.Log("");

    }

}

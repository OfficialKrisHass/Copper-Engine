using System;
using Copper;

public class TriggerTest : Component {

    private void OnTriggerEnter(Entity other) {

        Editor.Log("Trigger entered by: " + other.name);

    }
    private void OnTriggerLeave(Entity other) {

        Editor.Log("Trigger left by: " + other.name);

    }

}

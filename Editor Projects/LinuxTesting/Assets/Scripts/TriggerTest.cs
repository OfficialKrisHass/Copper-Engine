using Copper;

public class TriggerTest : Component {

    private void OnTriggerEnter(Entity other) {

        Editor.Log("Trigger Enter: " + other.name);

    }
    private void OnTriggerLeave(Entity other) {

        Editor.Log("Trigger Leave: " + other.name);

    }

}
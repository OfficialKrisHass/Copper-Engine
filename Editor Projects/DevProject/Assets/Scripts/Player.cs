using System;
using Copper;

public class Player : Component {
    
    void OnBegin() {

        Editor.Log("Begin has been called");
        Editor.Log(entity.name);

        entity.name = "Player UwU";

    }

}

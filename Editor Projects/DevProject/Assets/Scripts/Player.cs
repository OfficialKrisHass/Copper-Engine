using System;
using Copper;

public class Player : Component {
    
    int i = 1;

    void OnBegin() {

        Editor.Log("Begin has been called");

    }
    void OnUpdate() {

        Editor.Log("Frame #" + i.ToString());
        i++;

    }

}

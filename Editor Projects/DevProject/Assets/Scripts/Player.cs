using System;
using Copper;

public class Player : Component {

     int num = 69;

     public Player() {

        Editor.Log(xPos.ToString());
        Editor.Log("From inside Player constructor!");

     }

}

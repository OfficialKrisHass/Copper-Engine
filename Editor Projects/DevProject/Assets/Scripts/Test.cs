using System;
using Copper;

class Test : Component {

    public Player player;
    public float speed = 1.0f;
    public int objToMoveID = 2;

    void Create() {

        Copper.Object obj = new Copper.Object(objToMoveID);
        player = obj.GetComponent<Player>();

    }

    void Update() {

        if (player == null) return;

        player.transform.position += new Vector3(speed / 100, 0.0f, 0.0f);

    }

}
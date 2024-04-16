using System;
using System.Runtime.CompilerServices;

namespace Copper {

    public class Component {

        public uint entityID = 4294967295;

        public extern float xPos {

            [MethodImpl(MethodImplOptions.InternalCall)]
            get;
        
        }

        public Component() {

            Editor.Log("From inside component constructor!");
            Editor.Log(xPos.ToString());

        }

    }

}

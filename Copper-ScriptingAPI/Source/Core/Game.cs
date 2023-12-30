using System.Runtime.CompilerServices;

namespace Copper {

    public static class Game {

        public static float deltaTime { get { return Internal_GetDeltaTime(); }  }

        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static float Internal_GetDeltaTime();

    }

}
using System;
using System.Runtime.CompilerServices;

namespace Copper {

    public static class InternalCalls {

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void TestLog(string text, int paramater);

    }

    public class Main {

        public float var;

        public Main() {
            
            Console.WriteLine("C# Constructor Called!");
        
        }

        public void PrintMessage() { Console.WriteLine("C# says: Hello world!"); }
        public void PrintInt(float number) { Console.WriteLine("C# says a number: " + number); }
        public void PrintMessageCustom(string message) { Console.WriteLine("C# says: " + message); }

    }

}
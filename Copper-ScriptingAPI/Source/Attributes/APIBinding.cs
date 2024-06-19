using System;

namespace Copper {

    [AttributeUsage(AttributeTargets.Class)]
    public sealed class NativeClassAttribute : Attribute {

        //

    }

    [AttributeUsage(AttributeTargets.Method)]
    public sealed class NativeFunctionAttribute : Attribute {

        public NativeFunctionAttribute(string name) {

            this.name = name;

        }

        string name;

    }

}

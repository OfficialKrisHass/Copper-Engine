using System;

namespace Copper {

    [AttributeUsage(AttributeTargets.Class | AttributeTargets.Struct)]
    public sealed class NativeClassAttribute : Attribute {

        public NativeClassAttribute(string headerPath) {

            this.headerPath = headerPath;

        }

        string headerPath;

    }

    [AttributeUsage(AttributeTargets.Method)]
    public sealed class NativeFunctionAttribute : Attribute {

        public NativeFunctionAttribute(string name) {

            this.name = name;

        }

        string name;

    }

}

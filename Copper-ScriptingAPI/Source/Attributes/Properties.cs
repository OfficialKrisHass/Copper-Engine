using System;

namespace Copper {

    [AttributeUsage(AttributeTargets.Field)]
    public sealed class ShowInEditorAttribute : Attribute {}

    [AttributeUsage(AttributeTargets.Field)]
    public sealed class HideInEditorAttribute : Attribute {}

}

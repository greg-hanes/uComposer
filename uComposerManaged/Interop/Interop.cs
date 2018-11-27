using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;

namespace uComposer.Interop
{
    public class Interop
    {
        public const string kLibUComposer = "uComposer.Native.dll";

        [DllImport(kLibUComposer)]
        public static extern int GetEffectPluginCount();
    }
}

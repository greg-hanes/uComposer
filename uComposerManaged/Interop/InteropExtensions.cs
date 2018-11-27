using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace uComposer.Interop
{
    public static class InteropExtensions
    {
        public static IntPtr AddOffset(this IntPtr ptr, int offset)
        {
            return (IntPtr)((long)ptr + (long)offset);
        }
    }
}

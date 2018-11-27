using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SynthLibSimpleManaged
{
    internal static class MathUtil
    {
        public static float Clamp(float val, float min, float max)
        {
            return (val < min ? min : (val > max ? max : val));
        }

        public static int Clamp(int val, int min, int max)
        {
            return (val < min ? min : (val > max ? max : val));
        }
    }
}

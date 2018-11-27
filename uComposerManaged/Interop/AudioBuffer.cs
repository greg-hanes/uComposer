using System;
using System.Runtime.InteropServices;

namespace uComposer.Interop
{
    public class AudioBuffer
    {
        // Need extern function to allocate an array of floats.
        // Give: Channel count, 

        [DllImport(Interop.kLibUComposer)]
        private static extern IntPtr AllocateBuffer(int length);

        private int _length;
        private IntPtr _bufferPtr;

        internal IntPtr Pointer
        {
            get { return _bufferPtr; }
        }

        public int Length
        {
            get { return _length; }
        }

        public AudioBuffer(int length)
        {
            _length = length;
            _bufferPtr = AllocateBuffer(length);
        }

        public AudioBuffer(uint length) : this((int)length)
        {
        }

        public void CopyTo(float[] arr)
        {
            Marshal.Copy(_bufferPtr, arr, 0, _length); 
        }
    }
}

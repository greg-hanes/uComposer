using System;
using System.Diagnostics;
using System.Runtime.InteropServices;

namespace LowLevelKeyboard
{
    public class KeyEventArgs : EventArgs
    {
        public Keys KeyCode
        {
            get;
            private set;
        }

        public KeyEventArgs(Keys key)
        {
            KeyCode = key;
        }
    }

    public delegate void KeyEventHandler(Keys key, double time);

    public class KeyboardHook
    {
        private const int WH_KEYBOARD_LL = 13;
        private const int WM_KEYDOWN = 0x0100;
        private const int WM_KEYUP = 0x0101;
        private const int WM_SYSKEYDOWN = 0x0104;
        private const int WM_SYSKEYUP = 0x0105;

        #region DLL Imports
        [DllImport("user32.dll", CharSet = CharSet.Auto, SetLastError = true)]
        private static extern IntPtr SetWindowsHookEx(int idHook,
            KeyboardProcedure lpfn, IntPtr hMod, uint dwThreadId);

        [DllImport("user3d.dll", CharSet = CharSet.Auto, SetLastError = true)]
        private static extern bool TranslateMessage(IntPtr lpMsg);

        [DllImport("user32.dll", CharSet = CharSet.Auto, SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        private static extern bool UnhookWindowsHookEx(IntPtr hhk);

        [DllImport("user32.dll", CharSet = CharSet.Auto, SetLastError = true)]
        private static extern IntPtr CallNextHookEx(IntPtr hhk, int nCode,
            IntPtr wParam, IntPtr lParam);

        [DllImport("kernel32.dll", CharSet = CharSet.Auto, SetLastError = true)]
        private static extern IntPtr GetModuleHandle(string lpModuleName);
        #endregion

        #region Member Variables
        private KeyboardProcedure keyboardProcedure;

        private KeyboardProcedure CurrentKeyboardProcedure
        {
            get
            {
                if (keyboardProcedure == null)
                {
                    keyboardProcedure = HookCallback;
                }
                return keyboardProcedure;
            }
            set { keyboardProcedure = value; }
        }
        private IntPtr keyboardHookId = IntPtr.Zero;
        #endregion


        private delegate IntPtr KeyboardProcedure(int nCode, IntPtr wParam, IntPtr lParam);

        public event KeyEventHandler KeyPressDetected;
        public event KeyEventHandler KeyReleaseDetected;

        private Stopwatch _watch = new Stopwatch();

        public KeyboardHook()
        {
            _watch.Start();
            keyboardHookId = SetHook(CurrentKeyboardProcedure);
        }

        private IntPtr SetHook(KeyboardProcedure proc)
        {
            using (Process currentProcess = Process.GetCurrentProcess())
            using (ProcessModule currentModule = currentProcess.MainModule)
            {
                return SetWindowsHookEx(WH_KEYBOARD_LL, proc,
                    GetModuleHandle(currentModule.ModuleName), 0);
            }
        }

        private void OnKeyPressDetected(Keys key, int time)
        {
            if (KeyPressDetected != null)
            {
                KeyPressDetected(key, time);
            }
        }

        private void OnKeyReleaseDetected(Keys key, int time)
        {
            if (KeyReleaseDetected != null)
            {
                KeyReleaseDetected(key, time);
            }
        }

        [StructLayout(LayoutKind.Sequential)]
        struct KEYBDINPUT
        {
            public UInt32 Vk;
            public UInt32 Scan;
            public UInt32 Flags;
            public UInt32 Time;
            public UInt32 ExtraInfo;
        }

        private bool[] _allKeys = new bool[255];

        public bool LeftShift
        {
            get;
            private set;
        }

        public bool RightShift
        {
            get;
            private set;
        }

        public bool Shift
        {
            get { return LeftShift || RightShift; }
        }

        public bool LeftAlt
        {
            get;
            private set;
        }

        public bool RightAlt
        {
            get;
            private set;
        }

        public bool Alt
        {
            get { return LeftAlt || RightAlt; }
        }

        public bool LeftControl
        {
            get;
            private set;
        }

        public bool RightControl
        {
            get;
            private set;
        }

        public bool Control
        {
            get { return LeftControl || RightControl; }
        }

        private IntPtr HookCallback(int nCode, IntPtr wParam, IntPtr lParam)
        {
            if (nCode >= 0 && (wParam == (IntPtr)WM_KEYDOWN) || wParam == (IntPtr)WM_SYSKEYDOWN)
            {
                int vkCode = Marshal.ReadInt32(lParam);
                KEYBDINPUT k = (KEYBDINPUT)Marshal.PtrToStructure(lParam, typeof(KEYBDINPUT));
                int time = (int)k.Time;

                if (vkCode < 255)
                {
                    if (!_allKeys[vkCode])
                    {
                        _allKeys[vkCode] = true;
                    }
                    else
                    {
                        goto Skip;
                    }
                }

                Keys key = ((Keys)vkCode);

                if (key == Keys.LControlKey)
                {
                    LeftControl = true;
                }
                if (key == Keys.RControlKey)
                {
                    //key = key | Keys.Control;
                    RightControl = true;
                }

                if (key == Keys.LShiftKey)
                {
                    LeftShift = true;
                }
                if (key == Keys.RShiftKey)
                {
                    //key = key | Keys.Shift;
                    RightShift = true;
                }

                if (key == Keys.LMenu)
                {
                    //key = key | Keys.Alt;
                    LeftAlt = true;
                }
                if (key == Keys.RMenu)
                {
                    //key = key | Keys.Alt;
                    RightAlt = true;
                }

                OnKeyPressDetected(key, time);
            }

            if (nCode >= 0 && (wParam == (IntPtr)WM_KEYUP || wParam == (IntPtr)WM_SYSKEYUP))
            {
                int vkCode = Marshal.ReadInt32(lParam);
                KEYBDINPUT k = (KEYBDINPUT)Marshal.PtrToStructure(lParam, typeof(KEYBDINPUT));
                int time = (int)k.Time;
                if (vkCode < 255)
                {
                    _allKeys[vkCode] = false;
                }
                Keys key = ((Keys)vkCode);

                if (key == Keys.LControlKey)
                {
                    key = key | Keys.Control;
                    LeftControl = false;
                }
                if (key == Keys.RControlKey)
                {
                    key = key | Keys.Control;
                    RightControl = false;
                }

                if (key == Keys.LShiftKey)
                {
                    key = key | Keys.Shift;
                    LeftShift = false;
                }
                if (key == Keys.RShiftKey)
                {
                    key = key | Keys.Shift;
                    RightShift = false;
                }

                if (key == Keys.LMenu)
                {
                    key = key | Keys.Alt;
                    LeftAlt = false;
                }
                if (key == Keys.RMenu)
                {
                    key = key | Keys.Alt;
                    RightAlt = false;
                }

                OnKeyReleaseDetected(key, time);
            }

            Skip:
            return CallNextHookEx(keyboardHookId, nCode, wParam, lParam);
        }

        #region IDisposable Members

        public void Dispose()
        {
            UnhookWindowsHookEx(keyboardHookId);
        }

        #endregion
    }
}

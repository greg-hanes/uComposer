using System;
using System.Diagnostics;
using System.Runtime.InteropServices;

namespace LowLevelKeyboard
{
    public delegate void MouseMoveEventHandler(int x, int y);
    public delegate void MouseButtonEventHandler(int button, int x, int y);

    public class MouseHook
    {
        private delegate IntPtr LowLevelMouseProc(int nCode, IntPtr wParam, IntPtr lParam);
        private IntPtr _hookID = IntPtr.Zero;
        private LowLevelMouseProc _proc;

        public event MouseButtonEventHandler MouseDown;
        public event MouseButtonEventHandler MouseUp;
        public event MouseMoveEventHandler MouseMove;



        public MouseHook()
        {
            _proc = HookCallback;
            _hookID = SetHook(_proc);
        }

        public void Done()
        {
            UnhookWindowsHookEx(_hookID);
        }

        private IntPtr SetHook(LowLevelMouseProc proc)
        {
            using (Process curProcess = Process.GetCurrentProcess())
            using (ProcessModule curModule = curProcess.MainModule)
            {
                return SetWindowsHookEx(WH_MOUSE_LL, proc,
                    GetModuleHandle(curModule.ModuleName), 0);
            }
        }
        
        private IntPtr HookCallback(int nCode, IntPtr wParam, IntPtr lParam)
        {
            if (nCode >= 0)
            {
                MSLLHOOKSTRUCT hookStruct = (MSLLHOOKSTRUCT)Marshal.PtrToStructure(lParam, typeof(MSLLHOOKSTRUCT));

                switch ((MouseMessages)wParam)
                {
                case MouseMessages.WM_LBUTTONDOWN:
                    if (MouseDown != null)
                        MouseDown(0, hookStruct.pt.x, hookStruct.pt.y);
                    break;
                case MouseMessages.WM_LBUTTONUP:
                    if (MouseUp != null)
                        MouseUp(0, hookStruct.pt.x, hookStruct.pt.y);
                    break;
                case MouseMessages.WM_MOUSEMOVE:
                    if (MouseMove != null)
                        MouseMove(hookStruct.pt.x, hookStruct.pt.y);
                    break;
                case MouseMessages.WM_MOUSEWHEEL:
                    break;
                case MouseMessages.WM_RBUTTONDOWN:
                    if (MouseDown != null)
                        MouseDown(1, hookStruct.pt.x, hookStruct.pt.y);
                    break;
                case MouseMessages.WM_RBUTTONUP:
                    if (MouseUp != null)
                        MouseUp(1, hookStruct.pt.x, hookStruct.pt.y);
                    break;
                }
            }
            return CallNextHookEx(_hookID, nCode, wParam, lParam);
        }

        private const int WH_MOUSE_LL = 14;

        private enum MouseMessages
        {
            WM_LBUTTONDOWN = 0x0201,
            WM_LBUTTONUP = 0x0202,
            WM_MOUSEMOVE = 0x0200,
            WM_MOUSEWHEEL = 0x020A,
            WM_RBUTTONDOWN = 0x0204,
            WM_RBUTTONUP = 0x0205
        }

        [StructLayout(LayoutKind.Sequential)]
        private struct POINT
        {
            public int x;
            public int y;
        }

        [StructLayout(LayoutKind.Sequential)]
        private struct MSLLHOOKSTRUCT
        {
            public POINT pt;
            public uint mouseData;
            public uint flags;
            public uint time;
            public IntPtr dwExtraInfo;
        }

        [DllImport("user32.dll", CharSet = CharSet.Auto, SetLastError = true)]
        private static extern IntPtr SetWindowsHookEx(int idHook,
            LowLevelMouseProc lpfn, IntPtr hMod, uint dwThreadId);

        [DllImport("user32.dll", CharSet = CharSet.Auto, SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        private static extern bool UnhookWindowsHookEx(IntPtr hhk);

        [DllImport("user32.dll", CharSet = CharSet.Auto, SetLastError = true)]
        private static extern IntPtr CallNextHookEx(IntPtr hhk, int nCode,
            IntPtr wParam, IntPtr lParam);

        [DllImport("kernel32.dll", CharSet = CharSet.Auto, SetLastError = true)]
        private static extern IntPtr GetModuleHandle(string lpModuleName);
    }
}

using System;
using System.IO;
using uComposer.Interop;

using NAudio;
using NAudio.Wave;
using System.Windows.Forms;
using NAudio.CoreAudioApi;
using System.Runtime.InteropServices;

namespace uComposer.ScratchPad
{
    class Program
    {
        //static DeviceScratchTest test;
        static ScratchpadAudioTest test2;

        [STAThread]
        static void Main(string[] args)
        {
            //test = new DeviceScratchTest();
            test2 = new ScratchpadAudioTest();
            Application.ApplicationExit += Application_ApplicationExit;
            Application.Run();
        }

        private static void Application_ApplicationExit(object sender, EventArgs e)
        {
            test2.Close();
        }
    }
}

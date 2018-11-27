using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using NAudio.CoreAudioApi;
using NAudio.Wave;

namespace uComposer.ScratchPad
{
    class TestWaveProvider : WaveProvider32
    {
        bool first = true;

        public override int Read(float[] buffer, int offset, int sampleCount)
        {
            if (first)
            {
                Console.WriteLine("Buffer Length: " + buffer.Length);
                Console.WriteLine("Offset: " + offset);
                Console.WriteLine("Sample Count: " + sampleCount);
                Console.WriteLine("Channels: " + this.WaveFormat.Channels);
                Console.WriteLine("Encoding: " + WaveFormat.Encoding);
                first = false;
            }



            return sampleCount;
        }
    }

    public class DeviceScratchTest
    {
        public DeviceScratchTest()
        {
            AsioOut aout = new AsioOut();


            MMDeviceEnumerator mmDeviceEnumerator = new MMDeviceEnumerator();
            MMDeviceCollection mmDeviceCollection = mmDeviceEnumerator.EnumerateAudioEndPoints(DataFlow.All, DeviceState.All);

            foreach (var mmDevice in mmDeviceCollection)
            {
                Console.WriteLine("Device: " + mmDevice.FriendlyName);
                Console.WriteLine("\tDataFlow: " + mmDevice.DataFlow);
            }

            WasapiOut wout = new WasapiOut(AudioClientShareMode.Shared, 0);



            Console.WriteLine("Driver Input Channel Count: " + aout.DriverInputChannelCount);
            Console.WriteLine("Driver Output Channel Count: " + aout.DriverOutputChannelCount);
            Console.WriteLine("Input Channel Count: " + aout.NumberOfInputChannels);
            Console.WriteLine("Input Channel Count: " + aout.NumberOfOutputChannels);


            for (int i = 0; i < aout.DriverInputChannelCount; i++)
            {
                Console.WriteLine("Driver Input Channel " + i + ": " + aout.AsioInputChannelName(i));
            }

            for (int i = 0; i < aout.DriverOutputChannelCount; i++)
            {
                Console.WriteLine("Driver Output Channel " + i + ": " + aout.AsioOutputChannelName(i));
            }


            TestWaveProvider hm = new TestWaveProvider();
            hm.SetWaveFormat(44100, 4);

            aout.Init(hm);
            aout.Play();
        }

        private static void Aout_AudioAvailable(object sender, AsioAudioAvailableEventArgs e)
        {
        }
    }
}

using System;
using System.Runtime.InteropServices;
using NAudio.CoreAudioApi;
using NAudio.Wave;

using LowLevelKeyboard;
using System.Collections.Generic;
using System.IO;

namespace uComposer.ScratchPad
{
    public class ScratchpadAudioTest
    {
        ScratchpadWaveProvider scratchpadWaveProvider;
        KeyboardHook _keyboard;
        MIDI.MidiInput _midiInput;

        public ScratchpadAudioTest()
        {
            _midiInput = new MIDI.MidiInput();
            _midiInput.MidiInputEventReceived += _midiInput_MidiInputEventReceived;
            _midiInput.Start();

            _keyboard = new KeyboardHook();
            _keyboard.KeyPressDetected += _keyboard_KeyPressDetected;
            _keyboard.KeyReleaseDetected += _keyboard_KeyReleaseDetected;
            int bufferSize = 64;
            int channelCount = 2;
            int sampleRate = 44100;

            //WaveOut dout = new WaveOut();

            AsioOut dout = new AsioOut();

            //dout.DesiredLatency = 0;
            //dout.NumberOfBuffers = 2;
            //WasapiOut wout = new WasapiOut(AudioClientShareMode.Shared, 0);
            scratchpadWaveProvider = new ScratchpadWaveProvider(sampleRate, channelCount, bufferSize);
            scratchpadWaveProvider.SetWaveFormat(sampleRate, channelCount);

            dout.Init(scratchpadWaveProvider);

            dout.Play();
            //dout.Volume = 1.0f;
        }

        private void _midiInput_MidiInputEventReceived(MIDI.MessageType msgType, byte key, float velocity)
        {
            if (msgType == MIDI.MessageType.NoteOn)
            {
                ScratchpadInterop.SendMessage(0, EventType.NoteOnEvent, new Note() { m_velocity = velocity, m_key = key });
            }
            else if (msgType == MIDI.MessageType.NoteOff)
            {
                ScratchpadInterop.SendMessage(0, EventType.NoteOffEvent, new Note() { m_velocity = velocity, m_key = key });
            }
        }

        public void Close()
        {
            _midiInput.Stop();
            scratchpadWaveProvider.Close();
        }

        private static Dictionary<Keys, byte> s_notesByKey = new Dictionary<Keys, byte>()
        {
            { Keys.Q,  0 },
            { Keys.D2, 1 },
            { Keys.W,  2 },
            { Keys.D3, 3 },
            { Keys.E,  4 },
            { Keys.R,  5 },
            { Keys.D5, 6 },
            { Keys.T,  7 },
            { Keys.D6, 8 },
            { Keys.Y,  9 },
            { Keys.D7, 10 },
            { Keys.U,  11 },
            { Keys.I,  12 },
            { Keys.D9, 13 },
            { Keys.O,  14 },
            { Keys.D0, 15 },
            { Keys.P,  16 },
            { Keys.Z,  17 },
            { Keys.S,  18 },
            { Keys.X,  19 },
            { Keys.D,  20 },
            { Keys.C,  21 },
            { Keys.F,  22 },
            { Keys.V,  23 },
            { Keys.B,  24 },
            { Keys.H,  25 },
            { Keys.N,  26 },
            { Keys.J,  27 },
            { Keys.M,  28 },
        };

        private void _keyboard_KeyPressDetected(Keys key, double time)
        {
            if (!_keyboard.Control && s_notesByKey.ContainsKey(key))
            {
                byte noteKey = (byte)(s_notesByKey[key] + 24);
                Note note;

                note.m_key = noteKey;
                note.m_velocity = 1.0f;
                note.m_tuning = 0.0f;

                ScratchpadInterop.SendMessage(0, EventType.NoteOnEvent, note);
            }
            else if (_keyboard.Control)
            {
                if (key == Keys.Space)
                {
                    ScratchpadInterop.Play();
                }
                if (key == Keys.P)
                {
                    ScratchpadInterop.Pause();
                }
                if (key == Keys.S)
                {
                    ScratchpadInterop.Stop();
                }

            }
        }

        private void _keyboard_KeyReleaseDetected(Keys key, double time)
        {
            if (!_keyboard.Control && s_notesByKey.ContainsKey(key))
            {
                byte noteKey = (byte)(s_notesByKey[key] + 24);
                Note note;

                note.m_key = noteKey;
                note.m_velocity = 1.0f;
                note.m_tuning = 0.0f;

                ScratchpadInterop.SendMessage(0, EventType.NoteOffEvent, note);
            }
        }
    }

    public enum SpeakerConfiguration
    {
        Mono,
        Stereo,
        Quad,
        Surround,
        Surround5point1,
        Surround7point1
    }

    public struct Note
    {
        public byte m_key;
        public float m_velocity;
        public float m_tuning;
    }

    public enum EventType
    {
        NoteOnEvent,
        NoteOffEvent,
    }

    public static class ScratchpadInterop
    {
        [DllImport("uComposer.Native.dll")]
        public static extern void Process(IntPtr buffer, int nsamples, int nchannels);
        [DllImport("uComposer.Native.dll")]
        public static extern IntPtr GetBuffer(int size);
        [DllImport("uComposer.Native.dll")]
        public static extern void ReleaseBuffer(IntPtr buffer);

        [DllImport("uComposer.Native.dll")]
        public static extern void InitializeScratchpad(int sampleRate, SpeakerConfiguration config, int bufferSize);

        [DllImport("uComposer.Native.dll")]
        public static extern int GetTrackNodeCount();

        [DllImport("uComposer.Native.dll")]
        public static extern void SendMessage(int trackNodeIndex, EventType type, Note note);

        [DllImport("uComposer.Native.dll")]
        public static extern void Play();

        [DllImport("uComposer.Native.dll")]
        public static extern void Pause();

        [DllImport("uComposer.Native.dll")]
        public static extern void Stop();

    }

    class ScratchpadWaveProvider : WaveProvider32
    {
        private int _bufferSize;
        private int _channelCount;
        private int _sampleRate;

        IntPtr _nativeBufferPtr;
        int _currentSample;
        float[] _managedBuffer;

        public void Close()
        {
            if (_writeToFile)
            {
                _fileWriter.Flush();
                _fileWriter.Close();
            }
        }
        public ScratchpadWaveProvider(int sampleRate, int channelCount, int bufferSize)
        {
            _channelCount = channelCount;
            _sampleRate = sampleRate;
            _bufferSize = bufferSize;

            _currentSample = _bufferSize * _channelCount;

            _managedBuffer = new float[_bufferSize * channelCount];
            _nativeBufferPtr = ScratchpadInterop.GetBuffer(_bufferSize * channelCount);

            ScratchpadInterop.InitializeScratchpad(_sampleRate, SpeakerConfiguration.Stereo, _bufferSize);

            //Console.WriteLine(ScratchpadInterop.GetTrackNodeCount());

            if (_writeToFile)
                _fileWriter = new BinaryWriter(File.Open("waveout.txt", FileMode.Create));
        }

        BinaryWriter _fileWriter;

        bool _writeToFile = false;

        public override int Read(float[] buffer, int offset, int sampleCount)
        {
            for (int i = 0; i < sampleCount; i++)
            {
                if (_currentSample == _bufferSize * _channelCount)
                {
                    ScratchpadInterop.Process(_nativeBufferPtr, _bufferSize, _channelCount);
                    Marshal.Copy(_nativeBufferPtr, _managedBuffer, 0, _bufferSize * _channelCount);
                    _currentSample = 0;
                }

                buffer[i + offset] = _managedBuffer[_currentSample];
                if (_writeToFile)
                    _fileWriter.Write(_managedBuffer[_currentSample]);
                _currentSample++;
            }
            return sampleCount;
        }
    }


    namespace MIDI
    {
        public delegate void MidiInputEvent(MessageType msgType, byte key, float velocity);

        public class MidiInput
        {
            private InputPort _inputPort;

            public event MidiInputEvent MidiInputEventReceived
            {
                add { _inputPort.MidiInputReceived += value; }
                remove { _inputPort.MidiInputReceived -= value; }
            }

            public MidiInput()
            {
                _inputPort = new InputPort();
            }

            public void Start()
            {
                _inputPort.Open(0);
                _inputPort.Start();
            }

            public void Stop()
            {
                _inputPort.Stop();
                _inputPort.Close();
            }
        }

        public enum MessageType : byte
        {
            NoteOn = 144,
            NoteOff = 128
        }

        public class InputPort
        {
            private NativeMethods.MidiInProc midiInProc;
            private IntPtr handle;

            public event MidiInputEvent MidiInputReceived;

            public InputPort()
            {
                midiInProc = new NativeMethods.MidiInProc(MidiProc);
                handle = IntPtr.Zero;
            }

            public static int InputCount
            {
                get { return NativeMethods.midiInGetNumDevs(); }
            }

            public bool Close()
            {
                bool result = NativeMethods.midiInClose(handle)
                    == NativeMethods.MMSYSERR_NOERROR;
                handle = IntPtr.Zero;
                return result;
            }

            public bool Open(int id)
            {
                return NativeMethods.midiInOpen(
                    out handle,
                    id,
                    midiInProc,
                    IntPtr.Zero,
                    NativeMethods.CALLBACK_FUNCTION)
                        == NativeMethods.MMSYSERR_NOERROR;
            }

            public bool Start()
            {
                return NativeMethods.midiInStart(handle)
                    == NativeMethods.MMSYSERR_NOERROR;
            }

            public bool Stop()
            {
                return NativeMethods.midiInStop(handle)
                    == NativeMethods.MMSYSERR_NOERROR;
            }

            // note on: 144
            // note off: 128



            private void MidiProc(IntPtr hMidiIn,
                int wMsg,
                IntPtr dwInstance,
                int dwParam1,
                int dwParam2)
            {
                //if (wMsg == 963)
                //    return;
                if (dwParam1 == 254)
                    return;

                byte[] p1bytes = BitConverter.GetBytes(dwParam1);
                MessageType msgType = (MessageType)p1bytes[0];
                byte key = p1bytes[1];
                byte velocity = p1bytes[2];

                MidiInputReceived?.Invoke(msgType, key, velocity / 127.0f);


                //Console.WriteLine("MessageType: " + msgType + ", Note: " + key + ", Velocity: " + (velocity / 127.0f));

                //Console.WriteLine(p1bytes[0] + ", " + p1bytes[1] + ", " + p1bytes[2] + ", " + p1bytes[3]);

                //Console.WriteLine("Received message: wMsg=" + wMsg + ", dwParam1=" + dwParam1 + ", d2Param2=" + dwParam2);
            }
        }

        internal static class NativeMethods
        {
            internal const int MMSYSERR_NOERROR = 0;
            internal const int CALLBACK_FUNCTION = 0x00030000;

            internal delegate void MidiInProc(
                IntPtr hMidiIn,
                int wMsg,
                IntPtr dwInstance,
                int dwParam1,
                int dwParam2);

            [DllImport("winmm.dll")]
            internal static extern int midiInGetNumDevs();

            [DllImport("winmm.dll")]
            internal static extern int midiInClose(
                IntPtr hMidiIn);

            [DllImport("winmm.dll")]
            internal static extern int midiInOpen(
                out IntPtr lphMidiIn,
                int uDeviceID,
                MidiInProc dwCallback,
                IntPtr dwCallbackInstance,
                int dwFlags);

            [DllImport("winmm.dll")]
            internal static extern int midiInStart(
                IntPtr hMidiIn);

            [DllImport("winmm.dll")]
            internal static extern int midiInStop(
                IntPtr hMidiIn);
        }
    }
}

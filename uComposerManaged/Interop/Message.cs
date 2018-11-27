//using System;
//using System.Runtime.InteropServices;


//namespace uComposer.Interop
//{
//    public enum MessageType : int
//    {
//        NoteOn = 0,
//        NoteOff = 1,
//        SetParameterValue = 2,
//    }

//    [StructLayout(LayoutKind.Explicit)]
//    public struct NoteMessageData
//    {
//        [FieldOffset(0)]
//        public int Note;
//        [FieldOffset(4)]
//        public float Velocity;        
//    }
//    [StructLayout(LayoutKind.Explicit)]
//    public struct ParameterMessageData
//    {
//        [FieldOffset(0)]
//        public int ParameterId;
//        [FieldOffset(4)]
//        public ParameterValue ParameterValue;
//    }

//    [StructLayout(LayoutKind.Explicit)]
//    public struct ParameterValue
//    {
//        [FieldOffset(0)]
//        public float floatValue;
//        [FieldOffset(0)]
//        public bool boolValue;
//        [FieldOffset(0)]
//        public int intValue;
//    }

//    [StructLayout(LayoutKind.Explicit)]
//    public struct MessageData
//    {
//        [FieldOffset(0)]
//        public NoteMessageData NoteOn;
//        [FieldOffset(0)]
//        public NoteMessageData NoteOff;
//        [FieldOffset(0)]
//        public ParameterMessageData SetParameterValue;
//    }

//    [StructLayout(LayoutKind.Explicit)]
//    public struct Message
//    {
//        [FieldOffset(0)]
//        public MessageType MessageType;
//        [FieldOffset(8)]
//        public ulong Offset;
//        [FieldOffset(16)]
//        public MessageData MessageData;

//        public static Message CreateNoteOn(ulong offset, int note, float velocity)
//        {
//            Message msg = new Message();
//            msg.MessageType = MessageType.NoteOn;
//            msg.Offset = offset;
//            msg.MessageData.NoteOn.Note = note;
//            msg.MessageData.NoteOn.Velocity = velocity;
//            return msg;
//        }

//        public static Message CreateNoteOff(ulong offset, int note, float velocity)
//        {
//            Message msg = new Message();
//            msg.MessageType = MessageType.NoteOff;
//            msg.Offset = offset;
//            msg.MessageData.NoteOff.Note = note;
//            msg.MessageData.NoteOff.Velocity = velocity;
//            return msg;
//        }

//        public static Message CreateControlMessage(ulong offset, int parameterId, float parameterValue)
//        {
//            Message msg = new Message();
//            msg.MessageType = MessageType.SetParameterValue;
//            msg.Offset = offset;
//            msg.MessageData.SetParameterValue.ParameterId = parameterId;
//            msg.MessageData.SetParameterValue.ParameterValue.floatValue = parameterValue;
//            return msg;
//        }

//        public static Message CreateControlMessage(ulong offset, int parameterId, bool parameterValue)
//        {
//            Message msg = new Message();
//            msg.MessageType = MessageType.SetParameterValue;
//            msg.Offset = offset;
//            msg.MessageData.SetParameterValue.ParameterId = parameterId;
//            msg.MessageData.SetParameterValue.ParameterValue.boolValue = parameterValue;
//            return msg;
//        }

//        public static Message CreateControlMessage(ulong offset, int parameterId, int parameterValue)
//        {
//            Message msg = new Message();
//            msg.MessageType = MessageType.SetParameterValue;
//            msg.Offset = offset;
//            msg.MessageData.SetParameterValue.ParameterId = parameterId;
//            msg.MessageData.SetParameterValue.ParameterValue.intValue = parameterValue;
//            return msg;
//        }
//    }
//}

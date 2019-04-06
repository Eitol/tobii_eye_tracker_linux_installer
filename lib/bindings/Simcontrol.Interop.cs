using System;
using System.Runtime.InteropServices;
using System.Text;
// ReSharper disable MemberCanBePrivate.Global
// ReSharper disable FieldCanBeMadeReadOnly.Global

namespace Tobii.StreamEngine.Simcontrol
{

    public enum simcontrol_error_t
    {
        NoError,
        Internal,
        NotAvailable,
        InvalidParameter,
        AllocationFailed,
        ConnectionFailed,
        NotSupported,
        BufferTooSmall,
        OperationFailed,
        FirmwareNoResponse,
        BadState,
        TooManySubscribers,
        UnexpectedTransactionId,
        UnexpectedMessageType,
        Protocol,
        Unknown,
        AlreadySubscribed,
        NotSubscribed,
        InsufficientLicense,
        TimedOut,
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct point2d
    {
        public float x;
        public float y;
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct point3d
    {
        public float x;
        public float y;
        public float z;
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct simcontrol_point2d_t
    {
        public point2d left_eye_xy;
        public point2d right_eye_xy;
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct simcontrol_point3d_t
    {
        public point3d left_eye_xyz;
        public point3d right_eye_xyz;
    }

    public enum simcontrol_presence_t
    {
        Unknow,
        NotPresent,
        Present,
    }

    public enum simcontrol_validity_t
    {
        Invalid,
        Valid,
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct simcontrol_eye_validity_t
    {
        public simcontrol_validity_t left;
        public simcontrol_validity_t right;
    }

    public enum simcontrol_notification_type_t
    {
        SIMCONTROL_NOTIFICATION_TYPE_CALIBRATION_STATE_CHANGED,
        SIMCONTROL_NOTIFICATION_TYPE_EXCLUSIVE_MODE_STATE_CHANGED,
        SIMCONTROL_NOTIFICATION_TYPE_TRACK_BOX_CHANGED,
        SIMCONTROL_NOTIFICATION_TYPE_DISPLAY_AREA_CHANGED,
        SIMCONTROL_NOTIFICATION_TYPE_FRAMERATE_CHANGED,
        SIMCONTROL_NOTIFICATION_TYPE_POWER_SAVE_STATE_CHANGED,
        SIMCONTROL_NOTIFICATION_TYPE_DEVICE_PAUSED_STATE_CHANGED,
        SIMCONTROL_NOTIFICATION_TYPE_CALIBRATION_ENABLED_EYE_CHANGED,
        SIMCONTROL_NOTIFICATION_TYPE_CALIBRATION_ID_CHANGED,
        SIMCONTROL_NOTIFICATION_TYPE_COMBINED_GAZE_EYE_SELECTION_CHANGED,
        SIMCONTROL_NOTIFICATION_TYPE_FAULTS_CHANGED,
        SIMCONTROL_NOTIFICATION_TYPE_WARNINGS_CHANGED,
    }

    
    [StructLayout(LayoutKind.Sequential)]
    public struct simcontrol_display_area_t
    {
        public point3d top_left_mm_xyz;
        public point3d top_right_mm_xyz;
        public point3d bottom_left_mm_xyz;
    }

    public enum simcontrol_enabled_eye_t
    {
        SIMCONTROL_ENABLED_EYE_LEFT,
        SIMCONTROL_ENABLED_EYE_RIGHT,
        SIMCONTROL_ENABLED_EYE_BOTH,
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct simcontrol_notification_data_t
    {
        public float float_;
        public string string_;
        public bool state;
        public uint uint_;
        public simcontrol_display_area_t display_area;
        public simcontrol_enabled_eye_t enabled_eye;
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct simcontrol_notification_t
    {
       public simcontrol_notification_type_t notification_type;
       public simcontrol_notification_data_t data;
    }

    public static class Interop
    {
        private const string SimcontrolDll = "simcontrol";

        [DllImport(SimcontrolDll, CallingConvention = CallingConvention.Cdecl)]
        public static extern simcontrol_error_t simcontrol_context_create_with_license(out IntPtr context, string url, string license_file_path);

        [DllImport(SimcontrolDll, CallingConvention = CallingConvention.Cdecl)]
        public static extern simcontrol_error_t simcontrol_context_destroy(IntPtr context);


        [DllImport(SimcontrolDll, CallingConvention = CallingConvention.Cdecl)]
        public static extern simcontrol_error_t simcontrol_set_gaze_point(IntPtr context, ref simcontrol_point2d_t gaze_point);

        [DllImport(SimcontrolDll, CallingConvention = CallingConvention.Cdecl)]
        public static extern simcontrol_error_t simcontrol_set_gaze_origin(IntPtr context, ref simcontrol_point3d_t gaze_origin);

        [DllImport(SimcontrolDll, CallingConvention = CallingConvention.Cdecl)]
        public static extern simcontrol_error_t simcontrol_set_gaze_origin_mm(IntPtr context, ref simcontrol_point3d_t gaze_origin_mm);

        [DllImport(SimcontrolDll, CallingConvention = CallingConvention.Cdecl)]
        public static extern simcontrol_error_t simcontrol_set_center_of_eye_mm(IntPtr context, ref simcontrol_point3d_t center_of_eye);

        [DllImport(SimcontrolDll, CallingConvention = CallingConvention.Cdecl)]
        public static extern simcontrol_error_t simcontrol_set_gaze_point_cloud_radius(IntPtr context, float radius);

        [DllImport(SimcontrolDll, CallingConvention = CallingConvention.Cdecl)]
        public static extern simcontrol_error_t simcontrol_set_eye_tracked(IntPtr context, simcontrol_eye_validity_t eye_tracked);

        [DllImport(SimcontrolDll, CallingConvention = CallingConvention.Cdecl)]
        public static extern simcontrol_error_t simcontrol_set_eye_detected(IntPtr context, simcontrol_eye_validity_t eye_detected);

        [DllImport(SimcontrolDll, CallingConvention = CallingConvention.Cdecl)]
        public static extern simcontrol_error_t simcontrol_set_presence_state(IntPtr context, simcontrol_presence_t presence_state);


        [DllImport(SimcontrolDll, CallingConvention = CallingConvention.Cdecl)]
        public static extern simcontrol_error_t simcontrol_tracking_activate(IntPtr context);

        [DllImport(SimcontrolDll, CallingConvention = CallingConvention.Cdecl)]
        public static extern simcontrol_error_t simcontrol_tracking_deactivate(IntPtr context);

        [DllImport(SimcontrolDll, CallingConvention = CallingConvention.Cdecl)]
        public static extern simcontrol_error_t simcontrol_presence_activate(IntPtr context);

        [DllImport(SimcontrolDll, CallingConvention = CallingConvention.Cdecl)]
        public static extern simcontrol_error_t simcontrol_presence_deactivate(IntPtr context);

        [DllImport(SimcontrolDll, CallingConvention = CallingConvention.Cdecl)]
        public static extern simcontrol_error_t simcontrol_gaze_point_cloud_activate(IntPtr context);

        [DllImport(SimcontrolDll, CallingConvention = CallingConvention.Cdecl)]
        public static extern simcontrol_error_t simcontrol_gaze_point_cloud_deactivate(IntPtr context);

        [DllImport(SimcontrolDll, CallingConvention = CallingConvention.Cdecl)]
        public static extern simcontrol_error_t simcontrol_set_gaze_direction(IntPtr context, ref simcontrol_point3d_t gaze_direction);

        [DllImport(SimcontrolDll, CallingConvention = CallingConvention.Cdecl)]
        public static extern simcontrol_error_t simcontrol_set_eyeopenness_validity(IntPtr context, simcontrol_eye_validity_t eyeopenness_validity);

        [DllImport(SimcontrolDll, CallingConvention = CallingConvention.Cdecl)]
        public static extern simcontrol_error_t simcontrol_set_eyeopenness(IntPtr context, float eyeopenness);

        [DllImport(SimcontrolDll, CallingConvention = CallingConvention.Cdecl)]
        public static extern simcontrol_error_t simcontrol_set_pupildiameter_validity(IntPtr context, simcontrol_eye_validity_t pupildiameter_validity);

        [DllImport(SimcontrolDll, CallingConvention = CallingConvention.Cdecl)]
        public static extern simcontrol_error_t simcontrol_set_pupildiameter(IntPtr context, float pupildiameter);

        [DllImport(SimcontrolDll, CallingConvention = CallingConvention.Cdecl)]
        public static extern simcontrol_error_t simcontrol_trigger_notification(IntPtr context, ref simcontrol_notification_t notification);
    }
}
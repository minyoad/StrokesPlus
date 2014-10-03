// This file contains basic type definitions for the Synaptics SDK

#ifndef __SYNCOMDEFS_H_
#define __SYNCOMDEFS_H_

#ifndef __SYNCTRLLib_LIBRARY_DEFINED__

// Property specifiers

// API property specifiers

enum SynAPIProperty {
  SP_First = 0x10000000,
  SP_Version,
  SP_SynComDllVersion = SP_Version,
  SP_MaxDevices,
  SP_DevicesPresent,
  SP_DriverVersion,
  SP_RequiredDriverVersion,
  SP_APILast = SP_First + 0x100
};

// Device property specifiers

enum SynDeviceProperty {
  SP_DeviceFirst = SP_APILast,
  // Direct values (cached).
  SP_Handle,
  SP_DeviceType,
  SP_ConnectionType,
  
  SP_FWVersion,
  SP_Geometry,
  SP_SensorType,
  SP_ProductType,
  SP_ASICType = SP_ProductType + 2,
  SP_ReportRate = SP_ASICType + 4,
  SP_Gestures,
  SP_SecondaryGestures,
  SP_EdgeMotionOptions,
  SP_EdgeMotionSpeed,
  SP_MotionRotationAngle,

  SP_XDPI,
  SP_YDPI,
  SP_XLoSensor,
  SP_YLoSensor,
  SP_XHiSensor,
  SP_YHiSensor,
  SP_XLoRim,
  SP_YLoRim,
  SP_XHiRim,
  SP_YHiRim,
  SP_XLoBorder,
  SP_YLoBorder,
  SP_XHiBorder,
  SP_YHiBorder,
  SP_YLoBorderVScroll,
  SP_YHiBorderVScroll,
  SP_XLoWideBorder,
  SP_YLoWideBorder,
  SP_XHiWideBorder,
  SP_YHiWideBorder,
  SP_ZMaximum,
  SP_ZTouchThreshold,

  SP_TopLeftCornerWidth,
  SP_TopLeftCornerHeight,
  SP_TopRightCornerWidth,
  SP_TopRightCornerHeight,
  SP_BottomRightCornerWidth,
  SP_BottomRightCornerHeight,
  SP_BottomLeftCornerWidth,
  SP_BottomLeftCornerHeight,

  SP_TopLeftCornerAction,
  SP_TopRightCornerAction,
  SP_BottomRightCornerAction,
  SP_BottomLeftCornerAction,

  SP_LeftButtonAction,
  SP_RightButtonAction,
  SP_BothButtonAction,  
  SP_MiddleButtonAction,
  SP_UpButtonAction,
  SP_DownButtonAction,
  SP_Ex1ButtonAction,
  SP_Ex2ButtonAction,
  SP_Ex3ButtonAction,
  SP_Ex4ButtonAction,
  SP_Ex5ButtonAction,
  SP_Ex6ButtonAction,
  SP_Ex7ButtonAction,
  SP_Ex8ButtonAction,

  SP_ExtendedButtons,

  // Boolean properties.
  SP_HasMiddleButton,
  SP_HasUpDownButtons,

  SP_IsMultiFingerCapable,
  SP_IsPenCapable,
  SP_IsVScroll,
  SP_IsHScroll,
  SP_IsWEMode,
  SP_IsLowReportRate,
  SP_IsHighReportRate,

  SP_IsTapEnabled,
  SP_IsDragEnabled,
  SP_IsDragLockEnabled,
  SP_IsCornerTapEnabled,
  SP_IsEdgeMotionEnabled,
  SP_IsEdgeMotionDragEnabled,
  SP_IsEdgeMotionMoveEnabled,

  SP_IsReleaseToSelectEnabled, 
  SP_IsMiddleTapToHelpEnabled, 
  SP_IsMiddleButtonBlockEnabled, 
  SP_IsPressureDragEnabled, 
  SP_Is3ButtonEnabled, 
  SP_IsPressureEdgeMotionEnabled, 
  SP_IsMiddleButtonLock, 

  // Button 4 & 5 support
  SP_Button4Action,
  SP_Button5Action,

  // Somewhat vetted enhancements app properties.
  SP_VerticalScrollingFlags = SP_Button5Action + 17,
  SP_HorizontalScrollingFlags,

  // New COM specific properties
  SP_DisplayFlags,

  // Newer properties.
  SP_ModelId,
  SP_DisableState,

  SP_DeviceLast = SP_DeviceFirst + 0x200
};

// Packet property specifiers

enum SynPacketProperty {
  SP_PacketFirst = SP_DeviceLast,
  SP_AssociatedDeviceHandle,
  SP_SequenceNumber,
  SP_TimeStamp,
  SP_XRaw,
  SP_ZXRaw = SP_XRaw,
  SP_YRaw,
  SP_ZRaw,
  SP_ZYRaw,
  SP_W,
  SP_X,
  SP_Y,
  SP_Z,
  SP_XDelta,
  SP_YDelta,
  SP_ZDelta,
  SP_XMickeys,
  SP_YMickeys,
  SP_AnachronisticState,
  SP_FingerState,
  SP_ExtendedState,
  SP_ButtonState,
  SP_ExtraFingerState,
  SP_PacketLast = SP_PacketFirst + 0x100
};

// Display property specifiers

enum SynDisplayProperty {
  SP_DisplayFirst = SP_PacketLast,
  SP_BackLightState,
  SP_DisplayRows,
  SP_DisplayColumns,
  SP_DisplayOwned,
  SP_BackLightOnOffOnce,
  SP_DisplayLast = SP_DisplayFirst + 0x100
};

// String properties

// API strings.

enum SynAPIStringProperty {
  SP_StringFirst = SP_DisplayLast,
  SP_APIStringFirst = SP_StringFirst,
  SP_VersionString,
  SP_APIStringLast = SP_APIStringFirst + 0x100
};

// Device strings.

enum SynDeviceStringProperty {
  SP_DeviceStringFirst = SP_APIStringLast,
  SP_ModelString,
  SP_PnPID,
  SP_ShortName,
  SP_DeviceStringLast = SP_DeviceStringFirst + 0x100,
  SP_StringLast = SP_DeviceStringLast
};


// For use with ISynAPI::FindDevice

// The various types of devices.
enum SynDeviceType {
  SE_DeviceAny,
  SE_DeviceUnknown = SE_DeviceAny,
  SE_DeviceMouse,
  SE_DeviceTouchPad,
  SE_DeviceWheelMouse,
  SE_DeviceIBMCompatibleStick,
  SE_DeviceStyk,
  SE_DeviceFiveButtonWheelMouse,
  SE_DevicecPad,
  SE_DeviceTouchScreen = SE_DeviceTouchPad
};

// The various ways that devices may be connected.
enum SynConnectionType {
  SE_ConnectionAny,
  SE_ConnectionCOM,
  SE_ConnectionPS2,
  SE_ConnectionUSB,
  SE_ConnectionUnknown = SE_ConnectionAny
};

// Interpret notification reasons returned by ISynAPI::GetEventParameter
enum SynNotificationReason
{
  SE_Configuration_Changed = 0x00000001,
  SE_DeviceRemoved = 0x00000002,
  SE_DeviceAdded = 0x00000004,
  SE_InternalPS2DeviceDisabled = 0x00000008,
  SE_InternalPS2DeviceInCompatibilityMode = 0x00000010
  // Specialized Enhancements App notifications are not passed through
  // in this release.
};

// Interpretation of SP_ButtonState, each bit indicates a particular button.
enum SynButtonFlags { // Not fleshed-out yet.
  SF_ButtonLeft           = 0x00000001,
  SF_ButtonRight          = 0x00000002,
  SF_ButtonMiddle         = 0x00000004,
  SF_ButtonUp             = 0x00000010,
  SF_ButtonDown           = 0x00000020,
  SF_Button4              = 0x00000040,
  SF_Button5              = 0x00000080,
  SF_ButtonExtended1      = 0x00000100,
  SF_ButtonExtended2      = 0x00000200,
  SF_ButtonExtended3      = 0x00000400,
  SF_ButtonExtended4      = 0x00000800,
  SF_ButtonExtended5      = 0x00001000,
  SF_ButtonExtended6      = 0x00002000,
  SF_ButtonExtended7      = 0x00004000,
  SF_ButtonExtended8      = 0x00008000,
  SF_ButtonReportedLeft    = 0x00010000,
  SF_ButtonReportedRight   = 0x00020000,
  SF_ButtonReportedMiddle  = 0x00040000,
  SF_ButtonReported4       = 0x00080000,
  SF_ButtonReported5       = 0x01000000,
  SF_ButtonVirtualLeft   = 0x00100000,
  SF_ButtonVirtualRight  = 0x00200000,
  SF_ButtonVirtualMiddle = 0x00400000,
  SF_ButtonVirtual4      = 0x00800000,
  SF_ButtonVirtual5      = 0x02000000,
  SF_ButtonAnyVirtual    = 0x02f00000,
  SF_ButtonAnyReported   = 0x010f0000,
  SF_ButtonAnyPhysical   = 0x0000ffff,
  SF_ButtonAny           = 0x03ffffff
};


// Interpretation of vertical and horizontal scrolling flags.
enum SynScrollingFlags { 
  SF_ScrollingEnabled       = 0x00000001,
  SF_ScrollingEdgeMotion    = 0x00000002,
  SF_ScrollingMomentum      = 0x00000004,
  SF_ScrollingUnderPointer  = 0x00000008,
  SF_ScrollingReversed      = 0x00000040,
  SF_ScrollingAxisSwapped   = 0x00000080
};

// Interpretation of vertical and horizontal scrolling flags.
enum SynHardwareBroadcastFlags { 
  SF_Detect,
  SF_Configure,
};

enum SynPersistFlags {
  SF_PersistMachine
};

enum SynFingerFlags {
  SF_FingerProx           = 0x00000200, // Z exceeds proximity threshold.
  SF_FingerTouch          = 0x00000400, // Z exceeds touch threshold.
  SF_FingerHeavy          = 0x00000800, // Z exceeds heavy touch threshold.
  SF_FingerPress          = 0x00001000, // "Press hard" gesture in progress.
  SF_FingerPresent        = 0x00002000, // Finger present on TouchPad.
  SF_FingerPossTap        = 0x00004000, // Early stroke, tap still possible.
  SF_FingerStylus         = 0x00008000, // Stylus present on TouchPad.

  SF_FingerTap            = 0x00010000, // Tap gesture in progress.
  SF_FingerDrag           = 0x00020000, // Drag gesture in progress.
  SF_FingerDragLock       = 0x00040000, // Drag-lock gesture in progress.
  SF_FingerPrimGest       = 0x00080000, // Primary gesture in progress.
  SF_FingerSecGest        = 0x00100000, // Secondary gesture in progress.
  SF_FingerAuxGest        = 0x00200000, // Auxiliary gesture in progress.
  SF_FingerMotion         = 0x00400000, // Motion deltas are non-zero.
  SF_FingerMoving         = 0x00800000, // Significant finger motion.

  SF_FingerTopLeftTap     = 0x01000000, // Tap in zone in progress.
  SF_FingerTopRightTap    = 0x02000000, // Tap in zone in progress.
  SF_FingerBottomLeftTap  = 0x04000000, // Tap in zone in progress.
  SF_FingerBottomRightTap = 0x08000000, // Tap in zone in progress.

  // Flags for multi finger pads
  SF_FingerTap2           = 0x40000000, // 2 finger tap
  SF_FingerTap3           = 0x80000000, // 3+ finger tap

  SF_FingerAllCorner      = 0x0F000000, // Corner gestures.
  SF_FingerAllTap         = 0xCF000000, // Any and all taps
  SF_FingerAsButton       = 0xCF380000, // Finger state as buttons
  SF_FingerAllGest        = 0xCF3F1000, // Any and all gestures.
};

enum SynStateFlags {
  SF_StateFinger = 0x00002000
};

enum SynExtendedStateFlags {
  SF_StateScrollFinger = 0x00000100 // Finger in scroll area
};

enum SynDisplayFlags {
  SF_DisplayNone,
  SF_DisplayPresent = 0x00000001
};

enum SynFlushMethod {
  SE_FlushSynchronous,
  SE_FlushAsynchronous
};

enum SynDisplayAquisitionMethod {
  SE_AcquireCooperative,
  SE_AcquireExclusive,
  SE_AcquirePassive
};

// This version of the Synaptics SDK requires a driver version of at least
// the following. The Synaptics driver version can be obtained using the 
// SP_DriverVersion property of the SynAPI interface.
enum SynRequiredVersion {
  SE_RequiredDriverVersion    = 0x07080300,
  SE_RequiredSynCOMDllVersion = 0x07080300,
};

// Interpret notification messages returned by ISynDisplay::GetEventParameter
enum SynDisplayMessage
{
  SE_AcquisitionLost        = 0x00000001,
  SE_AcquisitionGained      = 0x00000002,
  SE_BackgroundImageChanged = 0x00000003,
};

// Use with SP_Gestures to enable or disable certain gestures.
enum SynGestures {
  SF_GestureTap              = 0x00000001, // Enable tap gestures.
  SF_GesturePressToSelect    = 0x00000001, // Enable press to select gestures.
  SF_GestureDrag             = 0x00000002, // Enable drag gestures.
  SF_GestureDragLock         = 0x00000004, // Enable drag lock.
  SF_GesturePressureDrag     = 0x00000008, // Press-to-drag.
  SF_GestureReleaseToSelect  = 0x00000010, // Enable Release-to-Select
  SF_GestureMiddleTapToHelp  = 0x00000020, // 3-middle button taps to help
  SF_GestureMiddleButtonLock = 0x00000040, // Middle Button Lock state
  SF_GestureMiddleButtonBlock= 0x00000080, // Middle Button Block state
  SF_GestureTapMask          = 0x00000007, // Mask for tap gestures.
  SF_GestureAll              = 0x000000FF
};

// Use with SP_*Action properties to associate an action with a particular
// device event.
enum SynActions {
  SF_ActionNone       = 0x00000000, // Generate no gestures
  SF_ActionPrimary    = 0x00000001, // Generate primary gestures
  SF_ActionSecondary  = 0x00000002, // Generate secondary gestures
  SF_ActionAuxilliary = 0x00000004, // Generate auxiliary gestures
  SF_ActionAll        = 0x00000007
};

enum SynAcquisitionFlags { // Use with SynDevice::Acquire
  SF_AcquireAll           = 0x00000000, // Acquire both motion and buttons.
  SF_AcquireMotionOnly    = 0x00000001  // Let button changes through to OS.
};

#endif
#endif
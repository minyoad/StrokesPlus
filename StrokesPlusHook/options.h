/****************************************************************************************************************************
	PRECOMPILER DIRECTIVES 
	
	Global switches to enable/disable certain functionality - Line uncommented == feature enabled
****************************************************************************************************************************/

//#define SP_DEBUG_MODE			//Uncomment this line to build a version of S+ with debugging output sent via OutputDebugString (only some code uses this)
//#define SP_TOUCH				//toggles whether touch-related stuff should be compiled in (incomplete, only initial discovery code prototyping) 

#define SP_SYNAPTIC				//toggles whether Synaptics code is compiled 
#define SP_AFTERRELEASESCRIPT	//toggles whether allow after script functionality is enabled 
#define SP_WIN8_CHECKS			//toggles things like IsImmersive checks, charm windows, etc 
#define SP_NOGLOBALACTIONS		//toggles whether the ability to diable global actions is present 
#define SP_MOUSE_CLICK_ACTION	//toggles whether the ability to have a global sp_BTN_mouse_up() function fire on mouse button up events
#define SP_MONITOR_BRIGHTNESS	//toggles whether the ability to change monitor brightness is enabled
#define SP_WHEEL_HOOK			//toggles whether the sp_wheel_tick function is called on each wheel tick
/****************************************************************************************************************************/
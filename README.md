------------------------------------------------------
Required:
------------------------------------------------------

- Install VS2010 Ultimate (compiled EXE/DLL crash when using VS2012...didn't fully investigate or resolve)
    (only C++ selected)
- Install VS2010 SP1
- Install Windows SDK for Windows 7 
    (Default Options, Install Path: C:\Program Files\Microsoft SDKs\Windows\v7.1)
- Extract contents of StrokesPlus_2.7.2.zip to C:\Development\StrokesPlus
  *** The next section is incomplete as I've removed the certificate. Remove signing or make your own cert ***
- Install StrokesPlus Certificate: 
    Open an instance of Windows SDK 7.1 Command Prompt (under MS Windows SDK v7.1 Start Menu Group)
    type: makecert -ss StrokesPlusCertStore (press enter, should receive a Succeeded message)
    Open Certificate Manager (Start > Run > certmgr.msc)
    Expand the StrokesPlusCertStore and click Certificates
    Select and delete the default certificate created ("Joe's Software Emporium", or something like that)
    Right-click C:\Development\StrokesPlus\bin\[REMOVED], select Install PFX
    Click Next
    Click Next
    Enter Password: [REMOVED] (only leave checked Include all extended properties.), click Next
    Select Place all certificates in the following store and click Browse...
    Select StrokesPlusCertStore, click OK
    Click Next
    Click Finish

Certificate Note: This certificate is only for the signing of the EXE, you'll need to either manually install 
the C:\Development\StrokesPlus\bin\StrokesPlus.cer file into the Trusted Root Certification Authorities, or simply 
run the StrokesPlus Signed setup EXE to install the certificate. If the certificate is not present, you will receive 
the error "A referral was returned from the server" upon running a signed version. Ignore the StrokesPlus.pfx, 
that's my version with a separate password; AWEStrokesPlus.pfx is a full export of that cert, but with a different 
password. The AWEStrokesPlus.pfx cert can sign for the StrokesPlus.cer, so anywhere you've already installed 
StrokesPlus will still work when you build and sign with the AWEStrokesPlus cert.


------------------------------------------------------
Optional, used for my own package and deploy purposes:
------------------------------------------------------

- Installed Inno Setup 5.5.3 (Default Options, Install Path: C:\Program Files (x86)\Inno Setup 5)
- Installed 7-Zip 9.20 (Install Path: C:\Program Files\7-Zip)
- Update Inno Setup file for each release, replacing the BaseFolder to use C:\Development\StrokesPlus


------------------------------------------------------
Building the solution:
------------------------------------------------------

Open C:\Development\StrokesPlus\StrokesPlus.sln

In the Resource View pane, both StrokesPlus > Version > VS_VERSION_INFO and StrokesPlusHook > Version > VS_VERSION_INFO 
should be updated to reflect the corrent release version before building. 

In the standard toolbar, there should be a pair of dropdowns to select the build configuration and target platform. 
There are 4 total configuration options:

- Release (Signed) > x64
- Release (Signed) > x86
- Release (Unsigned) > x64
- Release (Unsigned) > x86

Note that they must be built separately, Batch Build appears to get stuck in a recursive build process which I 
haven't bothered to further investigate.

You cannot run the solution in Debug mode. Due to the nature of low-level mouse and keyboard hooks, running in debug 
mode cannot be accomplished. If you need to debug, there is a pre-compiler definition named SP_DEBUG_MODE in options.h under
StrokesPlusHook > Header Files (via Solution Explorer) which may be uncommented to trigger calls to OutputDebugString. 
Note that there are not a lot of debug output spots in the code, only for areas which I need to troubleshoot. If 
SP_DEBUG_MODE is enabled, you can add the following call anywhere to trigger a call to OutputDebugString: LOGMESSAGE(str). 
You can then view the debug string output using DebugView (from Windows Sysinternals, Microsoft).

Note that for Lua and/or Alien (FFI) to work when compiled to 64-bit, there is a source file under 
StrokesPlusHook > Source Files > win64.asm. This file is excluded via the x86 build configurations. You shouldn't need 
to do anything, but just so you understand why it's excluded in certain build targets.

In the C:\Development\StrokesPlus\bin folder, there are seemingly duplicate directories. The Relase and Release (Unsigned) 
directories contain the actual output of solution builds. The _CopyAndDeploy.bat file copies the output binaries from those 
folders into the appropriate StrokesPlus_{platform}_{signed/unsigned} folder. The bat file then copies the 
C:\Development\StrokesPlus\Help files into the StrokesPlus_{plat}_{signed} folders, cleans up the Upload folder, ZIPs the 
StrokesPlus_{plat}_{signed} folders into the proper outputs to be uploaded to the StrokesPlus site, and finally calls Inno Setup 
to produce the install EXE for each build version. After the ZIP files are copied to the bin\Upload folder, they have "VER" 
in the file name as a placeholder for the actual version, which would need to be manually updated.

Note that to benefit from system-wide interaction starting with Windows Vista when UAC is enabled, you need to use the signed 
version and the signed version must be installed under Program Files. Otherwise, programs which run as Administrator along 
with interfaces like the Windows 8 Start Screen will not allow StrokesPlus to hook and interact with those programs as they 
run in a separate authentication protection workspace.

By default, StrokesPlus install packages will place the ActionHelp.xml, StrokesPlus.lua, and StrokesPlus.xml files under 
the user's roaming data folder (C:\Users\USERNAME\AppData\Roaming\StrokesPlus).  Upon loading, StrokesPlus will first attempt 
to use the files from that location. If they don't exist, it will check the folder from which StrokesPlus is running. If the 
files aren't found there (or there is insufficient permissions), StrokesPlus will create the StrokesPlus.lua and StrokesPlus.xml 
file under the user's roaming data folder.



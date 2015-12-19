; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppName "TDP005"
#define MyAppPublisher "A15 Entertainment"
#define MyAppExeName "TDP005.exe"
#define OutputDir SourcePath
#define AppVersion GetFileVersion('..\win32\Release\TDP005.exe')

#define SDL2_PATH GetEnv('SDL2_PATH')
#define SDL2_IMAGE_PATH GetEnv('SDL2_IMAGE_PATH')
#define SDL2_TTF_PATH GetEnv('SDL2_TTF_PATH')
#define SDL2_GFX_PATH GetEnv('SDL2_GFX_PATH')
#define SDL2_MIXER_PATH GetEnv('SDL2_MIXER_PATH')

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{43187B75-1FFA-475A-B230-56B6D4CC5EBC}
AppName={#MyAppName}
AppVersion={#AppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
DefaultDirName={pf}\{#MyAppName}
DefaultGroupName={#MyAppName}
AllowNoIcons=yes
OutputBaseFilename={#MyAppName}-Setup-{#AppVersion}
OutputDir={#OutputDir}
Compression=lzma
SolidCompression=yes
LicenseFile="../LICENCE.md"

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "..\win32\Release\{#MyAppExeName}"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\assets\fonts\*"; DestDir: "{app}\fonts"; Flags: ignoreversion
Source: "..\assets\gfx\*"; DestDir: "{app}\gfx"; Flags: ignoreversion recursesubdirs
Source: "..\assets\snd\*"; DestDir: "{app}\snd"; Flags: ignoreversion
Source: "..\LICENCE.md"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\CREDITS.md"; DestDir: "{app}"; Flags: ignoreversion
; SDL2
Source: "{#SDL2_PATH}\lib\x86\*.dll"; DestDir: "{app}"; Flags: ignoreversion
; SDL2 Image
Source: "{#SDL2_IMAGE_PATH}\lib\x86\SDL2_image.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#SDL2_IMAGE_PATH}\lib\x86\zlib1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#SDL2_IMAGE_PATH}\lib\x86\libpng16-16.dll"; DestDir: "{app}"; Flags: ignoreversion
; SDL2 TTF
Source: "{#SDL2_TTF_PATH}\lib\x86\SDL2_ttf.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#SDL2_TTF_PATH}\lib\x86\libfreetype-6.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#SDL2_TTF_PATH}\lib\x86\zlib1.dll"; DestDir: "{app}"; Flags: ignoreversion
; SDL2 GFX
Source: "{#SDL2_GFX_PATH}\lib\x86\SDL2_gfx.dll"; DestDir: "{app}"; Flags: ignoreversion
; SDL2 Mixer
Source: "{#SDL2_MIXER_PATH}\lib\x86\SDL2_mixer.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#SDL2_MIXER_PATH}\lib\x86\libvorbis-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#SDL2_MIXER_PATH}\lib\x86\libvorbisfile-3.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#SDL2_MIXER_PATH}\lib\x86\libogg-0.dll"; DestDir: "{app}"; Flags: ignoreversion
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[UninstallDelete]
Type: files; Name: "{app}\settings.cfg"
Type: files; Name: "{app}\highscore"

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{group}\{cm:UninstallProgram,{#MyAppName}}"; Filename: "{uninstallexe}"
Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent


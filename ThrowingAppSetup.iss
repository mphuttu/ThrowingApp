; Inno Setup Script for ThrowingApp
; Throwing trajectory analysis for shot put and javelin

#define MyAppName "ThrowingApp"
#define MyAppVersion "1.0.0.1"
#define MyAppPublisher "Mika Huttunen"
#define MyAppURL "https://github.com/mphuttu/ThrowingApp"
#define MyAppExeName "ThrowingApp.exe"

[Setup]
AppId={{A1B2C3D4-E5F6-7890-ABCD-EF1234567890}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={autopf}\{#MyAppName}
DefaultGroupName={#MyAppName}
AllowNoIcons=yes
; Output setup executable to the project root
OutputDir=.\Setup
OutputBaseFilename=ThrowingAppSetup_{#MyAppVersion}
SetupIconFile=ThrowingApp\res\ThrowingAppIcon.ico
Compression=lzma
SolidCompression=yes
WizardStyle=modern
; Require 64-bit Windows
ArchitecturesInstallIn64BitMode=x64compatible

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
; Main executable (built in Release configuration)
Source: "ThrowingApp\Release\{#MyAppExeName}"; DestDir: "{app}"; Flags: ignoreversion

; Help files
Source: "Help\ThrowingApp.chm"; DestDir: "{app}\Help"; Flags: ignoreversion
Source: "Help\index.html"; DestDir: "{app}\Help"; Flags: ignoreversion
Source: "Help\about.html"; DestDir: "{app}\Help"; Flags: ignoreversion
Source: "Help\contact.html"; DestDir: "{app}\Help"; Flags: ignoreversion
Source: "Help\services.html"; DestDir: "{app}\Help"; Flags: ignoreversion
Source: "Help\stylesheets\default.css"; DestDir: "{app}\Help\stylesheets"; Flags: ignoreversion
Source: "Help\images\*"; DestDir: "{app}\Help\images"; Flags: ignoreversion recursesubdirs

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{group}\{cm:UninstallProgram,{#MyAppName}}"; Filename: "{uninstallexe}"
Name: "{autodesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

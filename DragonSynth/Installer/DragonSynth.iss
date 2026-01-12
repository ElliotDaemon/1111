; Dragon Synth Installer Script for Inno Setup
; Download Inno Setup from: https://jrsoftware.org/isinfo.php

#define MyAppName "Dragon Synth"
#define MyAppVersion "1.0.0"
#define MyAppPublisher "Dragon Audio"
#define MyAppURL "https://github.com/dragonaudio/dragonsynth"
#define MyAppExeName "Dragon Synth.exe"

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
AppId={{8F4E8B2A-5C3D-4E6F-9A1B-7D8E9F0A1B2C}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={autopf}\{#MyAppName}
DefaultGroupName={#MyAppName}
AllowNoIcons=yes
; Output settings
OutputDir=..\build\Installer
OutputBaseFilename=DragonSynth_v{#MyAppVersion}_Setup
; Compression
Compression=lzma2/ultra64
SolidCompression=yes
; Installer appearance
WizardStyle=modern
SetupIconFile=..\Resources\icon.ico
; Privileges - need admin for VST3 folder
PrivilegesRequired=admin
; Architecture
ArchitecturesAllowed=x64
ArchitecturesInstallIn64BitMode=x64

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Types]
Name: "full"; Description: "Full installation (VST3 + Standalone)"
Name: "vst3only"; Description: "VST3 Plugin only"
Name: "standalone"; Description: "Standalone application only"
Name: "custom"; Description: "Custom installation"; Flags: iscustom

[Components]
Name: "vst3"; Description: "VST3 Plugin"; Types: full vst3only custom
Name: "standalone"; Description: "Standalone Application"; Types: full standalone custom

[Files]
; VST3 Plugin - install to Common Files\VST3
Source: "..\build\DragonSynth_artefacts\Release\VST3\Dragon Synth.vst3\*"; DestDir: "{commoncf64}\VST3\Dragon Synth.vst3"; Components: vst3; Flags: ignoreversion recursesubdirs createallsubdirs

; Standalone Application
Source: "..\build\DragonSynth_artefacts\Release\Standalone\Dragon Synth.exe"; DestDir: "{app}"; Components: standalone; Flags: ignoreversion

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Components: standalone
Name: "{group}\{cm:UninstallProgram,{#MyAppName}}"; Filename: "{uninstallexe}"
Name: "{autodesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Components: standalone; Tasks: desktopicon

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Components: standalone; Flags: unchecked

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent; Components: standalone

[Messages]
WelcomeLabel2=This will install [name/ver] on your computer.%n%nDragon Synth is a powerful synthesizer with:%n%n• Dual oscillators with 5 waveforms%n• State Variable Filter%n• Dual ADSR envelopes%n• Beautiful dark dragon glass UI%n• Marble synthesizer keyboard%n• PC keyboard support%n%nClick Next to continue.

[Code]
procedure CurPageChanged(CurPageID: Integer);
begin
  if CurPageID = wpFinished then
  begin
    WizardForm.FinishedLabel.Caption := 
      'Dragon Synth has been installed successfully!' + #13#10 + #13#10 +
      'VST3 Plugin Location:' + #13#10 +
      '   C:\Program Files\Common Files\VST3\Dragon Synth.vst3' + #13#10 + #13#10 +
      'To use in your DAW:' + #13#10 +
      '   1. Open your DAW (FL Studio, Ableton, etc.)' + #13#10 +
      '   2. Rescan your VST3 plugins' + #13#10 +
      '   3. Find "Dragon Synth" in your plugin list' + #13#10 + #13#10 +
      'Enjoy making music with Dragon Synth!';
  end;
end;

[Setup]
AppName=Miner
AppVersion=0.1
OutputBaseFilename=MinerSetup
DefaultDirName={autopf}\Miner
DefaultGroupName=Miner

[Languages]
Name: "russian"; MessagesFile: "compiler:Languages\Russian.isl"

[Files]
Source: "cmake-build-release\vc_redist.x64.exe"; DestDir: {tmp}; Flags: deleteafterinstall
Source: "cmake-build-release\*"; DestDir: {app}; Excludes: "vc_redist*.exe,*.pdb,*.ilk,*.obj,*.lib"; Flags: recursesubdirs ignoreversion

[Run]
Filename: "{tmp}\vc_redist.x64.exe"
Filename: "{app}\miner.exe"
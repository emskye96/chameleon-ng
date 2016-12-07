# chameleon-ng

chameleon-ng is an extended version of the [Chameleon](https://github.com/aixxe/Chameleon) project, an internal skin & knife changer for Counter-Strike: Global Offensive. Like the original project, the code contains extensive comments intended for those new to game manipulation.

* Allows usage of skins on any weapon without restrictions.
* Includes primitive fix for special knife animations.
* Full in-game graphical interface for configuration.
* Network variable offsets are retrieved at runtime.
* Ability to override weapon kill feed icons.
* Persistent JSON configuration files.
* Button to instantly apply changes.

## Usage

Download the source code at the [Releases](https://github.com/emskye96/chameleon-ng/releases) page or clone the repository with submodules.

```
git clone --recursive https://github.com/emskye96/chameleon-ng.git
```

Compile with Visual Studio and inject the output DLL with your preferred injector. Note that usage of some public injectors can result in a Valve Anti-Cheat ban. It is highly recommended to use your own DLL injector.

Once injected, press the <kbd>Insert</kbd> key to open the configuration menu. Preset files are saved to the directory containing the chameleon-ng DLL file. Ensure that you append the '.cfg' extension when saving a new preset.

## Additional requirements

* [Microsoft DirectX 9 SDK](https://www.microsoft.com/en-gb/download/details.aspx?id=6812)
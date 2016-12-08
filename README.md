![chameleon-ng demonstration](https://cdn.aixxe.net/projects/chameleon-ng/ingame-demo.jpg)

### chameleon-ng

chameleon-ng is an extended version of the [Chameleon](https://github.com/aixxe/Chameleon) project, an internal skin & knife changer for Counter-Strike: Global Offensive. Like the original project, the code contains extensive comments intended for those new to game manipulation.

**Make sure to enable** `net_graph` **or** `cl_showfps` **if the menu is not drawing in-game.**

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

Once injected, press the <kbd>Insert</kbd> key to open the configuration menu.

Use the 'Items' header to set weapon skins and other related values. For example, to change a default CT knife to a Karambit you would open the 'Knife (Counter-Terrorists)' section, set the override item index to 507 and press apply. Refer to the [full list of skin IDs](https://github.com/adamb70/CSGO-skin-ID-dumper/blob/master/item_index.txt) for possible paint kit values and the [item definition index](https://github.com/emskye96/chameleon-ng/blob/master/src/ItemDefinitions.hpp) list for possible item overrides. The kill icon will automatically be set to the appropriate value based on the override item index.

To save a preset, scroll down to the 'Presets' header, type a filename followed by the '.cfg' extension and press New. After you have made additional changes to the preset you can press the 'Save' button to write your changes to the file.

To load a preset, simply click the 'Load' button below the specified preset. By default this will only overwrite skins specified in the preset. If you want to reset everything else while loading a preset make sure to check the 'Reset settings on load' box.
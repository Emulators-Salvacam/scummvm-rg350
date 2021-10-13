# Scummvm for the RG350

You can view the original ScummVM readme with tons of great general info [here](https://github.com/scummvm/scummvm)
Huge props to the scummvm dev crew for building an amazing engine.

## What is this?

Fork of the core scummvm project, with some modifications to make it function properly on a RG350 (will most likely work on other gcw0 hardware as well but I personally have not tested). This fork includes:

- smooth joystick support
- highres game support (including Blade Runner)
- fluidsynth audio option support (although it is defaulted to adlib as it is generally less CPU-intensive)
- support for games with mp3 music track playback (such as with Monkey Island 1 CD-ROM version)
- optional debug-mode for better more verbose logging
- CPU-savings on audio engine (bitrate changed to 48k)

This is still very much a WIP, and should be considered more of a preview until these changes get merged back into the upstream Scummvm project.

## How to install
Download newest [scummvm-rg350.opk release](https://github.com/craigsc/scummvm-rg350/releases) and place it at /media/sdcard/apps/scummvm-rg350.opk . Restart your device for good measure and you should see the Scummvm icon pop up under your emulators section.

## Controls
- Joystick/d-pad: mouse cursor movement
- A: left-mouse click
- B: right-mouse click
- X: ' ' - Either pauses or skips current line of text depending on game
- Y: '.' - skips current dialog line on several games
- Start: 'F5' - Scummvm pause menu
- Select: 'Esc' - skips cutscenes on many games
- L-trigger: 'Shift' - i'm not aware of a game that uses it lol
- R-trigger: show/hide virtual keyboard

## Notes on audio
Different games play better using various audio settings. In general, using Adlib emulation is suggested as it seems to be the fastest HOWEVER if you are having issues with either scratchy or out-of-sync audio then I recommend you change the game settings and select 'Fluidsynth' instead. Of note, this fixes a audio/video sync issue on Blade Runner that appears on the RG 350 when Adlib is used.

## Contributing
Generally Scummvm contributions should be made directly to the [main scummvm/scummvm repo](https://github.com/scummvm/scummvm). If you instead would like to contribute RG 350-specific logic and don't feel that the main scummvm line is the right place for that code, then feel free to make a pull request or reach out directly but keep in mind that this fork is mainly just a preview fork of code that is not yet stable enough for the main scummvm repo.


# fmod-gms2

This extension adds bindings to the FMOD Studio API (version 2.01.00) for Game Maker Studio 2. 
I originally wanted to update the bindings created by [mstop4](https://github.com/mstop4/FMODGMS), but decided to rewrite the extension, adding support for the Studio API and removing support for the low-level API. Thanks to mstop4 to the useful insight in creating a Game Maker extension.

This extension is work-in-progress, works with Windows and macOS in the editor (Run mode), but haven't tested on any final build. Pull requests are welcome! 😊

Note: FMOD is not free, visit https://www.fmod.com/licensing for more info about licensing.

## Table of Contents 

[Installation](#installation)

[Building](#bulding)

[Usage](#installation)

[Examples](#examples)

## Installation

1. Import the fmodgms2.ymps file as a local package to your Game Maker Studio project (Tools->Import Local Package. In the following import window, click on the Extension on the left and then on the Add button in the middle of the popup. Import the package.

2. A new folder named `fmodgms2` will automatically be created inside your `extensions` directory. 

3. Copy the required fmodgms2.dll or libfmodgms2.dylib file to that folder.

4. Download the FMOD Studio API (2.01.03 / x86) and add the release versions of fmodstudio.dll/libfmodstudio.dylib and fmod.dll/libfmod.dll to the fmodgms2 extensions folder.

## Building
This project uses SCons. Navigate to the folder containing the SConstruct file and run:
```
scons platform=windows target=release fmod_api="C:\Program Files (x86)\FMOD SoundSystem\FMOD Studio API Windows" 
```
Replace platform (windows, osx) and the fmod_api path with the path of your FMOD API installation.

## Usage

### System functions
```
FMOD_Sys_Create();
```
will create the Studio System object. Returns -1 if failed, 0 if succeded.

```
FMOD_Sys_Initialize(double outputType, double realChannels, double virtualChannels, double studioInitFlags);
```
will initialize the Studio System. Change Real and Virtual Channels to your liking. Use `FMOD_OUTPUTTYPE_AUTODETECT` for outputType. `studioInitFlags` can be:
- FMOD_STUDIO_INIT_NORMAL
- FMOD_STUDIO_INIT_LIVEUPDATE
- FMOD_STUDIO_INIT_SYNCHRONOUS_UPDATE 
- FMOD_STUDIO_INIT_MEMORY_TRACKING

Returns -1 if failed, 0 if succeded.

```
FMOD_Sys_Update();
```
To update the Studio System (call it in a step event).

```
FMOD_Sys_Close();
```
To unload and release the Studio System. Returns -1 if failed, 0 if succeded.

### Listener 

#### Setting num. of listeners
```
FMOD_SetNumListeners(double listener);
```

#### Setting listener position
```
FMOD_SetListener3DPosition(double listener, double posX, double posY, double posZ, double dirX, double dirY, double dirZ, double oX, double oY, double oZ)
```
`dirX` `dirY` `dirZ` is the forward vector, `oX` `oY` `oZ` the up vector.

### Banks

#### Loading a bank
```
FMOD_LoadBank(string bankPath, double flags);
```
`bankPath` is the path to the bank file. `flags` can be:

- FMOD_STUDIO_LOAD_BANK_NORMAL
- FMOD_STUDIO_LOAD_BANK_NONBLOCKING 
- FMOD_STUDIO_LOAD_BANK_DECOMPRESS_SAMPLES

Will return an index to the bank, use it to unload the bank if necessary:
```
FMOD_UnloadBank(double index);
```

### EventInstances

#### Create an instance
```
FMOD_CreateInstance(string eventPath);
```
returns an index to the EventInstance.

#### Starting an instance
```
FMOD_StartInstance(double index);
```
Returns -1 if failed, 0 if succeeded.

#### Stopping an instance
```
FMOD_StopInstance(double index, double stopMode);
```
`stopMode`  can be:

- FMOD_STUDIO_STOP_ALLOWFADEOUT
- FMOD_STUDIO_STOP_IMMEDIATE

#### Releasing an instance
```
FMOD_ReleaseInstance(double index);
```
Returns -1 if failed, 0 if succeeded. Will remove the instance from the index.

#### Setting the 3D Attributes on an instance
```
FMOD_Set3DAttributes(double index, double posX, double posY, double posZ, double dirX, double dirY, double dirZ, double oX, double oY, double oZ);
```

`dirX` `dirY` `dirZ` is the forward vector, `oX` `oY` `oZ` the up vector. Returns -1 if failed, 0 if succeeded.

#### Playing a simple 2D event (no spatializer)
```
FMOD_PlayOneShot2D(string eventPath);
```
Will automatically call EventInstance::release after starting. No need to take care of the instance.

#### Playing a simple 3D event 
```
FMOD_PlayOneShot3D(double index, double posX, double posY, double posZ, double dirX, double dirY, double dirZ, double oX, double oY, double oZ);
```
You need to create the instance before calling this. It will automatically call EventInstance::release after playing. `dirX` `dirY` `dirZ` is the forward vector, `oX` `oY` `oZ` the up vector. Returns -1 if failed, 0 if succeeded.

#### Setting the volume of an instance
```
FMOD_SetInstanceVolume(double index, double volume);
```
`volume` is 0-1.

#### Check if an instance is playing
```
FMOD_IsPlaying(double index);
```
Returns -1 if failed, 1 if the instance is playing, 0 if is not playing.

#### Set the paused state of an instance
```
FMOD_SetInstancePaused(double index, double pause);
```
`pause` should be 0 or 1;

#### Set the paused state of an instance
```
FMOD_SetInstancePaused(double index, double pause);
```
`pause` should be 0 or 1;

#### Set the pitch of an instance
```
FMOD_SetInstancePitch(double index, double pitch);
```

#### Get the timeline position of an instance
```
FMOD_GetTimelinePosition(double index);
```
Returns the timeline position.

#### Set the timeline position of an instance
```
FMOD_SetTimelinePosition(double index, double position);
```

#### Get the RMS value of a playing instance
```
FMOD_GetInstanceRMS(double index);
```
Returns the RMS value of an instance.

### Parameters

#### Get a global parameter value by name 
```
FMOD_GetGlobalParameterByName(string parameterName);
```
Takes the parameter name and returns the value.

#### Set a global parameter value by name 
```
FMOD_SetGlobalParameterByName(string parameterName, double value, double ignoreSeekSpeed);
```
Returns -1 if it failed, 0 if succeded.

#### Get a local parameter value by name 
```
FMOD_GetParameterByName(double instanceIndex, string parameterName);
```
Takes the instance index and the parameter name and returns its value.

#### Set a local parameter value by name 
```
FMOD_SetParameterByName(double instanceIndex, string parameterName, double value, double ignoreSeekSpeed);
```
Returns -1 if it failed, 0 if succeded.

### Busses

#### Getting a bus
```
FMOD_GetBus(string busPath);
```
Takes the bus path and returns an index to that bus. Use it to get or set the bus volume.

#### Get the bus volume
```
FMOD_GetBusVolume(double index);
```
Returns the bus volume.

#### Set the bus volume
```
FMOD_SetBusVolume(double index, double volume);
```
Returns -1 if failed, 0 if succeeded.

### VCAs

#### Getting a VCA
```
FMOD_GetVCA(string vcaPath);
```
Takes the VCA path and returns an index to that VCA. Use it to get or set the VCA volume.

#### Get the VCA volume
```
FMOD_GetVCAVolume(double index);
```
Returns the VCA volume.

#### Set the VCA volume
```
FMOD_SetVCAVolume(double index, double volume);
```
Returns -1 if failed, 0 if succeeded.

## Examples

### Initialising the Studio System
```
FMOD_Sys_Create();
FMOD_Sys_Initialize(FMOD_OUTPUTTYPE_AUTODETECT, 32, 128, FMOD_STUDIO_INIT_LIVEUPDATE);
```

### Loading the master banks
```
FMOD_LoadBank("Audio Banks/Desktop/Master.bank", FMOD_STUDIO_LOAD_BANK_NORMAL);
FMOD_LoadBank("Audio Banks/Desktop/Master.strings.bank", FMOD_STUDIO_LOAD_BANK_NORMAL);
```
The path on Windows is relative to the "Included" directory in GMS2's resources. Add the banks the included resources. Note: on macOS you have to add working_directory before the path. Also whitespaces will be removed and uppercase letter will be transformer to lowercase one's (?!). The same code would be:

```
FMOD_LoadBank(working_directory + "audiobanks/desktop/master.bank", FMOD_STUDIO_LOAD_BANK_NORMAL);
FMOD_LoadBank(working_directory + "audiobanks/desktop/master.strings.bank", FMOD_STUDIO_LOAD_BANK_NORMAL);
```

### Playing a 3d sound by manually managing instances
```
var instance = FMOD_CreateInstance("event:/Player/TestEvent");
FMOD_Set3DAttributes(instance obj_player.x, obj_player.y, 0, 0, 0, 1, 0, 1, 0);
FMOD_StartInstance(instance);
FMOD_ReleaseInstance(instance);
```

### Playing a 2d sound with PlayOneShot2D
```
FMOD_PlayOneShot2D("event:/Player/TestEvent");
```

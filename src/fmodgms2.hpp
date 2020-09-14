#ifndef FMODGMS2_HPP
#define FMODGMS2_HPP

#if !defined( _MSC_VER)
#define GMexport extern "C" __attribute__((visibility("default")))
#else
#define GMexport extern "C" __declspec (dllexport)
#endif

#include "fmod_studio.hpp"	
#include "fmod.hpp"
#include <unordered_map>

 struct Vector3 {
	 float x;
	 float y;
	 float z;
 };

// System Functions
GMexport double FMOD_Sys_Create();
GMexport double FMOD_Sys_Initialize(double outputType, double realChannels, double virtualChannels, double studioInitFlags);
GMexport double FMOD_Sys_Update();
GMexport double FMOD_Sys_Close();

// Listener
GMexport double FMOD_SetNumListeners(double listener);
GMexport double FMOD_SetListener3DPosition(double listener, double posX, double posY, double posZ, double dirX, double dirY, double dirZ, double oX, double oY, double oZ);

// Banks
GMexport double FMOD_LoadBank(char* bankName, double flags);
GMexport double FMOD_UnloadBank(double index);

// Event Instances
GMexport double FMOD_CreateInstance(char* eventName);
GMexport double FMOD_StartInstance(double index);
GMexport double FMOD_StopInstance(double index, double stopMode);
GMexport double FMOD_ReleaseInstance(double index);
GMexport double FMOD_Set3DAttributes(double index, double posX, double posY, double posZ, double dirX, double dirY, double dirZ, double oX, double oY, double oZ);
GMexport double FMOD_PlayOneShot2D(char* eventName);
GMexport double FMOD_PlayOneShot3D(double index, double posX, double posY, double posZ, double dirX, double dirY, double dirZ, double oX, double oY, double oZ);
GMexport double FMOD_SetInstanceVolume(double index, double volume);
GMexport double FMOD_GetPlaybackState(double index);
GMexport double FMOD_IsPlaying(double index);
GMexport double FMOD_SetInstancePaused(double index, double pause);
GMexport double FMOD_SetInstancePitch(double index, double pitch);
GMexport double FMOD_GetTimelinePosition(double index);
GMexport double FMOD_SetTimelinePosition(double index, double position);
GMexport double FMOD_GetInstanceRMS(double index);

// Parameters
GMexport double FMOD_GetGlobalParameterByName(char* parameterName);
GMexport double FMOD_SetGlobalParameterByName(char* parameterName, double value, double ignoreSeekSpeed);
GMexport double FMOD_GetParameterByName(double index, char* parameterName);
GMexport double FMOD_SetParameterByName(double index, char* parameterName, double value, double ignoreSeekSpeed);


// Busses
GMexport double FMOD_GetBus(char* busPath);
GMexport double FMOD_GetBusVolume(double index);
GMexport double FMOD_SetBusVolume(double index, double volume);

// VCA's
GMexport double FMOD_GetVCA(char* vcaPath);
GMexport double FMOD_GetVCAVolume(double index);
GMexport double FMOD_SetVCAVolume(double index, double volume);

// Utils
FMOD_3D_ATTRIBUTES To3DAttributes(const Vector3& vec, const Vector3& forward, const Vector3& up);
FMOD_VECTOR ToFMODVector(const Vector3& position);

#endif // FMODGMS2_HPP
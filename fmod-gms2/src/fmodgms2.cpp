/*--------------------------------------------------------
//  fmodgms2.cpp
//
//  FMODGMS2 v.0.1.0
//
//  GML bindings to the FMOD Studio API for
//  GameMaker Studio 2.
//
//  FMOD Studio version: 2.01.00
----------------------------------------------------------*/

#ifndef FMODGMS2_CPP
#define FMODGMS2_CPP

#include "fmodgms2.hpp"

FMOD::Studio::System* studioSystem = NULL;
FMOD::System* coreSystem = NULL;

std::unordered_map <std::size_t, FMOD::Studio::EventInstance*> instanceList;
std::size_t nInstances = 0;

std::unordered_map <std::size_t, FMOD::Studio::Bank*> bankList;
std::size_t nBanks = 0;

std::unordered_map <std::size_t, FMOD::Studio::Bus*> busList;
std::size_t nBusses = 0;

std::unordered_map <std::size_t, FMOD::Studio::VCA*> vcaList;
std::size_t nVCAs = 0;

#pragma region System Functions

GMexport double FMOD_Sys_Create()
{
	auto result = FMOD::Studio::System::create(&studioSystem);

	if (result != FMOD_OK)
	{
		return -1;
	}

	result = studioSystem->getCoreSystem(&coreSystem);

	if (result != FMOD_OK)
	{
		return -1;
	}

	return 0;
}

GMexport double FMOD_Sys_Initialize(double outputType, double realChannels, double virtualChannels, double studioInitFlags)
{

	auto result = coreSystem->setOutput((FMOD_OUTPUTTYPE)static_cast<int>(outputType));

	if (result != FMOD_OK)
	{
		return -1;
	}

	result = coreSystem->setSoftwareChannels(static_cast<int>(realChannels));

	if (result != FMOD_OK)
	{
		return -1;
	}

	result = studioSystem->initialize(static_cast<int>(virtualChannels), (FMOD_STUDIO_INITFLAGS)static_cast<int>(studioInitFlags), FMOD_INIT_NORMAL, NULL);

	if (result != FMOD_OK)
	{
		return -1;
	}

	return 0;
}

GMexport double FMOD_Sys_Update()
{
	auto result = studioSystem->update();
	if (result != FMOD_OK)
	{
		return -1;
	}

	return 0;
}

GMexport double FMOD_Sys_Close()
{
	auto result = studioSystem->unloadAll();
	if (result != FMOD_OK)
		return -1;

	result = studioSystem->release();
	if (result != FMOD_OK)
		return -1;

	return 0;
}

#pragma endregion

#pragma region Listener

GMexport double FMOD_SetNumListeners(double listener)
{
	int listeners = static_cast<int>(listener);
	auto result = studioSystem->setNumListeners(listeners);

	if (result != FMOD_OK)
	{
		return -1;
	}

	return 0;
}

GMexport double FMOD_SetListener3DPosition(double listener, double posX, double posY, double posZ, double dirX, double dirY, double dirZ, double oX, double oY, double oZ)
{
	int l = static_cast<int>(listener);

	Vector3 pos = { static_cast<float>(posX), static_cast<float>(posY), static_cast<float>(posZ) };
	Vector3 forward = { static_cast<float>(dirX), static_cast<float>(dirY), static_cast<float>(dirZ) };
	Vector3 up = { static_cast<float>(oX), static_cast<float>(oY), static_cast<float>(oZ) };

	FMOD_3D_ATTRIBUTES attributes = To3DAttributes(pos, forward, up);

	auto result = studioSystem->setListenerAttributes(l, &attributes);

	if (result != FMOD_OK)
	{
		return -1;
	}

	return 0;
}

#pragma endregion

#pragma region Banks

GMexport double FMOD_LoadBank(char* bankName, double flags)
{
	FMOD::Studio::Bank* bank;
	auto result = studioSystem->loadBankFile(bankName, (FMOD_STUDIO_LOAD_BANK_FLAGS)flags, &bank);

	if (result != FMOD_OK)
	{
		return -1;
	}

	if (bank) {
		bankList.emplace(nBanks++, bank);
		return nBanks - 1;
	}
	else return -1;
}

GMexport double FMOD_UnloadBank(double index)
{
	std::size_t i = (std::size_t)round(index);

	if (bankList.count(i) == 0)
	{
		return -1;
	}

	auto result = bankList[i]->unload();

	if (result != FMOD_OK) {
		return -1;
	}

	return 0;
}

#pragma endregion

#pragma region Event Instances

GMexport double FMOD_CreateInstance(char* eventName)
{
	FMOD::Studio::EventDescription* eventDescription = nullptr;
	auto result = studioSystem->getEvent(eventName, &eventDescription);

	if (result != FMOD_OK)
	{
		return -1;
	}

	if (eventDescription)
	{
		FMOD::Studio::EventInstance* eventInstance = nullptr;
		auto result = eventDescription->createInstance(&eventInstance);
		if (result == FMOD_OK)
		{
			instanceList.emplace(nInstances++, eventInstance);
			return nInstances - 1;
		}
		else return -1;

	}
	else return -1;
}

GMexport double FMOD_StartInstance(double index)
{
	std::size_t i = (std::size_t)round(index);

	if (instanceList.count(i) == 0)
	{
		return -1;
	}
	auto result = instanceList[i]->start();

	if (result != FMOD_OK)
	{
		return -1;
	}

	return 0;
}

GMexport double FMOD_StopInstance(double index, double stopMode)
{
	std::size_t i = (std::size_t)round(index);

	if (instanceList.count(i) == 1)
	{
		auto result = instanceList[i]->stop((FMOD_STUDIO_STOP_MODE)static_cast<int>(stopMode));

		if (result != FMOD_OK)
		{
			return -1;
		}
	}

	return 0;
}

GMexport double FMOD_ReleaseInstance(double index)
{
	std::size_t i = (std::size_t)round(index);

	if (instanceList.count(i) == 1)
	{
		auto result = instanceList[i]->release();

		if (result != FMOD_OK)
		{
			return -1;
		}

		instanceList.erase(i);
	}

	return 0;
}

GMexport double FMOD_Set3DAttributes(double index, double posX, double posY, double posZ, double dirX, double dirY, double dirZ, double oX, double oY, double oZ)
{
	std::size_t i = (std::size_t)round(index);

	if (instanceList.count(i) == 0)
	{
		return -1;
	}

	Vector3 pos = { static_cast<float>(posX), static_cast<float>(posY), static_cast<float>(posZ) };
	Vector3 forward = { static_cast<float>(dirX), static_cast<float>(dirY), static_cast<float>(dirZ) };
	Vector3 up = { static_cast<float>(oX), static_cast<float>(oY), static_cast<float>(oZ) };

	FMOD_3D_ATTRIBUTES attributes;
	attributes = To3DAttributes(pos, forward, up);

	auto result = instanceList[i]->set3DAttributes(&attributes);
	if (result != FMOD_OK)
	{
		return -1;
	}

	return 0;
}

GMexport double FMOD_PlayOneShot2D(char* eventName) {
	FMOD::Studio::EventDescription* eventDescription = NULL;
	auto result = studioSystem->getEvent(eventName, &eventDescription);

	if (result != FMOD_OK)
	{
		return -1;
	}

	if (eventDescription)
	{
		FMOD::Studio::EventInstance* eventInstance = NULL;
		auto result = eventDescription->createInstance(&eventInstance);

		if (result == FMOD_OK && eventInstance)
		{
			eventInstance->start();
			eventInstance->release();
			return 0;
		}
		else return -1;
	}
	else return -1;
}

GMexport double FMOD_PlayOneShot3D(double index, double posX, double posY, double posZ, double dirX, double dirY, double dirZ, double oX, double oY, double oZ)
{

	std::size_t i = (std::size_t)round(index);

	if (instanceList.count(i) == 0)
	{
		return -1;
	}

	FMOD::Studio::EventDescription* eventDescription = NULL;
	auto result = instanceList[i]->getDescription(&eventDescription);

	if (result != FMOD_OK)
	{
		return -1;
	}

	if (eventDescription)
	{
		bool is3D;
		eventDescription->is3D(&is3D);

		if (!is3D) {
			return -1;
		}

		Vector3 pos = { static_cast<float>(posX),  static_cast<float>(posY),  static_cast<float>(posZ) };
		Vector3 forward = { static_cast<float>(dirX), static_cast<float>(dirY), static_cast<float>(dirZ) };
		Vector3 up = { static_cast<float>(oX), static_cast<float>(oY), static_cast<float>(oZ) };
		FMOD_3D_ATTRIBUTES attributes = To3DAttributes(pos, forward, up);

		instanceList[i]->set3DAttributes(&attributes);
		instanceList[i]->start();
		instanceList[i]->release();

		instanceList.erase(i);
		return 0;
	}
	else return -1;
}

GMexport double FMOD_SetInstanceVolume(double index, double volume)
{
	std::size_t i = (std::size_t)round(index);

	if (instanceList.count(i) == 0)
	{
		return -1;
	}

	auto result = instanceList[i]->setVolume(static_cast<float>(volume));

	if (result != FMOD_OK)
	{
		return -1;
	}

	return 0;
}

GMexport double FMOD_GetPlaybackState(double index)
{
	std::size_t i = (std::size_t)round(index);

	if (instanceList.count(i) == 0)
	{
		return -1;
	}

	FMOD_STUDIO_PLAYBACK_STATE pS = FMOD_STUDIO_PLAYBACK_STOPPED;

	auto result = instanceList[i]->getPlaybackState(&pS);

	if (result != FMOD_OK)
	{
		return -1;
	}

	return static_cast<double>(pS);
}

GMexport double FMOD_IsPlaying(double index)
{
	std::size_t i = (std::size_t)round(index);

	if (instanceList.count(i) == 0)
	{
		return -1;
	}

	FMOD_STUDIO_PLAYBACK_STATE pS = FMOD_STUDIO_PLAYBACK_STOPPED;

	auto result = instanceList[i]->getPlaybackState(&pS);

	if (result != FMOD_OK)
	{
		return -1;
	}

	if (pS == FMOD_STUDIO_PLAYBACK_PLAYING) {
		return 1;
	}
	else return 0;
}

GMexport double FMOD_SetInstancePaused(double index, double pause)
{
	std::size_t i = (std::size_t)round(index);

	if (instanceList.count(i) == 0)
	{
		return -1;
	}

	auto result = instanceList[i]->setPaused(static_cast<int>(pause));

	if (result != FMOD_OK) {
		return -1;
	}

	return 0;
}

GMexport double FMOD_SetInstancePitch(double index, double pitch)
{
	std::size_t i = (std::size_t)round(index);

	if (instanceList.count(i) == 0)
	{
		return -1;
	}

	auto result = instanceList[i]->setPitch(static_cast<float>(pitch));

	if (result != FMOD_OK) {
		return -1;
	}

	return 0;
}

GMexport double FMOD_GetTimelinePosition(double index)
{
	std::size_t i = (std::size_t)round(index);

	if (instanceList.count(i) == 0)
	{
		return -1;
	}

	int position = 0;

	auto result = instanceList[i]->getTimelinePosition(&position);

	if (result != FMOD_OK) {
		return -1;
	}

	return static_cast<double>(position);
}

GMexport double FMOD_SetTimelinePosition(double index, double position)
{
	std::size_t i = (std::size_t)round(index);

	if (instanceList.count(i) == 0)
	{
		return -1;
	}

	auto result = instanceList[i]->setTimelinePosition(static_cast<int>(position));

	if (result != FMOD_OK) {
		return -1;
	}

	return 0;
}

GMexport double FMOD_GetInstanceRMS(double index)
{
	std::size_t i = (std::size_t)round(index);

	if (instanceList.count(i) == 0)
	{
		return -1;
	}

	FMOD::ChannelGroup* ChanGroup = nullptr;
	instanceList[i]->getChannelGroup(&ChanGroup);
	if (ChanGroup)
	{
		FMOD::DSP* ChanDSP = nullptr;
		ChanGroup->getDSP(0, &ChanDSP);
		if (ChanDSP)
		{
			ChanDSP->setMeteringEnabled(false, true);
			FMOD_DSP_METERING_INFO Info = {};
			ChanDSP->getMeteringInfo(nullptr, &Info);


			float rms = 0.f;

			for (int i = 0; i < Info.numchannels; i++)
			{
				rms += Info.rmslevel[i] * Info.rmslevel[i];
			}

			rms = std::sqrt(rms / (float)Info.numchannels);

			float dB = rms > 0 ? 20.0f * std::log10(rms * std::sqrt(2.0f)) : -80.0f;
			if (dB > 10.0f) dB = 10.0f;
			return static_cast<double>(dB);
		}
		else return -1;
	}
	else return -1;
}

#pragma endregion

#pragma region Parameters

GMexport double FMOD_GetGlobalParameterByName(char* parameterName)
{
	float value, finalValue;
	auto result = studioSystem->getParameterByName(parameterName, &value, &finalValue);

	if (result != FMOD_OK) {
		return -1;
	}

	return static_cast<double>(finalValue);

}

GMexport double FMOD_SetGlobalParameterByName(char* parameterName, double value, double ignoreSeekSpeed)
{
	auto result = studioSystem->setParameterByName(parameterName, static_cast<float>(value), static_cast<int>(ignoreSeekSpeed));

	if (result != FMOD_OK) {
		return -1;
	}

	return 0;
}

GMexport double FMOD_GetParameterByName(double index, char* parameterName)
{
	std::size_t i = (std::size_t)round(index);

	if (instanceList.count(i) == 0)
	{
		return -1;
	}

	float value = 0.f;
	float finalValue = 0.f;
	auto result = instanceList[i]->getParameterByName(parameterName, &value, &finalValue);

	if (result != FMOD_OK)
	{
		return -1;
	}

	return static_cast<double>(finalValue);
}

GMexport double FMOD_SetParameterByName(double index, char* parameterName, double value, double ignoreSeekSpeed)
{
	std::size_t i = (std::size_t)round(index);

	if (instanceList.count(i) == 0)
	{
		return -1;
	}

	auto result = instanceList[i]->setParameterByName(parameterName, static_cast<float>(value), static_cast<int>(ignoreSeekSpeed));

	if (result != FMOD_OK)
	{
		return -1;
	}

	return 0;
}

#pragma endregion

#pragma region Busses

GMexport double FMOD_GetBus(char* busPath)
{
	FMOD::Studio::Bus* bus = nullptr;
	auto result = studioSystem->getBus(busPath, &bus);
	if (result == FMOD_OK)
	{
		busList.emplace(nBusses++, bus);
		return nBusses - 1;
	}
	else return -1;
}

GMexport double FMOD_GetBusVolume(double index)
{
	std::size_t i = (std::size_t)round(index);

	if (busList.count(i) == 0)
	{
		return -1;
	}

	float volume, finalVolume;

	auto result = busList[i]->getVolume(&volume, &finalVolume);

	if (result != FMOD_OK) {
		return -1;
	}
	else return static_cast<double>(finalVolume);
}

GMexport double FMOD_SetBusVolume(double index, double volume)
{
	std::size_t i = (std::size_t)round(index);

	if (busList.count(i) == 0)
	{
		return -1;
	}

	auto result = busList[i]->setVolume(static_cast<float>(volume));

	if (result != FMOD_OK) {
		return -1;
	}
	else return 0;
}

#pragma endregion

#pragma region VCAs

GMexport double FMOD_GetVCA(char* vcaPath)
{
	FMOD::Studio::VCA* vca = nullptr;
	auto result = studioSystem->getVCA(vcaPath, &vca);
	if (result == FMOD_OK)
	{
		vcaList.emplace(nVCAs++, vca);
		return nVCAs - 1;
	}
	else return -1;
}

GMexport double FMOD_GetVCAVolume(double index)
{
	std::size_t i = (std::size_t)round(index);

	if (vcaList.count(i) == 0)
	{
		return -1;
	}

	float volume, finalVolume;

	auto result = vcaList[i]->getVolume(&volume, &finalVolume);

	if (result != FMOD_OK) {
		return -1;
	}
	else return static_cast<double>(finalVolume);
}

GMexport double FMOD_SetVCAVolume(double index, double volume)
{
	std::size_t i = (std::size_t)round(index);

	if (vcaList.count(i) == 0)
	{
		return -1;
	}

	auto result = vcaList[i]->setVolume(static_cast<float>(volume));

	if (result != FMOD_OK) {
		return -1;
	}
	else return 0;
}

#pragma endregion

#pragma region Utils

FMOD_3D_ATTRIBUTES To3DAttributes(const Vector3& vec, const Vector3& forward, const Vector3& up)
{
	FMOD_3D_ATTRIBUTES attributes;
	attributes.forward = ToFMODVector(forward);
	attributes.up = ToFMODVector(up);
	attributes.position = ToFMODVector(vec);

	return attributes;
}

FMOD_VECTOR ToFMODVector(const Vector3& position) {
	FMOD_VECTOR fVec;
	fVec.x = position.x;
	fVec.y = position.y;
	fVec.z = position.z;
	return fVec;
}

#pragma endregion

#endif //FMODGMS2_CPP

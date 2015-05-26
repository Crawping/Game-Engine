#include"precompiled.h"
#include "AudioSystem.h"
#include<assert.h>


AudioSystem::AudioSystem(void) : m_system(nullptr), m_sfx_channel(nullptr), m_ambient_channel(nullptr), m_music_channel(nullptr)
{

}
AudioSystem::~AudioSystem(void){}

bool AudioSystem::Init(){
	m_distance_factor = 1.0f;		// Units per meter.  I.e feet would = 3.28.  centimeters would = 100.
	FMOD_RESULT result = FMOD::System_Create(&m_system);
	if (!ErrorCheck(result)) return false;

	unsigned int version = 0;
	result = m_system->getVersion(&version);
	if (!ErrorCheck(result)) return false;
	if (version < FMOD_VERSION){
		assert(false);
		return false;
	}
	int numdrivers = 0;
	result = m_system->getNumDrivers(&numdrivers);
	if (!ErrorCheck(result)) return false;
	if (numdrivers == 0){
		result = m_system->setOutput(FMOD_OUTPUTTYPE_NOSOUND);
		if (!ErrorCheck(result)) return false;
	}
	else {
		FMOD_SPEAKERMODE speakermode;
		FMOD_CAPS caps;
		result = m_system->getDriverCaps(0, &caps, 0, &speakermode);
		if (!ErrorCheck(result)) return false;
		result = m_system->setSpeakerMode(speakermode);
		if (!ErrorCheck(result)) return false;
		if (caps & FMOD_CAPS_HARDWARE_EMULATED){
			result = m_system->setDSPBufferSize(1024, 10);
			if (!ErrorCheck(result)) return false;
		}
		char name[256];
		result = m_system->getDriverInfo(0, name, 256, 0);
		if (!ErrorCheck(result)) return false;
		if (strstr(name, "SigmaTel")){
			result = m_system->setSoftwareFormat(48000, FMOD_SOUND_FORMAT_PCMFLOAT, 0, 0, FMOD_DSP_RESAMPLER_LINEAR);
			if (!ErrorCheck(result)) return false;
		}
	}
	result = m_system->init(100, FMOD_INIT_NORMAL, 0);
	if (result == FMOD_ERR_OUTPUT_CREATEBUFFER){
		result = m_system->setSpeakerMode(FMOD_SPEAKERMODE_STEREO);
		if (!ErrorCheck(result)) return false;
		result = m_system->init(100, FMOD_INIT_NORMAL, 0);
		if (!ErrorCheck(result)) return false;
	}
	result = m_system->set3DSettings(1.0f, m_distance_factor, 1.0f);
	if (!ErrorCheck(result)) return false;

	//float a, b, c;
	//m_system->get3DSettings(&a, &b, &c);
	//m_system->set3DSettings(10000.0f, 1000000.0f, 1.0f);


	return true;
}

void AudioSystem::Shut(){
	if (m_system != nullptr){
		FMOD_RESULT result;
		result = m_system->close();
		ErrorCheck(result);
		result = m_system->release();
		ErrorCheck(result);
	}
}

bool AudioSystem::ErrorCheck(FMOD_RESULT result){
	if (result != FMOD_OK){
		assert(false);
		return false;
	}
	return true;
}

void AudioSystem::UpdatePosition(float x, float y, float z){
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
	FMOD_RESULT result = m_system->set3DListenerAttributes(0, &m_position, 0, 0, 0);
	ErrorCheck(result);
}

void AudioSystem::Update(){
	m_system->update();
}

bool AudioSystem::Load3DSound(const string& filepath, FMOD::Sound** sound){
	FMOD_RESULT result = m_system->createSound(filepath.c_str(), FMOD_3D, 0, &*sound);
	if (!ErrorCheck(result)) return false;
	result = (*sound)->set3DMinMaxDistance(0.5f * m_distance_factor, 8.0f * m_distance_factor);
	if (!ErrorCheck(result)) return false;
	result = (*sound)->setMode(FMOD_LOOP_OFF);
	if (!ErrorCheck(result)) return false;

	return true;
}

bool AudioSystem::Load2DSound(const string& filepath, FMOD::Sound** sound){
	FMOD_RESULT result = m_system->createSound(filepath.c_str(), FMOD_2D, 0, &*sound);
	if (!ErrorCheck(result)) return false;
	result = (*sound)->setMode(FMOD_LOOP_OFF);
	if (!ErrorCheck(result)) return false;
	return true;
}

bool AudioSystem::LoadSoundFromStream(const string& filepath, FMOD::Sound** sound){
	FMOD_RESULT result = m_system->createStream(filepath.c_str(), FMOD_HARDWARE | FMOD_2D, 0, &*sound);
	if (!ErrorCheck(result)) return false;
	return true;
}

void AudioSystem::Play3DSound(FMOD::Sound* sound, const FMOD_VECTOR& position, FMOD::Channel** channel){
	FMOD_RESULT result = m_system->playSound(FMOD_CHANNEL_FREE, sound, true, &*channel);
	ErrorCheck(result);
	result = (*channel)->set3DAttributes(&position, 0);
	ErrorCheck(result);
	result = (*channel)->setPaused(false);
	ErrorCheck(result);
}

void AudioSystem::Play2DSound(FMOD::Sound* sound, FMOD::Channel** channel){
	FMOD_RESULT result = m_system->playSound(FMOD_CHANNEL_FREE, sound, false, &*channel);
	ErrorCheck(result);
}

void AudioSystem::PlayStreamingSound(FMOD::Sound* sound, FMOD::Channel** channel){
	FMOD_RESULT result = m_system->playSound(FMOD_CHANNEL_FREE, sound, false, &*channel);
	ErrorCheck(result);
}

void AudioSystem::StopSound(FMOD::Sound* sound){

}

void AudioSystem::StopMusic(FMOD::Sound* sound){

}

void AudioSystem::PauseSound(FMOD::Sound* sound, FMOD::Channel** channel){
	bool pause = false;
	(*channel)->getPaused(&pause);
	(*channel)->setPaused(!pause);
}

void AudioSystem::PauseMusic(FMOD::Sound* sound, FMOD::Channel** channel){
	bool pause = false;
	(*channel)->getPaused(&pause);
	(*channel)->setPaused(!pause);
}

void AudioSystem::SetLoop(FMOD::Sound* sound, bool value){
	int loop = 0;
	if (value){
		loop = FMOD_LOOP_NORMAL;
	}
	else {
		loop = FMOD_LOOP_OFF;
	}
	sound->setMode(loop);
}
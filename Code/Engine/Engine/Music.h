#pragma once
#include"precompiled.h"
#include "AudioSystemPrereqs.h"

class Music : public IAudio{
public:
	Music(AudioSystem* p_system);
	~Music(void);

	bool Load(const string& filepath);	// Set Sound type 3D or 2D
	void Play();
	void Stop();
	void Pause();
	void SetLoop(bool value);
	void SetVolume(float volume);
private:
	FMOD::Sound* m_sound;
	FMOD::Channel* m_channel;
	float m_volume;
};
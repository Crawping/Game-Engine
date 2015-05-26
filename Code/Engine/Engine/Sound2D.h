#pragma once
#include"precompiled.h"
#include "AudioSystemPrereqs.h"

class Sound2D : public IAudio{
public:
	Sound2D(AudioSystem* p_system);
	~Sound2D(void);

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

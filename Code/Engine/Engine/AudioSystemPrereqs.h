#pragma once
#include<string>
#include "../../External/FMOD/fmod.hpp"
#include "../../External/FMOD/fmod_errors.h"
using namespace std;
class AudioSystem;
class IAudio{
public:
	virtual ~IAudio(void){}
	IAudio(AudioSystem* p_system)
	{
		s_system = p_system;
	}

	virtual bool Load(const string& filepath) = 0;
	virtual void Play() = 0;
	virtual void Pause() = 0;
	virtual void Stop() = 0;
	virtual void SetLoop(bool value) = 0;

private:
	static void SetAudioSystem(AudioSystem* p_system) {
		s_system = p_system;
	}
protected:
	static AudioSystem* s_system;
};

class SoundListener{
public:
	SoundListener(void){}
	virtual ~SoundListener(void){}
	virtual void UpdatePosition(float x, float y, float z) = 0;
protected:
	FMOD_VECTOR m_position;
};
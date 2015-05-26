#pragma once
#include"precompiled.h"
#include"Vector3.h"
#include "AudioSystemPrereqs.h"

using namespace Engine;
class Sound3D : public IAudio{
public:
	Sound3D(AudioSystem* p_system);
	~Sound3D(void);

	bool Load(const string& filepath);	// Set Sound type 3D or 2D
	void Play();
	void Stop();
	void Pause();
	void SetLoop(bool value);
	void SetVolume(float volume);
	void SetPosition(const Vector3& position)	{ m_position.x = position.getx(); m_position.y = position.gety(); m_position.z = position.getz(); }
	void SetPosition(float x, float y, float z)	{
		m_position.x = x; m_position.y = y; m_position.z = z;
		(m_channel)->set3DAttributes(&m_position, 0);
	}
	float getVolume(){ return m_volume; }
	void SetMinMaxDistance(float min, float max);
private:
	FMOD::Sound* m_sound;
	FMOD::Channel* m_channel;
	FMOD_VECTOR m_position;
	float m_volume;
};



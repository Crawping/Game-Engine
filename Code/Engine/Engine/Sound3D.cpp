#include"precompiled.h"
#include "Sound3D.h"
#include "AudioSystem.h"

Sound3D::Sound3D(AudioSystem* p_system) : IAudio(p_system), m_sound(nullptr), m_volume(1.0f) {}
Sound3D::~Sound3D(void){
	if (m_sound) { m_sound->release(); }
}

bool Sound3D::Load(const string& filepath){
	if (!s_system->Load3DSound(filepath, &m_sound)) {
		return false;
	}
	return true;
}

void Sound3D::Play(){
	s_system->Play3DSound(m_sound, m_position, &m_channel);
	m_channel->setVolume(m_volume);

}

void Sound3D::Stop(){
	s_system->StopSound(m_sound);
}

void Sound3D::Pause(){
	s_system->PauseSound(m_sound, &m_channel);
}

void Sound3D::SetLoop(bool value){
	s_system->SetLoop(m_sound, value);
}

void Sound3D::SetVolume(float volume){
	m_volume = volume;
	m_channel->setVolume(volume);
}

void Sound3D::SetMinMaxDistance(float min, float max){
	float prevMin;
	float prevMax;
	m_sound->get3DMinMaxDistance(&prevMin, &prevMax);
	m_sound->set3DMinMaxDistance(min, max);

}
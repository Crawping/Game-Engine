#include"precompiled.h"
#include "Music.h"
#include "AudioSystem.h"

Music::Music(AudioSystem* p_system) : IAudio(p_system), m_sound(nullptr), m_volume(1.0f) {}
Music::~Music(void){
	if (m_sound) { m_sound->release(); }
}

bool Music::Load(const string& filepath){
	if (!s_system->LoadSoundFromStream(filepath, &m_sound)) {
		return false;
	}

	return true;
}

void Music::Play(){
	s_system->Play2DSound(m_sound, &m_channel);
	m_channel->setVolume(m_volume);
}

void Music::Stop(){
	s_system->StopSound(m_sound);
}

void Music::Pause(){
	s_system->PauseMusic(m_sound, &m_channel);
}

void Music::SetLoop(bool value){
	s_system->SetLoop(m_sound, value);
}

void Music::SetVolume(float volume){
	m_volume = volume;
	m_channel->setVolume(volume);
}
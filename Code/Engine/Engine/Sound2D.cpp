#include"precompiled.h"
#include "Sound2D.h"
#include "AudioSystem.h"

Sound2D::Sound2D(AudioSystem* p_system) : IAudio(p_system), m_sound(nullptr), m_volume(1.0f) {}
Sound2D::~Sound2D(void){
	if (m_sound) { m_sound->release(); }
}

bool Sound2D::Load(const string& filepath){
	if (!s_system->Load2DSound(filepath, &m_sound)) {
		return false;
	}
	return true;
}

void Sound2D::Play(){
	s_system->Play2DSound(m_sound, &m_channel);
	m_channel->setVolume(m_volume);
}

void Sound2D::Stop(){

}

void Sound2D::Pause(){
	s_system->PauseSound(m_sound, &m_channel);
}

void Sound2D::SetLoop(bool value){
	s_system->SetLoop(m_sound, value);
}

void Sound2D::SetVolume(float volume){
	m_volume = volume;
	if (m_channel != nullptr){
		m_channel->setVolume(m_volume);
	}
}
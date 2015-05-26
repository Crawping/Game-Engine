#pragma once
#include "AudioSystemPrereqs.h"

class AudioSystem : public SoundListener
{
public:
	AudioSystem(void);
	~AudioSystem(void);

	bool Init();
	void Shut();
	void UpdatePosition(float x, float y, float z);
	void Update();

	bool Load3DSound(const string& filepath, FMOD::Sound** sound);
	bool Load2DSound(const string& filepath, FMOD::Sound** sound);
	bool LoadSoundFromStream(const string& filepath, FMOD::Sound** sound);
	void Play3DSound(FMOD::Sound* sound, const FMOD_VECTOR& position, FMOD::Channel** channel);
	void Play2DSound(FMOD::Sound* sound, FMOD::Channel** channel);
	void PlayStreamingSound(FMOD::Sound* sound, FMOD::Channel** channel);
	void StopSound(FMOD::Sound* sound);
	void StopMusic(FMOD::Sound* sound);
	void PauseSound(FMOD::Sound* sound, FMOD::Channel** channel);
	void PauseMusic(FMOD::Sound* sound, FMOD::Channel** channel);
	void SetLoop(FMOD::Sound* sound, bool value);
	static AudioSystem* GetInstance()
	{
		static AudioSystem* objectInstance;
		if (objectInstance == nullptr)
		{
			objectInstance = new AudioSystem;
			objectInstance->Init();
		}
		return objectInstance;

	}
private:
	bool ErrorCheck(FMOD_RESULT result);
	FMOD::Channel* m_sfx_channel;
	FMOD::Channel* m_ambient_channel;
	FMOD::Channel* m_music_channel;
	FMOD::System* m_system;
	float m_distance_factor;
};


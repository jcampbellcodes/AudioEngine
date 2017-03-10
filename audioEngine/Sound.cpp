#include "Sound.h"
#include "SoundManager.h"
#include "AzulCore.h"
#include "VoiceManager.h"
#include "BufferManager.h"
#include "CallbackCommand.h"
#include "Track.h"


Sound::Sound(WAVEFORMATEXTENSIBLE _wfx, XAUDIO2_BUFFER* _buffer) : wfx(_wfx), buffer(*_buffer), track(nullptr)
{
	lengthInBytes = buffer.AudioBytes;
	startPosition = 0;
	playing = false;
}

Sound::~Sound()
{
	//SoundManager::Remove(this);

	if (track)
	{
		// return voice to voice manager
		//VoiceManager::ReturnVoice(track, wfx);
		delete track;
	}
	DebugMsg::out("sound destroyed\n");
}

snd_err Sound::Play()
{
	if (track)
	{

		//track->voice->Start(startPosition);
		track->voice->Start();
		playing = true;
		return snd_err::OK;
	}

	return snd_err::NO_TRACK;
}

snd_err Sound::Stop()
{
	if (track)
	{
		track->voice->Stop();
		playing = false;
		//track->voice->FlushSourceBuffers();
		return snd_err::OK;
	}

	return snd_err::NO_TRACK;
}

snd_err Sound::Pause()
{
	if (track)
	{
		track->voice->Stop();
		playing = false;
		//track->voice->get
		return snd_err::OK;
	}

	return snd_err::NO_TRACK;
}

snd_err Sound::Pan(float pan)
{
	if (track)
	{
		// doesnt work now

		pan;

		return snd_err::OK;
	}

	return snd_err::NO_TRACK;
}

snd_err Sound::Pitch(float pitch)
{
	if (track)
	{
		track->voice->SetFrequencyRatio(pitch);
		return snd_err::OK;
	}

	return snd_err::NO_TRACK;
}

snd_err Sound::Volume(float volume)
{
	if (track)
	{
		track->voice->SetVolume(volume);
		return snd_err::OK;
	}

	return snd_err::NO_TRACK;
}

snd_err Sound::Loop(bool toLoop)
{
	if (track)
	{
		track->voice->FlushSourceBuffers();
		if (toLoop)
		{
			buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
		}
		else
		{
			track->voice->ExitLoop();
			buffer.LoopCount = 0;
		}
		return snd_err::OK;
	}

	return snd_err::NO_TRACK;
}

snd_err Sound::SubmitCallback(CallbackCommand * cmd)
{
	snd_err err = snd_err::OK;
	if (track)
	{
		if (cmd)
		{
			track->cbMailbox->SubmitCallback(cmd);
		}
		else
		{
			err = snd_err::NULLPTR;
		}
	}
	else
	{
		this->cmdStack.push(cmd);
		err = snd_err::NO_TRACK;
	}
	return err;
}

snd_err Sound::SendToTrack(int priority)
{
	snd_err status = snd_err::OK;
	if (!track)
	{
		status = VoiceManager::GetVoice(this->track, wfx, priority, this);

		// now that we have a voice and are being prepared to play, we have somewhere to send commands that were added at weird times
		if (status == snd_err::OK)
		{
			track->voice->SubmitSourceBuffer(&buffer);
			while (!this->cmdStack.empty())
			{
				this->SubmitCallback(cmdStack.top());
				this->cmdStack.pop();
			}
		}
	}

	return status;
}

snd_err Sound::AddToChannel(Channel * channel)
{
	if (track)
	{
		XAUDIO2_SEND_DESCRIPTOR sendDesc = { 0,channel->GetSubmixVoice() };
		XAUDIO2_VOICE_SENDS sendList = { 1, &sendDesc };
		this->track->voice->SetOutputVoices(&sendList);
	}
	else
	{
		return snd_err::NO_TRACK;
	}

	return snd_err::OK;
}

snd_err Sound::ClearTrack()
{
	snd_err err = snd_err::OK;
	if (track)
	{
		this->track = nullptr;
	}
	else
	{
		err = snd_err::NO_TRACK;
	}
	return err;
}

bool Sound::IsPlaying()
{
	return playing;
}

unsigned int Sound::LengthInBytes()
{
	return lengthInBytes;
}

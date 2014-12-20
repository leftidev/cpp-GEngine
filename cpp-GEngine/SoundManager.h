#pragma once

#include <map>
#include <string>

#include <SDL/SDL_mixer.h>


namespace GEngine {
	class SoundEffect {
	public:
		friend class SoundManager;

		// Plays the effect file
		// @param loops: If loops == -1, loop forever,
		// otherwise play it loops+1 times
		void play(int loops = 0);

	private:
		Mix_Chunk* m_chunk = nullptr;
	};

	class Music {
	public:
		friend class SoundManager;

		// Plays the music file
		// @param loops: If loops == -1, loop forever,
		// otherwise play it loops times
		void play(int loops = 1);

		static void pause();
		static void stop();
		static void resume();
	private:
		Mix_Music* m_music = nullptr;
	};

	class SoundManager {
	public:
		SoundManager();
		~SoundManager();

		void init();
		void destroy();

		SoundEffect loadSoundEffect(const std::string& filePath);
		Music loadMusic(const std::string& filePath);

	private:
		std::map<std::string, Mix_Chunk*> m_effectMap; // Effects cache
		std::map<std::string, Mix_Music*> m_musicMap; // Musics cache

		bool m_isInitialized = false;
	};
}
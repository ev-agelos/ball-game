#include <map>
#include <string>

#include "raylib.h"

#include "sounds.hpp"

extern bool IS_GOAL;
extern bool PAUSE;
std::map<std::string, Sound> SOUNDS;
Music BG_MUSIC;


void load_sounds()
{
    InitAudioDevice();

    std::pair<std::string, std::pair <std::string, float>> sound_files[3] = {
        std::make_pair("crowd", std::make_pair("media/sounds/crowd.wav", 0.3)),
        std::make_pair("crowd_cheering", std::make_pair("media/sounds/crowd.wav", 0.8f)),
        std::make_pair("ball_sound", std::make_pair("media/sounds/ballsound.wav", 1.f))
    };

    for (const auto& s : sound_files)
    {
        Sound sound = LoadSound(s.second.first.c_str());
        SetSoundVolume(sound, s.second.second);
        SOUNDS.insert({s.first, sound});
    }

    BG_MUSIC = LoadMusicStream("media/sounds/bg_music.mp3");
    PlayMusicStream(BG_MUSIC);
}


void unload_sounds()
{
    for (auto& s : SOUNDS)
        UnloadSound(s.second);
    UnloadMusicStream(BG_MUSIC);

    CloseAudioDevice();
}


void play_crowd_sound()
{
    if (not IsSoundPlaying(SOUNDS["crowd"]))
        PlaySound(SOUNDS["crowd"]);
}


void play_ball_sound()
{
    PlaySound(SOUNDS["ball_sound"]);
}


void play_sounds()
{
    if (IsMusicPlaying(BG_MUSIC))
        UpdateMusicStream(BG_MUSIC);

    if (not PAUSE)
        play_crowd_sound();

    if (IS_GOAL)
        PlaySound(SOUNDS["crowd_cheering"]);
}

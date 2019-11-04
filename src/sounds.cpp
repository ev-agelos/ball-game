#include <map>
#include <string>

#include "raylib.h"
#include "sounds.h"

std::map<std::string, Sound> SOUNDS;


void load_sounds()
{
    InitAudioDevice();

    std::pair<std::string, std::pair <std::string, float>> sound_files[3] = {
        std::make_pair("crowd", std::make_pair("media/sounds/crowd01.wav", 0.05)),
        std::make_pair("crowd_cheering", std::make_pair("media/sounds/crowd02.wav", 0.05)),
        std::make_pair("ball_sound", std::make_pair("media/sounds/ballsound.wav", 1.f))
    };

    for (const auto& s : sound_files)
    {
        Sound sound = LoadSound(s.second.first.c_str());
        SetSoundVolume(sound, s.second.second);
        SOUNDS.insert({s.first, sound});
    }

}


void unload_sounds()
{
    for (auto& s : SOUNDS)
        UnloadSound(s.second);

    CloseAudioDevice();
}


void play_crowd_sound()
{
    if (!IsSoundPlaying(SOUNDS["crowd"]))
        PlaySound(SOUNDS["crowd"]);
}

void play_crowd_cheering()
{
    PlaySound(SOUNDS["crowd_cheering"]);
}


void play_ball_sound()
{
    PlaySound(SOUNDS["ball_sound"]);
}

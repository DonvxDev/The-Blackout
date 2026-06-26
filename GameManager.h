#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "Audio/AudioManager.h"
#include "Maps/Map.h"
#include "Player/PlayerControll.h"
#include "Player/PlayerDebugger.h"
#include "Player/PlayerMovement.h"
#include <iostream>

class GameManager
{
public:
    GameManager()
        : activeMap(GameMap::CreateExampleMap("Unknown"))
        , stepSoundCooldown(0.0f)
    {
        RegisterDefaultSounds();
    }

    void LoadMap(const GameMap& map)
    {
        activeMap = map;
        audioManager.PlaySound(SoundEffectType::Ambient);
        std::cout << "[Game] Loaded map: " << activeMap.GetDescription() << "\n";
        std::cout << "[Game] Layers: " << activeMap.GetLayerSummary() << "\n";
    }

    void SetPlayerInput(bool w, bool a, bool s, bool d)
    {
        controll.SetWASDState(w, a, s, d);
    }

    void Update(float deltaTime)
    {
        movement.Update(deltaTime, controll);
        PlayStepSound(deltaTime);
    }

    void DebugFrame() const
    {
        debugger.PrintDebugState(movement, controll);
        std::cout << "Map: " << activeMap.GetDescription() << "\n";
        std::cout << "Layers: " << activeMap.GetLayerSummary() << "\n";
        std::cout << "-------------------------\n";
    }

    AudioManager& GetAudioManager()
    {
        return audioManager;
    }

    PlayerControll& GetControl()
    {
        return controll;
    }

private:
    void RegisterDefaultSounds()
    {
        audioManager.RegisterSound(SoundEffectType::Footstep, SoundEffect("Footstep", "Fx/Sounds/footstep.wav", 0.7f));
        audioManager.RegisterSound(SoundEffectType::Jump, SoundEffect("Jump", "Fx/Sounds/jump.wav", 0.8f));
        audioManager.RegisterSound(SoundEffectType::Attack, SoundEffect("Attack", "Fx/Sounds/attack.wav", 1.0f));
        audioManager.RegisterSound(SoundEffectType::Damage, SoundEffect("Damage", "Fx/Sounds/damage.wav", 0.9f));
        audioManager.RegisterSound(SoundEffectType::Explosion, SoundEffect("Explosion", "Fx/Sounds/explosion.wav", 1.0f));
        audioManager.RegisterSound(SoundEffectType::Ambient, SoundEffect("Ambient Loop", "Fx/Sounds/ambient_loop.wav", 0.5f, true));
    }

    void PlayStepSound(float deltaTime)
    {
        if (controll.IsMoving())
        {
            stepSoundCooldown -= deltaTime;
            if (stepSoundCooldown <= 0.0f)
            {
                audioManager.PlaySound(SoundEffectType::Footstep);
                stepSoundCooldown = 0.28f;
            }
        }
        else
        {
            stepSoundCooldown = 0.0f;
        }
    }

    GameMap activeMap;
    PlayerControll controll;
    PlayerMovement movement;
    PlayerDebugger debugger;
    AudioManager audioManager;
    float stepSoundCooldown;
};

#endif // GAME_MANAGER_H

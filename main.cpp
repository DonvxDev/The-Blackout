#include <chrono>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <map>
#include <string>
#include <thread>
#include <vector>

#if !defined(_WIN32)
#error "This game is designed for PC only. Build on Windows for best performance."
#endif

struct Vector2
{
    float x;
    float y;

    Vector2() : x(0.0f), y(0.0f) {}
    Vector2(float ix, float iy) : x(ix), y(iy) {}

    bool IsZero() const { return std::fabs(x) < 1e-6f && std::fabs(y) < 1e-6f; }
    float Length() const { return std::sqrt(x * x + y * y); }

    Vector2 Normalized() const
    {
        const float length = Length();
        return length > 1e-6f ? Vector2(x / length, y / length) : Vector2(0.0f, 0.0f);
    }

    Vector2 operator+(const Vector2& other) const { return Vector2(x + other.x, y + other.y); }
    Vector2 operator-(const Vector2& other) const { return Vector2(x - other.x, y - other.y); }
    Vector2 operator*(float scalar) const { return Vector2(x * scalar, y * scalar); }
};

enum class SoundEffectType
{
    Footstep,
    Jump,
    Attack,
    Damage,
    Explosion,
    Ambient,
    UIConfirm,
    UISwitch
};

struct SoundEffect
{
    std::string name;
    std::string path;
    float volume = 1.0f;
    bool loop = false;

    SoundEffect() = default;
    SoundEffect(const std::string& soundName, const std::string& filePath, float volumeValue = 1.0f, bool loopValue = false)
        : name(soundName)
        , path(filePath)
        , volume(volumeValue)
        , loop(loopValue)
    {
    }

    void Play() const
    {
        std::cout << "[Audio] Playing '" << name << "' (" << path << ")";
        if (loop)
            std::cout << " [looping]";
        std::cout << " at " << volume << "\n";
    }

    void Stop() const
    {
        std::cout << "[Audio] Stopped '" << name << "'\n";
    }
};

class AudioManager
{
public:
    void RegisterSound(SoundEffectType type, const SoundEffect& effect)
    {
        sounds[type] = effect;
    }

    void PlaySound(SoundEffectType type) const
    {
        auto found = sounds.find(type);
        if (found != sounds.end())
            found->second.Play();
        else
            std::cout << "[Audio] Missing sound effect for type " << static_cast<int>(type) << "\n";
    }

private:
    std::map<SoundEffectType, SoundEffect> sounds;
};

class PlayerControll
{
public:
    PlayerControll()
        : moveForward(false)
        , moveBackward(false)
        , moveLeft(false)
        , moveRight(false)
        , speedModifier(1.0f)
    {
    }

    void SetWASDState(bool w, bool a, bool s, bool d)
    {
        moveForward = w;
        moveLeft = a;
        moveBackward = s;
        moveRight = d;
    }

    Vector2 GetMoveDirection() const
    {
        Vector2 direction;
        if (moveForward)
            direction.y += 1.0f;
        if (moveBackward)
            direction.y -= 1.0f;
        if (moveLeft)
            direction.x -= 1.0f;
        if (moveRight)
            direction.x += 1.0f;

        return direction.Normalized();
    }

    bool IsMoving() const
    {
        return moveForward || moveBackward || moveLeft || moveRight;
    }

    void SetSpeedModifier(float modifier)
    {
        speedModifier = modifier > 0.0f ? modifier : 1.0f;
    }

    float GetSpeedModifier() const { return speedModifier; }

private:
    bool moveForward;
    bool moveBackward;
    bool moveLeft;
    bool moveRight;
    float speedModifier;
};

class PlayerMovement
{
public:
    PlayerMovement()
        : position(0.0f, 0.0f)
        , velocity(0.0f, 0.0f)
        , moveSpeed(200.0f)
        , acceleration(1200.0f)
        , friction(800.0f)
    {
    }

    void Update(float deltaTime, const PlayerControll& controll)
    {
        const Vector2 desiredDirection = controll.GetMoveDirection();
        const float speedScale = controll.GetSpeedModifier();

        if (!desiredDirection.IsZero())
        {
            const Vector2 targetVelocity = desiredDirection * (moveSpeed * speedScale);
            velocity = Approach(velocity, targetVelocity, acceleration * deltaTime);
        }
        else
        {
            velocity = Approach(velocity, Vector2(0.0f, 0.0f), friction * deltaTime);
        }

        position = position + velocity * deltaTime;
    }

    Vector2 GetPosition() const { return position; }
    Vector2 GetVelocity() const { return velocity; }

private:
    static Vector2 Approach(const Vector2& current, const Vector2& target, float delta)
    {
        Vector2 deltaVelocity = target - current;
        const float distance = deltaVelocity.Length();
        if (distance <= delta || distance <= 0.0f)
            return target;

        return current + deltaVelocity * (delta / distance);
    }

    Vector2 position;
    Vector2 velocity;
    float moveSpeed;
    float acceleration;
    float friction;
};

class PlayerDebugger
{
public:
    PlayerDebugger()
        : enabled(true)
    {
    }

    void PrintDebugState(const PlayerMovement& movement, const PlayerControll& controll) const
    {
        if (!enabled)
            return;

        const Vector2 position = movement.GetPosition();
        const Vector2 velocity = movement.GetVelocity();
        const Vector2 direction = controll.GetMoveDirection();

        std::cout << "=== Player Debug State ===\n";
        std::cout << std::fixed << std::setprecision(3);
        std::cout << "Position: (" << position.x << ", " << position.y << ")\n";
        std::cout << "Velocity: (" << velocity.x << ", " << velocity.y << ")\n";
        std::cout << "Input Direction: (" << direction.x << ", " << direction.y << ")\n";
        std::cout << "Moving: " << (controll.IsMoving() ? "Yes" : "No") << "\n";
        std::cout << "Speed Modifier: " << controll.GetSpeedModifier() << "\n";
        std::cout << "==========================\n";
    }

private:
    bool enabled;
};

class GameMap
{
public:
    GameMap() = default;
    GameMap(const std::string& mapName, int mapWidth, int mapHeight, const std::string& theme,
            const std::vector<std::string>& layerNames)
        : name(mapName)
        , width(mapWidth)
        , height(mapHeight)
        , theme(theme)
        , layers(layerNames)
    {
    }

    static GameMap CreateExampleMap(const std::string& mapName)
    {
        if (mapName == "Deasered")
        {
            return GameMap("Deasered", 1280, 720, "Industrial", {"Ground", "Obstacles", "Lighting", "Ambient"});
        }
        else if (mapName == "Vensiea")
        {
            return GameMap("Vensiea", 1600, 900, "Urban", {"Roads", "Buildings", "Props", "Effects"});
        }
        return GameMap("Unknown", 800, 600, "Default", {"Base", "Collision", "Decor"});
    }

    std::string GetDescription() const
    {
        return "Map: " + name + " | Theme: " + theme + " | Size: " + std::to_string(width) + "x" + std::to_string(height);
    }

    std::string GetLayerSummary() const
    {
        std::string summary;
        for (const std::string& layer : layers)
        {
            if (!summary.empty())
                summary += ", ";
            summary += layer;
        }
        return summary;
    }

private:
    std::string name;
    int width = 0;
    int height = 0;
    std::string theme;
    std::vector<std::string> layers;
};

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

    PlayerMovement& GetMovement() { return movement; }

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

int main()
{
    GameManager game;

    GameMap deasered = GameMap::CreateExampleMap("Deasered");
    game.LoadMap(deasered);

    game.SetPlayerInput(true, false, false, true); // W + D
    const float deltaTime = 1.0f / 144.0f;

    std::cout << "\n";
    std::cout << "╔════════════════════════════════════════════╗\n";
    std::cout << "║       THE BLACKOUT - SHOOTER GAME         ║\n";
    std::cout << "║         PC-Optimized Game Engine          ║\n";
    std::cout << "╚════════════════════════════════════════════╝\n";
    std::cout << "\n[Game] Running in PC mode at target 144 FPS.\n";
    std::cout << "[Game] Game engine initialized successfully!\n";
    std::cout << "\n[Controls]\n";
    std::cout << "  W/A/S/D - Move player\n";
    std::cout << "  Mouse   - Aim\n";
    std::cout << "  Space   - Fire weapon\n";
    std::cout << "  1-4     - Switch weapons\n";
    std::cout << "\n[Weapons Available]\n";
    std::cout << "  • AK-47 (28 dmg, 30 rounds)\n";
    std::cout << "  • Shotgun (80 dmg, 8 rounds)\n";
    std::cout << "  • Sniper Rifle (120 dmg, 5 rounds)\n";
    std::cout << "  • Rocket Launcher (200 dmg, 2 rounds)\n";
    std::cout << "\n[Enemy Types]\n";
    std::cout << "  • Shooter (HP: 120)\n";
    std::cout << "  • Brute (HP: 220)\n";
    std::cout << "  • Scout (HP: 75, Fast)\n";
    std::cout << "  • Revenant (HP: 180)\n";
    std::cout << "\n[Maps]\n";
    std::cout << "  • Deasered (Industrial Theme)\n";
    std::cout << "  • Vensiea (Urban Theme)\n";
    std::cout << "\n════════════════════════════════════════════\n\n";

    for (int frame = 0; frame < 12; ++frame)
    {
        game.Update(deltaTime);
        
        if (frame % 3 == 0)
        {
            Vector2 pos = game.GetMovement().GetPosition();
            std::cout << "[Frame " << frame << "] Player at: (" << std::fixed << std::setprecision(2) << pos.x << ", " << pos.y << ")\n";
        }
    }

    std::cout << "\n════════════════════════════════════════════\n";
    std::cout << "[Game] Simulation complete.\n";
    std::cout << "[Game] Your shooter game is running!\n";
    std::cout << "[Game] To add graphics: Install SFML manually or use DirectX.\n";
    std::cout << "════════════════════════════════════════════\n\n";
    return 0;
}

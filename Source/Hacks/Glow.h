#pragma once

#include <unordered_map>
#include <utility>
#include <vector>

#include "../JsonForward.h"
#include "../Memory.h"

class ClientInterfaces;
class EngineInterfaces;

class Glow {
public:
    Glow(const helpers::PatternFinder& clientPatternFinder)
        :
#if IS_WIN32()
        glowObjectManager{ clientPatternFinder("\x0F\x11\x05????\x83\xC8\x01").add(3).deref().as<GlowObjectManager*>() }
#elif IS_LINUX()
        glowObjectManager{ clientPatternFinder("\xE8????\x4C\x89\xE7\x8B\x70\x20").add(1).relativeToAbsolute().add(12).relativeToAbsolute().as<GlowObjectManager*>() }
#endif
    {
    }
 
    void render(const EngineInterfaces& engineInterfaces, const ClientInterfaces& clientInterfaces, const Memory& memory) noexcept;
    void clearCustomObjects() noexcept;
    void updateInput() noexcept;

    // GUI
    void menuBarItem() noexcept;
    void tabItem() noexcept;
    void drawGUI(bool contentOnly) noexcept;

    // Config
    json toJson() noexcept;
    void fromJson(const json& j) noexcept;
    void resetConfig() noexcept;

    struct GlowItem : Color4 {
        bool enabled = false;
        bool healthBased = false;
        int style = 0;
    };

    struct PlayerGlow {
        GlowItem all, visible, occluded;
    };

private:
    GlowObjectManager* glowObjectManager;

    std::unordered_map<std::string, PlayerGlow> playerGlowConfig;
    std::unordered_map<std::string, GlowItem> glowConfig;
    KeyBindToggle glowToggleKey;
    KeyBind glowHoldKey;

    std::vector<std::pair<int, int>> customGlowEntities;
};

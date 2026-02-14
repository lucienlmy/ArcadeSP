#include "Script.hpp"
#include "Constants.hpp"
#include "Events.hpp"
#include "Hooks.hpp"
#include "Keyboard.hpp"
#include "Memory.hpp"
#include "ScriptData.hpp"
#include "ScriptFunction.hpp"
#include "rage/scrProgram.hpp"
#include "rage/scrThread.hpp"
#include "rage/scrValue.hpp"
#include <natives.h>

struct Arcade
{
    enum class eState
    {
        IDLE,
        FADE_OUT,
        LAUNCH_SCRIPT,
        FADE_IN,
        READY,
        CLEANUP,
        FORCE_CLEANUP
    };

    int Location = 0;
    eState State = eState::IDLE;
    Blip Blips[NUM_ARCADE_LOCATIONS] = {};
    rage::scrThread* Thread = nullptr;
    rage::scrProgram* Program = nullptr;

    void Init(rage::scrValue* args)
    {
        ScriptFunction::Call(Thread, Program, g_ScriptData.Func.AmMpArcade.Init, args);
    }

    void Cleanup()
    {
        ScriptFunction::Call(Thread, Program, g_ScriptData.Func.AmMpArcade.Cleanup);
    }

    template <size_t... I>
    bool LaunchACM(rage::scrValue* args, std::index_sequence<I...>)
    {
        return ScriptFunction::Call<bool>(Thread, Program, g_ScriptData.Func.AmMpArcade.LaunchACM, args[I].Any...);
    }

    bool MaintainControls(int unused = 0)
    {
        return ScriptFunction::Call<bool>(Thread, Program, g_ScriptData.Func.AmMpArcade.MaintainControls, unused);
    }

    void MaintainAudio()
    {
        ScriptFunction::Call(Thread, Program, g_ScriptData.Func.AmMpArcade.MaintainAudio);
    }

    void MaintainTimecycle()
    {
        ScriptFunction::Call(Thread, Program, g_ScriptData.Func.AmMpArcade.MaintainTimecycle);
    }

    void MaintainPeds()
    {
        ScriptFunction::Call(Thread, Program, g_ScriptData.Func.AmMpArcade.MaintainPeds);
    }

    void MaintainJukebox()
    {
        ScriptFunction::Call(Thread, Program, g_ScriptData.Func.AmMpArcade.MaintainJukebox);
    }

    void MaintainSeating()
    {
        ScriptFunction::Call(Thread, Program, g_ScriptData.Func.AmMpArcade.MaintainSeating);
    }

    void MaintainFortuneTellerCreation()
    {
        ScriptFunction::Call(Thread, Program, g_ScriptData.Func.AmMpArcade.MaintainFortuneTellerCreation);
    }

    void MaintainLaptops()
    {
        ScriptFunction::Call(Thread, Program, g_ScriptData.Func.AmMpArcade.MaintainLaptops);
    }

    void MaintainChairs()
    {
        ScriptFunction::Call(Thread, Program, g_ScriptData.Func.AmMpArcade.MaintainChairs);
    }

    void MaintainTrophies()
    {
        ScriptFunction::Call(Thread, Program, g_ScriptData.Func.AmMpArcade.MaintainTrophies);
    }

    void MaintainScriptEvents()
    {
        ScriptFunction::Call(Thread, Program, g_ScriptData.Func.AmMpArcade.MaintainScriptEvents);
    }

    void MaintainHighScoreScreens(rage::scrValue* data, Player player, int property)
    {
        ScriptFunction::Call(Thread, Program, g_ScriptData.Func.AmMpArcade.MaintainHighScoreScreens, data, player, property);
    }

    void MaintainBar(rage::scrValue* data, rage::scrValue* data2, rage::scrValue* data3)
    {
        ScriptFunction::Call(Thread, Program, g_ScriptData.Func.AmMpArcade.MaintainBar, data, data2, data3);
    }

    void MaintainBartender(rage::scrValue* data, rage::scrValue* data2, bool deletePeds)
    {
        ScriptFunction::Call(Thread, Program, g_ScriptData.Func.AmMpArcade.MaintainBartender, data, data2, deletePeds);
    }

} g_Arcade;

void SpoofGlobals()
{
    *getGlobalPtr(g_ScriptData.Glb.Gpbd.Index + (1 + (0 * g_ScriptData.Glb.Gpbd.Size))) = 4; // player state valid
    *getGlobalPtr(g_ScriptData.Glb.Gpbd.Index + (1 + (0 * g_ScriptData.Glb.Gpbd.Size)) + g_ScriptData.Glb.Gpbd.SimpleInteriorData + g_ScriptData.Glb.Gpbd.curSimpleInteriorOwner) = 0;
    *getGlobalPtr(g_ScriptData.Glb.Gpbd.Index + (1 + (0 * g_ScriptData.Glb.Gpbd.Size)) + g_ScriptData.Glb.Gpbd.SimpleInteriorData + g_ScriptData.Glb.Gpbd.SimpleInteriorEntryType) = 0; // 0 is front door, default is -1, required for am_mp_arcade
    *getGlobalPtr(g_ScriptData.Glb.GpbdFm.Index + (1 + (0 * g_ScriptData.Glb.GpbdFm.Size)) + g_ScriptData.Glb.GpbdFm.PropertyData + g_ScriptData.Glb.GpbdFm.ArcData + 1) = 480256036;   // bits 2=cabinets setup, 5=floor 3, 13=ceiling 2, 21=wall 7, 23=gunslinger, 26=screens, 27=drone, 28=mct
    *getGlobalPtr(g_ScriptData.Glb.GpbdFm.Index + (1 + (0 * g_ScriptData.Glb.GpbdFm.Size)) + g_ScriptData.Glb.GpbdFm.PropertyData + g_ScriptData.Glb.GpbdFm.ArcData + 2) = 4160508044;  // bits 2=love meter, 3=claw crane, 7=neon 4, 12=degenatron, 14=fortune teller, 18-26=plushies, 28=race n chase, 29=iap, 30=ggsm, 31=street crimes
    *getGlobalPtr(g_ScriptData.Glb.GpbdFm.Index + (1 + (0 * g_ScriptData.Glb.GpbdFm.Size)) + g_ScriptData.Glb.GpbdFm.PropertyData + g_ScriptData.Glb.GpbdFm.ArcData + 3) = 63;          // bits 0=wizard, 1=all cabinets, 2=all trophies, 3=strength test, 4=qub3d, 5=camhedz
    *getGlobalPtr(g_ScriptData.Glb.GpbdFm.Index + (1 + (0 * g_ScriptData.Glb.GpbdFm.Size)) + g_ScriptData.Glb.GpbdFm.PropertyData + g_ScriptData.Glb.GpbdFm.ArcCabFlags + 0) = -1;      // set all purchased flags
    *getGlobalPtr(g_ScriptData.Glb.GpbdFm.Index + (1 + (0 * g_ScriptData.Glb.GpbdFm.Size)) + g_ScriptData.Glb.GpbdFm.PropertyData + g_ScriptData.Glb.GpbdFm.ArcCabFlags + 1) = -1;      // set all delivered flags
    *getGlobalPtr(g_ScriptData.Glb.MissionObjectFlags) |= (1 << 0);
    *getGlobalPtr(g_ScriptData.Glb.NumReservedMissionObjects) = 80;
    *getGlobalPtr(g_ScriptData.Glb.SmplInteriorIntState) = 7;                                                           // am_mp_smpl_interior_int state, required for peds and acm
    *getGlobalPtr(g_ScriptData.Glb.SmplInteriorData.Index + g_ScriptData.Glb.SmplInteriorData.OwnedFlags) |= (1 << 30); // required for am_mp_arcade
}

void UpdateBlips()
{
    if (g_Arcade.State != Arcade::eState::IDLE)
        return;

    for (int i = 0; i < NUM_ARCADE_LOCATIONS; i++)
    {
        auto& blip = g_Arcade.Blips[i];
        if (HUD::DOES_BLIP_EXIST(blip))
            continue;

        auto& coords = g_ArcadeBlipCoords[i];

        blip = HUD::ADD_BLIP_FOR_COORD(coords.x, coords.y, coords.z);
        HUD::SET_BLIP_ROUTE(blip, FALSE);
        HUD::SET_BLIP_SPRITE(blip, 740);
        HUD::SET_BLIP_SCALE(blip, 1.0f);
        HUD::SET_BLIP_PRIORITY(blip, 7);
        HUD::SET_BLIP_COLOUR(blip, 4);
        HUD::SET_BLIP_AS_SHORT_RANGE(blip, TRUE);
    }
}

void UpdateMarkers()
{
    if (g_Arcade.State != Arcade::eState::IDLE)
        return;

    for (int i = 0; i < NUM_ARCADE_LOCATIONS; i++)
    {
        int r, g, b, a;
        HUD::GET_HUD_COLOUR(9, &r, &g, &b, &a);
        a = 100;

        auto& coords = g_ArcadeEntryCoords[i];
        GRAPHICS::DRAW_MARKER(1, coords.x, coords.y, coords.z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.75f, 0.75f, 0.75f, r, g, b, a, FALSE, FALSE, 2, FALSE, NULL, NULL, FALSE);
    }
}

void UpdateEntry()
{
    if (g_Arcade.State != Arcade::eState::IDLE)
        return;

    for (int i = 0; i < NUM_ARCADE_LOCATIONS; i++)
    {
        auto& coords = g_ArcadeEntryCoords[i];
        if (ENTITY::IS_ENTITY_AT_COORD(PLAYER::PLAYER_PED_ID(), coords.x, coords.y, coords.z, 1.0f, 1.0f, 1.0f, FALSE, TRUE, 0))
        {
            g_Arcade.Location = i;
            g_Arcade.State = Arcade::eState::FADE_OUT;
            LOG("Player is in angled area, fading out.");
            break;
        }
    }
}

void FadeOut()
{
    if (g_Arcade.State != Arcade::eState::FADE_OUT)
        return;

    PLAYER::SET_PLAYER_CONTROL(PLAYER::PLAYER_ID(), FALSE, 0);
    ENTITY::FREEZE_ENTITY_POSITION(PLAYER::PLAYER_PED_ID(), TRUE);

    CAM::DO_SCREEN_FADE_OUT(1000);
    while (!CAM::IS_SCREEN_FADED_OUT())
        WAIT(0);

    LOG("Faded out, launching script.");
    g_Arcade.State = Arcade::eState::LAUNCH_SCRIPT;
}

void LaunchScript()
{
    if (g_Arcade.State != Arcade::eState::LAUNCH_SCRIPT)
        return;

    while (!SCRIPT::HAS_SCRIPT_WITH_NAME_HASH_LOADED("am_mp_arcade"_J))
    {
        SCRIPT::REQUEST_SCRIPT_WITH_NAME_HASH("am_mp_arcade"_J);
        WAIT(0);
    }

    *getGlobalPtr(g_ScriptData.Glb.Gpbd.Index + (1 + (0 * g_ScriptData.Glb.Gpbd.Size)) + g_ScriptData.Glb.Gpbd.SimpleInteriorData + g_ScriptData.Glb.Gpbd.CurSimpleInterior) = 128 + g_Arcade.Location;
    *getGlobalPtr(g_ScriptData.Glb.GpbdFm.Index + (1 + (0 * g_ScriptData.Glb.GpbdFm.Size)) + g_ScriptData.Glb.GpbdFm.PropertyData + g_ScriptData.Glb.GpbdFm.ArcData + 0) = g_Arcade.Location + 1; // required for arcade_seating tv
    for (int i = 0; i < 41; i++)
        *getGlobalPtr(g_ScriptData.Glb.GpbdFm.Index + (1 + (0 * g_ScriptData.Glb.GpbdFm.Size)) + g_ScriptData.Glb.GpbdFm.PropertyData + g_ScriptData.Glb.GpbdFm.ArcCabSaveSlots + 1 + i) = static_cast<int64_t>(g_ArcadeSaveSlots[i]); // can be overriden by the management menu later

    int64_t arcArgs[5]{};
    arcArgs[0] = 128 + g_Arcade.Location; // no need to set the 4th field as the script doesn't even use it
    BUILTIN::START_NEW_SCRIPT_WITH_NAME_HASH_AND_ARGS("am_mp_arcade"_J, reinterpret_cast<Any*>(&arcArgs[0]), 5, 14100);
    SCRIPT::SET_SCRIPT_WITH_NAME_HASH_AS_NO_LONGER_NEEDED("am_mp_arcade"_J);

    g_Arcade.Thread = rage::scrThread::GetThread("am_mp_arcade"_J);
    g_Arcade.Program = rage::scrProgram::GetProgram(*g_Arcade.Thread->Context()->ProgramHash());
    *g_Arcade.Thread->Context()->State() = rage::scrThreadState::PAUSED;

    auto stack = g_Arcade.Thread->Stack();

    g_Arcade.Init(&stack[g_Arcade.Program->m_StaticCount - g_Arcade.Program->m_ArgCount]); // passing script args
    stack[g_ScriptData.Stc.AmMpArcade.ArcData.Index + 4].Int = 1;                          // arcade state loaded (offset for this isn't likely to change)

    while (!g_Arcade.LaunchACM(&stack[g_ScriptData.Stc.AmMpArcade.AcmLaunchData], std::make_index_sequence<87>{})) // function takes a copy of the struct, so we have to pass all the fields
        WAIT(0);

    LOG("Script launched, fading in.");
    g_Arcade.State = Arcade::eState::FADE_IN;
}

void FadeIn()
{
    if (g_Arcade.State != Arcade::eState::FADE_IN)
        return;

    ENTITY::SET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 2737.91f, -374.29f, -48.0f, FALSE, FALSE, FALSE, FALSE);
    ENTITY::SET_ENTITY_HEADING(PLAYER::PLAYER_PED_ID(), 179.55f);
    CAM::SET_GAMEPLAY_CAM_RELATIVE_HEADING(0.0f);
    PLAYER::SET_PLAYER_CONTROL(PLAYER::PLAYER_ID(), TRUE, 0);
    ENTITY::FREEZE_ENTITY_POSITION(PLAYER::PLAYER_PED_ID(), FALSE);

    WAIT(2000);

    CAM::DO_SCREEN_FADE_IN(1000);

    LOG("Faded in, arcade is ready.");
    g_Arcade.State = Arcade::eState::READY;
}

void UpdateArcade()
{
    if (g_Arcade.State != Arcade::eState::READY)
        return;

    auto stack = g_Arcade.Thread->Stack();

    g_Arcade.MaintainControls();
    g_Arcade.MaintainAudio();
    g_Arcade.MaintainTimecycle();
    g_Arcade.MaintainPeds();    // launches am_mp_arcade_peds
    g_Arcade.MaintainJukebox(); // launches ob_jukebox
    g_Arcade.MaintainSeating(); // launches arcade_seating
    g_Arcade.MaintainFortuneTellerCreation();
    g_Arcade.MaintainLaptops();
    g_Arcade.MaintainChairs();
    g_Arcade.MaintainTrophies();
    g_Arcade.MaintainScriptEvents();
    g_Arcade.MaintainHighScoreScreens(&stack[g_ScriptData.Stc.AmMpArcade.HiScoreData], 0, 0); // arg 1 is player id, arg 2 is property type (0 is arcade, 1 is auto shop, etc.)
    g_Arcade.MaintainBar(&stack[g_ScriptData.Stc.AmMpArcade.HostBd.Index + g_ScriptData.Stc.AmMpArcade.HostBd.BarData], &stack[g_ScriptData.Stc.AmMpArcade.BarPlayerData], &stack[g_ScriptData.Stc.AmMpArcade.ArcData.Index + g_ScriptData.Stc.AmMpArcade.ArcData.BarData]);
    g_Arcade.MaintainBartender(&stack[g_ScriptData.Stc.AmMpArcade.HostBd.Index + g_ScriptData.Stc.AmMpArcade.HostBd.BarData], &stack[g_ScriptData.Stc.AmMpArcade.ArcData.Index + g_ScriptData.Stc.AmMpArcade.ArcData.BarData], false);
}

void UpdatePlayerVisibility()
{
    if (g_Arcade.State != Arcade::eState::READY)
        return;

    // workaround for SET_ENTITY_LOCALLY_INVISIBLE, which doesn't work in SP

    BOOL visible = TRUE;

    if (*getGlobalPtr(g_ScriptData.Glb.AcmData) & (1 << 1))
    {
        visible = FALSE;
    }
    else
    {
        if (auto thread = rage::scrThread::GetThread("am_mp_arcade_claw_crane"_J))
        {
            if (auto stack = thread->Stack())
                visible = !CAM::DOES_CAM_EXIST(stack[g_ScriptData.Stc.ArcClawCrane.ClawCraneData + g_ScriptData.Stc.ArcClawCrane.ClawCraneCamera].Int);
        }
    }

    ENTITY::SET_ENTITY_VISIBLE(PLAYER::PLAYER_PED_ID(), visible, FALSE);
}

void UpdateExit()
{
    if (g_Arcade.State != Arcade::eState::READY)
        return;

    if (ENTITY::IS_ENTITY_IN_ANGLED_AREA(PLAYER::PLAYER_PED_ID(), 2737.304f, -373.593f, -48.979f, 2738.514f, -373.562f, -46.750f, 1.500f, FALSE, TRUE, 1))
    {
        if (!HUD::IS_HELP_MESSAGE_BEING_DISPLAYED())
        {
            HUD::BEGIN_TEXT_COMMAND_DISPLAY_HELP("ILH_H_PORT2"); // Press ~INPUT_CONTEXT~ to exit.
            HUD::END_TEXT_COMMAND_DISPLAY_HELP(0, TRUE, TRUE, -1);
        }

        if (PAD::IS_CONTROL_JUST_PRESSED(2, eControl::ControlContext))
        {
            HUD::CLEAR_HELP(TRUE);

            g_Arcade.State = Arcade::eState::CLEANUP;
        }
    }
    else
    {
        HUD::BEGIN_TEXT_COMMAND_IS_THIS_HELP_MESSAGE_BEING_DISPLAYED("ILH_H_PORT2");
        if (HUD::END_TEXT_COMMAND_IS_THIS_HELP_MESSAGE_BEING_DISPLAYED(0))
            HUD::CLEAR_HELP(TRUE);
    }
}

void UpdateBoundsCheck()
{
    if (g_Arcade.State != Arcade::eState::READY)
        return;

    if (!ENTITY::IS_ENTITY_IN_AREA(PLAYER::PLAYER_PED_ID(), 2661.738f, -402.823f, -57.637f, 2741.5615f, -351.346f, -44.5974f, FALSE, TRUE, 0))
        g_Arcade.State = Arcade::eState::FORCE_CLEANUP;
}

void Cleanup(bool force)
{
    if (!force)
    {
        PLAYER::SET_PLAYER_CONTROL(PLAYER::PLAYER_ID(), FALSE, 0);
        ENTITY::FREEZE_ENTITY_POSITION(PLAYER::PLAYER_PED_ID(), TRUE);

        CAM::DO_SCREEN_FADE_OUT(1000);
        while (!CAM::IS_SCREEN_FADED_OUT())
            WAIT(0);
    }

    *getGlobalPtr(g_ScriptData.Glb.Gpbd.Index + (1 + (0 * g_ScriptData.Glb.Gpbd.Size)) + g_ScriptData.Glb.Gpbd.SimpleInteriorData + g_ScriptData.Glb.Gpbd.CurSimpleInterior) = -1;
    *getGlobalPtr(g_ScriptData.Glb.GpbdFm.Index + (1 + (0 * g_ScriptData.Glb.GpbdFm.Size)) + g_ScriptData.Glb.GpbdFm.PropertyData + g_ScriptData.Glb.GpbdFm.ArcData + 0) = 0;

    g_Arcade.Cleanup(); // this will make other scripts started by am_mp_arcade terminate as well
    g_Arcade.Thread->Kill();

    if (!force)
    {
        PLAYER::SET_PLAYER_CONTROL(PLAYER::PLAYER_ID(), TRUE, 0);
        ENTITY::FREEZE_ENTITY_POSITION(PLAYER::PLAYER_PED_ID(), FALSE);

        auto& coords = g_ArcadeExitCoords[g_Arcade.Location].first;
        auto& heading = g_ArcadeExitCoords[g_Arcade.Location].second;
        ENTITY::SET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), coords.x, coords.y, coords.z, FALSE, FALSE, FALSE, FALSE);
        ENTITY::SET_ENTITY_HEADING(PLAYER::PLAYER_PED_ID(), heading);
        CAM::SET_GAMEPLAY_CAM_RELATIVE_HEADING(0.0f);

        WAIT(2000);

        CAM::DO_SCREEN_FADE_IN(1000);
    }

    g_Arcade.Location = -1;
    g_Arcade.State = Arcade::eState::IDLE;
    g_Arcade.Thread = nullptr;
    g_Arcade.Program = nullptr;

    LOG("Cleaned up script.");
}

void Run()
{
    SpoofGlobals();
    ProcessScriptEvents();

    switch (g_Arcade.State)
    {
    case Arcade::eState::IDLE:
        UpdateBlips();
        UpdateMarkers();
        UpdateEntry();
        break;
    case Arcade::eState::FADE_OUT:
        FadeOut();
        break;
    case Arcade::eState::LAUNCH_SCRIPT:
        LaunchScript();
        break;
    case Arcade::eState::FADE_IN:
        FadeIn();
        break;
    case Arcade::eState::READY:
        UpdateArcade();
        UpdatePlayerVisibility();
        UpdateExit();
        UpdateBoundsCheck();
        break;
    case Arcade::eState::CLEANUP:
        Cleanup(false);
        break;
    case Arcade::eState::FORCE_CLEANUP:
        Cleanup(true);
        break;
    }
}

bool LoadArcadeMap()
{
    uint8_t* extraMetadataPtr = nullptr;
    void (*revertContentChangesetGroup)(void*, uint32_t, uint32_t) = nullptr;
    void (*executeContentChangesetGroup)(void*, void*, uint32_t*) = nullptr; // this is inlined in enhanced so we use the internal one which takes a content pointer instead of hash

    if (g_IsEnhanced)
    {
        if (auto addr = Memory::ScanPattern("48 8B 05 ? ? ? ? 89 B8"))
            extraMetadataPtr = *addr->Add(3).Rip().As<uint8_t**>();

        if (auto addr = Memory::ScanPattern("E8 ? ? ? ? 0F B7 46 ? EB ? 48 8D 0D"))
            revertContentChangesetGroup = addr->Add(1).Rip().As<decltype(revertContentChangesetGroup)>();

        if (auto addr = Memory::ScanPattern("E8 ? ? ? ? 48 8B 0D ? ? ? ? 0F B7 47"))
            executeContentChangesetGroup = addr->Add(1).Rip().As<decltype(executeContentChangesetGroup)>();
    }
    else
    {
        if (auto addr = Memory::ScanPattern("48 8B 05 ? ? ? ? 44 89 2D"))
            extraMetadataPtr = *addr->Add(3).Rip().As<uint8_t**>();

        if (auto addr = Memory::ScanPattern("48 85 C0 74 ? 4C 8D 44 24 ? 41 83 C9 FF"))
            revertContentChangesetGroup = addr->Sub(0x1C).As<decltype(revertContentChangesetGroup)>();

        if (auto addr = Memory::ScanPattern("48 85 D2 0F 84 ? ? ? ? 48 8B C4 48 89 58 ? 48 89 68 ? 56"))
            executeContentChangesetGroup = addr->As<decltype(executeContentChangesetGroup)>();
    }

    if (!extraMetadataPtr || !revertContentChangesetGroup || !executeContentChangesetGroup)
        return false;

    uint32_t contentHash = "MPHEIST3"_J;
    uint32_t groupHashSp = "GROUP_MAP_SP"_J;
    uint32_t groupHashMp = "GROUP_MAP"_J;

    auto base = *reinterpret_cast<uint8_t**>(extraMetadataPtr + 0x28);
    auto count = *reinterpret_cast<uint32_t*>(extraMetadataPtr + 0x30);

    if (!base || !count)
        return false;

    void* content = nullptr;

    for (uint32_t i = 0; i < count; i++)
    {
        auto entry = base + ((g_IsEnhanced ? 0x120 : 0xF0) * i);

        if (*reinterpret_cast<uint32_t*>(entry + (g_IsEnhanced ? 0x80 : 0x60)) == contentHash)
        {
            content = entry;
            break;
        }
    }

    if (!content)
        return false;

    revertContentChangesetGroup(extraMetadataPtr, contentHash, groupHashSp);
    executeContentChangesetGroup(extraMetadataPtr, content, &groupHashMp);

    WAIT(1500);

    SCRIPT::SHUTDOWN_LOADING_SCREEN();
    return true;
}

bool Init()
{
    if (!InitHooks())
    {
        LOG("Failed to initialize hooks.");
        return false;
    }

    if (!g_ScriptData.Init())
    {
        LOG("Failed to initialize script data.");
        return false;
    }

    if (!LoadArcadeMap())
    {
        LOG("Failed to arcade map parts.");
        return false;
    }

    LOG("Script initialized successfully.");
    return true;
}

void ScriptMain()
{
    g_GameVersion = getGameVersion();
    if (g_GameVersion < eGameVersion::VER_1_0_3095_0)
    {
        LOGF("Unsupported game version: %d", static_cast<int>(g_GameVersion));
        return;
    }

    if (g_GameVersion > 1000)
        g_IsEnhanced = true;

    LOGF("Game type is %s. Game version is %d.", g_IsEnhanced ? "Enhanced" : "Legacy", static_cast<int>(g_GameVersion));

    if (!Init())
        return;

    while (true)
    {
        Run();
        WAIT(0);
    }
}
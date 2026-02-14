#include "ScriptData.hpp"
#include "rage/scrProgram.hpp"
#include <natives.h>

bool readFailed = false;

void ReadScript(rage::scrProgram* program, const char* pattern, const std::vector<std::pair<int32_t, bool>>& offsetAndRip, void* out, uint32_t size, bool logHex = false)
{
    auto addressOpt = program->ScanPattern(pattern);
    if (!addressOpt.has_value())
    {
        readFailed = true;
        LOGF("Failed to find script pattern '%s' in script program %s.", pattern, program->m_Name);
        return;
    }

    uint32_t address = addressOpt.value();

    for (const auto& [offset, rip] : offsetAndRip)
    {
        address += offset;
        if (rip)
        {
            auto code = program->GetCode(address);
            address = code[0] | (code[1] << 8) | (code[2] << 16);
        }
    }

    std::memcpy(out, &address, size);

    int32_t value = 0;
    std::memcpy(&value, out, size);

    if (logHex)
        LOGF("Read script code in script program %s, value is 0x%08X.", program->m_Name, value);
    else
        LOGF("Read script code in script program %s, value is %d.", program->m_Name, value);
}

bool ScriptData::Init()
{
    while (
        !SCRIPT::HAS_SCRIPT_WITH_NAME_HASH_LOADED("am_mp_arcade"_J) ||
        !SCRIPT::HAS_SCRIPT_WITH_NAME_HASH_LOADED("am_mp_arc_cab_manager"_J) ||
        !SCRIPT::HAS_SCRIPT_WITH_NAME_HASH_LOADED("am_mp_arcade_claw_crane"_J))
    {
        SCRIPT::REQUEST_SCRIPT_WITH_NAME_HASH("am_mp_arcade"_J);
        SCRIPT::REQUEST_SCRIPT_WITH_NAME_HASH("am_mp_arc_cab_manager"_J);
        SCRIPT::REQUEST_SCRIPT_WITH_NAME_HASH("am_mp_arcade_claw_crane"_J);
        WAIT(0);
    }

    auto program1 = rage::scrProgram::GetProgram("am_mp_arcade"_J);
    auto program2 = rage::scrProgram::GetProgram("am_mp_arc_cab_manager"_J);
    auto program3 = rage::scrProgram::GetProgram("am_mp_arcade_claw_crane"_J);
    if (!program1 || !program2 || !program3)
        return false;

    // Functions

    ReadScript(program1, "2D 01 04 00 00 5D ? ? ? 73", {}, &Func.AmMpArcade.Init, 3, true);
    ReadScript(program1, "2D 00 02 00 00 61 ? ? ? 46 ? ? 5D", {}, &Func.AmMpArcade.Cleanup, 3, true);
    ReadScript(program1, "2D 57 59 00 00", {}, &Func.AmMpArcade.LaunchACM, 3, true);
    ReadScript(program1, "5D ? ? ? 09 2A 56 ? ? 28 D9 67 11 21", {{-37, false}}, &Func.AmMpArcade.MaintainControls, 3, true);
    ReadScript(program1, "2D 00 02 00 00 2C 01 ? ? 2C 05 ? ? 06", {}, &Func.AmMpArcade.MaintainAudio, 3, true);
    ReadScript(program1, "2D 00 02 00 00 5D ? ? ? 56 ? ? 5D", {}, &Func.AmMpArcade.MaintainTimecycle, 3, true);
    ReadScript(program1, "2D 00 05 00 00 62 ? ? ? 56 ? ? 62", {}, &Func.AmMpArcade.MaintainPeds, 3, true);
    ReadScript(program1, "2D 00 02 00 00 62 ? ? ? 73 82 56", {}, &Func.AmMpArcade.MaintainJukebox, 3, true);
    ReadScript(program1, "2D 00 02 00 00 50 ? ? 25 ? 82 06 2A 56 ? ? 2C 01 ? ? 5D ? ? ? 1F", {}, &Func.AmMpArcade.MaintainSeating, 3, true);
    ReadScript(program1, "2D 00 08 00 00 4F ? ? 47 ? ? 2C", {}, &Func.AmMpArcade.MaintainFortuneTellerCreation, 3, true);
    ReadScript(program1, "2D 00 04 00 00 71 39 03 38 03 73", {}, &Func.AmMpArcade.MaintainLaptops, 3, true);
    ReadScript(program1, "2D 00 03 00 00 71 5D", {}, &Func.AmMpArcade.MaintainChairs, 3, true);
    ReadScript(program1, "2D 00 05 00 00 71 39 02 38 02 74", {}, &Func.AmMpArcade.MaintainTrophies, 3, true);
    ReadScript(program1, "2D 00 07 00 00 71 39 02 38 02 72", {}, &Func.AmMpArcade.MaintainScriptEvents, 3, true);
    ReadScript(program1, "2D 03 05 00 00 38 01 5D ? ? ? 06 56 ? ? 2E 03 00", {}, &Func.AmMpArcade.MaintainHighScoreScreens, 3, true);
    ReadScript(program1, "2D 03 15 00 00 72", {}, &Func.AmMpArcade.MaintainBar, 3, true);
    ReadScript(program1, "2D 03 05 00 00 38 00 38 01 38 02 5D ? ? ? 38 00", {}, &Func.AmMpArcade.MaintainBartender, 3, true);

    // Globals

    ReadScript(program2, "61 ? ? ? 40 03 35 01 37 0C", {{1, true}}, &Glb.AcmData, 3);
    ReadScript(program2, "62 ? ? ? 78 08 1F", {{1, true}}, &Glb.SmplInteriorIntState, 3);

    // this has two occurrences, but the scan should always find the first one, which is what we need
    ReadScript(program2, "38 01 38 00 2C 01 ? ? 61 ? ? ? 49 ? ? 46 ? ? 46 ? ? 36", {{9, true}}, &Glb.GpbdFm.Index, 3);
    ReadScript(program2, "38 01 38 00 2C 01 ? ? 61 ? ? ? 49 ? ? 46 ? ? 46 ? ? 36", {{13, true}}, &Glb.GpbdFm.Size, 2);
    ReadScript(program2, "38 01 38 00 2C 01 ? ? 61 ? ? ? 49 ? ? 46 ? ? 46 ? ? 36", {{16, true}}, &Glb.GpbdFm.PropertyData, 2);
    ReadScript(program2, "38 01 38 00 2C 01 ? ? 61 ? ? ? 49 ? ? 46 ? ? 46 ? ? 36", {{19, true}}, &Glb.GpbdFm.ArcCabSaveSlots, 2);
    Glb.GpbdFm.ArcData = Glb.GpbdFm.ArcCabSaveSlots - 8;
    Glb.GpbdFm.ArcCabFlags = Glb.GpbdFm.ArcCabSaveSlots - 2;

    ReadScript(program2, "61 ? ? ? 49 ? ? 46 ? ? 41 ? 5D ? ? ? 25 11", {{1, true}}, &Glb.Gpbd.Index, 3);
    ReadScript(program2, "61 ? ? ? 49 ? ? 46 ? ? 41 ? 5D ? ? ? 25 11", {{5, true}}, &Glb.Gpbd.Size, 2);
    ReadScript(program2, "61 ? ? ? 49 ? ? 46 ? ? 41 ? 5D ? ? ? 25 11", {{8, true}}, &Glb.Gpbd.SimpleInteriorData, 2);
    ReadScript(program2, "61 ? ? ? 49 ? ? 46 ? ? 41 ? 5D ? ? ? 25 11", {{11, true}}, &Glb.Gpbd.CurSimpleInterior, 1);
    ReadScript(program2, "5D ? ? ? 73 08 2A 06 56 ? ? 5D ? ? ? 77", {{1, true}, {20, true}}, &Glb.Gpbd.SimpleInteriorEntryType, 1); // 5 + 4 + 4 + 3 + 3 + 1
    Glb.Gpbd.curSimpleInteriorOwner = Glb.Gpbd.CurSimpleInterior + 3;

    ReadScript(program2, "61 ? ? ? 47 ? ? 25 1E 82 20 1F 2A", {{1, true}}, &Glb.SmplInteriorData.Index, 3);
    ReadScript(program2, "61 ? ? ? 47 ? ? 25 1E 82 20 1F 2A", {{5, true}}, &Glb.SmplInteriorData.OwnedFlags, 2);

    ReadScript(program2, "2D 05 09 00 00 62", {{6, true}}, &Glb.MissionObjectFlags, 3);
    Glb.NumReservedMissionObjects = Glb.MissionObjectFlags + 10;

    // Statics

    ReadScript(program1, "51 ? ? 28 46 00 24 DD", {{1, true}}, &Stc.AmMpArcade.AcmLaunchData, 2);
    ReadScript(program1, "4F ? ? 5D ? ? ? 4F ? ? 47 ? ? 2C 01 ? ? 08 2A", {{1, true}}, &Stc.AmMpArcade.HiScoreData, 2);
    ReadScript(program1, "4F ? ? 40 ? 4F ? ? 4F ? ? 46 ? ? 72 72 72 5D", {{1, true}}, &Stc.AmMpArcade.HostBd.Index, 2);
    ReadScript(program1, "4F ? ? 40 ? 4F ? ? 4F ? ? 46 ? ? 72 72 72 5D", {{4, true}}, &Stc.AmMpArcade.HostBd.BarData, 1);
    ReadScript(program1, "4F ? ? 40 ? 4F ? ? 4F ? ? 46 ? ? 72 72 72 5D", {{6, true}}, &Stc.AmMpArcade.BarPlayerData, 2);
    ReadScript(program1, "4F ? ? 40 ? 4F ? ? 4F ? ? 46 ? ? 72 72 72 5D", {{9, true}}, &Stc.AmMpArcade.ArcData.Index, 2);
    ReadScript(program1, "4F ? ? 40 ? 4F ? ? 4F ? ? 46 ? ? 72 72 72 5D", {{12, true}}, &Stc.AmMpArcade.ArcData.BarData, 2);

    ReadScript(program3, "4F ? ? 41 ? 2C 05 ? ? 2A", {{1, true}}, &Stc.ArcClawCrane.ClawCraneData, 2);
    ReadScript(program3, "4F ? ? 41 ? 2C 05 ? ? 2A", {{4, true}}, &Stc.ArcClawCrane.ClawCraneCamera, 1);

    SCRIPT::SET_SCRIPT_WITH_NAME_HASH_AS_NO_LONGER_NEEDED("am_mp_arcade"_J);
    SCRIPT::SET_SCRIPT_WITH_NAME_HASH_AS_NO_LONGER_NEEDED("am_mp_arc_cab_manager"_J);
    SCRIPT::SET_SCRIPT_WITH_NAME_HASH_AS_NO_LONGER_NEEDED("am_mp_arcade_claw_crane"_J);

    if (readFailed)
    {
        *this = {};
        readFailed = false;
        return false;
    }

    return true;
}
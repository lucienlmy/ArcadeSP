#pragma once

struct ScriptData
{
    struct
    {
        struct
        {
            int Init = 0;
            int Cleanup = 0;
            int LaunchACM = 0;
            int MaintainControls = 0;
            int MaintainAudio = 0;
            int MaintainTimecycle = 0;
            int MaintainPeds = 0;
            int MaintainJukebox = 0;
            int MaintainSeating = 0;
            int MaintainFortuneTellerCreation = 0;
            int MaintainLaptops = 0;
            int MaintainChairs = 0;
            int MaintainTrophies = 0;
            int MaintainScriptEvents = 0;
            int MaintainHighScoreScreens = 0;
            int MaintainBar = 0;
            int MaintainBartender = 0;
        } AmMpArcade;
    } Func;

    struct
    {
        int AcmData = 0;
        int SmplInteriorIntState = 0;

        struct
        {
            int Index = 0;
            int Size = 0;
            int PropertyData = 0;
            int ArcData = 0;
            int ArcCabFlags = 0;
            int ArcCabSaveSlots = 0;
        } GpbdFm;

        struct
        {
            int Index = 0;
            int Size = 0;
            int SimpleInteriorData = 0;
            int CurSimpleInterior = 0;
            int SimpleInteriorEntryType = 0;
            int curSimpleInteriorOwner = 0;
        } Gpbd;

        struct
        {
            int Index = 0;
            int OwnedFlags = 0;
        } SmplInteriorData;

        int MissionObjectFlags = 0;
        int NumReservedMissionObjects = 0;
    } Glb;

    struct
    {
        struct
        {
            int ClawCraneData = 0;
            int ClawCraneCamera = 0;
        } ArcClawCrane;

        struct
        {
            struct
            {
                int Index = 0;
                int BarData = 0;
            } HostBd;

            struct
            {
                int Index = 0;
                int BarData = 0;
            } ArcData;

            int AcmLaunchData = 0;
            int HiScoreData = 0;
            int BarPlayerData = 0;
        } AmMpArcade;
    } Stc;

    bool Init();
} inline g_ScriptData;
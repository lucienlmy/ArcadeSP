#include "Events.hpp"
#include <natives.h>

// Generate random leaderboard names and scores
void HandleTSE_457347143(int index)
{
    int64_t data[4]{};

    BOOL read = FALSE;
    if (g_GameVersion >= eGameVersion::VER_1_0_3095_0)
        read = SCRIPT::GET_EVENT_DATA(1, index, reinterpret_cast<Any*>(&data), 4);
    else
        read = SCRIPT::GET_EVENT_DATA(1, index, reinterpret_cast<Any*>(&data), 3);

    if (!read)
        return;

    const int base = (g_GameVersion >= eGameVersion::VER_1_0_3095_0) ? 3 : 2;

    int64_t dataToSend[26]{};
    dataToSend[0] = -2031658982;
    dataToSend[1] = PLAYER::PLAYER_ID();
    dataToSend[base + 0] = data[base]; // game type
    dataToSend[base + 1] = 10; // names array size
    dataToSend[base + 12] = 10; // scores array size

    int scores[10];
    for (int i = 0; i < 10; i++)
        scores[i] = MISC::GET_RANDOM_INT_IN_RANGE(1000, 100000);

    std::sort(scores, scores + 10, std::greater<int>());

    for (int i = 0; i < 10; i++)
    {
        int a = MISC::GET_RANDOM_INT_IN_RANGE(0, 35);
        int b = MISC::GET_RANDOM_INT_IN_RANGE(0, 35);
        int c = MISC::GET_RANDOM_INT_IN_RANGE(0, 35);
        dataToSend[base + 2 + i] = (int64_t)(a & 0x3F) | ((int64_t)(b & 0x3F) << 6) | ((int64_t)(c & 0x3F) << 12); // names
        dataToSend[base + 13 + i] = scores[i]; // scores
    }

    // Send the data back to whichever arcade script requested it
    if (g_GameVersion >= eGameVersion::VER_1_0_3095_0)
        SCRIPT::_SEND_TU_SCRIPT_EVENT_NEW(1, reinterpret_cast<Any*>(&dataToSend[0]), 26, -1, -2031658982);
    else
        invoke<Void>(0x01F0B819E78A18A1, 1, reinterpret_cast<Any*>(&dataToSend[0]), 25, -1); // SEND_TU_SCRIPT_EVENT
}

void ProcessScriptEvents()
{
    int numEvents = SCRIPT::GET_NUMBER_OF_EVENTS(1);

    for (int i = 0; i < numEvents; i++)
    {
        int event = SCRIPT::GET_EVENT_AT_INDEX(1, i);
        if (event != 174)
            continue;

        int64_t scriptEventHash = 0;
        SCRIPT::GET_EVENT_DATA(1, i, reinterpret_cast<Any*>(&scriptEventHash), 1);

        switch (scriptEventHash)
        {
        case 457347143:
            HandleTSE_457347143(i);
            break;
        }
    }
}
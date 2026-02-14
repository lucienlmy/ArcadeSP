#pragma once

constexpr int NUM_ARCADE_LOCATIONS = 6;
constexpr int NUM_ARCADE_OBJECTS = 6;

enum class eArcadeSaveSlot
{
    INVALID,
    STREET_CRIMES_0,
    STREET_CRIMES_1,
    STREET_CRIMES_2,
    STREET_CRIMES_3,
    GG_SPACE_MONKEY,
    BADLANDS_REVENGE,
    RACE_N_CHASE_ROCKET,
    RACE_N_CHASE_STREET,
    RACE_N_CHASE_TRUCKIN,
    WIZARDS_RUIN,
    DEFENDER_OF_THE_FAITH,
    MONKEYS_PARADISE,
    PENETRATOR,
    SWK_CLAW,
    NAZAR_SPEAKS,
    LOVE_PROFESSOR,
    INVADE_AND_PERSUADE,
    AXE_OF_FURY = 21,
    QUB3D,
    CAMHEDZ
};

constexpr Vector3 g_ArcadeBlipCoords[] = {
    {-247.6898f, 6212.915f, 30.944f},
    {1695.1714f, 4785.1177f, 40.9847f},
    {-116.3816f, -1772.1368f, 28.8592f},
    {-599.5152f, 279.6308f, 81.074f},
    {-1273.2231f, -304.1054f, 37.2289f},
    {758.3455f, -815.9312f, 25.2905f}
};

constexpr Vector3 g_ArcadeEntryCoords[] = {
    {-245.64f, 6210.96f, 30.94f},
    {1695.85f, 4783.85f, 41.02f},
    {-115.17f, -1771.64f, 28.86f},
    {-601.11f, 280.47f, 81.04f},
    {-1269.72f, -304.09f, 36.0f},
    {758.46f, -814.57f, 25.3f}
};

constexpr std::pair<Vector3, float> g_ArcadeExitCoords[] = {
    {
        {-248.1222f, 6212.625f, 30.944f},
        136.405f
    },
    {
        {1695.4316f, 4785.1533f, 40.9945f},
        89.645f
    },
    {
        {-116.2575f, -1772.1418f, 28.8594f},
        34.56f
    },
    {
        {-599.5079f, 279.6345f, 81.0742f},
        173.975f
    },
    {
        {-1269.7025f, -305.4233f, 35.9952f},
        250.2f
    },
    {
        {759.0959f, -816.0111f, 25.2974f},
        268.95f
    }
};

constexpr eArcadeSaveSlot g_ArcadeSaveSlots[] = {
    eArcadeSaveSlot::SWK_CLAW,
    eArcadeSaveSlot::AXE_OF_FURY,
    eArcadeSaveSlot::LOVE_PROFESSOR,
    eArcadeSaveSlot::STREET_CRIMES_0,
    eArcadeSaveSlot::STREET_CRIMES_1,
    eArcadeSaveSlot::STREET_CRIMES_2,
    eArcadeSaveSlot::STREET_CRIMES_3,
    eArcadeSaveSlot::BADLANDS_REVENGE,
    eArcadeSaveSlot::INVALID,
    eArcadeSaveSlot::NAZAR_SPEAKS,
    eArcadeSaveSlot::GG_SPACE_MONKEY,
    eArcadeSaveSlot::INVADE_AND_PERSUADE,
    eArcadeSaveSlot::WIZARDS_RUIN,
    eArcadeSaveSlot::CAMHEDZ,
    eArcadeSaveSlot::INVALID,
    eArcadeSaveSlot::RACE_N_CHASE_ROCKET,
    eArcadeSaveSlot::INVALID,
    eArcadeSaveSlot::STREET_CRIMES_0,
    eArcadeSaveSlot::STREET_CRIMES_1,
    eArcadeSaveSlot::STREET_CRIMES_2,
    eArcadeSaveSlot::STREET_CRIMES_3,
    eArcadeSaveSlot::PENETRATOR,
    eArcadeSaveSlot::DEFENDER_OF_THE_FAITH,
    eArcadeSaveSlot::MONKEYS_PARADISE,
    eArcadeSaveSlot::DEFENDER_OF_THE_FAITH,
    eArcadeSaveSlot::PENETRATOR,
    eArcadeSaveSlot::GG_SPACE_MONKEY,
    eArcadeSaveSlot::WIZARDS_RUIN,
    eArcadeSaveSlot::INVADE_AND_PERSUADE,
    eArcadeSaveSlot::QUB3D,
    eArcadeSaveSlot::RACE_N_CHASE_TRUCKIN,
    eArcadeSaveSlot::INVALID,
    eArcadeSaveSlot::QUB3D,
    eArcadeSaveSlot::WIZARDS_RUIN,
    eArcadeSaveSlot::GG_SPACE_MONKEY,
    eArcadeSaveSlot::INVALID,
    eArcadeSaveSlot::INVALID,
    eArcadeSaveSlot::INVALID,
    eArcadeSaveSlot::INVALID,
    eArcadeSaveSlot::INVALID,
    eArcadeSaveSlot::INVALID
};
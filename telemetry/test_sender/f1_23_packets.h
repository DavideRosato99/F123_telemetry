#pragma once
#include <cstdint>

// Each packet is packed with no extra padding
#pragma pack(push, 1)

// ---------------------------------------------------------------------
//                             PACKET HEADER
// ---------------------------------------------------------------------
struct PacketHeader
{
    uint16_t m_packetFormat;           // 2023
    uint8_t m_gameYear;                // Last two digits e.g. 23
    uint8_t m_gameMajorVersion;        // Game major version
    uint8_t m_gameMinorVersion;        // Game minor version
    uint8_t m_packetVersion;           // Version of this packet type
    uint8_t m_packetId;                // Identifier for the packet type
    uint64_t m_sessionUID;             // Unique identifier for the session
    float m_sessionTime;               // Session timestamp
    uint32_t m_frameIdentifier;        // Identifier for the frame the data was retrieved on
    uint32_t m_overallFrameIdentifier; // Overall identifier for the frame
    uint8_t m_playerCarIndex;          // Index of player's car in the array
    uint8_t m_secondaryPlayerCarIndex; // Index of secondary player's car in the array (255 if none)
};

// ---------------------------------------------------------------------
//                          PACKET IDs (for reference)
// ---------------------------------------------------------------------
enum PacketID : uint8_t
{
    MOTION = 0,
    SESSION = 1,
    LAP_DATA = 2,
    EVENT = 3,
    PARTICIPANTS = 4,
    CAR_SETUPS = 5,
    CAR_TELEMETRY = 6,
    CAR_STATUS = 7,
    FINAL_CLASSIFICATION = 8,
    LOBBY_INFO = 9,
    CAR_DAMAGE = 10,
    SESSION_HISTORY = 11,
    TYRE_SETS = 12,
    MOTION_EX = 13
};

// ---------------------------------------------------------------------
//                              MOTION (ID=0)
// ---------------------------------------------------------------------
struct CarMotionData
{
    float m_worldPositionX;
    float m_worldPositionY;
    float m_worldPositionZ;
    float m_worldVelocityX;
    float m_worldVelocityY;
    float m_worldVelocityZ;
    int16_t m_worldForwardDirX;
    int16_t m_worldForwardDirY;
    int16_t m_worldForwardDirZ;
    int16_t m_worldRightDirX;
    int16_t m_worldRightDirY;
    int16_t m_worldRightDirZ;
    float m_gForceLateral;
    float m_gForceLongitudinal;
    float m_gForceVertical;
    float m_yaw;
    float m_pitch;
    float m_roll;
};

struct PacketMotionData
{
    PacketHeader m_header;             // Header
    CarMotionData m_carMotionData[22]; // Data for all cars
    // In F1 23, the rest of the "Motion" data for the player's car was moved to "MotionEx"
    // so the official Motion packet is smaller than in older games.
};

// ---------------------------------------------------------------------
//                          SESSION (ID=1)
// ---------------------------------------------------------------------
struct MarshalZone
{
    float m_zoneStart; // Fraction (0..1) of way through the lap the marshal zone starts
    int8_t m_zoneFlag; // -1 = invalid/unknown, 0 = none, 1 = green, 2 = blue, 3 = yellow
};

struct WeatherForecastSample
{
    uint8_t m_sessionType;           // 0 = unknown, 1 = P1, 2 = P2, etc.
    uint8_t m_timeOffset;            // Time in minutes the forecast is for
    uint8_t m_weather;               // 0=clear,1=light cloud,2=overcast,3=light rain,...
    int8_t m_trackTemperature;       // Track temp in Celsius
    int8_t m_trackTemperatureChange; // 0=up,1=down,2=no change
    int8_t m_airTemperature;         // Air temp in Celsius
    int8_t m_airTemperatureChange;   // 0=up,1=down,2=no change
    uint8_t m_rainPercentage;        // 0-100
};

struct PacketSessionData
{
    PacketHeader m_header;

    uint8_t m_weather;         // 0..5
    int8_t m_trackTemperature; // Celsius
    int8_t m_airTemperature;   // Celsius
    uint8_t m_totalLaps;
    uint16_t m_trackLength;     // metres
    uint8_t m_sessionType;      // 0..13
    int8_t m_trackId;           // -1 for unknown, otherwise see doc
    uint8_t m_formula;          // 0..?
    uint16_t m_sessionTimeLeft; // seconds
    uint16_t m_sessionDuration; // seconds
    uint8_t m_pitSpeedLimit;    // kph
    uint8_t m_gamePaused;       // 0=off,1=on
    uint8_t m_isSpectating;     // 0=off,1=on
    uint8_t m_spectatorCarIndex;
    uint8_t m_sliProNativeSupport; // 0=inactive,1=active
    uint8_t m_numMarshalZones;
    MarshalZone m_marshalZones[21];
    uint8_t m_safetyCarStatus; // 0=none,1=full,2=virtual,3=formation
    uint8_t m_networkGame;     // 0=offline,1=online
    uint8_t m_numWeatherForecastSamples;
    WeatherForecastSample m_weatherForecastSamples[56];
    uint8_t m_forecastAccuracy; // 0=perfect,1=approx
    uint8_t m_aiDifficulty;     // 0..110
    uint32_t m_seasonLinkIdentifier;
    uint32_t m_weekendLinkIdentifier;
    uint32_t m_sessionLinkIdentifier;
    uint8_t m_pitStopWindowIdealLap;
    uint8_t m_pitStopWindowLatestLap;
    uint8_t m_pitStopRejoinPosition;
    uint8_t m_steeringAssist;                  // 0=off,1=on
    uint8_t m_brakingAssist;                   // 0=off..3=high
    uint8_t m_gearboxAssist;                   // 1=manual,2=manual&suggested,3=auto
    uint8_t m_pitAssist;                       // 0=off,1=on
    uint8_t m_pitReleaseAssist;                // 0=off,1=on
    uint8_t m_ERSAssist;                       // 0=off,1=on
    uint8_t m_DRSAssist;                       // 0=off,1=on
    uint8_t m_dynamicRacingLine;               // 0=off,1=corners only,2=full
    uint8_t m_dynamicRacingLineType;           // 0=2D,1=3D
    uint8_t m_gameMode;                        // see doc
    uint8_t m_ruleSet;                         // see doc
    uint32_t m_timeOfDay;                      // minutes since midnight
    uint8_t m_sessionLength;                   // 0=None,...,7=Full
    uint8_t m_speedUnitsLeadPlayer;            // 0=MPH,1=KPH
    uint8_t m_temperatureUnitsLeadPlayer;      // 0=C,1=F
    uint8_t m_speedUnitsSecondaryPlayer;       // 0=MPH,1=KPH
    uint8_t m_temperatureUnitsSecondaryPlayer; // 0=C,1=F
    uint8_t m_numSafetyCarPeriods;
    uint8_t m_numVirtualSafetyCarPeriods;
    uint8_t m_numRedFlagPeriods;
};

// ---------------------------------------------------------------------
//                         LAP DATA (ID=2)
// ---------------------------------------------------------------------
struct LapData
{
    uint32_t m_lastLapTimeInMS;
    uint32_t m_currentLapTimeInMS;
    uint16_t m_sector1TimeInMS;
    uint8_t m_sector1TimeMinutes;
    uint16_t m_sector2TimeInMS;
    uint8_t m_sector2TimeMinutes;
    uint16_t m_deltaToCarInFrontInMS;
    uint16_t m_deltaToRaceLeaderInMS;
    float m_lapDistance;
    float m_totalDistance;
    float m_safetyCarDelta;
    uint8_t m_carPosition;
    uint8_t m_currentLapNum;
    uint8_t m_pitStatus;
    uint8_t m_numPitStops;
    uint8_t m_sector;
    uint8_t m_currentLapInvalid;
    uint8_t m_penalties;
    uint8_t m_totalWarnings;
    uint8_t m_cornerCuttingWarnings;
    uint8_t m_numUnservedDriveThroughPens;
    uint8_t m_numUnservedStopGoPens;
    uint8_t m_gridPosition;
    uint8_t m_driverStatus;
    uint8_t m_resultStatus;
    uint8_t m_pitLaneTimerActive;
    uint16_t m_pitLaneTimeInLaneInMS;
    uint16_t m_pitStopTimerInMS;
    uint8_t m_pitStopShouldServePen;
};

struct PacketLapData
{
    PacketHeader m_header;
    LapData m_lapData[22];

    uint8_t m_timeTrialPBCarIdx;    // 255 if invalid
    uint8_t m_timeTrialRivalCarIdx; // 255 if invalid
};

// ---------------------------------------------------------------------
//                         EVENT (ID=3)
// ---------------------------------------------------------------------
union EventDataDetails
{
    struct
    {
        uint8_t vehicleIdx;
        float lapTime;
    } FastestLap;

    struct
    {
        uint8_t vehicleIdx;
    } Retirement;

    struct
    {
        uint8_t vehicleIdx;
    } TeamMateInPits;

    struct
    {
        uint8_t vehicleIdx;
    } RaceWinner;

    struct
    {
        uint8_t penaltyType;
        uint8_t infringementType;
        uint8_t vehicleIdx;
        uint8_t otherVehicleIdx;
        uint8_t time;
        uint8_t lapNum;
        uint8_t placesGained;
    } Penalty;

    struct
    {
        uint8_t vehicleIdx;
        float speed;
        uint8_t isOverallFastestInSession;
        uint8_t isDriverFastestInSession;
        uint8_t fastestVehicleIdxInSession;
        float fastestSpeedInSession;
    } SpeedTrap;

    struct
    {
        uint8_t numLights;
    } StartLights;

    struct
    {
        uint8_t vehicleIdx;
    } DriveThroughPenaltyServed;

    struct
    {
        uint8_t vehicleIdx;
    } StopGoPenaltyServed;

    struct
    {
        uint32_t flashbackFrameIdentifier;
        float flashbackSessionTime;
    } Flashback;

    struct
    {
        uint32_t buttonStatus;
    } Buttons;

    struct
    {
        uint8_t overtakingVehicleIdx;
        uint8_t beingOvertakenVehicleIdx;
    } Overtake;
};

struct PacketEventData
{
    PacketHeader m_header;
    uint8_t m_eventStringCode[4]; // e.g. "SSTA","SEND","FTLP","RTMT",...
    EventDataDetails m_eventDetails;
};

// ---------------------------------------------------------------------
//                         PARTICIPANTS (ID=4)
// ---------------------------------------------------------------------
struct ParticipantData
{
    uint8_t m_aiControlled;
    uint8_t m_driverId;
    uint8_t m_networkId;
    uint8_t m_teamId;
    uint8_t m_myTeam;
    uint8_t m_raceNumber;
    uint8_t m_nationality;
    char m_name[48];           // UTF-8 null-terminated
    uint8_t m_yourTelemetry;   // 0=restricted,1=public
    uint8_t m_showOnlineNames; // 0=off,1=on
    uint8_t m_platform;        // 1=Steam,3=PSN,4=XB,6=Origin,255=unknown
};

struct PacketParticipantsData
{
    PacketHeader m_header;
    uint8_t m_numActiveCars;
    ParticipantData m_participants[22];
};

// ---------------------------------------------------------------------
//                        CAR SETUPS (ID=5)
// ---------------------------------------------------------------------
struct CarSetupData
{
    uint8_t m_frontWing;
    uint8_t m_rearWing;
    uint8_t m_onThrottle;
    uint8_t m_offThrottle;
    float m_frontCamber;
    float m_rearCamber;
    float m_frontToe;
    float m_rearToe;
    uint8_t m_frontSuspension;
    uint8_t m_rearSuspension;
    uint8_t m_frontAntiRollBar;
    uint8_t m_rearAntiRollBar;
    uint8_t m_frontSuspensionHeight;
    uint8_t m_rearSuspensionHeight;
    uint8_t m_brakePressure;
    uint8_t m_brakeBias;
    float m_rearLeftTyrePressure;
    float m_rearRightTyrePressure;
    float m_frontLeftTyrePressure;
    float m_frontRightTyrePressure;
    uint8_t m_ballast;
    float m_fuelLoad;
};

struct PacketCarSetupData
{
    PacketHeader m_header;
    CarSetupData m_carSetups[22];
};

// ---------------------------------------------------------------------
//                       CAR TELEMETRY (ID=6)
// ---------------------------------------------------------------------
struct CarTelemetryData
{
    uint16_t m_speed;
    float m_throttle;
    float m_steer;
    float m_brake;
    uint8_t m_clutch;
    int8_t m_gear;
    uint16_t m_engineRPM;
    uint8_t m_drs;
    uint8_t m_revLightsPercent;
    uint16_t m_revLightsBitValue;
    uint16_t m_brakesTemperature[4];
    uint8_t m_tyresSurfaceTemperature[4];
    uint8_t m_tyresInnerTemperature[4];
    uint16_t m_engineTemperature;
    float m_tyresPressure[4];
    uint8_t m_surfaceType[4]; // see doc
};

struct PacketCarTelemetryData
{
    PacketHeader m_header;
    CarTelemetryData m_carTelemetryData[22];
    uint8_t m_mfdPanelIndex;
    uint8_t m_mfdPanelIndexSecondaryPlayer;
    int8_t m_suggestedGear; // 0 if no gear suggested
};

// ---------------------------------------------------------------------
//                       CAR STATUS (ID=7)
// ---------------------------------------------------------------------
struct CarStatusData
{
    uint8_t m_tractionControl;
    uint8_t m_antiLockBrakes;
    uint8_t m_fuelMix;
    uint8_t m_frontBrakeBias;
    uint8_t m_pitLimiterStatus;
    float m_fuelInTank;
    float m_fuelCapacity;
    float m_fuelRemainingLaps;
    uint16_t m_maxRPM;
    uint16_t m_idleRPM;
    uint8_t m_maxGears;
    uint8_t m_drsAllowed;
    uint16_t m_drsActivationDistance;
    uint8_t m_actualTyreCompound;
    uint8_t m_visualTyreCompound;
    uint8_t m_tyresAgeLaps;
    int8_t m_vehicleFiaFlags;
    float m_enginePowerICE;
    float m_enginePowerMGUK;
    float m_ersStoreEnergy;
    uint8_t m_ersDeployMode; // 0=none,1=medium,2=hotlap,3=overtake
    float m_ersHarvestedThisLapMGUK;
    float m_ersHarvestedThisLapMGUH;
    float m_ersDeployedThisLap;
    uint8_t m_networkPaused;
};

struct PacketCarStatusData
{
    PacketHeader m_header;
    CarStatusData m_carStatusData[22];
};

// ---------------------------------------------------------------------
//                   FINAL CLASSIFICATION (ID=8)
// ---------------------------------------------------------------------
struct FinalClassificationData
{
    uint8_t m_position;
    uint8_t m_numLaps;
    uint8_t m_gridPosition;
    uint8_t m_points;
    uint8_t m_numPitStops;
    uint8_t m_resultStatus;
    uint32_t m_bestLapTimeInMS;
    double m_totalRaceTime;
    uint8_t m_penaltiesTime;
    uint8_t m_numPenalties;
    uint8_t m_numTyreStints;
    uint8_t m_tyreStintsActual[8];
    uint8_t m_tyreStintsVisual[8];
    uint8_t m_tyreStintsEndLaps[8];
};

struct PacketFinalClassificationData
{
    PacketHeader m_header;
    uint8_t m_numCars;
    FinalClassificationData m_classificationData[22];
};

// ---------------------------------------------------------------------
//                         LOBBY INFO (ID=9)
// ---------------------------------------------------------------------
struct LobbyInfoData
{
    uint8_t m_aiControlled;
    uint8_t m_teamId;
    uint8_t m_nationality;
    uint8_t m_platform; // 1=Steam,3=PS,4=XB,6=Origin,255=unk
    char m_name[48];    // UTF-8
    uint8_t m_carNumber;
    uint8_t m_readyStatus; // 0=not ready,1=ready,2=spectating
};

struct PacketLobbyInfoData
{
    PacketHeader m_header;
    uint8_t m_numPlayers;
    LobbyInfoData m_lobbyPlayers[22];
};

// ---------------------------------------------------------------------
//                         CAR DAMAGE (ID=10)
// ---------------------------------------------------------------------
struct CarDamageData
{
    float m_tyresWear[4];
    uint8_t m_tyresDamage[4];
    uint8_t m_brakesDamage[4];
    uint8_t m_frontLeftWingDamage;
    uint8_t m_frontRightWingDamage;
    uint8_t m_rearWingDamage;
    uint8_t m_floorDamage;
    uint8_t m_diffuserDamage;
    uint8_t m_sidepodDamage;
    uint8_t m_drsFault;
    uint8_t m_ersFault;
    uint8_t m_gearBoxDamage;
    uint8_t m_engineDamage;
    uint8_t m_engineMGUHWear;
    uint8_t m_engineESWear;
    uint8_t m_engineCEWear;
    uint8_t m_engineICEWear;
    uint8_t m_engineMGUKWear;
    uint8_t m_engineTCWear;
    uint8_t m_engineBlown;
    uint8_t m_engineSeized;
};

struct PacketCarDamageData
{
    PacketHeader m_header;
    CarDamageData m_carDamageData[22];
};

// ---------------------------------------------------------------------
//                      SESSION HISTORY (ID=11)
// ---------------------------------------------------------------------
struct LapHistoryData
{
    uint32_t m_lapTimeInMS;
    uint16_t m_sector1TimeInMS;
    uint8_t m_sector1TimeMinutes;
    uint16_t m_sector2TimeInMS;
    uint8_t m_sector2TimeMinutes;
    uint16_t m_sector3TimeInMS;
    uint8_t m_sector3TimeMinutes;
    uint8_t m_lapValidBitFlags; // bit 0 = lap valid, bit 1=sector1, bit2=sector2, bit3=sector3
};

struct TyreStintHistoryData
{
    uint8_t m_endLap;
    uint8_t m_tyreActualCompound;
    uint8_t m_tyreVisualCompound;
};

struct PacketSessionHistoryData
{
    PacketHeader m_header;
    uint8_t m_carIdx;
    uint8_t m_numLaps;
    uint8_t m_numTyreStints;
    uint8_t m_bestLapTimeLapNum;
    uint8_t m_bestSector1LapNum;
    uint8_t m_bestSector2LapNum;
    uint8_t m_bestSector3LapNum;
    LapHistoryData m_lapHistoryData[100];
    TyreStintHistoryData m_tyreStintsHistoryData[8];
};

// ---------------------------------------------------------------------
//                          TYRE SETS (ID=12)
// ---------------------------------------------------------------------
struct TyreSetData
{
    uint8_t m_actualTyreCompound;
    uint8_t m_visualTyreCompound;
    uint8_t m_wear;      // percentage
    uint8_t m_available; // 0=unavailable,1=available
    uint8_t m_recommendedSession;
    uint8_t m_lifeSpan;
    uint8_t m_usableLife;
    int16_t m_lapDeltaTime; // in ms
    uint8_t m_fitted;       // 0=not fitted,1=fitted
};

struct PacketTyreSetsData
{
    PacketHeader m_header;
    uint8_t m_carIdx;
    TyreSetData m_tyreSetData[20];
    uint8_t m_fittedIdx;
};

// ---------------------------------------------------------------------
//                       MOTION EX (ID=13)
// ---------------------------------------------------------------------
struct PacketMotionExData
{
    PacketHeader m_header;

    // Player car extended motion data
    float m_suspensionPosition[4]; // RL, RR, FL, FR
    float m_suspensionVelocity[4];
    float m_suspensionAcceleration[4];
    float m_wheelSpeed[4];
    float m_wheelSlipRatio[4];
    float m_wheelSlipAngle[4];
    float m_wheelLatForce[4];
    float m_wheelLongForce[4];
    float m_heightOfCOGAboveGround;
    float m_localVelocityX;
    float m_localVelocityY;
    float m_localVelocityZ;
    float m_angularVelocityX;
    float m_angularVelocityY;
    float m_angularVelocityZ;
    float m_angularAccelerationX;
    float m_angularAccelerationY;
    float m_angularAccelerationZ;
    float m_frontWheelsAngle;
    float m_wheelVertForce[4];
};

#pragma pack(pop)

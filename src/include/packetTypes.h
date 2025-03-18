#ifndef PACKETTYPES_H
#define PACKETTYPES_H

#include "packetHeader.h"

#pragma pack(push, 1)

// CAR MOTION DATA (0) ------------------------------------
struct CarMotionData
{
    float m_worldPositionX;      // World space X position - metres
    float m_worldPositionY;      // World space Y position
    float m_worldPositionZ;      // World space Z position
    float m_worldVelocityX;      // Velocity in world space X – metres/s
    float m_worldVelocityY;      // Velocity in world space Y
    float m_worldVelocityZ;      // Velocity in world space Z
    uint16_t m_worldForwardDirX; // World space forward X direction (normalised)
    uint16_t m_worldForwardDirY; // World space forward Y direction (normalised)
    uint16_t m_worldForwardDirZ; // World space forward Z direction (normalised)
    uint16_t m_worldRightDirX;   // World space right X direction (normalised)
    uint16_t m_worldRightDirY;   // World space right Y direction (normalised)
    uint16_t m_worldRightDirZ;   // World space right Z direction (normalised)
    float m_gForceLateral;       // Lateral G-Force component
    float m_gForceLongitudinal;  // Longitudinal G-Force component
    float m_gForceVertical;      // Vertical G-Force component
    float m_yaw;                 // Yaw angle in radians
    float m_pitch;               // Pitch angle in radians
    float m_roll;                // Roll angle in radians
};

struct PacketMotionData
{
    PacketHeader m_header;             // Header
    CarMotionData m_carMotionData[22]; // Data for all cars on track
};

// SESSION (1) ---------------------------------
struct MarshalZone
{
    float m_zoneStart; // Fraction (0..1) of way through the lap the marshal zone starts
    int8_t m_zoneFlag; // -1 = invalid/unknown, 0 = none, 1 = green, 2 = blue, 3 = yellow
};

struct WeatherForecastSample
{
    uint8_t m_sessionType;           // 0 = unknown, 1 = P1, 2 = P2, 3 = P3, 4 = Short P, 5 = Q1
                                     // 6 = Q2, 7 = Q3, 8 = Short Q, 9 = OSQ, 10 = R, 11 = R2
                                     // 12 = R3, 13 = Time Trial
    uint8_t m_timeOffset;            // Time in minutes the forecast is for
    uint8_t m_weather;               // Weather - 0 = clear, 1 = light cloud, 2 = overcast
                                     // 3 = light rain, 4 = heavy rain, 5 = storm
    int8_t m_trackTemperature;       // Track temp. in degrees Celsius
    int8_t m_trackTemperatureChange; // Track temp. change – 0 = up, 1 = down, 2 = no change
    int8_t m_airTemperature;         // Air temp. in degrees celsius
    int8_t m_airTemperatureChange;   // Air temp. change – 0 = up, 1 = down, 2 = no change
    uint8_t m_rainPercentage;        // Rain percentage (0-100)
};

struct PacketSessionData
{
    PacketHeader m_header; // Header

    uint8_t m_weather;                                  // Weather - 0 = clear, 1 = light cloud, 2 = overcast
                                                        // 3 = light rain, 4 = heavy rain, 5 = storm
    int8_t m_trackTemperature;                          // Track temp. in degrees celsius
    int8_t m_airTemperature;                            // Air temp. in degrees celsius
    uint8_t m_totalLaps;                                // Total number of laps in this race
    uint16_t m_trackLength;                             // Track length in metres
    uint8_t m_sessionType;                              // 0 = unknown, 1 = P1, 2 = P2, 3 = P3, 4 = Short P
                                                        // 5 = Q1, 6 = Q2, 7 = Q3, 8 = Short Q, 9 = OSQ
                                                        // 10 = R, 11 = R2, 12 = R3, 13 = Time Trial
    int8_t m_trackId;                                   // -1 for unknown, see appendix
    uint8_t m_formula;                                  // Formula, 0 = F1 Modern, 1 = F1 Classic, 2 = F2,
                                                        // 3 = F1 Generic, 4 = Beta, 5 = Supercars
                                                        // 6 = Esports, 7 = F2 2021
    uint16_t m_sessionTimeLeft;                         // Time left in session in seconds
    uint16_t m_sessionDuration;                         // Session duration in seconds
    uint8_t m_pitSpeedLimit;                            // Pit speed limit in kilometres per hour
    uint8_t m_gamePaused;                               // Whether the game is paused – network game only
    uint8_t m_isSpectating;                             // Whether the player is spectating
    uint8_t m_spectatorCarIndex;                        // Index of the car being spectated
    uint8_t m_sliProNativeSupport;                      // SLI Pro support, 0 = inactive, 1 = active
    uint8_t m_numMarshalZones;                          // Number of marshal zones to follow
    MarshalZone m_marshalZones[21];                     // List of marshal zones – max 21
    uint8_t m_safetyCarStatus;                          // 0 = no safety car, 1 = full
                                                        // 2 = virtual, 3 = formation lap
    uint8_t m_networkGame;                              // 0 = offline, 1 = online
    uint8_t m_numWeatherForecastSamples;                // Number of weather samples to follow
    WeatherForecastSample m_weatherForecastSamples[56]; // Array of weather forecast samples
    uint8_t m_forecastAccuracy;                         // 0 = Perfect, 1 = Approximate
    uint8_t m_aiDifficulty;                             // AI Difficulty rating – 0-110
    uint32_t m_seasonLinkIdentifier;                    // Identifier for season - persists across saves
    uint32_t m_weekendLinkIdentifier;                   // Identifier for weekend - persists across saves
    uint32_t m_sessionLinkIdentifier;                   // Identifier for session - persists across saves
    uint8_t m_pitStopWindowIdealLap;                    // Ideal lap to pit on for current strategy (player)
    uint8_t m_pitStopWindowLatestLap;                   // Latest lap to pit on for current strategy (player)
    uint8_t m_pitStopRejoinPosition;                    // Predicted position to rejoin at (player)
    uint8_t m_steeringAssist;                           // 0 = off, 1 = on
    uint8_t m_brakingAssist;                            // 0 = off, 1 = low, 2 = medium, 3 = high
    uint8_t m_gearboxAssist;                            // 1 = manual, 2 = manual & suggested gear, 3 = auto
    uint8_t m_pitAssist;                                // 0 = off, 1 = on
    uint8_t m_pitReleaseAssist;                         // 0 = off, 1 = on
    uint8_t m_ERSAssist;                                // 0 = off, 1 = on
    uint8_t m_DRSAssist;                                // 0 = off, 1 = on
    uint8_t m_dynamicRacingLine;                        // 0 = off, 1 = corners only, 2 = full
    uint8_t m_dynamicRacingLineType;                    // 0 = 2D, 1 = 3D
    uint8_t m_gameMode;                                 // Game mode id - see appendix
    uint8_t m_ruleSet;                                  // Ruleset - see appendix
    uint32_t m_timeOfDay;                               // Local time of day - minutes since midnight
    uint8_t m_sessionLength;                            // 0 = None, 2 = Very Short, 3 = Short, 4 = Medium
                                                        // 5 = Medium Long, 6 = Long, 7 = Full
    uint8_t m_speedUnitsLeadPlayer;                     // 0 = MPH, 1 = KPH
    uint8_t m_temperatureUnitsLeadPlayer;               // 0 = Celsius, 1 = Fahrenheit
    uint8_t m_speedUnitsSecondaryPlayer;                // 0 = MPH, 1 = KPH
    uint8_t m_temperatureUnitsSecondaryPlayer;          // 0 = Celsius, 1 = Fahrenheit
    uint8_t m_numSafetyCarPeriods;                      // Number of safety cars called during session
    uint8_t m_numVirtualSafetyCarPeriods;               // Number of virtual safety cars called
    uint8_t m_numRedFlagPeriods;                        // Number of red flags called during session
};

// LAP DATA (2) --------------------------------
struct LapData
{
    uint32_t m_lastLapTimeInMS;            // Last lap time in milliseconds
    uint32_t m_currentLapTimeInMS;         // Current time around the lap in milliseconds
    uint16_t m_sector1TimeInMS;            // Sector 1 time in milliseconds
    uint8_t m_sector1TimeMinutes;          // Sector 1 whole minute part
    uint16_t m_sector2TimeInMS;            // Sector 2 time in milliseconds
    uint8_t m_sector2TimeMinutes;          // Sector 2 whole minute part
    uint16_t m_deltaToCarInFrontInMS;      // Time delta to car in front in milliseconds
    uint16_t m_deltaToRaceLeaderInMS;      // Time delta to race leader in milliseconds
    float m_lapDistance;                   // Distance vehicle is around current lap in metres – could
                                           // be negative if line hasn’t been crossed yet
    float m_totalDistance;                 // Total distance travelled in session in metres – could
                                           // be negative if line hasn’t been crossed yet
    float m_safetyCarDelta;                // Delta in seconds for safety car
    uint8_t m_carPosition;                 // Car race position
    uint8_t m_currentLapNum;               // Current lap number
    uint8_t m_pitStatus;                   // 0 = none, 1 = pitting, 2 = in pit area
    uint8_t m_numPitStops;                 // Number of pit stops taken in this race
    uint8_t m_sector;                      // 0 = sector1, 1 = sector2, 2 = sector3
    uint8_t m_currentLapInvalid;           // Current lap invalid - 0 = valid, 1 = invalid
    uint8_t m_penalties;                   // Accumulated time penalties in seconds to be added
    uint8_t m_totalWarnings;               // Accumulated number of warnings issued
    uint8_t m_cornerCuttingWarnings;       // Accumulated number of corner cutting warnings issued
    uint8_t m_numUnservedDriveThroughPens; // Num drive through pens left to serve
    uint8_t m_numUnservedStopGoPens;       // Num stop go pens left to serve
    uint8_t m_gridPosition;                // Grid position the vehicle started the race in
    uint8_t m_driverStatus;                // Status of driver - 0 = in garage, 1 = flying lap
                                           // 2 = in lap, 3 = out lap, 4 = on track
    uint8_t m_resultStatus;                // Result status - 0 = invalid, 1 = inactive, 2 = active
                                           // 3 = finished, 4 = didnotfinish, 5 = disqualified
                                           // 6 = not classified, 7 = retired
    uint8_t m_pitLaneTimerActive;          // Pit lane timing, 0 = inactive, 1 = active
    uint16_t m_pitLaneTimeInLaneInMS;      // If active, the current time spent in the pit lane in ms
    uint16_t m_pitStopTimerInMS;           // Time of the actual pit stop in ms
    uint8_t m_pitStopShouldServePen;       // Whether the car should serve a penalty at this stop
};

struct PacketLapData
{
    PacketHeader m_header; // Header

    LapData m_lapData[22];          // Lap data for all cars on track
    uint8_t m_timeTrialPBCarIdx;    // Index of Personal Best car in time trial (255 if invalid)
    uint8_t m_timeTrialRivalCarIdx; // Index of Rival car in time trial (255 if invalid)
};

// EVENT (3) -----------------------------
// The event details packet is different for each type of event.
// Make sure only the correct type is interpreted.
union EventDataDetails
{
    struct
    {
        uint8_t vehicleIdx; // Vehicle index of car achieving fastest lap
        float lapTime;      // Lap time is in seconds
    } FastestLap;

    struct
    {
        uint8_t vehicleIdx; // Vehicle index of car retiring
    } Retirement;

    struct
    {
        uint8_t vehicleIdx; // Vehicle index of team mate
    } TeamMateInPits;

    struct
    {
        uint8_t vehicleIdx; // Vehicle index of the race winner
    } RaceWinner;

    struct
    {
        uint8_t penaltyType;      // Penalty type – see Appendices
        uint8_t infringementType; // Infringement type – see Appendices
        uint8_t vehicleIdx;       // Vehicle index of the car the penalty is applied to
        uint8_t otherVehicleIdx;  // Vehicle index of the other car involved
        uint8_t time;             // Time gained, or time spent doing action in seconds
        uint8_t lapNum;           // Lap the penalty occurred on
        uint8_t placesGained;     // Number of places gained by this
    } Penalty;

    struct
    {
        uint8_t vehicleIdx;                 // Vehicle index of the vehicle triggering speed trap
        float speed;                        // Top speed achieved in kilometres per hour
        uint8_t isOverallFastestInSession;  // Overall fastest speed in session = 1, otherwise 0
        uint8_t isDriverFastestInSession;   // Fastest speed for driver in session = 1, otherwise 0
        uint8_t fastestVehicleIdxInSession; // Vehicle index of the vehicle that is the fastest
                                            // in this session
        float fastestSpeedInSession;        // Speed of the vehicle that is the fastest
                                            // in this session
    } SpeedTrap;

    struct
    {
        uint8_t numLights; // Number of lights showing
    } StartLIghts;

    struct
    {
        uint8_t vehicleIdx; // Vehicle index of the vehicle serving drive through
    } DriveThroughPenaltyServed;

    struct
    {
        uint8_t vehicleIdx; // Vehicle index of the vehicle serving stop go
    } StopGoPenaltyServed;

    struct
    {
        uint32_t flashbackFrameIdentifier; // Frame identifier flashed back to
        float flashbackSessionTime;        // Session time flashed back to
    } Flashback;

    struct
    {
        uint32_t buttonStatus; // Bit flags specifying which buttons are being pressed
                               // currently - see appendices
    } Buttons;

    struct
    {
        uint8_t overtakingVehicleIdx;     // Vehicle index of the vehicle overtaking
        uint8_t beingOvertakenVehicleIdx; // Vehicle index of the vehicle being overtaken
    } Overtake;
};

struct PacketEventData
{
    PacketHeader m_header; // Header

    uint8_t m_eventStringCode[4];    // Event string code, see below
    EventDataDetails m_eventDetails; // Event details - should be interpreted differently
                                     // for each type
};

// Session Started      |   “SSTA”   |  Sent when the session starts
// Session Ended        |   “SEND”   |  Sent when the session ends
// Fastest Lap          |   “FTLP”   |  When a driver achieves the fastest lap
// Retirement           |   “RTMT”   |  When a driver retires
// DRS enabled          |   “DRSE”   |  Race control have enabled DRS
// DRS disabled         |   “DRSD”   |  Race control have disabled DRS
// Team mate in pits    |   “TMPT”   |  Your team mate has entered the pits
// Chequered flag       |   “CHQF”   |  The chequered flag has been waved
// Race Winner          |   “RCWN”   |  The race winner is announced
// Penalty Issued       |   “PENA”   |  A penalty has been issued – details in event
// Speed Trap Triggered |   “SPTP”   |  Speed trap has been triggered by fastest speed
// Start lights         |   “STLG”   |  Start lights – number shown
// Lights out           |   “LGOT”   |  Lights out
// Drive through served |   “DTSV”   |  Drive through penalty served
// Stop go served       |   “SGSV”   |  Stop go penalty served
// Flashback            |   “FLBK”   |  Flashback activated
// Button status        |   “BUTN”   |  Button status changed
// Red Flag             |   “RDFL”   |  Red flag shown
// Overtake             |   “OVTK”   |  Overtake occurred

// PARTICIPANTS (4)----------------------------------
struct ParticipantData
{
    uint8_t m_aiControlled;    // Whether the vehicle is AI (1) or Human (0) controlled
    uint8_t m_driverId;        // Driver id - see appendix, 255 if network human
    uint8_t m_networkId;       // Network id – unique identifier for network players
    uint8_t m_teamId;          // Team id - see appendix
    uint8_t m_myTeam;          // My team flag – 1 = My Team, 0 = otherwise
    uint8_t m_raceNumber;      // Race number of the car
    uint8_t m_nationality;     // Nationality of the driver
    char m_name[48];           // Name of participant in UTF-8 format – null terminated
                               // Will be truncated with … (U+2026) if too long
    uint8_t m_yourTelemetry;   // The player's UDP setting, 0 = restricted, 1 = public
    uint8_t m_showOnlineNames; // The player's show online names setting, 0 = off, 1 = on
    uint8_t m_platform;        // 1 = Steam, 3 = PlayStation, 4 = Xbox, 6 = Origin, 255 = unknown
};

struct PacketParticipantsData
{
    PacketHeader m_header; // Header

    uint8_t m_numActiveCars; // Number of active cars in the data – should match number of
                             // cars on HUD
    ParticipantData m_participants[22];
};

// CAR SETUP (5) -----------------------------------------
struct CarSetupData
{
    uint8_t m_frontWing;             // Front wing aero
    uint8_t m_rearWing;              // Rear wing aero
    uint8_t m_onThrottle;            // Differential adjustment on throttle (percentage)
    uint8_t m_offThrottle;           // Differential adjustment off throttle (percentage)
    float m_frontCamber;             // Front camber angle (suspension geometry)
    float m_rearCamber;              // Rear camber angle (suspension geometry)
    float m_frontToe;                // Front toe angle (suspension geometry)
    float m_rearToe;                 // Rear toe angle (suspension geometry)
    uint8_t m_frontSuspension;       // Front suspension
    uint8_t m_rearSuspension;        // Rear suspension
    uint8_t m_frontAntiRollBar;      // Front anti-roll bar
    uint8_t m_rearAntiRollBar;       // Front anti-roll bar
    uint8_t m_frontSuspensionHeight; // Front ride height
    uint8_t m_rearSuspensionHeight;  // Rear ride height
    uint8_t m_brakePressure;         // Brake pressure (percentage)
    uint8_t m_brakeBias;             // Brake bias (percentage)
    float m_rearLeftTyrePressure;    // Rear left tyre pressure (PSI)
    float m_rearRightTyrePressure;   // Rear right tyre pressure (PSI)
    float m_frontLeftTyrePressure;   // Front left tyre pressure (PSI)
    float m_frontRightTyrePressure;  // Front right tyre pressure (PSI)
    uint8_t m_ballast;               // Ballast
    float m_fuelLoad;                // Fuel load
};

struct PacketCarSetupData
{
    PacketHeader m_header; // Header

    CarSetupData m_carSetups[22];
};

// CAR TELEMETRY DATA (6) ---------------------------------
struct CarTelemetryData
{
    uint16_t m_speed;                     // Speed of car in kilometres per hour
    float m_throttle;                     // Amount of throttle applied (0.0 to 1.0)
    float m_steer;                        // Steering (-1.0 (full lock left) to 1.0 (full lock right))
    float m_brake;                        // Amount of brake applied (0.0 to 1.0)
    uint8_t m_clutch;                     // Amount of clutch applied (0 to 100)
    int8_t m_gear;                        // Gear selected (1-8, N=0, R=-1)
    uint16_t m_engineRPM;                 // Engine RPM
    uint8_t m_drs;                        // 0 = off, 1 = on
    uint8_t m_revLightsPercent;           // Rev lights indicator (percentage)
    uint16_t m_revLightsBitValue;         // Rev lights (bit 0 = leftmost LED, bit 14 = rightmost LED)
    uint16_t m_brakesTemperature[4];      // Brakes temperature (celsius)
    uint8_t m_tyresSurfaceTemperature[4]; // Tyres surface temperature (celsius)
    uint8_t m_tyresInnerTemperature[4];   // Tyres inner temperature (celsius)
    uint16_t m_engineTemperature;         // Engine temperature (celsius)
    float m_tyresPressure[4];             // Tyres pressure (PSI)
    uint8_t m_surfaceType[4];             // Driving surface, see appendices
};

struct PacketCarTelemetryData
{
    PacketHeader m_header; // Header
    CarTelemetryData m_carTelemetryData[22];
    uint8_t m_mfdPanelIndex;                // Index of MFD panel open - 255 = MFD closed
                                            // Single player, race – 0 = Car setup, 1 = Pits
                                            // 2 = Damage, 3 =  Engine, 4 = Temperatures
                                            // May vary depending on game mode
    uint8_t m_mfdPanelIndexSecondaryPlayer; // See above
    int8_t m_suggestedGear;                 // Suggested gear for the player (1-8)
                                            // 0 if no gear suggested
};

// CAR STATUS (7) ----------------------------
struct CarStatusData
{
    uint8_t m_tractionControl;        // Traction control - 0 = off, 1 = medium, 2 = full
    uint8_t m_antiLockBrakes;         // 0 (off) - 1 (on)
    uint8_t m_fuelMix;                // Fuel mix - 0 = lean, 1 = standard, 2 = rich, 3 = max
    uint8_t m_frontBrakeBias;         // Front brake bias (percentage)
    uint8_t m_pitLimiterStatus;       // Pit limiter status - 0 = off, 1 = on
    float m_fuelInTank;               // Current fuel mass
    float m_fuelCapacity;             // Fuel capacity
    float m_fuelRemainingLaps;        // Fuel remaining in terms of laps (value on MFD)
    uint16_t m_maxRPM;                // Cars max RPM, point of rev limiter
    uint16_t m_idleRPM;               // Cars idle RPM
    uint8_t m_maxGears;               // Maximum number of gears
    uint8_t m_drsAllowed;             // 0 = not allowed, 1 = allowed
    uint16_t m_drsActivationDistance; // 0 = DRS not available, non-zero - DRS will be available
                                      // in [X] metres
    uint8_t m_actualTyreCompound;     // F1 Modern - 16 = C5, 17 = C4, 18 = C3, 19 = C2, 20 = C1
                                      // 21 = C0, 7 = inter, 8 = wet
                                      // F1 Classic - 9 = dry, 10 = wet
                                      // F2 – 11 = super soft, 12 = soft, 13 = medium, 14 = hard
                                      // 15 = wet
    uint8_t m_visualTyreCompound;     // F1 visual (can be different from actual compound)
                                      // 16 = soft, 17 = medium, 18 = hard, 7 = inter, 8 = wet
                                      // F1 Classic – same as above
                                      // F2 ‘19, 15 = wet, 19 – super soft, 20 = soft
                                      // 21 = medium , 22 = hard
    uint8_t m_tyresAgeLaps;           // Age in laps of the current set of tyres
    int8_t m_vehicleFiaFlags;         // -1 = invalid/unknown, 0 = none, 1 = green
                                      // 2 = blue, 3 = yellow
    float m_enginePowerICE;           // Engine power output of ICE (W)
    float m_enginePowerMGUK;          // Engine power output of MGU-K (W)
    float m_ersStoreEnergy;           // ERS energy store in Joules
    uint8_t m_ersDeployMode;          // ERS deployment mode, 0 = none, 1 = medium
                                      // 2 = hotlap, 3 = overtake
    float m_ersHarvestedThisLapMGUK;  // ERS energy harvested this lap by MGU-K
    float m_ersHarvestedThisLapMGUH;  // ERS energy harvested this lap by MGU-H
    float m_ersDeployedThisLap;       // ERS energy deployed this lap
    uint8_t m_networkPaused;          // Whether the car is paused in a network game
};

struct PacketCarStatusData
{
    PacketHeader m_header; // Header

    CarStatusData m_carStatusData[22];
};

// TYRE SETS (12) --------------------------------------------
struct TyreSetData
{
    uint8_t m_actualTyreCompound; // Actual tyre compound used
    uint8_t m_visualTyreCompound; // Visual tyre compound used
    uint8_t m_wear;               // Tyre wear (percentage)
    uint8_t m_available;          // Whether this set is currently available
    uint8_t m_recommendedSession; // Recommended session for tyre set
    uint8_t m_lifeSpan;           // Laps left in this tyre set
    uint8_t m_usableLife;         // Max number of laps recommended for this compound
    int16_t m_lapDeltaTime;       // Lap delta time in milliseconds compared to fitted set
    uint8_t m_fitted;             // Whether the set is fitted or not
};

struct PacketTyreSetsData
{
    PacketHeader m_header; // Header

    uint8_t m_carIdx; // Index of the car this data relates to

    TyreSetData m_tyreSetData[20]; // 13 (dry) + 7 (wet)

    uint8_t m_fittedIdx; // Index into array of fitted tyre
};

// MOTION EX (13) -------------------------
struct PacketMotionExData
{
    PacketHeader m_header; // Header

    // Extra player car ONLY data
    float m_suspensionPosition[4];     // Note: All wheel arrays have the following order:
    float m_suspensionVelocity[4];     // RL, RR, FL, FR
    float m_suspensionAcceleration[4]; // RL, RR, FL, FR
    float m_wheelSpeed[4];             // Speed of each wheel
    float m_wheelSlipRatio[4];         // Slip ratio for each wheel
    float m_wheelSlipAngle[4];         // Slip angles for each wheel
    float m_wheelLatForce[4];          // Lateral forces for each wheel
    float m_wheelLongForce[4];         // Longitudinal forces for each wheel
    float m_heightOfCOGAboveGround;    // Height of centre of gravity above ground
    float m_localVelocityX;            // Velocity in local space – metres/s
    float m_localVelocityY;            // Velocity in local space
    float m_localVelocityZ;            // Velocity in local space
    float m_angularVelocityX;          // Angular velocity x-component – radians/s
    float m_angularVelocityY;          // Angular velocity y-component
    float m_angularVelocityZ;          // Angular velocity z-component
    float m_angularAccelerationX;      // Angular acceleration x-component – radians/s/s
    float m_angularAccelerationY;      // Angular acceleration y-component
    float m_angularAccelerationZ;      // Angular acceleration z-component
    float m_frontWheelsAngle;          // Current front wheels angle in radians
    float m_wheelVertForce[4];         // Vertical forces for each wheel
};

#pragma pack(pop)

#endif // PACKETTYPES_H

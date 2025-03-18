#include "include/telemetryHandler.h"
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <cstring>
#include <filesystem>

TelemetryHandler::TelemetryHandler(const std::string &folderPath, const char *plotterIp, uint16_t plotterPort)
{
    std::cout << "Initializing telemetry files in folder: " << folderPath << std::endl; // Debugging print

    initializeFiles(folderPath);

    // ✅ Setup UDP Socket for Rust Plotter
    plotterSocket = socket(AF_INET, SOCK_DGRAM, 0);
    memset(&plotterAddr, 0, sizeof(plotterAddr));
    plotterAddr.sin_family = AF_INET;
    plotterAddr.sin_port = htons(plotterPort);
    inet_pton(AF_INET, plotterIp, &plotterAddr.sin_addr);
}

TelemetryHandler::~TelemetryHandler()
{
    if (motionFile.is_open())
        motionFile.close();
    if (sessionFile.is_open())
        sessionFile.close();
    if (lapDataFile.is_open())
        lapDataFile.close();
    if (eventFile.is_open())
        eventFile.close();
    if (carSetupFile.is_open())
        carSetupFile.close();
    if (carTelemetryFile.is_open())
        carTelemetryFile.close();
    if (carStatusFile.is_open())
        carStatusFile.close();

    close(plotterSocket);
}

void TelemetryHandler::initializeFiles(const std::string &folderPath)
{
    std::cout << "Opening telemetry log files..." << std::endl; // Debugging print

    // Ensure directory exists
    std::filesystem::create_directories(folderPath);

    // ✅ Open each file properly
    motionFile.open(std::filesystem::path(folderPath) / "motionTelemetry.csv", std::ios::out);
    sessionFile.open(std::filesystem::path(folderPath) / "sessionTelemetry.csv", std::ios::out);
    lapDataFile.open(std::filesystem::path(folderPath) / "lapTelemetry.csv", std::ios::out);
    eventFile.open(std::filesystem::path(folderPath) / "eventTelemetry.csv", std::ios::out);
    carSetupFile.open(std::filesystem::path(folderPath) / "carSetupTelemetry.csv", std::ios::out);
    carTelemetryFile.open(std::filesystem::path(folderPath) / "carTelemetry.csv", std::ios::out);
    carStatusFile.open(std::filesystem::path(folderPath) / "carStatusTelemetry.csv", std::ios::out);
    tyreSetsFile.open(std::filesystem::path(folderPath) / "tyreSetTemeletry.csv", std::ios::out);
    motionExFile.open(std::filesystem::path(folderPath) / "motionExTelemetry.csv", std::ios::out);

    // ✅ Validate file openings
    if (!carTelemetryFile.is_open())
    {
        std::cerr << "Error: Failed to open carTelemetry.csv!" << std::endl;
        exit(EXIT_FAILURE); // Prevent segfault by stopping early
    }

    // ✅ Write headers
    if (motionFile.is_open())
        motionFile << "SessionTime,world_x_m,world_y_m,world_z_m,vx_m/s,vy_m/s,vz_m/s,world_fw_x,world_fw_y,world_fw_z,world_rg_x,world_rg_y,world_rg_z,lat_g,long_g,vert_g,yaw_rad,pitch_rad,roll_rad\n";

    if (sessionFile.is_open())
        sessionFile << "SessionTime,Weather,TrackTemp,AirTemp,TotalLaps,TrackLength,SessionType,"
                       "TrackId,Formula,SessionTimeLeft,SessionDuration,PitSpeedLimit,SafetyCarStatus,"
                       "NumMarshalZones,NumWeatherForecastSamples\n";

    if (lapDataFile.is_open())
        lapDataFile << "SessionTime,LastLapTime,CurrentLapTime,Sector1Time,Sector1Minutes,Sector2Time,Sector2Minutes,"
                       "DeltaToCarInFront,DeltaToRaceLeader,LapDistance,TotalDistance,SafetyCarDelta,"
                       "CarPosition,CurrentLapNum,PitStatus,NumPitStops,Sector,CurrentLapInvalid,Penalties,TotalWarnings,"
                       "CornerCutWarnings,NumUnservedDriveThroughPens,NumUnservedStopGoPens,GridPosition,"
                       "DriverStatus,ResultStatus,PitLaneTimerActive,PitLaneTimeInLane,PitStopTimer,ShouldServePenalty\n";

    if (eventFile.is_open())
        eventFile << "SessionTime,EventType,VehicleIdx,LapTime,PenaltyType,InfringementType,OtherVehicleIdx,Time,LapNum,PlacesGained,"
                     "Speed,IsOverallFastest,IsDriverFastest,FastestVehicleIdx,FastestSpeed,"
                     "NumLights,FlashbackFrame,FlashbackSessionTime,ButtonStatus,OvertakingVehicle,BeingOvertakenVehicle\n";

    if (carSetupFile.is_open())
        carSetupFile << "SessionTime,FrontWing,RearWing,OnThrottle,OffThrottle,FrontCamber,RearCamber,"
                        "FrontToe,RearToe,FrontSuspension,RearSuspension,FrontAntiRollBar,RearAntiRollBar,"
                        "FrontSuspensionHeight,RearSuspensionHeight,BrakePressure,BrakeBias,"
                        "RearLeftTyrePressure,RearRightTyrePressure,FrontLeftTyrePressure,FrontRightTyrePressure,"
                        "Ballast,FuelLoad\n";

    if (carTelemetryFile.is_open())
        carTelemetryFile << "SessionTime,Speed,Gear,RPM\n";

    if (carStatusFile.is_open())
        carStatusFile << "SessionTime,TractionControl,AntiLockBrakes,FuelMix,FrontBrakeBias,PitLimiterStatus,"
                         "FuelInTank,FuelCapacity,FuelRemainingLaps,MaxRPM,IdleRPM,MaxGears,DRSAllowed,"
                         "DRSActivationDistance,ActualTyreCompound,VisualTyreCompound,TyresAgeLaps,VehicleFIAFlags,"
                         "EnginePowerICE,EnginePowerMGUK,ERSStoreEnergy,ERSDeployMode,ERSHarvestedThisLapMGUK,"
                         "ERSHarvestedThisLapMGUH,ERSDeployedThisLap,NetworkPaused\n";

    if (tyreSetsFile.is_open())
        tyreSetsFile << "CarIdx,TyreIdx,ActualTyreCompound,VisualTyreCompound,TyreWear,Available,"
                        "RecommendedSession,LifeSpan,UsableLife,LapDeltaTime,Fitted\n";

    if (motionExFile.is_open())
        motionExFile << "SessionTime,"
                        "SuspPosRL,SuspPosRR,SuspPosFL,SuspPosFR,"
                        "SuspVelRL,SuspVelRR,SuspVelFL,SuspVelFR,"
                        "SuspAccRL,SuspAccRR,SuspAccFL,SuspAccFR,"
                        "WheelSpeedRL,WheelSpeedRR,WheelSpeedFL,WheelSpeedFR,"
                        "WheelSlipRatioRL,WheelSlipRatioRR,WheelSlipRatioFL,WheelSlipFR,"
                        "WheelSlipAngleRL,WheelSlipAngleRR,WheelSlipAngleFL,WheelSlipAngleFR,"
                        "WheelLatForceRL,WheelLatForceRR,WheelLatForceFL,WheelLatForceFR,"
                        "WheelLongForceRL,WheelLongForceRR,WheelLongForceFL,WheelLongForceFR,"
                        "COG_Height,LocalVelX,LocalVelY,LocalVelZ,"
                        "AngularVelX,AngularVelY,AngularVelZ,"
                        "AngularAccX,AngularAccY,AngularAccZ,"
                        "FrontWheelsAngle,"
                        "WheelVertForceRL,WheelVertForceRR,WheelVertForceFL,WheelVertForceFR\n";
}

void TelemetryHandler::processPacket(const char *buffer, size_t size)
{
    if (size < sizeof(PacketHeader))
        return;

    const PacketHeader *header = reinterpret_cast<const PacketHeader *>(buffer);

    if (header->m_packetId == 0 && size >= sizeof(PacketMotionData))
    {
        handleMotionTelemetry(reinterpret_cast<const PacketMotionData *>(buffer));
    }

    if (header->m_packetId == 1 && size >= sizeof(PacketSessionData))
    {
        handleSessionTelemetry(reinterpret_cast<const PacketSessionData *>(buffer));
    }

    if (header->m_packetId == 2 && size >= sizeof(PacketLapData))
    {
        handleLapDataTelemetry(reinterpret_cast<const PacketLapData *>(buffer));
    }

    if (header->m_packetId == 3 && size >= sizeof(PacketEventData))
    {
        handleEventTelemetry(reinterpret_cast<const PacketEventData *>(buffer));
    }

    if (header->m_packetId == 5 && size >= sizeof(PacketCarSetupData))
    {
        handleCarSetupTelemetry(reinterpret_cast<const PacketCarSetupData *>(buffer));
    }

    if (header->m_packetId == 6 && size >= sizeof(PacketCarTelemetryData))
    {
        handleCarTelemetry(reinterpret_cast<const PacketCarTelemetryData *>(buffer));
    }

    if (header->m_packetId == 7 && size >= sizeof(PacketCarStatusData))
    {
        handleCarStatusTelemetry(reinterpret_cast<const PacketCarStatusData *>(buffer));
    }

    if (header->m_packetId == 12 && size >= sizeof(PacketTyreSetsData))
    {
        handleTyreSetsTelemetry(reinterpret_cast<const PacketTyreSetsData *>(buffer));
    }

    if (header->m_packetId == 13 && size >= sizeof(PacketMotionExData))
    {
        handleMotionExTelemetry(reinterpret_cast<const PacketMotionExData *>(buffer));
    }
}

void TelemetryHandler::handleMotionTelemetry(const PacketMotionData *telemetry)
{
    const CarMotionData &myCar = telemetry->m_carMotionData[telemetry->m_header.m_playerCarIndex];

    float sessionTime = telemetry->m_header.m_sessionTime;
    float world_x_m = myCar.m_worldPositionX;
    float world_y_m = myCar.m_worldPositionY;
    float world_z_m = myCar.m_worldPositionZ;
    float vx_m_s = myCar.m_worldVelocityX;
    float vy_m_s = myCar.m_worldVelocityY;
    float vz_m_s = myCar.m_worldVelocityZ;
    float world_fw_x = static_cast<float>(myCar.m_worldForwardDirX) / 32767.0f; // Normalize
    float world_fw_y = static_cast<float>(myCar.m_worldForwardDirY) / 32767.0f;
    float world_fw_z = static_cast<float>(myCar.m_worldForwardDirZ) / 32767.0f;
    float world_rg_x = static_cast<float>(myCar.m_worldRightDirX) / 32767.0f;
    float world_rg_y = static_cast<float>(myCar.m_worldRightDirY) / 32767.0f;
    float world_rg_z = static_cast<float>(myCar.m_worldRightDirZ) / 32767.0f;
    float lat_g = myCar.m_gForceLateral;
    float long_g = myCar.m_gForceLongitudinal;
    float vert_g = myCar.m_gForceVertical;
    float yaw_rad = myCar.m_yaw;
    float pitch_rad = myCar.m_pitch;
    float roll_rad = myCar.m_roll;

    // ✅ Prevent segmentation fault: Ensure file is open before writing
    if (motionFile.is_open())
    {
        motionFile << sessionTime << "," << world_x_m << "," << world_y_m << "," << world_z_m << ","
                   << vx_m_s << "," << vy_m_s << "," << vz_m_s << ","
                   << world_fw_x << "," << world_fw_y << "," << world_fw_z << ","
                   << world_rg_x << "," << world_rg_y << "," << world_rg_z << ","
                   << lat_g << "," << long_g << "," << vert_g << ","
                   << yaw_rad << "," << pitch_rad << "," << roll_rad << "\n";
    }
    else
    {
        std::cerr << "[ERROR] motionTelemetry.csv file is not open!" << std::endl;
    }
}

void TelemetryHandler::handleSessionTelemetry(const PacketSessionData *telemetry)
{
    float sessionTime = telemetry->m_header.m_sessionTime;
    uint8_t weather = telemetry->m_weather;
    int8_t trackTemperature = telemetry->m_trackTemperature;
    int8_t airTemperature = telemetry->m_airTemperature;
    uint8_t totalLaps = telemetry->m_totalLaps;
    uint16_t trackLength = telemetry->m_trackLength;
    uint8_t sessionType = telemetry->m_sessionType;
    int8_t trackId = telemetry->m_trackId;
    uint8_t formula = telemetry->m_formula;
    uint16_t sessionTimeLeft = telemetry->m_sessionTimeLeft;
    uint16_t sessionDuration = telemetry->m_sessionDuration;
    uint8_t pitSpeedLimit = telemetry->m_pitSpeedLimit;
    uint8_t safetyCarStatus = telemetry->m_safetyCarStatus;
    uint8_t numMarshalZones = telemetry->m_numMarshalZones;
    uint8_t numWeatherForecastSamples = telemetry->m_numWeatherForecastSamples;

    // ✅ Prevent segmentation fault: Ensure file is open before writing
    if (sessionFile.is_open())
    {
        sessionFile << sessionTime << "," << (int)weather << "," << (int)trackTemperature << "," << (int)airTemperature << ","
                    << (int)totalLaps << "," << trackLength << "," << (int)sessionType << ","
                    << (int)trackId << "," << (int)formula << "," << sessionTimeLeft << "," << sessionDuration << ","
                    << (int)pitSpeedLimit << "," << (int)safetyCarStatus << "," << (int)numMarshalZones << ","
                    << (int)numWeatherForecastSamples << "\n";
    }
    else
    {
        std::cerr << "[ERROR] sessionTelemetry.csv file is not open!" << std::endl;
    }
}

void TelemetryHandler::handleLapDataTelemetry(const PacketLapData *telemetry)
{
    const LapData &myCar = telemetry->m_lapData[telemetry->m_header.m_playerCarIndex];

    float sessionTime = telemetry->m_header.m_sessionTime;
    uint32_t lastLapTimeInMS = myCar.m_lastLapTimeInMS;
    uint32_t currentLapTimeInMS = myCar.m_currentLapTimeInMS;
    uint16_t sector1TimeInMS = myCar.m_sector1TimeInMS;
    uint8_t sector1TimeMinutes = myCar.m_sector1TimeMinutes;
    uint16_t sector2TimeInMS = myCar.m_sector2TimeInMS;
    uint8_t sector2TimeMinutes = myCar.m_sector2TimeMinutes;
    uint16_t deltaToCarInFrontInMS = myCar.m_deltaToCarInFrontInMS;
    uint16_t deltaToRaceLeaderInMS = myCar.m_deltaToRaceLeaderInMS;
    float lapDistance = myCar.m_lapDistance;
    float totalDistance = myCar.m_totalDistance;
    float safetyCarDelta = myCar.m_safetyCarDelta;
    uint8_t carPosition = myCar.m_carPosition;
    uint8_t currentLapNum = myCar.m_currentLapNum;
    uint8_t pitStatus = myCar.m_pitStatus;
    uint8_t numPitStops = myCar.m_numPitStops;
    uint8_t sector = myCar.m_sector;
    uint8_t currentLapInvalid = myCar.m_currentLapInvalid;
    uint8_t penalties = myCar.m_penalties;
    uint8_t totalWarnings = myCar.m_totalWarnings;
    uint8_t cornerCuttingWarnings = myCar.m_cornerCuttingWarnings;
    uint8_t numUnservedDriveThroughPens = myCar.m_numUnservedDriveThroughPens;
    uint8_t numUnservedStopGoPens = myCar.m_numUnservedStopGoPens;
    uint8_t gridPosition = myCar.m_gridPosition;
    uint8_t driverStatus = myCar.m_driverStatus;
    uint8_t resultStatus = myCar.m_resultStatus;
    uint8_t pitLaneTimerActive = myCar.m_pitLaneTimerActive;
    uint16_t pitLaneTimeInLaneInMS = myCar.m_pitLaneTimeInLaneInMS;
    uint16_t pitStopTimerInMS = myCar.m_pitStopTimerInMS;
    uint8_t pitStopShouldServePen = myCar.m_pitStopShouldServePen;

    // ✅ Prevent segmentation fault: Ensure file is open before writing
    if (lapDataFile.is_open())
    {
        lapDataFile << sessionTime << "," << lastLapTimeInMS << "," << currentLapTimeInMS << ","
                    << sector1TimeInMS << "," << (int)sector1TimeMinutes << ","
                    << sector2TimeInMS << "," << (int)sector2TimeMinutes << ","
                    << deltaToCarInFrontInMS << "," << deltaToRaceLeaderInMS << ","
                    << lapDistance << "," << totalDistance << "," << safetyCarDelta << ","
                    << (int)carPosition << "," << (int)currentLapNum << "," << (int)pitStatus << ","
                    << (int)numPitStops << "," << (int)sector << "," << (int)currentLapInvalid << ","
                    << (int)penalties << "," << (int)totalWarnings << "," << (int)cornerCuttingWarnings << ","
                    << (int)numUnservedDriveThroughPens << "," << (int)numUnservedStopGoPens << ","
                    << (int)gridPosition << "," << (int)driverStatus << "," << (int)resultStatus << ","
                    << (int)pitLaneTimerActive << "," << pitLaneTimeInLaneInMS << ","
                    << pitStopTimerInMS << "," << (int)pitStopShouldServePen << "\n";
    }
    else
    {
        std::cerr << "[ERROR] lapData.csv file is not open!" << std::endl;
    }
}

void TelemetryHandler::handleEventTelemetry(const PacketEventData *telemetry)
{
    float sessionTime = telemetry->m_header.m_sessionTime;

    // Convert event string code to a C++ string
    std::string eventCode(reinterpret_cast<const char *>(telemetry->m_eventStringCode), 4);

    std::ostringstream eventData;
    eventData << sessionTime << "," << eventCode;

    // Handle different event types
    if (eventCode == "FTLP") // Fastest Lap
    {
        eventData << "," << (int)telemetry->m_eventDetails.FastestLap.vehicleIdx
                  << "," << telemetry->m_eventDetails.FastestLap.lapTime;
    }
    else if (eventCode == "RTMT") // Retirement
    {
        eventData << "," << (int)telemetry->m_eventDetails.Retirement.vehicleIdx;
    }
    else if (eventCode == "TMPT") // Team Mate in Pits
    {
        eventData << "," << (int)telemetry->m_eventDetails.TeamMateInPits.vehicleIdx;
    }
    else if (eventCode == "RCWN") // Race Winner
    {
        eventData << "," << (int)telemetry->m_eventDetails.RaceWinner.vehicleIdx;
    }
    else if (eventCode == "PENA") // Penalty Issued
    {
        eventData << "," << (int)telemetry->m_eventDetails.Penalty.penaltyType
                  << "," << (int)telemetry->m_eventDetails.Penalty.infringementType
                  << "," << (int)telemetry->m_eventDetails.Penalty.vehicleIdx
                  << "," << (int)telemetry->m_eventDetails.Penalty.otherVehicleIdx
                  << "," << (int)telemetry->m_eventDetails.Penalty.time
                  << "," << (int)telemetry->m_eventDetails.Penalty.lapNum
                  << "," << (int)telemetry->m_eventDetails.Penalty.placesGained;
    }
    else if (eventCode == "SPTP") // Speed Trap Triggered
    {
        eventData << "," << (int)telemetry->m_eventDetails.SpeedTrap.vehicleIdx
                  << "," << telemetry->m_eventDetails.SpeedTrap.speed
                  << "," << (int)telemetry->m_eventDetails.SpeedTrap.isOverallFastestInSession
                  << "," << (int)telemetry->m_eventDetails.SpeedTrap.isDriverFastestInSession
                  << "," << (int)telemetry->m_eventDetails.SpeedTrap.fastestVehicleIdxInSession
                  << "," << telemetry->m_eventDetails.SpeedTrap.fastestSpeedInSession;
    }
    else if (eventCode == "STLG") // Start Lights
    {
        eventData << "," << (int)telemetry->m_eventDetails.StartLIghts.numLights;
    }
    else if (eventCode == "DTSV") // Drive Through Penalty Served
    {
        eventData << "," << (int)telemetry->m_eventDetails.DriveThroughPenaltyServed.vehicleIdx;
    }
    else if (eventCode == "SGSV") // Stop Go Penalty Served
    {
        eventData << "," << (int)telemetry->m_eventDetails.StopGoPenaltyServed.vehicleIdx;
    }
    else if (eventCode == "FLBK") // Flashback
    {
        eventData << "," << telemetry->m_eventDetails.Flashback.flashbackFrameIdentifier
                  << "," << telemetry->m_eventDetails.Flashback.flashbackSessionTime;
    }
    else if (eventCode == "BUTN") // Button Pressed
    {
        eventData << "," << telemetry->m_eventDetails.Buttons.buttonStatus;
    }
    else if (eventCode == "OVTK") // Overtake
    {
        eventData << "," << (int)telemetry->m_eventDetails.Overtake.overtakingVehicleIdx
                  << "," << (int)telemetry->m_eventDetails.Overtake.beingOvertakenVehicleIdx;
    }

    // Prevent segmentation fault: Ensure file is open before writing
    if (eventFile.is_open())
    {
        eventFile << eventData.str() << "\n";
    }
    else
    {
        std::cerr << "[ERROR] eventTelemetry.csv file is not open!" << std::endl;
    }
}

void TelemetryHandler::handleCarSetupTelemetry(const PacketCarSetupData *telemetry)
{
    const CarSetupData &myCar = telemetry->m_carSetups[telemetry->m_header.m_playerCarIndex];

    float sessionTime = telemetry->m_header.m_sessionTime;
    uint8_t frontWing = myCar.m_frontWing;
    uint8_t rearWing = myCar.m_rearWing;
    uint8_t onThrottle = myCar.m_onThrottle;
    uint8_t offThrottle = myCar.m_offThrottle;
    float frontCamber = myCar.m_frontCamber;
    float rearCamber = myCar.m_rearCamber;
    float frontToe = myCar.m_frontToe;
    float rearToe = myCar.m_rearToe;
    uint8_t frontSuspension = myCar.m_frontSuspension;
    uint8_t rearSuspension = myCar.m_rearSuspension;
    uint8_t frontAntiRollBar = myCar.m_frontAntiRollBar;
    uint8_t rearAntiRollBar = myCar.m_rearAntiRollBar;
    uint8_t frontSuspensionHeight = myCar.m_frontSuspensionHeight;
    uint8_t rearSuspensionHeight = myCar.m_rearSuspensionHeight;
    uint8_t brakePressure = myCar.m_brakePressure;
    uint8_t brakeBias = myCar.m_brakeBias;
    float rearLeftTyrePressure = myCar.m_rearLeftTyrePressure;
    float rearRightTyrePressure = myCar.m_rearRightTyrePressure;
    float frontLeftTyrePressure = myCar.m_frontLeftTyrePressure;
    float frontRightTyrePressure = myCar.m_frontRightTyrePressure;
    uint8_t ballast = myCar.m_ballast;
    float fuelLoad = myCar.m_fuelLoad;

    // ✅ Prevent segmentation fault: Ensure file is open before writing
    if (carSetupFile.is_open())
    {
        carSetupFile << sessionTime << "," << (int)frontWing << "," << (int)rearWing << ","
                     << (int)onThrottle << "," << (int)offThrottle << ","
                     << frontCamber << "," << rearCamber << "," << frontToe << "," << rearToe << ","
                     << (int)frontSuspension << "," << (int)rearSuspension << ","
                     << (int)frontAntiRollBar << "," << (int)rearAntiRollBar << ","
                     << (int)frontSuspensionHeight << "," << (int)rearSuspensionHeight << ","
                     << (int)brakePressure << "," << (int)brakeBias << ","
                     << rearLeftTyrePressure << "," << rearRightTyrePressure << ","
                     << frontLeftTyrePressure << "," << frontRightTyrePressure << ","
                     << (int)ballast << "," << fuelLoad << "\n";
    }
    else
    {
        std::cerr << "[ERROR] carSetup.csv file is not open!" << std::endl;
    }
}

void TelemetryHandler::handleCarTelemetry(const PacketCarTelemetryData *telemetry)
{
    const CarTelemetryData &myCar = telemetry->m_carTelemetryData[telemetry->m_header.m_playerCarIndex];

    float sessionTime = telemetry->m_header.m_sessionTime;
    uint16_t speed = myCar.m_speed;
    float throttle = myCar.m_throttle;
    float steer = myCar.m_steer;
    float brake = myCar.m_brake;
    uint8_t clutch = myCar.m_clutch;
    int8_t gear = myCar.m_gear;
    uint16_t engineRPM = myCar.m_engineRPM;
    uint8_t drs = myCar.m_drs;
    uint8_t revLightsPercent = myCar.m_revLightsPercent;
    uint16_t revLightsBitValue = myCar.m_revLightsBitValue;
    uint16_t brakesTemperature[4];
    uint8_t tyresSurfaceTemperature[4];
    uint8_t tyresInnerTemperature[4];
    uint16_t engineTemperature = myCar.m_engineTemperature;
    float tyresPressure[4];
    uint8_t surfaceType[4];

    // Copy array values
    for (int i = 0; i < 4; i++)
    {
        brakesTemperature[i] = myCar.m_brakesTemperature[i];
        tyresSurfaceTemperature[i] = myCar.m_tyresSurfaceTemperature[i];
        tyresInnerTemperature[i] = myCar.m_tyresInnerTemperature[i];
        tyresPressure[i] = myCar.m_tyresPressure[i];
        surfaceType[i] = myCar.m_surfaceType[i];
    }

    // ✅ Prevent segmentation fault: Ensure file is open before writing
    if (carTelemetryFile.is_open())
    {
        carTelemetryFile << sessionTime << "," << speed << "," << throttle << "," << steer << "," << brake << ","
                         << (int)clutch << "," << (int)gear << "," << engineRPM << "," << (int)drs << ","
                         << (int)revLightsPercent << "," << revLightsBitValue << ","
                         << brakesTemperature[0] << "," << brakesTemperature[1] << "," << brakesTemperature[2] << "," << brakesTemperature[3] << ","
                         << (int)tyresSurfaceTemperature[0] << "," << (int)tyresSurfaceTemperature[1] << "," << (int)tyresSurfaceTemperature[2] << "," << (int)tyresSurfaceTemperature[3] << ","
                         << (int)tyresInnerTemperature[0] << "," << (int)tyresInnerTemperature[1] << "," << (int)tyresInnerTemperature[2] << "," << (int)tyresInnerTemperature[3] << ","
                         << engineTemperature << ","
                         << tyresPressure[0] << "," << tyresPressure[1] << "," << tyresPressure[2] << "," << tyresPressure[3] << ","
                         << (int)surfaceType[0] << "," << (int)surfaceType[1] << "," << (int)surfaceType[2] << "," << (int)surfaceType[3] << "\n";
    }
    else
    {
        std::cerr << "[ERROR] carTelemetry.csv file is not open!" << std::endl;
    }

    sendToRustPlotter(sessionTime, speed, gear, engineRPM);
}

void TelemetryHandler::handleCarStatusTelemetry(const PacketCarStatusData *telemetry)
{
    const CarStatusData &myCar = telemetry->m_carStatusData[telemetry->m_header.m_playerCarIndex];

    float sessionTime = telemetry->m_header.m_sessionTime;
    uint8_t tractionControl = myCar.m_tractionControl;
    uint8_t antiLockBrakes = myCar.m_antiLockBrakes;
    uint8_t fuelMix = myCar.m_fuelMix;
    uint8_t frontBrakeBias = myCar.m_frontBrakeBias;
    uint8_t pitLimiterStatus = myCar.m_pitLimiterStatus;
    float fuelInTank = myCar.m_fuelInTank;
    float fuelCapacity = myCar.m_fuelCapacity;
    float fuelRemainingLaps = myCar.m_fuelRemainingLaps;
    uint16_t maxRPM = myCar.m_maxRPM;
    uint16_t idleRPM = myCar.m_idleRPM;
    uint8_t maxGears = myCar.m_maxGears;
    uint8_t drsAllowed = myCar.m_drsAllowed;
    uint16_t drsActivationDistance = myCar.m_drsActivationDistance;
    uint8_t actualTyreCompound = myCar.m_actualTyreCompound;
    uint8_t visualTyreCompound = myCar.m_visualTyreCompound;
    uint8_t tyresAgeLaps = myCar.m_tyresAgeLaps;
    int8_t vehicleFiaFlags = myCar.m_vehicleFiaFlags;
    float enginePowerICE = myCar.m_enginePowerICE;
    float enginePowerMGUK = myCar.m_enginePowerMGUK;
    float ersStoreEnergy = myCar.m_ersStoreEnergy;
    uint8_t ersDeployMode = myCar.m_ersDeployMode;
    float ersHarvestedThisLapMGUK = myCar.m_ersHarvestedThisLapMGUK;
    float ersHarvestedThisLapMGUH = myCar.m_ersHarvestedThisLapMGUH;
    float ersDeployedThisLap = myCar.m_ersDeployedThisLap;
    uint8_t networkPaused = myCar.m_networkPaused;

    // ✅ Prevent segmentation fault: Ensure file is open before writing
    if (carStatusFile.is_open())
    {
        carStatusFile << sessionTime << "," << (int)tractionControl << "," << (int)antiLockBrakes << ","
                      << (int)fuelMix << "," << (int)frontBrakeBias << "," << (int)pitLimiterStatus << ","
                      << fuelInTank << "," << fuelCapacity << "," << fuelRemainingLaps << ","
                      << maxRPM << "," << idleRPM << "," << (int)maxGears << ","
                      << (int)drsAllowed << "," << drsActivationDistance << ","
                      << (int)actualTyreCompound << "," << (int)visualTyreCompound << "," << (int)tyresAgeLaps << ","
                      << (int)vehicleFiaFlags << "," << enginePowerICE << "," << enginePowerMGUK << ","
                      << ersStoreEnergy << "," << (int)ersDeployMode << "," << ersHarvestedThisLapMGUK << ","
                      << ersHarvestedThisLapMGUH << "," << ersDeployedThisLap << "," << (int)networkPaused << "\n";
    }
    else
    {
        std::cerr << "[ERROR] carStatus.csv file is not open!" << std::endl;
    }
}

void TelemetryHandler::handleTyreSetsTelemetry(const PacketTyreSetsData *tyreSets)
{
    uint8_t carIdx = tyreSets->m_carIdx;
    uint8_t fittedIdx = tyreSets->m_fittedIdx;

    // ✅ Prevent segmentation fault: Ensure file is open before writing
    if (tyreSetsFile.is_open())
    {
        for (int i = 0; i < 20; i++)
        {
            const TyreSetData &tyre = tyreSets->m_tyreSetData[i];

            tyreSetsFile << (int)carIdx << ","
                         << (int)i << ","
                         << (int)tyre.m_actualTyreCompound << ","
                         << (int)tyre.m_visualTyreCompound << ","
                         << (int)tyre.m_wear << ","
                         << (int)tyre.m_available << ","
                         << (int)tyre.m_recommendedSession << ","
                         << (int)tyre.m_lifeSpan << ","
                         << (int)tyre.m_usableLife << ","
                         << tyre.m_lapDeltaTime << ","
                         << (i == fittedIdx ? 1 : 0) << "\n";
        }
    }
    else
    {
        std::cerr << "[ERROR] tyreSets.csv file is not open!" << std::endl;
    }
}

void TelemetryHandler::handleMotionExTelemetry(const PacketMotionExData *motionEx)
{
    // ✅ Ensure file is open before writing
    if (motionExFile.is_open())
    {
        motionExFile << motionEx->m_header.m_sessionTime << ",";

        for (int i = 0; i < 4; i++)
        {
            motionExFile << motionEx->m_suspensionPosition[i] << ","
                         << motionEx->m_suspensionVelocity[i] << ","
                         << motionEx->m_suspensionAcceleration[i] << ","
                         << motionEx->m_wheelSpeed[i] << ","
                         << motionEx->m_wheelSlipRatio[i] << ","
                         << motionEx->m_wheelSlipAngle[i] << ","
                         << motionEx->m_wheelLatForce[i] << ","
                         << motionEx->m_wheelLongForce[i] << ",";
        }

        motionExFile << motionEx->m_heightOfCOGAboveGround << ","
                     << motionEx->m_localVelocityX << ","
                     << motionEx->m_localVelocityY << ","
                     << motionEx->m_localVelocityZ << ","
                     << motionEx->m_angularVelocityX << ","
                     << motionEx->m_angularVelocityY << ","
                     << motionEx->m_angularVelocityZ << ","
                     << motionEx->m_angularAccelerationX << ","
                     << motionEx->m_angularAccelerationY << ","
                     << motionEx->m_angularAccelerationZ << ","
                     << motionEx->m_frontWheelsAngle << ",";

        for (int i = 0; i < 4; i++)
        {
            motionExFile << motionEx->m_wheelVertForce[i] << (i < 3 ? "," : "\n");
        }
    }
    else
    {
        std::cerr << "[ERROR] motionEx.csv file is not open!" << std::endl;
    }
}

void TelemetryHandler::sendToRustPlotter(float sessionTime, int speed, int gear, int rpm)
{
    std::ostringstream oss;
    oss << sessionTime << "," << speed << "," << gear << "," << rpm;
    std::string msg = oss.str();

    sendto(plotterSocket, msg.c_str(), msg.size(), 0,
           (sockaddr *)&plotterAddr, sizeof(plotterAddr));
}

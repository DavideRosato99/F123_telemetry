# 🏎️ F1 23 Telemetry Reader

## 🚩 Overview

The **F1 23 Telemetry Reader** is a lightweight, efficient tool designed to capture and display live telemetry data directly from the **F1 23** racing game. It provides real-time monitoring of crucial vehicle parameters such as speed, gear, throttle, brake inputs, RPM, tire temperatures, fuel usage, and more. Additionally, it supports data logging, enabling further performance analysis.

---

## 🎯 Key Features

- **Real-time Telemetry**: Instantly capture and display telemetry data from your car in F1 23.
- **Data Logging**: Automatically save telemetry data for detailed post-session analysis.
- **Flexible Modes**:
  - **Receiver Mode**: Collect real telemetry from the game.
  - **Sender Mode**: Simulate telemetry data for testing purposes.
- **Easy Setup**: Quick compilation and execution through a simple Makefile.
- **Cross-platform**: Compatible with Windows (MinGW/Cygwin) and Unix-based systems.

---

## ⚙️ Installation

### Prerequisites

- **C++ Compiler**: `g++` (MinGW, Cygwin, GCC)
- **Make**: For easy compilation
- **F1 23 Game**: Ensure UDP telemetry broadcasting is enabled in-game settings.

### Compilation

Clone the repository and build the executable:

```bash
git clone https://github.com/DavideRosato99/F123_telemetry.git
cd F123_telemetry
make
```

---

## ▶️ Usage

Run the program in the desired mode:

### Telemetry Receiver

To capture and log telemetry data:

```bash
./F123_telemetry receiver
```

Telemetry data will be logged into `telemetry_log.csv`.

### Telemetry Sender (Testing Mode)

For testing and debugging purposes, you can simulate telemetry data:

```bash
./F123_telemetry sender
```

The sender mode continuously generates random telemetry data to validate the receiver.

---

## 📋 Logged Data Format

The telemetry data saved in `telemetry_log.csv` contains:

- **Session Time (seconds)**
- **Speed (km/h)**
- **Gear**
- **Engine RPM**

Example CSV output:

```
123.4,300,7,12000
124.4,305,7,12200
125.4,312,8,13000
...
```

---

## 🚧 Project Structure

```
F123_telemetry/
├── include/
│   ├── packetHeader.h
│   ├── packetTypes.h
│   ├── telemetryHandler.h
│   ├── telemetrySender.h
│   |── udpReceiver.h
|   └── telemetrySender/
│       ├── telemetrySender.h
├── src/
│   ├── main.cpp
│   ├── telemetryHandler.cpp
│   ├── udpReceiver.cpp
│   └── telemetrySender/
│       ├── telemetrySender.cpp
└── Makefile
```

---

## Pull all updates

To pull all the updates run:
```bash
git pull
git submodule update --init --recursive

```

or simply
```bash
git pull --recurse-submodules

```

---

## 📈 Future Improvements

- Enhanced graphical visualization using plotting libraries.
- Integration with third-party analytics tools.
- More detailed telemetry data capturing (e.g., tire wear, fuel).

---

## 🛠️ Contributing

Contributions are welcome! Feel free to submit issues, feature requests, or pull requests.

---

## 📜 License

This project is released under the MIT License. See `LICENSE` for more details.
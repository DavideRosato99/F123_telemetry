#![warn(clippy::all, rust_2018_idioms)]
#![cfg_attr(not(debug_assertions), windows_subsystem = "windows")]

use rust_data_inspector::{DataInspector, DataInspectorAPI};
use rust_data_inspector_signals::{PlotSampleSender, PlotSignalSample, PlotSignals};
use std::net::UdpSocket;
use std::sync::atomic::{AtomicBool, Ordering::Relaxed};
use std::sync::{Arc, Mutex};
use std::thread::{self, JoinHandle};
use std::time::Duration;

#[cfg(not(target_arch = "wasm32"))]
fn main() -> eframe::Result<()> {
    let mut signals = PlotSignals::default();
    let pause = Arc::new(AtomicBool::new(false));

    // Create the required telemetry signals
    let (_, speed_sender) = signals.add_signal("/Telemetry/speed").unwrap();
    let (_, gear_sender) = signals.add_signal("/Telemetry/gear").unwrap();
    let (_, rpm_sender) = signals.add_signal("/Telemetry/RPM").unwrap();

    // Start UDP listener for telemetry data
    new_signal_producer(speed_sender, gear_sender, rpm_sender, pause.clone());

    DataInspector::run_native(
        "Telemetry Plotter",
        signals,
        Some(move |ui: &mut egui::Ui, _: &mut DataInspectorAPI| {
            let mut p = pause.load(Relaxed);
            ui.toggle_value(&mut p, "Pause");
            pause.store(p, Relaxed);
        }),
    )
}

// Function to receive telemetry and send data to the plot
pub fn new_signal_producer(
    speed_sender: PlotSampleSender,
    gear_sender: PlotSampleSender,
    rpm_sender: PlotSampleSender,
    pause: Arc<AtomicBool>,
) -> JoinHandle<()> {
    let socket = UdpSocket::bind("127.0.0.1:20888").expect("Couldn't bind to UDP socket");
    socket.set_nonblocking(true).unwrap();

    thread::spawn(move || {
        let mut buf = [0u8; 1024];

        loop {
            if pause.load(Relaxed) {
                thread::sleep(Duration::from_millis(100)); // Prevent CPU overuse
                continue;
            }

            if let Ok((amt, _)) = socket.recv_from(&mut buf) {
                if let Ok(msg) = std::str::from_utf8(&buf[..amt]) {
                    let parts: Vec<&str> = msg.trim().split(',').collect();
                    if parts.len() == 4 {
                        if let (Ok(time), Ok(speed), Ok(gear), Ok(rpm)) = (
                            parts[0].parse::<f64>(), // Session time
                            parts[1].parse::<f64>(), // Speed
                            parts[2].parse::<f64>(), // Gear
                            parts[3].parse::<f64>(), // RPM
                        ) {
                            // Send data to respective signals
                            let _ = speed_sender.send(PlotSignalSample { time, value: speed });
                            let _ = gear_sender.send(PlotSignalSample { time, value: gear });
                            let _ = rpm_sender.send(PlotSignalSample { time, value: rpm });
                        }
                    }
                }
            }
            thread::sleep(Duration::from_millis(10)); // Avoid excessive CPU usage
        }
    })
}

🚦 Smart Traffic Light Control System
A robust, interactive traffic light simulation developed for the ESP32-S3 microcontroller using Embedded C++ and the ESP-IDF framework. This project replicates a real-world intersection scenario, managing traffic flow for vehicles and pedestrians with safety features and on-demand interaction.

📝 Project Overview
The system operates in a default "Green for Cars" state to optimize traffic flow. It utilizes a push-button input to trigger a pedestrian crossing sequence. The logic includes state management, input debouncing (edge detection), and a safety cooldown timer to prevent rapid cycling of the traffic signals.

🌟 Key Features
Object-Oriented Design (OOP): Hardware components (LEDs, Buttons) are encapsulated in custom C++ classes (Led, Button) for modularity and reusability.

Real-Time Operating System (RTOS): Utilizes FreeRTOS task delays (vTaskDelay) for non-blocking timing and efficient CPU usage.

Edge Detection Input: Implements a software-based "Falling Edge" detection algorithm to register button clicks precisely, distinguishing between a "press" event and a "held" state.

Safety Cooldown: Enforces a mandatory "Minimum Green Time" for vehicles (5-second countdown) after a pedestrian cycle to ensure traffic safety.

Hardware Abstraction: Direct manipulation of ESP32 GPIO registers using the ESP-IDF driver (driver/gpio.h).

🛠️ Hardware Requirements
Microcontroller: ESP32-S3 (DevKitC-1)

Actuators:

3x LEDs (Red, Yellow, Green) - Vehicle Signal

2x LEDs (Red, Green) - Pedestrian Signal

Sensors:

1x Push Button (Configured with Internal Pull-Up Resistor)

Components: Resistors (330Ω for LEDs).

💻 Software Stack
Language: C++ (Embedded)

Framework: Espressif IoT Development Framework (ESP-IDF)

Simulator: Wokwi

⚙️ How It Works
Idle State: Cars have a GREEN light; Pedestrians have a RED light.

Trigger: When a pedestrian presses the button, the system detects the input signal (Active Low).

Sequence:

Cars: Green -> Yellow -> Red.

Pedestrians: Red -> GREEN (Safe to cross).

Reset: After the crossing time elapses, the pedestrian light turns Red, and cars get the Green light again.

Cooldown: The system enters a "Safety Lock" mode for 5 seconds, ignoring further button presses to prevent traffic congestion.

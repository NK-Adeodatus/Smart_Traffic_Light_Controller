# Smart Traffic Light Controller
This project is a smart traffic light controller. It is designed to manage the flow of traffic at two intersections. It determines how long the green light will be on for a certain road depending on the number of cars at that road. And it can display the information about the traffic.

### Key Features
- ** Dynamic Timing of the green light**: It can increase or decrease the amount of time the green light stays on based on the number of vehicles.
- **Concurrency**: Uses `millis()` instead of `delay()`.
- **Serial Interface**: Interactive menu to monitor system status and toggle manual overrides.
- **Error Handling**: Error handling is implemented in order to avoid and handle invalid states.

---

## 🚀 How to Run (Tinkercad)
1. Copy the code in the file `smart_traffic_light_controller.c`.
2. Open [Tinkercad Circuits](https://www.tinkercad.com/).
3. Create a new circuit with an Arduino Uno, 6 LEDs (2 Red, 2 Yellow, 2 Green), 6 Resistors, and 2 Push Buttons.
4. To see the circuit work, paste the code into the Code Editor and click on start the simulation.
5. The Serial Monitor is used to observe data and overrides the system manually.

---




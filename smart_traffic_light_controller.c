#include <stdlib.h> 

// 1. THE DATA STRUCTURE
struct TrafficLight {
    int red, yellow, green, button;
    int vehicleCount;
    int greenDuration; // Smart timing variable
};

// 2. GLOBAL POINTERS (Dynamic Memory)
struct TrafficLight *roadA;
struct TrafficLight *roadB;

unsigned long previousMillis = 0;
int currentState = 0;
long interval = 5000;
bool manualOverride = false;

void setup() {
    Serial.begin(9600);

    // --- STEP 1: DYNAMIC MEMORY ALLOCATION ---
    // We ask the Arduino for memory while the program is running
    roadA = (struct TrafficLight *)malloc(sizeof(struct TrafficLight));
    roadB = (struct TrafficLight *)malloc(sizeof(struct TrafficLight));

    // Initialize Road A pins and data
    roadA->red = 2; roadA->yellow = 3; roadA->green = 4; roadA->button = 5;
    roadA->vehicleCount = 0; roadA->greenDuration = 5000;

    // Initialize Road B pins and data
    roadB->red = 8; roadB->yellow = 9; roadB->green = 10; roadB->button = 6;
    roadB->vehicleCount = 0; roadB->greenDuration = 5000;

    // Set Pin Modes using pointers
    pinMode(roadA->red, OUTPUT); pinMode(roadA->yellow, OUTPUT); pinMode(roadA->green, OUTPUT);
    pinMode(roadB->red, OUTPUT); pinMode(roadB->yellow, OUTPUT); pinMode(roadB->green, OUTPUT);
    pinMode(roadA->button, INPUT_PULLUP); pinMode(roadB->button, INPUT_PULLUP);

    printMenu();
}

void loop() {
    unsigned long currentMillis = millis();

    // --- STEP 2: CONCURRENCY (Requirement) ---
    // Always checking buttons and Serial menu while lights run
    if (!manualOverride) {
        readSensors(roadA);
        readSensors(roadB);

        if (currentMillis - previousMillis >= interval) {
            previousMillis = currentMillis;
            advanceState();
        }
    }

    handleSerialMenu();
}

// --- STEP 3: POINTER LOGIC ---
void readSensors(struct TrafficLight *road) {
    if (digitalRead(road->button) == LOW) {
        road->vehicleCount++;
        delay(200); // Simple debounce
    }
}

void advanceState() {
    currentState++;
    if (currentState > 3) {
        currentState = 0;
        calculateSmartTiming(); // --- STEP 4: SMART LOGIC (Requirement) ---
    }

    allOff();

    if (currentState == 0) { // Road A Green
        digitalWrite(roadA->green, HIGH); digitalWrite(roadB->red, HIGH);
        interval = roadA->greenDuration;
        Serial.print("\n[A Moving] Next Green Time: "); Serial.println(interval);
    } 
    else if (currentState == 1) { // Road A Yellow
        digitalWrite(roadA->yellow, HIGH); digitalWrite(roadB->red, HIGH);
        interval = 2000;
    }
    else if (currentState == 2) { // Road B Green
        digitalWrite(roadB->green, HIGH); digitalWrite(roadA->red, HIGH);
        interval = roadB->greenDuration;
        Serial.print("\n[B Moving] Next Green Time: "); Serial.println(interval);
    }
    else if (currentState == 3) { // Road B Yellow
        digitalWrite(roadB->yellow, HIGH); digitalWrite(roadA->red, HIGH);
        interval = 2000;
    }
}

// --- STEP 5: SMART TIMING CALCULATION ---
void calculateSmartTiming() {
    // If one road has 3x more cars than the other, give it 10s. Otherwise 5s.
    if (roadA->vehicleCount > (roadB->vehicleCount + 3)) {
        roadA->greenDuration = 10000;
        roadB->greenDuration = 3000;
    } else if (roadB->vehicleCount > (roadA->vehicleCount + 3)) {
        roadB->greenDuration = 10000;
        roadA->greenDuration = 3000;
    } else {
        roadA->greenDuration = 5000;
        roadB->greenDuration = 5000;
    }
    // Log the data then reset for next cycle
    Serial.print("Cycle Stats - A: "); Serial.print(roadA->vehicleCount);
    Serial.print(" | B: "); Serial.println(roadB->vehicleCount);
    roadA->vehicleCount = 0;
    roadB->vehicleCount = 0;
}

void handleSerialMenu() {
    if (Serial.available() > 0) {
        char cmd = Serial.read();
        if (cmd == 's') {
            Serial.println("\n--- SYSTEM STATUS ---");
            Serial.print("Road A Count: "); Serial.println(roadA->vehicleCount);
            Serial.print("Road B Count: "); Serial.println(roadB->vehicleCount);
            Serial.print("Manual Override: "); Serial.println(manualOverride ? "ON" : "OFF");
        } else if (cmd == 'm') {
            manualOverride = !manualOverride;
            Serial.println(manualOverride ? "MANUAL OVERRIDE ACTIVE" : "AUTO MODE ACTIVE");
        }
    }
}

void printMenu() {
    Serial.println("--- Traffic Controller Menu ---");
    Serial.println("Press 's' for Status Report");
    Serial.println("Press 'm' to Toggle Manual Override (Freeze Lights)");
}

void allOff() {
    digitalWrite(roadA->red, LOW); digitalWrite(roadA->yellow, LOW); digitalWrite(roadA->green, LOW);
    digitalWrite(roadB->red, LOW); digitalWrite(roadB->yellow, LOW); digitalWrite(roadB->green, LOW);
}
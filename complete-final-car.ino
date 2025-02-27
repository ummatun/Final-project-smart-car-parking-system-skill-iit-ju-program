#define BLYNK_TEMPLATE_ID "TMPL6pFMWDnqE"
#define BLYNK_TEMPLATE_NAME "smart car parking"
#define BLYNK_AUTH_TOKEN "tt3TYU0YJivGRS0ak3twkGmIIR4vpiAE"

#include <ESP32Servo.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

#define BLYNK_PRINT Serial

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Room 432";  
char pass[] = "Sumaia432ju";  

// IR Sensors & Servo
#define SLOT1_IR 33  
#define SLOT2_IR 32  
#define SLOT4_IR 34  
#define GATE_FRONT_IR 27  
#define GATE_BACK_IR 14  
#define SERVO_PIN 25  

// Real LED Indicators
#define SLOT1_LED 18  
#define SLOT2_LED 19  
#define SLOT4_LED 22  

Servo gateServo;
BlynkTimer timer;

bool booked[3] = {false, false, false};  
bool blocked = false;  
bool carParked[3] = {false, false, false};  

// **Gate Control (Entry & Exit)**
unsigned long gateOpenTime = 0;
const unsigned long gateDelay = 3000;  
bool gateOpen = false;

void setup() {
    Serial.begin(115200);
    Blynk.begin(auth, ssid, pass);
    
    pinMode(SLOT1_IR, INPUT);
    pinMode(SLOT2_IR, INPUT);
    pinMode(SLOT4_IR, INPUT);
    pinMode(GATE_FRONT_IR, INPUT);
    pinMode(GATE_BACK_IR, INPUT);
    
    pinMode(SLOT1_LED, OUTPUT);
    pinMode(SLOT2_LED, OUTPUT);
    pinMode(SLOT4_LED, OUTPUT);
    
    gateServo.attach(SERVO_PIN);
    gateServo.write(90);  
}

// **Blynk Switch for Booking (V5 - V8)**
BLYNK_WRITE(V5) { handleBooking(0, V5, V1, SLOT1_LED, param.asInt()); }
BLYNK_WRITE(V6) { handleBooking(1, V6, V2, SLOT2_LED, param.asInt()); }
BLYNK_WRITE(V8) { handleBooking(2, V8, V4, SLOT4_LED, param.asInt()); }

void handleBooking(int slot, int virtualSwitch, int virtualLED, int realLED, int bookingState) {
    if (blocked) {  
        Blynk.virtualWrite(virtualSwitch, 0);
        Serial.println("Booking failed: User is temporarily blocked!");
        return;
    }

    if (bookingState == 1) {  
        if (carParked[slot]) {  
            Serial.printf("Slot %d is already occupied. Booking denied.\n", slot + 1);
            Blynk.virtualWrite(virtualSwitch, 0);
            return;
        }

        booked[slot] = true;
        Serial.printf("Slot %d booked. Car must park within 10 sec!\n", slot + 1);
        Blynk.virtualWrite(virtualLED, 1);
        digitalWrite(realLED, HIGH);

        timer.setTimeout(10000L, [slot, virtualSwitch, virtualLED, realLED]() { checkParking(slot, virtualSwitch, virtualLED, realLED); });
    } else {  
        if (!carParked[slot]) {  
            booked[slot] = false;
            Serial.printf("Booking canceled for Slot %d.\n", slot + 1);
            Blynk.virtualWrite(virtualLED, 0);
            digitalWrite(realLED, LOW);
        }
    }
}

void checkParking(int slot, int virtualSwitch, int virtualLED, int realLED) {
    int slotIR[] = {SLOT1_IR, SLOT2_IR, SLOT4_IR};

    if (digitalRead(slotIR[slot]) == HIGH) {  
        Serial.printf("Car not detected at Slot %d! User blocked from booking for 20 sec.\n", slot + 1);
        booked[slot] = false;  
        blocked = true;  

        Blynk.virtualWrite(virtualSwitch, 0);  
        Blynk.virtualWrite(virtualLED, 0);  
        digitalWrite(realLED, LOW);  

        timer.setTimeout(20000L, []() { unblockAllSlots(); }); 
    } else {  
        Serial.printf("Car detected at Slot %d! Booking confirmed.\n", slot + 1);
        carParked[slot] = true;
    }
}

void unblockAllSlots() {
    blocked = false;
    Serial.println("User can book again.");
}

void controlGate() {
    if (digitalRead(GATE_FRONT_IR) == LOW && !gateOpen) {
        Serial.println("Car at front of gate! Opening...");
        gateServo.write(0);
        gateOpenTime = millis();
        gateOpen = true;
    } else if (digitalRead(GATE_BACK_IR) == LOW && !gateOpen) {
        Serial.println("Car at back of gate! Opening...");
        gateServo.write(0);
        gateOpenTime = millis();
        gateOpen = true;
    }

    if (gateOpen && millis() - gateOpenTime >= gateDelay) {
        gateServo.write(90);  
        gateOpen = false;
    }
}

void monitorSlots() {
    int slotIR[] = {SLOT1_IR, SLOT2_IR, SLOT4_IR};
    int virtualSwitch[] = {V5, V6, V8};
    int virtualLED[] = {V1, V2, V4};
    int realLED[] = {SLOT1_LED, SLOT2_LED, SLOT4_LED};

    for (int i = 0; i < 3; i++) {
        if (digitalRead(slotIR[i]) == LOW) {
            if (!carParked[i]) {
                Serial.printf("Car detected in Slot %d. Marking as occupied.\n", i + 1);
                carParked[i] = true;
                booked[i] = true;

                Blynk.virtualWrite(virtualSwitch[i], 1);
                Blynk.virtualWrite(virtualLED[i], 1);
                digitalWrite(realLED[i], HIGH);
            }
        } else {
            if (carParked[i]) {
                Serial.printf("Car left Slot %d. Resetting status.\n", i + 1);
                carParked[i] = false;
                booked[i] = false;

                Blynk.virtualWrite(virtualSwitch[i], 0);
                Blynk.virtualWrite(virtualLED[i], 0);
                digitalWrite(realLED[i], LOW);
            }
        }
    }
}

void loop() {
    Blynk.run();
    timer.run();
    controlGate();
    monitorSlots();
}

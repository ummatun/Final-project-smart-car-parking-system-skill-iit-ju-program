# Smart Car Parking System (IoT-Based) - EDGE IIT JU Program

## Project Overview
This IoT-based Smart Car Parking System is designed to efficiently manage parking spaces using real-time monitoring and remote booking. The system leverages ESP32, IR sensors, a servo motor, and Blynk for seamless operation. Key functionalities include automated entry and exit, real-time slot updates, and penalty enforcement for delayed occupancy.

## Features
- **Remote Booking via Blynk**: Users can book a parking slot remotely.
- **Automated Entry & Exit**: Servo motor controls the gate based on slot availability.
- **Real-Time Slot Updates**: Live monitoring of slot status using IR sensors.
- **Penalty System**: If a booked slot is not occupied within the set time, a penalty is applied.
- **Notifications**: 
  - Blynk notifications for slot updates and penalties.
- **Smart Penalty Reset**: Users penalized for missing a booking cannot rebook for a specific duration.

## Components Used
- **ESP32**: Main microcontroller for managing the system.
- **IR Sensors**: Detects vehicle presence in parking slots.
- **Servo Motor**: Controls the entry gate.
- **Blynk App**: Provides a user-friendly interface for booking and monitoring.

## Implementation Details
1. **Booking Process**: Users book slots via the Blynk app.
2. **Entry Mechanism**: The servo gate opens when a booked slot is confirmed.
3. **Slot Monitoring**: IR sensors detect vehicle presence.
4. **Penalty System**:
   - If occupied within 10 sec: No penalty.
   - If not occupied within 10 sec: 20 sec penalty.
   - Additional rule: If a booked slot is not occupied within 30 minutes, the user gets a 4-hour booking restriction.
5. **Exit Process**: The servo gate opens for a vehicle to exit.

## Demonstration Video
Watch the working demonstration of the project on YouTube: [Click Here](https://www.youtube.com/watch?v=rItZGbqJcT4)

## Future Improvements
- Integration with a payment system.
- Expanding to handle multiple parking lots.
- Enhancing the mobile app UI for better user experience.
- Adding an email notification system for booking confirmations and penalties.

## Contributors
- **Rifat Jahan Sumaia** (CSE, Jahangirnagar University)
- **Ummatun Khatun** (CSE, Jahangirnagar University)
- **Nishat Tanzim Mon** (GIS and Remote Sensing, Jahangirnagar University)

## Contact
For any queries or collaboration, feel free to reach out via email or GitHub.

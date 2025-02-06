#include <Arduino.h>
#include <header/engine.h>
#include <header/pin.h>
#include <Servo.h>

using namespace pin;


namespace engine {
  Servo esc; 
  const int esc_pin = 11;
  const int rpm_pin = 8;
  void controlEngine(){
    Serial.println(ESC_PIN);
  }
}

//   Servo esc;

//   int escPin = 11;             // Pin connected to the ESC signal wire
//   float currentSpeed = 1000;   // Current speed (in microseconds), starts at the minimum (1000µs)
//   float targetSpeed = 1000;    // Target speed (in microseconds)
//   int minSpeed = 1000;         // Minimum speed (1ms pulse)
//   int maxSpeed = 2000;         // Maximum speed (2ms pulse)
//   float interpolationStep = 1; // Speed change per iteration (the smaller, the smoother)

//   int rpmPin = 8;
//   bool rpmLastRead = false;

//   unsigned long timeLastRead = 0;
//   unsigned long lastTime = 0;

//   // Internal
//   unsigned long time;
//   float deltaTime;

//   bool rpmRead;
//   unsigned long timeRead;
//   unsigned long deltaTimeRead;
//   float rpm;

//   int readCount = 0;

//   void aa()
//   {
//     Serial.begin(230400);                     // Start serial communication
//     esc.attach(escPin);                       // Attach the ESC to the pin
//     esc.writeMicroseconds((int)currentSpeed); // Initialize ESC with the current speed

//     pinMode(rpmPin, INPUT);
//   }

//   void aaa()
//   {
//     // Check if serial data is available for motor control or tare command
//     if (Serial.available() > 0)
//     {
//       String input = Serial.readStringUntil('\n'); // Read the input as a string

//       // Otherwise, assume it's a speed input
//       int speedInput = input.toInt(); // Convert input to integer
//       if (speedInput >= 0 && speedInput <= 100)
//       {
//         // Map the input (0-100) to microseconds (1000-2000)
//         targetSpeed = map(speedInput, 0, 100, minSpeed, maxSpeed);
//         Serial.print("Setting target speed to: ");
//         Serial.println(targetSpeed);
//       }
//       else
//       {
//         Serial.println("Invalid input! Enter a value between 0 and 100");
//       }
//     }

//     time = micros();
//     deltaTime = time - lastTime;
//     lastTime = time;

//     // Smoothly interpolate the speed towards the target speed
//     if (currentSpeed < targetSpeed)
//     {
//       currentSpeed += interpolationStep / deltaTime; // Increase speed
//       if (currentSpeed > targetSpeed)
//       {
//         currentSpeed = targetSpeed; // Cap at the target speed
//       }
//     }
//     else if (currentSpeed > targetSpeed)
//     {
//       currentSpeed -= interpolationStep / deltaTime; // Decrease speed
//       if (currentSpeed < targetSpeed)
//       {
//         currentSpeed = targetSpeed; // Cap at the target speed
//       }
//     }

//     esc.writeMicroseconds((int)currentSpeed);

//     // RPM reading
//     rpmRead = digitalRead(rpmPin);

//     if (rpmRead && !rpmLastRead)
//     {
//       timeRead = micros();

//       deltaTimeRead = timeRead - timeLastRead;
//       timeLastRead = timeRead;

//       rpm = 60000000 / deltaTimeRead;

//       if (readCount % 10)
//       {
//         Serial.println(rpm);
//       }
//       readCount++;
//     }

//     rpmLastRead = rpmRead;
//   }
// }
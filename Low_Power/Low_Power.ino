/****************************************************************************
 * Low Power Example Sketch
 * Low_Power.ino
 * 29 Jan 2016 
 * https://github.com/sparkfun/Simblee_Tutorials
 *
 * This app is a way to show how the Simblee Low Power modes can be used.
 * 
 * Resources: Please install the Simblee support files before attempting to use
 * this sketch; see
 * https://learn.sparkfun.com/tutorials/simblee-concepts#setting-up-arduino for
 * details.
 * 
 * Development environment specifics: Arduino.cc IDE v1.6.7
 * 
 * This code is beerware; if you see me (or any other SparkFun employee) at the
 * local, and you've found our code helpful, please buy us a round!
 * ****************************************************************************/

// To use the SimbleeForMobile library, you must include this file at the top
// of your sketch. **DO NOT** include the SimbleeBLE.h file, as it will cause
// the library to silently break.
#include <SimbleeForMobile.h>

const int led = 2; // The Simblee BOB (WRL-13632) has an LED on pin 2.
int ledState = LOW;
const int button = 3;

void setup() 
{
  pinMode(led, OUTPUT);
  digitalWrite(led, ledState);
  pinMode(button, INPUT_PULLUP);
  
  // advertisementData shows up in the app as a line under deviceName. Note
  // that the length of these two fields combined must be less than 16
  // characters!
  SimbleeForMobile.advertisementData = "LoPow";
  SimbleeForMobile.deviceName = "WRL-13632";

  // txPowerLevel can be any multiple of 4 between -20 and +4, inclusive. The
  // default value is +4; at -20 range is only a few feet.
  SimbleeForMobile.txPowerLevel = -4;


  // This must be called *after* you've set up the variables above, as those
  // variables are only written during this function and changing them later
  // won't actually propagate the settings to the device.
  SimbleeForMobile.begin();
}

void loop() 
{
  // There are two low power modes: Ultra Low Power and Off. ULP consumes about
  // 1/10 of the power that a fully on Simblee consumes; in Off mode, the
  // device is using about 1/5000 of the normal power consumption!

  // The system can be brought out of ULP mode either on a timed basis (see the
  // tutorial for more information; broadly, pass a number of milliseconds to
  // the Simblee_ULPDelay() function and the processor will awaken that many
  // milliseconds later) or by a pin transition event. In Off mode, *only* a
  // pin event can wake the processor!

  // Simblee_pinWakeCallback() executes the passed function when a pin causes
  // the device to leave either sleep mode. Options for the second parameter
  // are HIGH, LOW, or NONE, and correspond to the triggering state of the pin.
  Simblee_pinWakeCallback(button, LOW, buttonCallback);

  // Simblee_pinWake() sets up the device to awaken from sleep mode, but does
  // NOT provide a callback function. In ULP mode, execution will resume on the
  // line after the function call to Simblee_ULPDelay(); in Off mode, the
  // processor will immediately revert to sleep mode unless a callback
  // function is available.
  //Simblee_pinWake(button, LOW);

  // Simblee_systemOff() puts the device in a very low power mode, where it
  // consumes less than 1uA of current (compared to about 5000uA during normal
  // operation). In this mode, *only* a pin transition can bring the device out
  // of sleep, and you *must* provide a callback function with
  // Simblee_pinWakeCallback() for any code to be executed!
  Simblee_systemOff();

  // Simblee_ULPDelay() causes the processor to sleep for some number of
  // milliseconds. There are utility macros defined for converting from larger,
  // more convenient units: DAY(x), HOURS(x), MINUTES(x), SECONDS(x) will each
  // cause the processor to wait for x of that number of units. Pass INFINITE
  // to delay until a pin event occurs.
  //Simblee_ULPDelay(INFINITE);

  // If a pin event woke the processor, you MUST call the
  // Simblee_resetPinWake() function for that pin, or the processor will
  // immediately exit sleep mode upon reentering it.
  Simblee_resetPinWake(button);

  // We're blinking the LED here a bit longer than in the callback function, so
  // we can differentiate between which section of code is executing.
  digitalWrite(led, HIGH);
  delay(1500);
  digitalWrite(led, LOW);

  // This function must be called regularly to process UI events.
  SimbleeForMobile.process();
}

// ui() is a SimbleeForMobile specific function which handles the specification
// of the GUI on the mobile device the Simblee connects to.
void ui()
{
  // Empty, for this sketch's purposes.
}

// This function is called whenever a UI event occurs. Events are fairly easy
// to predict; for instance, touching a button produces a "PRESS_EVENT" event.
// UI elements have default event generation settings that match their expected
// behavior, so you'll only rarely have to change them.
void ui_event(event_t &event)
{
  // Empty for this sketch's purposes.
}

// This function (the name of which is completely arbitrary) is executed after
// a pinWake event pulls the processor out of low power mode. Here, we just
// blink the LED on for 500ms, just as a visual check that we have pulled out
// of low power mode.
int buttonCallback(uint32_t button)
{
  Simblee_resetPinWake(button);
  digitalWrite(led, HIGH);
  delay(500);
  digitalWrite(led, LOW);
}


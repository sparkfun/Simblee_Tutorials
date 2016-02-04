/****************************************************************************
 * Read_Input Read a Simblee input from smart phone 
 * Mike Hord @ SparkFun Electronics 26 Jan 2016 
 * https://github.com/sparkfun/Simblee_Tutorials
 * 
 * This example demonstrates the use of the SimbleeForMobile library to read
 * the state of a pin on a Simblee module from a phone app. 
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

const int btn = 3; // The Simblee BOB (WRL-13632) has a button on pin 3.

// Every draw command returns a uint8_t result which is the object id that was
// created. If you wish to change the object later, you'll need this value, and
// if you want to catch an event created by an object, you'll need it there,
// too. Make sure you create these id variables outside of any function, as
// you'll need to refer to them in many other functions.

uint8_t boxID;

void setup() 
{
  // Protip: using INPUT_PULLUP very rarely causes any problems but can solve
  // a lot of problems with input signals that aren't pulled strongly.
  pinMode(btn, INPUT_PULLUP);
  
  // advertisementData shows up in the app as a line under deviceName. Note
  // that the length of these two fields combined must be less than 16
  // characters!
  SimbleeForMobile.advertisementData = "Button";
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
  // All we want to do is detect when the button is pressed and make the box on
  // the screen white while it's pressed.

  // This is important: before writing *any* UI element, make sure that the UI
  // is updatable!!! Failure to do so may crash your whole program.
  if (SimbleeForMobile.updatable)
  {
    // Okay, *now* we can worry about what the button is doing. The
    // updateColor() function takes the id returned when we created the box and
    // tells that object to change to the color parameter passed.
    if (digitalRead(btn) == LOW) SimbleeForMobile.updateColor(boxID, WHITE);
    else SimbleeForMobile.updateColor(boxID, BLACK);
  }
  // This function must be called regularly to process UI events.
  SimbleeForMobile.process();
}

// ui() is a SimbleeForMobile specific function which handles the specification
// of the GUI on the mobile device the Simblee connects to.
void ui()
{
  // color_t is a special type which contains red, green, blue, and alpha 
  // (transparency) information packed into a 32-bit value. The functions rgb()
  // and rgba() can be used to create a packed value.
  color_t darkgray = rgb(85,85,85);

  // These variable names are long...let's shorten them. They allow us to make
  // an interface that scales and scoots appropriately regardless of the screen
  // orientation or resolution.
  uint16_t wid = SimbleeForMobile.screenWidth;
  uint16_t hgt = SimbleeForMobile.screenHeight;

  // The beginScreen() function both sets the background color and serves as a
  // notification that the host should try to cache the UI functions which come
  // between this call and the subsequent endScreen() call.
  SimbleeForMobile.beginScreen(darkgray);

  // SimbleeForMobile doesn't really have an kind of indicator- but there IS a
  // drawRect() function, and we can freely change the color of the rectangle
  // after drawing it! The x,y coordinates are of the upper left hand corner.
  // If you pass a second color parameter, you'll get a fade from top to bottom
  // and you'll need to update *both* colors to get the whole box to change.
  boxID = SimbleeForMobile.drawRect(
                          (wid/2) - 50,        // x position
                          (hgt/2) - 50,        // y positon
                          100,                 // x dimension
                          100,                 // y dimensionrectangle
                          BLACK);              // color of rectangle.
                          
  SimbleeForMobile.endScreen();
}

// This function is called whenever a UI event occurs. Events are fairly easy
// to predict; for instance, touching a button produces a "PRESS_EVENT" event.
// UI elements have default event generation settings that match their expected
// behavior, so you'll only rarely have to change them.
void ui_event(event_t &event)
{
  // In this case, we're sending data back from the Simblee to the app, so we
  // don't really care about any events that might occur.
}


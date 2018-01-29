//=============================================================================
// "Arduino" example program for Crystalfontz ePaper. 
//
// This project is for the CFAP152152A0-0154 :
//   https://www.crystalfontz.com/product/cfap152152A00154
//
//	To see a list of ePaper displays, visit our ePaper section:
//	 https://www.crystalfontz.com/c/epaper-displays/519
//
//  Powered using a Seeeduino v4.2, an open-source 3.3v & 5v capable Arduino clone.
//    https://www.seeedstudio.com/Seeeduino-V4.2-p-2517.html
//    https://github.com/SeeedDocument/SeeeduinoV4/raw/master/resources/Seeeduino_v4.2_sch.pdf
//
// The code was written against a Seeduino v4.2 @3.3v. An Arduino UNO modified to
// operate at 3.3v should also work.
//
//
//=============================================================================
// This is free and unencumbered software released into the public domain.
// 
// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software, either in source code form or as a compiled
// binary, for any purpose, commercial or non-commercial, and by any
// means.
// 
// In jurisdictions that recognize copyright laws, the author or authors
// of this software dedicate any and all copyright interest in the
// software to the public domain. We make this dedication for the benefit
// of the public at large and to the detriment of our heirs and
// successors. We intend this dedication to be an overt act of
// relinquishment in perpetuity of all present and future rights to this
// software under copyright law.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.
// 
// For more information, please refer to <http://unlicense.org/>
//=============================================================================
// Connecting the Arduino to the display
//
//			|Adapter		|			|
//			|(pin 1 towards	|			|
// ARDUINO	|jumper headers)|Wire Color	|Function
// ---------+---------------+-----------+--------------------
// D2		|19				|Yellow		|BS1 Not Used
// D3		|17				|Green		|Busy Line
// D4		|18				|Brown		|Reset Line
// D5		|15				|Purple		|Data/Command Line
// D10		|16				|Blue		|Chip Select Line
// D11		|14				|White		|MOSI
// D13		|13				|Orange		|Clock
// 3.3V		|5				|Red		|Power
// GND		|3				|Black		|Ground
//
// short the following pins on the adapter board:
// GND -> BS2
// 0.47 -> RESE
//=============================================================================

// The display is SPI, include the library header.
#include <SPI.h>

// Include the ePaper module header.
#include "ePaper_CFAP152152A00154.h"

// Include the image arrays. 
#include "Images_for_CFAP152152A00154.h"

//=============================================================================
// Creating image arrays
//
// Bmp_to_epaper is code that will aid in creating bitmaps necessary from .bmp files.
// The code can be downloaded from the Crystalfontz website: 
// https://www.crystalfontz.com/product/bmptoepaper-bitmap-to-epaper-software
// or it can be downloaded from github: 
// https://github.com/crystalfontz/bmp_to_epaper
// This module is 152x152 pixels. 
//===========================================================================
//
//Make sure that the image files make sense.
#if ((WIDTH_PIXELS != (152)) || (HEIGHT_PIXELS  != (152)))
#error "IMAGE INCLUDE FILE IS WRONG SIZE"
#endif

void setup(void)
  {
  delay(50);
  //Debug port / Arduino Serial Monitor (optional)
  Serial.begin(9600);
 
  // Configure the pin directions
  pinMode(EPD_READY,  INPUT);
  pinMode(EPD_RESET,  OUTPUT);   
  pinMode(EPD_CS,     OUTPUT);     
  pinMode(EPD_DC,     OUTPUT);     
  pinMode(EPD_BUSSEL, OUTPUT);

  // Set output pins' default state
  digitalWrite(EPD_RESET,  HIGH); //active low
  digitalWrite(EPD_CS,     HIGH); //active low
  digitalWrite(EPD_DC,     HIGH); //active low 
  digitalWrite(EPD_BUSSEL, LOW);  // 4 wire mode
  
  //Set up SPI interface
  SPI.beginTransaction(SPISettings(500000, MSBFIRST, SPI_MODE0));
  SPI.begin();

  //Initialize the display controller hardware 
  ePaper_Init();
  Serial.println("setup complete");
  }
//=============================================================================
#define SHUTDOWN_BETWEEN_UPDATES (1)
#define PUT_BOOT_SCREEN_THEN_STOP (1)
void loop()
{
	Load_Flash_Image_To_Display_RAM(152,
		152,
		Mono_1BPP,
		Red_1BPP);

	Serial.println("first image sent");

	//Wait for the refresh to complete (~11sec).
	while (0 == digitalRead(EPD_READY));

#if(SHUTDOWN_BETWEEN_UPDATES)
	//Enter off / low power state
	ePaper_PowerOff();

	//Pause to view
	delay(5000);

	//Wake it up
	ePaper_Init();
#else
	delay(5000);
#endif

	Load_Inverse_Flash_Image_To_Display_RAM(152,
		152,
		Mono_1BPP,
		Red_1BPP);

	//Wait for the refresh to complete (~11sec).
	while (0 == digitalRead(EPD_READY));

#if(SHUTDOWN_BETWEEN_UPDATES)
	//Enter off / low power state
	ePaper_PowerOff();

	//Pause to view
	delay(5000);

	//Wake it up
	ePaper_Init();
#else
	delay(5000);
#endif

}
//=============================================================================

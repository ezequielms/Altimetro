// Include Libraries
#include "Arduino.h"
#include "SFE_BMP180.h"
#include "MPU6050.h"
#include "Wire.h"
#include "I2Cdev.h"
#include "SD.h"


// Pin Definitions
#define SDFILE_PIN_CS	10



// Global variables and defines
int16_t mpu6050Ax, mpu6050Ay, mpu6050Az;
int16_t mpu6050Gx, mpu6050Gy, mpu6050Gz;
// object initialization
File sdFile;
SFE_BMP180 bmp180;
MPU6050 mpu6050;



// define vars for testing menu
const int timeout = 10000;       //define timeout of 10 sec
char menuOption = 0;
long time0;

// Setup the essentials for your circuit to work. It runs first every time your circuit is powered with electricity.
void setup() 
{
    // Setup Serial which is useful for debugging
    // Use the Serial Monitor to view printed messages
    Serial.begin(9600);
    while (!Serial) ; // wait for serial port to connect. Needed for native USB
    Serial.println("start");
    
    //Initialize I2C device
    bmp180.begin();
    Wire.begin();
    mpu6050.initialize();
    // Set SPI SS pin to output otherwise the SD library functions will not work.
    // The SD is set to use SPI SS Arduino pin 10 as chip select(CS) by default.
    // To change the pin use SD.begin(SD_CS_PIN)
    pinMode(SDFILE_PIN_CS, OUTPUT);
    // Check if the card is present and can be initialized
    if (!SD.begin()) {
        Serial.println(F("Card failed, or not present"));
        while(1);
    }
    Serial.println(F("card initialized."));
    menuOption = menu();
    
}

// Main logic of your circuit. It defines the interaction between the components you selected. After setup, it runs over and over again, in an eternal loop.
void loop() 
{
    
    
    if(menuOption == '1') {
    // BMP180 - Test Code
    // Read Altitude from barometric sensor, note that the sensor is 1m accurate
    double bmp180Alt = bmp180.altitude();
    double bmp180Pressure = bmp180.getPressure();
    double bmp180TempC = bmp180.getTemperatureC();     //See also bmp180.getTemperatureF() for Fahrenheit
    
    Serial.print(F("Altitude: ")); Serial.print(bmp180Alt,1); Serial.print(F(" [m]"));
    Serial.print(F("\tpressure: ")); Serial.print(bmp180Pressure,1); Serial.print(F(" [hPa]"));
    Serial.print(F("\tTemperature: ")); Serial.print(bmp180TempC,1); Serial.println(F(" [°C]"));
    
    }
    else if(menuOption == '2') {
    // MPU6050 - Test Code
    mpu6050.getMotion6(&mpu6050Ax, &mpu6050Ay, &mpu6050Az, &mpu6050Gx, &mpu6050Gy, &mpu6050Gz);   //read accelerometer and gyroscope raw data in three axes
    double mpu6050Temp = ((double)mpu6050.getTemperature() + 12412.0) / 340.0;
    Serial.print("a/g-\t");
    Serial.print(mpu6050Ax); Serial.print("\t");
    Serial.print(mpu6050Ay); Serial.print("\t");
    Serial.print(mpu6050Az); Serial.print("\t");
    Serial.print(mpu6050Gx); Serial.print("\t");
    Serial.print(mpu6050Gy); Serial.print("\t");
    Serial.print(mpu6050Gz); Serial.print("\t");
    Serial.print(F("Temp- "));   
    Serial.println(mpu6050Temp);
    delay(100);
    
    }
    else if(menuOption == '3') {
    // SD - Test Code
    // The SD example code creates a datalog.txt file for logging sensor data
    // open the file. note that only one file can be open at a time,
    // so you have to close this one before opening another.
    sdFile = SD.open("datalog.txt", FILE_WRITE);
    // if the file exists in SD card, write sensor data
    if (sdFile) {
        //Write to file
        sdFile.println("ENTER SENSOR DATA HERE");
        // close the file
        sdFile.close();
        // Uncomment to print to the serial port
        //Serial.println("ENTER SENSOR DATA HERE");
    } 
    else {
        // if the file didn't open, print an error
        Serial.println(F("error opening file."));
    }
    }
    
    
    
    if (millis() - time0 > timeout)
    {
        menuOption = menu();
    }
    
}



// Menu function for selecting the components to be tested
// Follow serial monitor for instrcutions
char menu()
{

    Serial.println(F("\nWhich component would you like to test?"));
    Serial.println(F("(1) BMP180"));
    Serial.println(F("(2) MPU6050"));
    Serial.println(F("(3) SD"));
    Serial.println(F("(menu) send anything else or press on board reset button\n"));
    while (!Serial.available());

    // Read data from serial monitor if received
    while (Serial.available()) 
    {
        char c = Serial.read();
        if (isAlphaNumeric(c)) 
        {
            if(c == '1') 
    			Serial.println(F("Now Testing BMP180"));
    		else if(c == '2') 
    			Serial.println(F("Now Testing MPU6050"));
    		else if(c == '3') 
    			Serial.println(F("Now Testing SD"));
            else
            {
                Serial.println(F("illegal input!"));
                return 0;
            }
            time0 = millis();
            return c;
            }
        }
    }

/*******************************************************

*    Circuito.io is an automatic generator of schematics and code for off
*    the shelf hardware combinations.

*    Copyright (C) 2016 Roboplan Technologies Ltd.

*    This program is free software: you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.

*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.

*    You should have received a copy of the GNU General Public License
*    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*    In addition, and without limitation, to the disclaimers of warranties 
*    stated above and in the GNU General Public License version 3 (or any 
*    later version), Roboplan Technologies Ltd. ("Roboplan") offers this 
*    program subject to the following warranty disclaimers and by using 
*    this program you acknowledge and agree to the following:
*    THIS PROGRAM IS PROVIDED ON AN "AS IS" AND "AS AVAILABLE" BASIS, AND 
*    WITHOUT WARRANTIES OF ANY KIND EITHER EXPRESS OR IMPLIED.  ROBOPLAN 
*    HEREBY DISCLAIMS ALL WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT 
*    NOT LIMITED TO IMPLIED WARRANTIES OF MERCHANTABILITY, TITLE, FITNESS 
*    FOR A PARTICULAR PURPOSE, NON-INFRINGEMENT, AND THOSE ARISING BY 
*    STATUTE OR FROM A COURSE OF DEALING OR USAGE OF TRADE. 
*    YOUR RELIANCE ON, OR USE OF THIS PROGRAM IS AT YOUR SOLE RISK.
*    ROBOPLAN DOES NOT GUARANTEE THAT THE PROGRAM WILL BE FREE OF, OR NOT 
*    SUSCEPTIBLE TO, BUGS, SECURITY BREACHES, OR VIRUSES. ROBOPLAN DOES 
*    NOT WARRANT THAT YOUR USE OF THE PROGRAM, INCLUDING PURSUANT TO 
*    SCHEMATICS, INSTRUCTIONS OR RECOMMENDATIONS OF ROBOPLAN, WILL BE SAFE 
*    FOR PERSONAL USE OR FOR PRODUCTION OR COMMERCIAL USE, WILL NOT 
*    VIOLATE ANY THIRD PARTY RIGHTS, WILL PROVIDE THE INTENDED OR DESIRED
*    RESULTS, OR OPERATE AS YOU INTENDED OR AS MAY BE INDICATED BY ROBOPLAN. 
*    YOU HEREBY WAIVE, AGREE NOT TO ASSERT AGAINST, AND RELEASE ROBOPLAN, 
*    ITS LICENSORS AND AFFILIATES FROM, ANY CLAIMS IN CONNECTION WITH ANY OF 
*    THE ABOVE. 
********************************************************/
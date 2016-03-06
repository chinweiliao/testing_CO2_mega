#include <SoftwareSerial.h>
#include <Wire.h>
#include "rgb_lcd.h"
#include "DHT.h"

#define DHTPIN A2     // what pin we're connected to

// Uncomment whatever type you're using!
//#define DHTTYPE DHT11   // DHT 11 
#define DHTTYPE DHT22   // DHT 22  (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

DHT dht(DHTPIN, DHTTYPE);


#define DEBUG 0
 
const int pinRx = 13;
const int pinTx = 12;
 
SoftwareSerial sensor(pinTx,pinRx);

rgb_lcd lcd;

const int colorR = 255;
const int colorG = 0;
const int colorB = 0;

const unsigned char cmd_get_sensor[] =
{
    0xff, 0x01, 0x86, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x79
};
unsigned char dataRevice[9];
int temperature;
int CO2PPM;


 
void setup()
{
    sensor.begin(9600);
    dht.begin();
    Serial.begin(9600);
    Serial.println("get a 'g', begin to read from sensor!");
    Serial.println("********************************************************");
    Serial.println();

    lcd.begin(16, 2);
    
    lcd.setRGB(colorR, colorG, colorB);
    
    // Print a message to the LCD.
    //lcd.print("hello, world!");

}
 
void loop()
{
    if(dataRecieve())
    {
        //Serial.print("Temperature: ");
        //Serial.print(temperature);
        //Serial.print("  CO2: ");
        Serial.println(CO2PPM);
        //Serial.println("");
        //Serial.print("value from air quality sensor: ");
        //Serial.println(analogRead(A0));

        lcd.setCursor(0, 0);
        // print the number of seconds since reset:
        lcd.print("CO2:");
        lcd.print(CO2PPM);
        lcd.print(" ");
        lcd.print("air:");
        lcd.print(analogRead(A0));
        lcd.print(" ");
        
        lcd.setCursor(0, 1);
        // print the number of seconds since reset:
        float h = dht.readHumidity();
        float t = dht.readTemperature();
        lcd.print("T:");
        lcd.print(t);
        lcd.print(" H:");
        lcd.print(h);
        lcd.print("%");
    delay(5000);
        
    }
    //delay(1000);
}
 
bool dataRecieve(void)
{
    byte data[9];
    int i = 0;
 
    //transmit command data
    for(i=0; i<sizeof(cmd_get_sensor); i++)
    {
        sensor.write(cmd_get_sensor[i]);
    }
    delay(10);
    //begin reveiceing data
    if(sensor.available())
    {
        while(sensor.available())
        {
            for(int i=0;i<9; i++)
            {
                data[i] = sensor.read();
            }
        }
    }
 
#if DEBUG
    for(int j=0; j<9; j++)
    {
        Serial.print(data[j]);
        Serial.print(" ");
    }
    Serial.println("");
#endif
 
    if((i != 9) || (1 + (0xFF ^ (byte)(data[1] + data[2] + data[3]
    + data[4] + data[5] + data[6] + data[7]))) != data[8])
    {
        return false;
    }
    CO2PPM = (int)data[2] * 256 + (int)data[3];
    temperature = (int)data[4] - 40;
 
    return true;
}

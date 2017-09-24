// Incluindo as bibliotecas
#include "Arduino.h"
#include "SFE_BMP180.h"
#include "MPU6050.h"
#include "Wire.h"
#include "I2Cdev.h"
#include "SD.h"


// Definindo os pinos
#define SDFILE_PIN_CS	10



// Variaveis Globais
int16_t mpu6050Ax, mpu6050Ay, mpu6050Az;
int16_t mpu6050Gx, mpu6050Gy, mpu6050Gz;
// Inicializacao do modulo do cartao e sensores
File sdFile;
SFE_BMP180 bmp180;
MPU6050 mpu6050;



// Definindo as variaveis para o menu no monitor serial
const int timeout = 10000;       //Tempo de 10segundos
char menuOption = 0;
long time0;
double bmp180Alt = bmp180.altitude();
double bmp180Pressure = bmp180.getPressure();
double bmp180TempC = bmp180.getTemperatureC();   
double mpu6050Temp = ((double)mpu6050.getTemperature() + 12412.0) / 340.0;
void setup() 
{
    //Usando o monitor serial para visualizar as mensagens
    Serial.begin(9600);
    while (!Serial) ; // Esperando a porta usb ser conectada
    Serial.println("start");
    
    //Inicializando os componentes utilizados
    bmp180.begin();
    Wire.begin();
    mpu6050.initialize();
    // Define SPI SS pin para saida senão as funções da biblioteca SD não funcionarão.
    pinMode(SDFILE_PIN_CS, OUTPUT);
    // Confere se o cartão esta conectado e pode ser usado
    if (!SD.begin()) {
        Serial.println(F("Card failed, or not present"));
        while(1);
    }
    Serial.println(F("card initialized."));
    menuOption = menu();
    
}


void loop() 
{
    
    
    if(menuOption == '1') {
    // Teste do BMP180
    //A precisao do sensor e de 1m
    double bmp180Alt = bmp180.altitude();
    double bmp180Pressure = bmp180.getPressure();
    double bmp180TempC = bmp180.getTemperatureC();     
    
    Serial.print(F("Altitude: ")); Serial.print(bmp180Alt,1); Serial.print(F(" [m]"));
    Serial.print(F("\tpressure: ")); Serial.print(bmp180Pressure,1); Serial.print(F(" [hPa]"));
    Serial.print(F("\tTemperature: ")); Serial.print(bmp180TempC,1); Serial.println(F(" [°C]"));
    
    }
    else if(menuOption == '2') {
    // Teste do Giroscopio
    mpu6050.getMotion6(&mpu6050Ax, &mpu6050Ay, &mpu6050Az, &mpu6050Gx, &mpu6050Gy, &mpu6050Gz);   //Leitura dos dados em 3 eixos
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
    delay(10); //captura de 100 dados em 1 segundo
    
    }
    else if(menuOption == '3') {
    // Teste do cartao SD
    // Arquivo "datalog.txt" criado para salvar os dados no cartao de memoria
   
    sdFile = SD.open("datalog.txt", FILE_WRITE);
    // Se o arquivo existe no cartão começa a gravar os dados dos sensores
    if (sdFile) 
    {
        while (sdFile.available()) 
        {
        
        //Write to file
       
        sdFile.print(mpu6050Ax); sdFile.print("\t");
        sdFile.print(mpu6050Ay); sdFile.print("\t");
        sdFile.print(mpu6050Az); sdFile.print("\t");
        sdFile.print(mpu6050Gx); sdFile.print("\t");
        sdFile.print(mpu6050Gy); sdFile.print("\t");
        sdFile.print(mpu6050Gz); sdFile.print("\t");
        sdFile.print(F("Temp- "));   
        sdFile.println(mpu6050Temp);
        
       sdFile.print(F("Altitude: ")); sdFile.print(bmp180Alt,1); sdFile.print(F(" [m]"));
       sdFile.print(F("\tpressure: ")); sdFile.print(bmp180Pressure,1); sdFile.print(F(" [hPa]"));
       sdFile.print(F("\tTemperature: ")); sdFile.print(bmp180TempC,1); sdFile.println(F(" [°C]"));
        }
        // Fechando o arquivo
        sdFile.close();
       
    } 
    else {
        // Se o arquivo não abrir mostrar uma mensagem de erro
        Serial.println(F("error opening file."));
    }
    }
    
    
    
    if (millis() - time0 > timeout)
    {
        menuOption = menu();
    }
    
}



//Menu para ajudar a testar os componentes utilizados
char menu()
{

    Serial.println(F("\nWhich component would you like to test?"));
    Serial.println(F("(1) BMP180"));
    Serial.println(F("(2) MPU6050"));
    Serial.println(F("(3) SD"));
    Serial.println(F("(menu) send anything else or press on board reset button\n"));
    while (!Serial.available());

    // Lendo os dados
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



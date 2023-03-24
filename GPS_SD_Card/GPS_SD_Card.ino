#include <ArduinoJson.h>
#include <TinyGPS.h>
#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Bluetooth not available or not enabled. It is only available for the ESP32 chip.
#endif

BluetoothSerial SerialBT;

#define PMTK_SET_NMEA_UPDATE_1HZ  "$PMTK220,1000*1F"
#define PMTK_SET_NMEA_UPDATE_5HZ  "$PMTK220,200*2C"
#define PMTK_SET_NMEA_UPDATE_10HZ "$PMTK220,100*2F"

#define PMTK_SET_NMEA_OUTPUT_RMCONLY "$PMTK314,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29"
// turn on ALL THE DATA
#define PMTK_SET_NMEA_OUTPUT_ALLDATA "$PMTK314,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0*28"

#define LED 2
#define DISTANCE 5 //50
bool newState;

TinyGPS gps;

void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
    SerialBT.printf("Listing directory: %s\n", dirname);

    File root = fs.open(dirname);
    if(!root){
        SerialBT.println("Failed to open directory");
        return;
    }
    if(!root.isDirectory()){
        SerialBT.println("Not a directory");
        return;
    }

    File file = root.openNextFile();
    while(file){
        if(file.isDirectory()){
            SerialBT.print("  DIR : ");
            SerialBT.println(file.name());
            if(levels){
                listDir(fs, file.path(), levels -1);
            }
        } else {
            SerialBT.print("  FILE: ");
            SerialBT.print(file.name());
            SerialBT.print("  SIZE: ");
            SerialBT.println(file.size());
        }
        file = root.openNextFile();
    }
}

void createDir(fs::FS &fs, const char * path){
    SerialBT.printf("Creating Dir: %s\n", path);
    if(fs.mkdir(path)){
        SerialBT.println("Dir created");
    } else {
        SerialBT.println("mkdir failed");
    }
}

void removeDir(fs::FS &fs, const char * path){
    SerialBT.printf("Removing Dir: %s\n", path);
    if(fs.rmdir(path)){
        SerialBT.println("Dir removed");
    } else {
        SerialBT.println("rmdir failed");
    }
}

void readFile(fs::FS &fs, const char * path){
    SerialBT.printf("Reading file: %s\n", path);

    File file = fs.open(path);
    if(!file){
        SerialBT.println("Failed to open file for reading");
        return;
    }

    SerialBT.print("Read from file: ");
    while(file.available()){
        SerialBT.write(file.read());
    }
    file.close();
}

void writeFile(fs::FS &fs, const char * path, const char * message){
    SerialBT.printf("Writing file: %s\n", path);

    File file = fs.open(path, FILE_WRITE);
    if(!file){
        SerialBT.println("Failed to open file for writing");
        return;
    }
    if(file.print(message)){
        SerialBT.println("File written");
    } else {
        SerialBT.println("Write failed");
    }
    file.close();
}

void appendFile(fs::FS &fs, const char * path, const String message){
    SerialBT.printf("Appending to file: %s\n", path);

    File file = fs.open(path, FILE_APPEND);
    if(!file){
        SerialBT.println("Failed to open file for appending");
        return;
    }
    if(file.print(message)){
        SerialBT.println("Message appended");
    } else {
        SerialBT.println("Append failed");
    }
    file.close();
}

void renameFile(fs::FS &fs, const char * path1, const char * path2){
    SerialBT.printf("Renaming file %s to %s\n", path1, path2);
    if (fs.rename(path1, path2)) {
        SerialBT.println("File renamed");
    } else {
        SerialBT.println("Rename failed");
    }
}

void deleteFile(fs::FS &fs, const char * path){
    SerialBT.printf("Deleting file: %s\n", path);
    if(fs.remove(path)){
        SerialBT.println("File deleted");
    } else {
        SerialBT.println("Delete failed");
    }
}

void testFileIO(fs::FS &fs, const char * path){
    File file = fs.open(path);
    static uint8_t buf[512];
    size_t len = 0;
    uint32_t start = millis();
    uint32_t end = start;
    if(file){
        len = file.size();
        size_t flen = len;
        start = millis();
        while(len){
            size_t toRead = len;
            if(toRead > 512){
                toRead = 512;
            }
            file.read(buf, toRead);
            len -= toRead;
        }
        end = millis() - start;
        SerialBT.printf("%u bytes read for %u ms\n", flen, end);
        file.close();
    } else {
        SerialBT.println("Failed to open file for reading");
    }


    file = fs.open(path, FILE_WRITE);
    if(!file){
        SerialBT.println("Failed to open file for writing");
        return;
    }

    size_t i;
    start = millis();
    for(i=0; i<2048; i++){
        file.write(buf, 512);
    }
    end = millis() - start;
    SerialBT.printf("%u bytes written for %u ms\n", 2048 * 512, end);
    file.close();
}

void getGPS(){
  digitalWrite(LED, HIGH);
  float flat, flon;
  unsigned long age;
  int ano;
  byte mes, dia, hora, minuto, segundo;
  gps.f_get_position(&flat, &flon, &age);
  gps.crack_datetime(&ano,&mes, &dia, &hora, &minuto, &segundo);

  static float OLD_LAT, OLD_LONG;
  double distancia = TinyGPS::distance_between(OLD_LAT, OLD_LONG, flat, flon);
  
  DynamicJsonDocument doc(1024);
  doc["gps"]["lat"] = flat;
  doc["gps"]["long"] = flon;
  doc["sats"] = gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites();
  doc["speed"] = gps.f_speed_kmph();
  doc["alt"] = gps.f_altitude();
  doc["data"]["ano"] = ano;
  doc["data"]["mes"] = mes;
  doc["data"]["dia"] = dia;
  doc["time"]["hora"] = hora;
  doc["time"]["minuto"] = minuto;
  doc["time"]["segundo"] = segundo;
  
  String output;
  serializeJson(doc, output);
  
  if(distancia >= DISTANCE){
    OLD_LAT = flat;
    OLD_LONG = flon;
    appendFile(SD, "/newData.json", output + ",\n");
    //SerialBT.println(distancia);
    SerialBT.println(output);

  }
}//end get GPS

void setup(){
    //Serial.begin(115200);
    Serial2.begin(4800);
    SerialBT.begin("ESP32");
    
  // uncomment this line to turn on only the "minimum recommended" data for high update rates!
   Serial2.println(PMTK_SET_NMEA_OUTPUT_RMCONLY);

  // uncomment this line to turn on all the available data - for 9600 baud you'll want 1 Hz rate
  // Serial2.println(PMTK_SET_NMEA_OUTPUT_ALLDATA);
  
  // Set the update rate
  // 1 Hz update rate
  Serial2.println(PMTK_SET_NMEA_UPDATE_1HZ);
  // 5 Hz update rate- for 9600 baud you'll have to set the output to RMC only (see above)
  //Serial2.println(PMTK_SET_NMEA_UPDATE_5HZ);
  // 10 Hz update rate - for 9600 baud you'll have to set the output to RMC only (see above)
  //  Serial2.println(PMTK_SET_NMEA_UPDATE_10HZ);
  
    pinMode(LED, OUTPUT);
    digitalWrite(LED, HIGH);
    delay(500);

    if(!SD.begin()){
        SerialBT.println("Card Mount Failed");
        SerialBT.println("Card Mount Failed");
        return;
    }
    uint8_t cardType = SD.cardType();

    if(cardType == CARD_NONE){
        SerialBT.println("No SD card attached");
        SerialBT.println("No SD card attached");
        return;
    }

    SerialBT.print("SD Card Type: ");
    if(cardType == CARD_MMC){
        SerialBT.println("MMC");
    } else if(cardType == CARD_SD){
        SerialBT.println("SDSC");
    } else if(cardType == CARD_SDHC){
        SerialBT.println("SDHC");
    } else {
        SerialBT.println("UNKNOWN");
    }

    uint64_t cardSize = SD.cardSize() / (1024 * 1024);
    SerialBT.printf("SD Card Size: %lluMB\n", cardSize);

    listDir(SD, "/", 0);
    // createDir(SD, "/mydir");
    // listDir(SD, "/", 0);
    // removeDir(SD, "/mydir");
    // listDir(SD, "/", 2);
    // writeFile(SD, "/hello.txt", "Hello ");
    // appendFile(SD, "/hello.txt", "World!\n");
    // readFile(SD, "/hello.txt");
    // deleteFile(SD, "/data.json");
    // renameFile(SD, "/hello.txt", "/foo.txt");
    // readFile(SD, "/foo.txt");
    // testFileIO(SD, "/test.txt");
    SerialBT.printf("Total space: %lluMB\n", SD.totalBytes() / (1024 * 1024));
    SerialBT.printf("Used space: %lluMB\n", SD.usedBytes() / (1024 * 1024));
}//end setup

void loop(){
  unsigned long chars;
  unsigned short sentences, failed;

  for (unsigned long start = millis(); millis() - start < 100;){
    while (Serial2.available()){
      char c = Serial2.read();
      // SerialBT.print(c);
      
      if(gps.encode(c)) newState = true;      
      if(!newState) SerialBT.print(c);
      
    }//END WHILE
  }//END FOR
  
  gps.stats(&chars, &sentences, &failed);
  if(chars == 0) newState = false;

  if(newState){ 
    getGPS();
    digitalWrite(LED, LOW);
  }

  // SerialBT.println();
  // SerialBT.println(chars);
  // SerialBT.println(sentences);
  // SerialBT.println(failed);
  // SerialBT.println();


}//end loop

#define PMTK_SET_NMEA_UPDATE_1HZ  "$PMTK220,1000*1F"
#define PMTK_SET_NMEA_UPDATE_5HZ  "$PMTK220,200*2C"
#define PMTK_SET_NMEA_UPDATE_10HZ "$PMTK220,100*2F"

#define PMTK_SET_NMEA_OUTPUT_RMCONLY "$PMTK314,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29"
// turn on ALL THE DATA
#define PMTK_SET_NMEA_OUTPUT_ALLDATA "$PMTK314,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0*28"


void setup(){
  Serial.begin(115200);
  Serial.println("NMEA test!");

  // 9600 NMEA is the default baud rate
  Serial2.begin(4800);
  
  // uncomment this line to turn on only the "minimum recommended" data for high update rates!
//  Serial2.println(PMTK_SET_NMEA_OUTPUT_RMCONLY);

  // uncomment this line to turn on all the available data - for 9600 baud you'll want 1 Hz rate
  Serial2.println(PMTK_SET_NMEA_OUTPUT_ALLDATA);
  
  // Set the update rate
  // 1 Hz update rate
  Serial2.println(PMTK_SET_NMEA_UPDATE_1HZ);
  // 5 Hz update rate- for 9600 baud you'll have to set the output to RMC only (see above)
  //Serial2.println(PMTK_SET_NMEA_UPDATE_5HZ);
  // 10 Hz update rate - for 9600 baud you'll have to set the output to RMC only (see above)
//  Serial2.println(PMTK_SET_NMEA_UPDATE_10HZ);
}



void loop(){
  if (Serial2.available()) {
      Serial.print((char)Serial2.read());    
  }
  if (Serial.available()) {
      Serial2.print((char)Serial.read());
  }
}
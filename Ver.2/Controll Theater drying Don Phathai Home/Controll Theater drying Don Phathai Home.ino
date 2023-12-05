#include "Controll Theater drying.h"

void setup() {

  Serial.begin(115200);

  pinMode(RELAY_1, OUTPUT);
  pinMode(RELAY_2, OUTPUT);
  pinMode(RELAY_3, OUTPUT);
  pinMode(RELAY_4, OUTPUT);
  pinMode(LED_WIFI, OUTPUT);

  digitalWrite(RELAY_1, HIGH);
  digitalWrite(RELAY_2, HIGH);
  digitalWrite(RELAY_3, HIGH);
  digitalWrite(RELAY_4, HIGH);

  tickerWifi.attach(0.1, wifiTick);

  wifiManager.setConfigPortalTimeout(600); 

  wifiManager.setAPCallback(configModeCallBack);

  if (!wifiManager.autoConnect(SSID_NAME, PASSWORD)) {
    // Serial.println("Smart");
    // Serial.print("SSID or Password incorrect");
    ESP.reset(); // คำสั่งรีเซ็ต ESP
  }

  tickerWifi.detach();

  Blynk.config(auth);  //กำหนด Token key

  // Serial.println("NodeMCU/ESP8266 Run");


  delay(3000);
  
  tickerWifi.attach(1, Read_Sensor);
  
  timer.setInterval(1000, DataTO_Bylnk);
  timer.setInterval(100, MainRun);
  timer.setInterval(1000, StatusConnectionWiFi);


}

void loop() {
  
  Blynk.run();
  timer.run();

}

BLYNK_CONNECTED()
{
  
  Blynk.virtualWrite(V0, temp_c); 
  Blynk.virtualWrite(V1, humidity); 

  Blynk.virtualWrite(V4, Input_humidityOpen); //ส่งสถานะปัจจุบันไปยัง blynk เมื่อเชื่อมต่อใหม่
  Blynk.virtualWrite(V5, Input_humidityClose); //ส่งสถานะปัจจุบันไปยัง blynk เมื่อเชื่อมต่อใหม่
  Blynk.virtualWrite(V4, Input_humidityOpen); //ส่งสถานะปัจจุบันไปยัง blynk เมื่อเชื่อมต่อใหม่
  Blynk.virtualWrite(V5, Input_humidityClose); //ส่งสถานะปัจจุบันไปยัง blynk เมื่อเชื่อมต่อใหม่
  Blynk.virtualWrite(V2, Input_TempOpen); //ส่งสถานะปัจจุบันไปยัง blynk เมื่อเชื่อมต่อใหม่
  Blynk.virtualWrite(V3, Input_TempClose); //ส่งสถานะปัจจุบันไปยัง blynk เมื่อเชื่อมต่อใหม่
  

}

BLYNK_WRITE(V2) {
  Input_TempOpen = param.asFloat();
}

BLYNK_WRITE(V3) {
  Input_TempClose = param.asFloat();
}

BLYNK_WRITE(V4) {
  Input_humidityOpen = param.asFloat();
}

BLYNK_WRITE(V5) {
  Input_humidityClose = param.asFloat();
}



void MainRun() {

  // if(first_run < 10){
  //   Blynk.virtualWrite(V4, Input_humidityOpen);
  //   Blynk.virtualWrite(V5, Input_humidityClose);
  //   Blynk.virtualWrite(V2, Input_TempOpen);
  //   Blynk.virtualWrite(V3, Input_TempClose);

  //   first_run += 1;
  // }  
  

  if(Input_humidityOpen > 0 && Input_TempOpen > 0 && Input_humidityClose > 0 && Input_TempClose > 0){
    // Serial.println("Mod temp+rh");
    if (humidity >= Input_humidityOpen || temp_c >= Input_TempOpen) {
      // Serial.println("Mod rh+temp ON");
      digitalWrite(RELAY_1, LOW);
      digitalWrite(RELAY_2, LOW);
      digitalWrite(RELAY_3, LOW);
      digitalWrite(RELAY_4, LOW);

    } else if (humidity < Input_humidityClose && temp_c < Input_TempClose) {
      // Serial.println("Mod rh+temp OFF");
      digitalWrite(RELAY_1, HIGH);
      digitalWrite(RELAY_2, HIGH);
      digitalWrite(RELAY_3, HIGH);
      digitalWrite(RELAY_4, HIGH);

    }
  }else if(Input_humidityOpen > 0 && Input_TempOpen == 0 && Input_humidityClose > 0 && Input_TempClose == 0){
    // Serial.println("Mod rh");
    if (humidity >= Input_humidityOpen) {
      // Serial.println("Mod temp ON");
      digitalWrite(RELAY_1, LOW);
      digitalWrite(RELAY_2, LOW);
      digitalWrite(RELAY_3, LOW);
      digitalWrite(RELAY_4, LOW);

    } else if (humidity < Input_humidityClose) {
      // Serial.println("Mod rh OFF");
      digitalWrite(RELAY_1, HIGH);
      digitalWrite(RELAY_2, HIGH);
      digitalWrite(RELAY_3, HIGH);
      digitalWrite(RELAY_4, HIGH);

    }

  }else if(Input_humidityOpen == 0 && Input_TempOpen > 0 && Input_humidityClose == 0 && Input_TempClose > 0){
    // Serial.println("Mod temp");
    if (temp_c >= Input_TempOpen) {
      // Serial.println("Mod temp ON");
      digitalWrite(RELAY_1, LOW);
      digitalWrite(RELAY_2, LOW);
      digitalWrite(RELAY_3, LOW);
      digitalWrite(RELAY_4, LOW);

    } else if (temp_c < Input_TempClose) {
      // Serial.println("Mod temp OFF");
      digitalWrite(RELAY_1, HIGH);
      digitalWrite(RELAY_2, HIGH);
      digitalWrite(RELAY_3, HIGH);
      digitalWrite(RELAY_4, HIGH);

    }

  }else if (Input_humidityOpen == 0 && Input_TempOpen == 0 && Input_humidityClose == 0 && Input_TempClose == 0)
     
    digitalWrite(RELAY_1, HIGH);
    digitalWrite(RELAY_2, HIGH);
    digitalWrite(RELAY_3, HIGH);
    digitalWrite(RELAY_4, HIGH);
  {

  }

  
}

//**************************************************************************** อ่านและส่งค่าอุณหูมิความชื้น
void Read_Sensor() {


  temp_c = sht1x.readTemperatureC();
  humidity = sht1x.readHumidity();

  Serial.print("Temperature: ");
  Serial.print(temp_c);
  Serial.print("C / "); 
  Serial.print(humidity);
  Serial.println("%");

  if (isnan(humidity) || isnan(temp_c)) {
    humidity=0;
    temp_c=0;
  }

}

void DataTO_Bylnk() {


  Blynk.virtualWrite(V0, temp_c); 
  Blynk.virtualWrite(V1, humidity); 
  
}

//**************************************************************************** LED ในบอร์ดกระพริบ
void wifiTick() {
  digitalWrite(LED_WIFI, !digitalRead(LED_WIFI));
}

//**************************************************************************** กำหนดค่าโหมด WiFi
void configModeCallBack(WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  Serial.println(myWiFiManager->getConfigPortalSSID());
}

unsigned int conut_status_DeconnectionWiFi = 0;
unsigned int conut_status_Deconnectioninternet = 0;

void StatusConnectionWiFi(){
  if(WiFi.status() != WL_CONNECTED){

    conut_status_DeconnectionWiFi++;

    if(conut_status_DeconnectionWiFi == (5*60)){  // 5 นาที
      ESP.reset();  
    }

  }else{
    conut_status_DeconnectionWiFi = 0;

    if(Blynk.connected() == false){

      conut_status_Deconnectioninternet++;

      if(conut_status_Deconnectioninternet == (5*60)){  // 5 นาที
        ESP.reset(); 
      }
    }else{
      conut_status_Deconnectioninternet = 0;
    }
  }
}

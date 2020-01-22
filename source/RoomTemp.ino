#include <M5Stack.h>
#include "DHT12.h"
#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>
#include <Wire.h>     //The DHT12 uses I2C comunication.
DHT12 dht12;          //Preset scale CELSIUS and ID 0x5c.

#define WIFI_SSID "CodeYourWiFiSSID"
#define WIFI_PASS "CodeYourWiFiPass"
int transmit_interval = 3; //minute
const String server = "https://notify-api.line.me/api/notify";
const String api_key = "CodeYourAPIKey";//LINE Notify APIキーを取得して記述
String StrPerEncord(const char* c_str);
String escapeParameter(String param);
int transmit_time;


void connect_wifi_ap() {
    M5.Lcd.println(WIFI_SSID);

    WiFi.disconnect(true);
    M5.Lcd.printf(".");
    delay(1000);
    M5.Lcd.printf(".");
    WiFiMulti wifiMulti;
    wifiMulti.addAP(WIFI_SSID, WIFI_PASS);

    while (wifiMulti.run() != WL_CONNECTED) {
      delay(500);
      M5.Lcd.printf(".");
    }
    M5.Lcd.println("wifi connect ok");
    delay(1000);
    M5.Lcd.fillScreen(BLACK);
    
  }

void setup() {
  M5.begin();
  Wire.begin();
  M5.Lcd.println("DHT12 TEST:");
  M5.Lcd.setTextSize(3);
  connect_wifi_ap();
  transmit_time = 60 * transmit_interval;
}

void loop() {
  M5.Lcd.setCursor(0, 0);
  
  //Read temperature with preset scale.
  String s_temp = "Temp : " + (String)dht12.readTemperature() + "*C";
  String s_humid = "Humid : " + (String)dht12.readHumidity();

  M5.Lcd.println(s_temp +"\n" + s_humid);

  transmit_time--;
  M5.Lcd.println(transmit_time);
  
  if(transmit_time<=0){
      HTTPClient http;
      String msg = "%0A"+(String)StrPerEncord(escapeParameter(s_temp).c_str()) +"%0A"+(String)StrPerEncord(escapeParameter(s_humid).c_str());
      //String msg = s_temp + "%0A" +s_humid;
      http.begin(server + "?message="+ msg);
      http.addHeader("Authorization","Bearer "+api_key);
      int httpResponseCode = http.POST("POST FROM M5");
      M5.Lcd.printf("HTTP STATUS :");
      M5.Lcd.println(httpResponseCode);
      transmit_time = 60 * transmit_interval;
    }
  delay(1000);
}

String StrPerEncord(const char* c_str) {
  uint16_t i = 0;
  String str_ret = "";
  char c1[3], c2[3], c3[3];

  while (c_str[i] != '\0') {
    if (c_str[i] >= 0xC2 && c_str[i] <= 0xD1) { //2バイト文字
      sprintf(c1, "%2x", c_str[i]);
      sprintf(c2, "%2x", c_str[i + 1]);
      str_ret += "%" + String(c1) + "%" + String(c2);
      i = i + 2;
    } else if (c_str[i] >= 0xE2 && c_str[i] <= 0xEF) {
      sprintf(c1, "%2x", c_str[i]);
      sprintf(c2, "%2x", c_str[i + 1]);
      sprintf(c3, "%2x", c_str[i + 2]);
      str_ret += "%" + String(c1) + "%" + String(c2) + "%" + String(c3);
      i = i + 3;
    } else {
      str_ret += String(c_str[i]);
      i++;
    }
  }
  return str_ret;
}

String escapeParameter(String param) {
  param.replace("%", "%25");
  param.replace("+", "%2B");
  param.replace(" ", "+");
  param.replace("\"", "%22");
  param.replace("#", "%23");
  param.replace("$", "%24");
  param.replace("&", "%26");
  param.replace("'", "%27");
  param.replace("(", "%28");
  param.replace(")", "%29");
  param.replace("*", "%2A");
  param.replace(",", "%2C");
  param.replace("/", "%2F");
  param.replace(":", "%3A");
  param.replace(";", "%3B");
  param.replace("<", "%3C");
  param.replace("=", "%3D");
  param.replace(">", "%3E");
  param.replace("?", "%3F");
  param.replace("@", "%40");
  param.replace("[", "%5B");
  param.replace("\\", "%5C");
  param.replace("]", "%5D");
  param.replace("^", "%5E");
  param.replace("'", "%60");
  param.replace("{", "%7B");
  param.replace("|", "%7C");
  param.replace("}", "%7D");
  return param;
}

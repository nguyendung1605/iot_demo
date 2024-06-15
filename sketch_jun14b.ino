#include <WiFi.h>
#include <string.h>
#include <ThingsBoard.h>
#include <HTTPUpdate.h>
#include <WiFiUdp.h>
#include <coap-simple.h>
#include <ArduinoJson.h>


const char* ssid = "333";		//replace with your wifi ssid and password
const char* password = "12121212";


int temp;       //nhiet do va do am trong nha
int humid;


#define accessToken         "s7oapm6n6qak3op4t8ce"
#define thingsboardServer  "thingsboard.cloud"

//khai báo đối tượng http
 WiFiClient wifiClient;
 HTTPClient http;

void setup_wifi() {
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void setup() {
// put your setup code here, to run once:
  Serial.begin(115200);
  setup_wifi();
}

void send_data (int tem,int hum){
  HTTPClient http;

    // Địa chỉ URL của REST Endpoint trên ThingsBoard
    String url = "http://" + String(thingsboardServer) + "/api/v1/" + String(accessToken) + "/telemetry";

    http.begin(wifiClient,url);
    http.addHeader("Content-Type", "application/json");

    // Dữ liệu gửi đến ThingsBoard

    String payload = "{'tem':"+String(tem)+" , 'humidity':"+String(hum)+"}";
    
    int httpResponseCode = http.POST(payload);
    

    if (httpResponseCode > 0) {
      Serial.print("HTTP: ");
      Serial.println(httpResponseCode);
    } else {
      Serial.print("loi: ");
      Serial.println(httpResponseCode);
    }
      Serial.println(temp);
    http.end();
}

void get_data (){
  HTTPClient http;

    // Địa chỉ URL của REST Endpoint trên ThingsBoard
    String url = "http://" + String(thingsboardServer) + "/api/v1/" + String(accessToken) + "/attributes";

    http.begin(wifiClient,url);
    http.addHeader("Content-Type", "application/json");
    int httpResponseCode = http.GET();
    Serial.println ("get");
    Serial.println(httpResponseCode);
    
    if (httpResponseCode > 0) {
    String payload = http.getString();
    Serial.println("HTTP Response code: " + String(httpResponseCode));
    Serial.println("Response: " + payload);
    parseJson(payload);
  

    }
http.end();
}


void parseJson(String json) {
  StaticJsonDocument<1024> doc;
  DeserializationError error = deserializeJson(doc, json);

  if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.f_str());
    return;
  }

  
  String temperature = doc["shared"]["data"];
  

  Serial.print("Temperature: ");
  Serial.println(temperature);

  Serial.println("v");
  serializeJsonPretty(doc, Serial);
  Serial.println("v");
  Serial.println();

 
}


void loop() {
    temp = random(20,30);
    humid =random(60,95);
  send_data(temp,humid);
  get_data();
  
} 

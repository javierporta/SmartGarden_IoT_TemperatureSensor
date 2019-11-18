/* 
   Temperature and Humidity sensor sending data to WebAPI
*/

//Libraries
#include <DHT.h>;
#include <ESP8266WiFi.h> 
#include "EITIWifi.h"        // classe "EITIWifiClass" | Files (.cpp + .h) provided by professor

//Constants
#define DHTPIN 4     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
#define INTERVALO_ENVIOS    10      // em segundos

DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino


//BEGIN VARIABLES

//Sensor
float hum;  //Stores humidity value
float temp; //Stores temperature value

//WiFi
char* ssid = "labs";
char* password = "robot1cA!ESTG";

//API
String resourceGetTreshold = "http://smart-garden-api.azurewebsites.net/api/TemperatureAndHumidity/threshold"; 
String resourcePost = "http://smart-garden-api.azurewebsites.net/api/TemperatureAndHumidity?password=bangladesh!123"; 

//For local lights
int pinHot = 5;
int pinCold = 2;

EITIWifiClass EITIWiFi;

static uint32_t timer;
static uint32_t intervalo_tempo_s;
float threshold_temperature = 1; //default
float current_temperature;
float last_temperature = -100;
String jsonGetTemperatureAndHumidity = "";
bool temperatureToBeCold = 15;

void setup()
{
  // initialize GPIO outputs
  pinMode(pinHot, OUTPUT);
  pinMode(pinCold, OUTPUT);

    //++++ Initialize serial port +++++
  Serial.begin(115200);
  dht.begin();
  while (!Serial);
  Serial.println(F("\n[Temperature Sensor -> Sending data to Azure]"));
  //-----

    //+++++ Initialize WiFi +++++
  Serial.print(F("Initializing Wi-Fi "));
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
  }
  Serial.println(F("OK."));
  //-----

  //++++ Shiw wifi configs +++++
  Serial.println(F("--- Configuracoes Wi-Fi ---"));
  EITIWiFi.showWifiSettings();
  Serial.println(F("-----"));
  //-----

  //+++++ Init temporizer (to send later a request to init loop())+++++
  intervalo_tempo_s = (INTERVALO_ENVIOS * 1000000);
  timer = micros() - ((INTERVALO_ENVIOS + 1) * 1000000);
  //-----

  Serial.println();
}

void loop()
{
    //+++++ envia um pedido a cada INTERVALO_ENVIOS segundos (para não usar a função delay() no loop()) +++++ 
  if (micros() - timer >= intervalo_tempo_s) {

    //+++++ Se a ligação se mantém ativa... faz pedido(s)
    if (WiFi.status() == WL_CONNECTED) {
      
    //Read data and store it to variables hum and temp
    hum = dht.readHumidity();
    temp= dht.readTemperature();
    //Print temp and humidity values to serial monitor
    Serial.print("Humidity: ");
    Serial.print(hum);
    Serial.print(" %, Temp: ");
    Serial.print(temp);
    Serial.println(" Celsius");

    //To show some lights in local environment
    if (temp > temperatureToBeCold) {
      digitalWrite(pinHot, HIGH);   // turn the LED on (HIGH is the voltage level)
      digitalWrite(pinCold, LOW);
    } 
    else
    {
      digitalWrite(pinCold, HIGH);   // turn the LED on (HIGH is the voltage level)
      digitalWrite(pinHot, LOW);
    }

          //*************************************************
      //   HTTP: Perform the request
      //*************************************************


    //get current temperature from API

      //ToDO: Research how to Get this data (tansform void to function to get the json)
      threshold_temperature = EITIWiFi.httpGet(resourceGetTreshold).toFloat();
      Serial.println("Treshold");
      Serial.println(jsonGetTemperatureAndHumidity);
      

      if (current_temperature >= (last_temperature + threshold_temperature) || current_temperature <= (last_temperature - threshold_temperature)) {
          String data = "{ \"temperature\": " + String(temp) + ", \"humidity\": " + String(hum) + "}";
          Serial.println("Executing Post");          
          Serial.println("Url");          
          Serial.println(resourcePost);
          Serial.println("Parameters");
          Serial.println(data);
          EITIWiFi.httpPost(resourcePost, EITIWifi_POST_JSON, data);
          last_temperature = current_temperature;
     }
       
      //*************************************************
      
  }
        else{ // falha na ligação. Tenta ligar novamente...
      Serial.println(F("--- Attention  ---"));
      Serial.println(F("There is no WiFi conneciton."));
      Serial.print(F("Try again... "));
      WiFi.reconnect();
      Serial.println(F("-----"));
    }
    //-----

    timer = micros();     // iniciar nova contagem para novo envio
  }
  //-----
}

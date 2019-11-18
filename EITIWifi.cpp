#include "EITIWifi.h"

// **************************   CONTRUCTORS   ******************************************
// note:call constructor inside constructor doesn't do what you expected. It just creates a temporary without using it and 
// makes no effect. Then it immediately destructs that temporary when control flows over the ;
// The correct way is initializing the members in each constructor.You could extract their common code in a private init() member function and call it in each constructor.


// **************************     PRIVATE     ******************************************



// **************************     PROTECTED   ******************************************
// use init (as protected) to initilaze classe attributes  (do not call constructores inside contructores)
/**   @brief
*/
void EITIWifiClass::init()
{

}


// **************************     PUBLIC   *********************************************

// Initialize the mandatory object attributes
/**   @brief
*/
void EITIWifiClass::begin()
{

}


/**   @brief Mostra as configurações do módulo WiFi (na Consola)
*/
void EITIWifiClass::showModuleSetting(int show_dhcp_server)
{
	Serial.print(F("MAC: "));
	Serial.println(WiFi.macAddress());
	Serial.print(F("IP: "));
	Serial.println(WiFi.localIP());
	Serial.print(F("Netmask: "));
	Serial.println(WiFi.subnetMask());
	Serial.print(F("Gateway: "));
	Serial.println(WiFi.gatewayIP());
	Serial.print(F("DNS: "));
	Serial.println(WiFi.dnsIP());
	if (show_dhcp_server) {
		Serial.print(F("DHCP: "));
		Serial.println(WiFi.gatewayIP());
	}
}


/**   @brief Mostra as configurações da rede WiFi (na Consola)
*/
void EITIWifiClass::showWifiSettings()
{
	Serial.print(F("Hostname: "));
	Serial.println(WiFi.hostname());

	const char* modes[] = { "NULL", "STATION", "AP", "STATION+AP" };
	Serial.print(F("Modo funcionamento: "));
	Serial.println(modes[(int)WiFi.getMode()]);

	const char* phymodes[] = { "-", "B", "G", "N" };
	Serial.print(F("Modo fisico (norma): "));
	Serial.println(phymodes[(int)WiFi.getPhyMode()]);

	Serial.print(F("Canal: "));
	Serial.println(WiFi.channel());

	Serial.print(F("Potencia do sinal (RSSI): "));
	Serial.print(WiFi.RSSI());
	Serial.println(" dBm");

	Serial.print(F("Ligacao automatica: "));
	Serial.println((WiFi.getAutoConnect() ? F("sim") : F("nao")));

	Serial.print(F("Estado: "));
	Serial.println(WiFi.isConnected() ? F("ligado") : F("desligado"));
}


/**   @brief  Função que permite enviar um pediddo HTTP :: GET, receber a resposta do servidor e enviá-la para a consola
*     @param url - url COMPLETO para o recurso a utilizar (ex: http://www.iot.dei.estg.ipleiria.pt/eiR-plXX-gY/api/lab04_api_get_datetime.php)
*/
String EITIWifiClass::httpGet(String url)
{
  Serial.println(F("\n-----"));
  Serial.print(F("A enviar pedido HTTP-GET para o servidor... "));
  HTTPClient http;
  http.begin(url);              // efetua ligação GET ao servidor (url)
  int httpStatusCode = http.GET();      // ver códigos de estado HTTP em: https://pt.wikipedia.org/wiki/Lista_de_c%C3%B3digos_de_estado_HTTP
  if (httpStatusCode == HTTP_CODE_OK) {   // HTTP_CODE_OK == 200 -> significa que o servidor respondeu de forma correta ao nosso pedido
    Serial.println(F("OK."));
  }
  else {
    Serial.print(F("ERRO: "));
    Serial.println(httpStatusCode);
  }
  String resposta = http.getString();     // obtém o que for enviado no <body> do HTML enviado pelo servidor
  Serial.println(resposta);         // e mostra a inforrmação
  http.end();                 // termina ligação
  return resposta;
}
//-----


/**   @brief  Função que permite enviar um pediddo HTTP :: POST (com dados), receber a resposta do servidor e enviá-la para a consola
*     @param url - url COMPLETO para o recurso a utilizar (ex: http://www.iot.dei.estg.ipleiria.pt/eiR-plXX-gY/api/lab04_api_post_form.php)
*     @param data - dados a enviar junto com o pedido. Podem estar em qualqeur formato (ex: json ou x-www-form-urlencoded)
*/
void EITIWifiClass::httpPost(String url, EITIWifi_POST_Type post_type, String data)
{
	Serial.println(F("\n-----"));
	Serial.print(F("A enviar pedido HTTP-POST para o servidor... "));
	HTTPClient http;
	http.begin(url);							// efetua ligação GET ao servidor (url)
	if (post_type == EITIWifi_POST_FORM)
		http.addHeader("Content-Type", "application/x-www-form-urlencoded");  //Specify content-type header type: form
	else
		http.addHeader("Content-Type", "application/json");  //Specify content-type header type: json
	int httpStatusCode = http.POST(data);		// ver códigos de estado HTTP em: https://pt.wikipedia.org/wiki/Lista_de_c%C3%B3digos_de_estado_HTTP
	if (httpStatusCode == HTTP_CODE_OK) {		// HTTP_CODE_OK == 200 -> significa que o servidor respondeu de forma correta ao nosso pedido
		Serial.println(F("OK."));
	}
	else {
		Serial.print(F("ERRO: "));
		Serial.println(httpStatusCode);
	}
	String resposta = http.getString();			// obtém o que for enviado no <body> do HTML enviado pelo servidor
	Serial.println(resposta);					// e mostra a inforrmação
	http.end();									// termina ligação
}
//-----

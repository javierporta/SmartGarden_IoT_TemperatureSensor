#ifndef _EITI_WIFI_
#define _EITI_WIFI_

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <ESP8266WiFi.h> 
#include <ESP8266HTTPClient.h>

enum EITIWifi_POST_Type 
{
	EITIWifi_POST_FORM,
	EITIWifi_POST_JSON
};

class EITIWifiClass
{
private:
	// attributes

	// methods	

protected:
	// use init (as protected) to initilaze classe attributes  (do not call constructores inside contructores)
	void init(); 

public:
	// Initialize the mandatory object attributes
	void begin();

	// Mostra as configurações da módulo WiFi
	void showModuleSetting(int show_dhcp_server = 1);

	// Mostra as configurações da rede WiFi
	void showWifiSettings();

	// Enviar um pediddo HTTP :: GET, recebe a resposta do servidor e envia-a para a consola
	String httpGet(String url);

	// Enviar um pediddo HTTP :: POST, recebe a resposta do servidor e envia-a para a consola
	void httpPost(String url, EITIWifi_POST_Type post_type, String data);
};


#endif

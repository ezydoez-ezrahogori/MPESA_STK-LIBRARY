
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <MpesaSTK.h>

//get your credentials from;
//https://developer.safaricom.co.ke/user/me/apps
//https://developer.safaricom.co.ke/test_credentials
String consumer_key = "__consumer_key__";
String consumer_secret = "__consumer_secret__";
String pass_key = "__pass_key__";

WiFiMulti wifiMulti;

//MpesaSTK mpesa(consumer_key, consumer_secret, pass_key);	//defaults to SANDBOX environment
MpesaSTK mpesa(consumer_key, consumer_secret, pass_key, SANDBOX);	//environment can be SANDBOX(default) or PRODUCTION


char * WIFI_SSID = "__ssid__";
char * WIFI_PASS = "__password__";


void setup() {
  Serial.begin(115200);
  wifiMulti.addAP(WIFI_SSID, WIFI_PASS);

  Serial.print("Waiting for WiFi to connect...");
  while ((wifiMulti.run() != WL_CONNECTED)) {
    Serial.print(".");
  }
  Serial.println(" connected");
  
  mpesa.begin(TEST_CODE, PAYBILL, "https://mycallbackurl.com/checkout.php");   //call this in setup after connected to the internet

  String result = mpesa.pay("2547XXXX__", 20, "Arduino", "Test");	//STK request
  //you can also implement this in the loop but remember each call performs an STK Request
  Serial.println(result);


}

void loop() {
  //nothing here
}


#include <Arduino.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>
#include <MpesaSTK.h>
#include <ArduinoJson.h>

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

// the callback & check URL is based on the PHP files @

String callbackURL = "https://api.fbiego.com/v1/mpesa/callback";
String checkURL = "https://api.fbiego.com/v1/mpesa/check";


void setup() {
  Serial.begin(115200);
  wifiMulti.addAP(WIFI_SSID, WIFI_PASS);

  Serial.print("Waiting for WiFi to connect...");
  while ((wifiMulti.run() != WL_CONNECTED)) {
    Serial.print(".");
  }
  Serial.println(" connected");

  mpesa.begin(TEST_CODE, PAYBILL, callbackURL);   //call this in setup after connected to the internet


  Serial.println("Sending STK request");
  String result = mpesa.pay("2547XXXX__", 20, "Arduino", "Test");	//STK request (number, amount, reference, description)
  //you can also implement this in the loop but remember each call performs an STK Request

  Serial.println(result);
  DynamicJsonDocument json(1024);
  deserializeJson(json, result);
  String requestCode = json["MerchantRequestID"].as<String>();

  checkPayment(requestCode);

  Serial.println("Done");
}

void loop() {
  //nothing here
}

void checkPayment(String code) {
  HTTPClient http;
  while (true) { // loop until payment is confirmed or cancelled
    http.begin(checkURL + "?query=" + code);
    int httpResponseCode = http.GET();

    Serial.print("HTTP Code: ");
    Serial.println(httpResponseCode);
    String payload = http.getString();
    http.end();
    Serial.println(payload);
    DynamicJsonDocument json(1024);
    deserializeJson(json, payload);
    if (json["success"]) {
      if (json["paid"]) {
        Serial.println("Payment Successful");
      } else {
        Serial.println("Payment cancelled");
      }
      break;
    } else {
      Serial.println("Waiting for user action");
    }

  }

}

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <WiFiManager.h>
#include <HTTPClient.h>

String url = "https://www.howsmyssl.com/a/check";
bool res;

void setup()
{
  // WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
  // it is a good practice to make sure your code sets wifi mode how you want it.

  // put your setup code here, to run once:
  Serial.begin(115200);

  // WiFiManager, Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wm;

  // reset settings - wipe stored credentials for testing
  // these are stored by the esp library
  // wm.resetSettings();

  // Automatically connect using saved credentials,
  // if connection fails, it starts an access point with the specified name ( "AutoConnectAP"),
  // if empty will auto generate SSID, if password is blank it will be anonymous AP (wm.autoConnect())
  // then goes into a blocking loop awaiting configuration and will return success result

  // res = wm.autoConnect(); // auto generated AP name from chipid
  //  res = wm.autoConnect("AutoConnectAP"); // anonymous ap
  res = wm.autoConnect("AutoConnectAP", "password"); // password protected ap

  if (!res)
  {
    Serial.println("Failed to connect");
    // ESP.restart();
  }

  else
  {
    // if you get here you have connected to the WiFi
    Serial.println("connected...yeey :)");
  }
}

void loop()
{
  // put your main code here, to run repeatedly:
  WiFiClientSecure *client = new WiFiClientSecure;
  if (client && res)
  {
    // set secure client with certificate
    client->setInsecure();
    // create an HTTPClient instance
    HTTPClient https;

    // Initializing an HTTPS communication using the secure client
    Serial.print("[HTTPS] begin...\n");
    if (https.begin(*client, url))
    { // HTTPS
      Serial.print("[HTTPS] GET...\n");
      // start connection and send HTTP header
      int httpCode = https.GET();
      // httpCode will be negative on error
      if (httpCode > 0)
      {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTPS] GET... code: %d\n", httpCode);
        // file found at server
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
        {
          // print server response payload
          String payload = https.getString();
          Serial.println(payload);
        }
      }
      else
      {
        Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
      }
      https.end();
    }
  }
  else
  {
    Serial.printf("[HTTPS] Unable to connect\n");
  }
  Serial.println();
  Serial.println("Waiting 2min before the next round...");
  delay(120000);
}

// // Replace with your network credentials
// String url = "https://www.howsmyssl.com/a/check";
// const char *ssid = "Gabriel";
// const char *password = "vjxe24387j";

// void setup()
// {
//   Serial.begin(115200);
//   Serial.println();
//   // Initialize Wi-Fi
//   WiFi.mode(WIFI_STA);
//   WiFi.begin(ssid, password);
//   Serial.print("Connecting to WiFi ..");
//   while (WiFi.status() != WL_CONNECTED)
//   {
//     Serial.print('.');
//     delay(1000);
//   }
//   Serial.println(WiFi.localIP());
// }

// void loop()
// {
//   WiFiClientSecure *client = new WiFiClientSecure;
//   if (client)
//   {
//     // set secure client with certificate
//     client->setInsecure();
//     // create an HTTPClient instance
//     HTTPClient https;

//     // Initializing an HTTPS communication using the secure client
//     Serial.print("[HTTPS] begin...\n");
//     if (https.begin(*client, url))
//     { // HTTPS
//       Serial.print("[HTTPS] GET...\n");
//       // start connection and send HTTP header
//       int httpCode = https.GET();
//       // httpCode will be negative on error
//       if (httpCode > 0)
//       {
//         // HTTP header has been send and Server response header has been handled
//         Serial.printf("[HTTPS] GET... code: %d\n", httpCode);
//         // file found at server
//         if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
//         {
//           // print server response payload
//           String payload = https.getString();
//           Serial.println(payload);
//         }
//       }
//       else
//       {
//         Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
//       }
//       https.end();
//     }
//   }
//   else
//   {
//     Serial.printf("[HTTPS] Unable to connect\n");
//   }
//   Serial.println();
//   Serial.println("Waiting 2min before the next round...");
//   delay(120000);
// }

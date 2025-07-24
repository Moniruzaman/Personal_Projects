/**
   WiFiManager advanced demo, contains advanced configurartion options
   Implements TRIGGEN_PIN button press, press for ondemand configportal, hold for 3 seconds for reset settings.
*/
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager


#include <Arduino_JSON.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <SoftwareSerial.h>
#include <Arduino.h>
#include <rdm6300.h>

JSONVar myObject;
#define RDM6300_RX_PIN 14 // read the SoftwareSerial doc above! may need to change this pin to 10...
Rdm6300 rdm6300;
#define TRIGGER_PIN 15
int Buzzer = 12;
int relay = 5;
int Button = 4;
String T;
// wifimanager can run in a blocking mode or a non blocking mode
// Be sure to know how to process loops with no delay() if using non blocking
bool wm_nonblocking = false; // change to true to use non blocking

WiFiManager wm; // global wm instance
WiFiManagerParameter custom_field; // global param ( for non blocking w params )


//Your Domain name with URL path or IP address with path
String serverName = "http://sns.orientcomputers.com/api/device-data";
unsigned long lastTime = 0;
// Timer set to 10 minutes (600000)
//unsigned long timerDelay = 600000;
// Set timer to 5 seconds (5000)
unsigned long timerDelay = 2000;

void setup() {


  WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  delay(3000);
  Serial.println("\n Starting");
  pinMode(Buzzer, OUTPUT);
  pinMode(relay, OUTPUT);
  digitalWrite(Buzzer, LOW);
  pinMode(TRIGGER_PIN, INPUT);
  pinMode(Button, INPUT);
  
  pinMode(2, OUTPUT);// debug
  // wm.resetSettings(); // wipe settings

  if (wm_nonblocking) wm.setConfigPortalBlocking(false);

  // add a custom input field
  int customFieldLength = 40;


  // new (&custom_field) WiFiManagerParameter("customfieldid", "Custom Field Label", "Custom Field Value", customFieldLength,"placeholder=\"Custom Field Placeholder\"");

  // test custom html input type(checkbox)
  // new (&custom_field) WiFiManagerParameter("customfieldid", "Custom Field Label", "Custom Field Value", customFieldLength,"placeholder=\"Custom Field Placeholder\" type=\"checkbox\""); // custom html type

  // test custom html(radio)
  const char* custom_radio_str = "<br/><label for='customfieldid'>Custom Field Label</label><input type='radio' name='customfieldid' value='1' checked> One<br><input type='radio' name='customfieldid' value='2'> Two<br><input type='radio' name='customfieldid' value='3'> Three";
  new (&custom_field) WiFiManagerParameter(custom_radio_str); // custom html input

  wm.addParameter(&custom_field);
  wm.setSaveParamsCallback(saveParamCallback);

  // custom menu via array or vector
  //
  // menu tokens, "wifi","wifinoscan","info","param","close","sep","erase","restart","exit" (sep is seperator) (if param is in menu, params will not show up in wifi page!)
  // const char* menu[] = {"wifi","info","param","sep","restart","exit"};
  // wm.setMenu(menu,6);
  std::vector<const char *> menu = {"wifi", "info", "param", "sep", "restart", "exit"};
  wm.setMenu(menu);

  // set dark theme
  wm.setClass("invert");


  //set static ip
  // wm.setSTAStaticIPConfig(IPAddress(10,0,1,99), IPAddress(10,0,1,1), IPAddress(255,255,255,0)); // set static ip,gw,sn
  // wm.setShowStaticFields(true); // force show static ip fields
  // wm.setShowDnsFields(true);    // force show dns field always

  // wm.setConnectTimeout(20); // how long to try to connect for before continuing
  wm.setConfigPortalTimeout(120); // auto close configportal after n seconds
  // wm.setCaptivePortalEnable(false); // disable captive portal redirection
  // wm.setAPClientCheck(true); // avoid timeout if client connected to softap
  digitalWrite(2, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(300);
  digitalWrite(2, LOW);
  delay(300);
  digitalWrite(2, HIGH);
  delay(300);
  digitalWrite(2, LOW);
  delay(300);
  digitalWrite(2, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(300);
  digitalWrite(2, LOW);
  delay(300);
  digitalWrite(2, HIGH);
  delay(300);
  digitalWrite(2, LOW);
  delay(300);
  digitalWrite(2, HIGH);
  delay(300);
  digitalWrite(2, LOW);
  delay(300);
  digitalWrite(2, HIGH);
  delay(300);
  digitalWrite(2, LOW);
  delay(300);
  digitalWrite(2, HIGH);

  // wifi scan settings
  // wm.setRemoveDuplicateAPs(false); // do not remove duplicate ap names (true)
  // wm.setMinimumSignalQuality(20);  // set min RSSI (percentage) to show in scans, null = 8%
  // wm.setShowInfoErase(false);      // do not show erase button on info page
  // wm.setScanDispPerc(true);       // show RSSI as percentage not graph icons

  // wm.setBreakAfterConfig(true);   // always exit configportal even if wifi save fails

  bool res;
  // res = wm.autoConnect(); // auto generated AP name from chipid
  // res = wm.autoConnect("AutoConnectAP"); // anonymous ap
  res = wm.autoConnect("AutoConnectAP", "password"); // password protected ap

  if (!res) {
    Serial.println("Failed to connect or hit timeout");
    // ESP.restart();
  }
  else {
    //if you get here you have connected to the WiFi
    Serial.println("connected...yeey :)");
    digitalWrite(2, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(1000);
    digitalWrite(2, LOW);
    delay(1000);
    digitalWrite(2, HIGH);
    delay(1000);
    digitalWrite(2, LOW);
    delay(1000);
    digitalWrite(2, HIGH);

    digitalWrite(Buzzer, HIGH);
    delay(1000);
    digitalWrite(Buzzer, LOW);
  }
  rdm6300.begin(RDM6300_RX_PIN);
  Serial.println("Bring your RFID Card Closer...");
}

void checkButton() {
  // check for button press
  if ( digitalRead(TRIGGER_PIN) == HIGH ) {
    // poor mans debounce/press-hold, code not ideal for production
    delay(50);
    if ( digitalRead(TRIGGER_PIN) == HIGH ) {
      Serial.println("Button Pressed");
      // still holding button for 3000 ms, reset settings, code not ideaa for production
      delay(3000); // reset delay hold
      if ( digitalRead(TRIGGER_PIN) == HIGH ) {
        Serial.println("Button Held");
        Serial.println("Erasing Config, restarting");
        wm.resetSettings();
        ESP.restart();
      }

      // start portal w delay
      Serial.println("Starting config portal");
      wm.setConfigPortalTimeout(120);

      if (!wm.startConfigPortal("OnDemandAP", "password")) {
        Serial.println("failed to connect or hit timeout");
        delay(3000);
        // ESP.restart();
      } else {
        //if you get here you have connected to the WiFi
        Serial.println("connected...yeey :)");
        digitalWrite(2, HIGH);   // turn the LED on (HIGH is the voltage level)
        delay(3000);
        digitalWrite(2, LOW);

      }
    }
  }
}


String getParam(String name) {
  //read parameter from server, for customhmtl input
  String value;
  if (wm.server->hasArg(name)) {
    value = wm.server->arg(name);
  }
  return value;
}

void saveParamCallback() {
  Serial.println("[CALLBACK] saveParamCallback fired");
  Serial.println("PARAM customfieldid = " + getParam("customfieldid"));
}

void loop() {
  // if()
  if (rdm6300.get_new_tag_id())
  { T = String(rdm6300.get_tag_id(), DEC);

    Serial.println(T);
    //if(T=="14042556"){
    //   Serial.println(T); Serial.println("................................................");
    //
    //  }else{

    digitalWrite(Buzzer, HIGH);
    delay(1000);
    digitalWrite(Buzzer, LOW);

    get_request();
    delay(1000);
  }
  //  }



 if ( digitalRead(Button) == HIGH ) {
    // poor mans debounce/press-hold, code not ideal for production
//    delay(50);
        digitalWrite(Buzzer, HIGH);
        digitalWrite(relay, HIGH);
        delay(1000);
        digitalWrite(relay, LOW);
        // delay(1000);
        digitalWrite(Buzzer, LOW);
    }




  if (wm_nonblocking) wm.process(); // avoid delays() in loop when non-blocking and other long running code
  checkButton();
  // put your main code here, to run repeatedly:
}

void get_request() {
  //Check WiFi connection status
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    HTTPClient http;

    String serverPath = serverName + "?macid=12909&rfid=" + T;
    Serial.println(serverPath);
    // Your Domain name with URL path or IP address with path
    http.begin(client, serverPath.c_str());

    // Send HTTP GET request
    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      if (httpResponseCode == 200) {

      } else {
        //digitalWrite(2, LOW);
      }
      String payload = http.getString();
      Serial.print("payload: ");

      Serial.println(payload);
      myObject = JSON.parse(payload);
      Serial.println(myObject["access"]);
      if (int(myObject["access"]) == 1) {
        digitalWrite(Buzzer, HIGH);   // turn the LED on (HIGH is the voltage level)

        digitalWrite(relay, HIGH);
        delay(1000);
        digitalWrite(relay, LOW);
        // delay(1000);
        digitalWrite(Buzzer, LOW);
      } else {
        digitalWrite(2, LOW);
        delay(1000);
        digitalWrite(2, HIGH);
        digitalWrite(relay, LOW);
      }
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    // Free resources
    http.end();
  }
  else {
    Serial.println("WiFi Disconnected");
  }
}

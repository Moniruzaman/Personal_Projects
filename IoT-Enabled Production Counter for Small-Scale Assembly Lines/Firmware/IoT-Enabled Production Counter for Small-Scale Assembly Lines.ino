#include <Arduino.h>
#include <TaskScheduler.h>

#include <SPI.h>

#include <TFT_eSPI.h> // Hardware-specific library
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#include "time.h"

#define TRIGGER_PIN 35

bool wm_nonblocking = false; // change to true to use non blocking

WiFiManager wm; // global wm instance
WiFiManagerParameter custom_field; // global param ( for non blocking w params )


TFT_eSPI tft = TFT_eSPI();       // Invoke custom library

unsigned long drawTime = 0;
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 21600;
const int   daylightOffset_sec = 0;
Scheduler runner;

void core1Task(){
  while(1){
    // Code for core 1
    core_data();
  }
}

void core0Task(){
  while(1){
    // Code for core 0
  }
}

void setup() {
 
   tft.begin();
  tft.setRotation(1);
   int xpos;
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
   WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP  
  Serial.begin(115200);
  Serial.setDebugOutput(true);  
  delay(3000);
  Serial.println("\n Starting");

  pinMode(TRIGGER_PIN, INPUT);
  
  // wm.resetSettings(); // wipe settings

  if(wm_nonblocking) wm.setConfigPortalBlocking(false);

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
  std::vector<const char *> menu = {"wifi","info","param","sep","restart","exit"};
  wm.setMenu(menu);

  // set dark theme
  wm.setClass("invert");


  //set static ip
  // wm.setSTAStaticIPConfig(IPAddress(10,0,1,99), IPAddress(10,0,1,1), IPAddress(255,255,255,0)); // set static ip,gw,sn
  // wm.setShowStaticFields(true); // force show static ip fields
  // wm.setShowDnsFields(true);    // force show dns field always

  // wm.setConnectTimeout(20); // how long to try to connect for before continuing
  wm.setConfigPortalTimeout(30); // auto close configportal after n seconds
  // wm.setCaptivePortalEnable(false); // disable captive portal redirection
  // wm.setAPClientCheck(true); // avoid timeout if client connected to softap

  // wifi scan settings
  // wm.setRemoveDuplicateAPs(false); // do not remove duplicate ap names (true)
  // wm.setMinimumSignalQuality(20);  // set min RSSI (percentage) to show in scans, null = 8%
  // wm.setShowInfoErase(false);      // do not show erase button on info page
  // wm.setScanDispPerc(true);       // show RSSI as percentage not graph icons
  
  // wm.setBreakAfterConfig(true);   // always exit configportal even if wifi save fails

  bool res;
  // res = wm.autoConnect(); // auto generated AP name from chipid
  // res = wm.autoConnect("AutoConnectAP"); // anonymous ap
  res = wm.autoConnect("AutoConnectAP","password"); // password protected ap

  if(!res) {
    Serial.println("Failed to connect or hit timeout");
    // ESP.restart();
  } 
  else {
    //if you get here you have connected to the WiFi    
    Serial.println("connected...yeey :)");
  }
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();

   runner.init();

  runner.addTask(core0Task);
  core0Task.enable();

  runner.addTask(core1Task);
  core1Task.enable();
}

void loop() {
  runner.execute();
}

void core_data(){
   if(wm_nonblocking) wm.process(); // avoid delays() in loop when non-blocking and other long running code  
  checkButton();
  
  tft.setTextSize(2.3); // Set text size
  tft.setCursor(0, 0);
  tft.print("Total count:"); 
 for (int i = 900; i < 1000; i++) {
    yield(); tft.drawNumber(i, 150, 0, 1);
  }
   
tft.setCursor(0, 25);
 tft.print("Today Count:");
  // Print all numbers from 0 to 999 in font 7 and calculate character draw time
  for (int i = 900; i < 1000; i++) {
    yield(); tft.drawNumber(i, 150, 25, 1);
  }

tft.setCursor(50, 50);
 tft.print("Last Count:");
  // Print all numbers from 0 to 999 in font 7 and calculate character draw time
  for (int i = 900; i < 1000; i++) {
     tft.setTextColor(TFT_RED, TFT_BLACK);
    yield(); tft.drawNumber(i, 75, 70, 2);
     tft.setTextColor(TFT_WHITE, TFT_BLACK);
  }
  
tft.setCursor(0, 110);
// tft.print("Time");
  printLocalTime();
   
  
  
  
  }


  
void printLocalTime(){
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  tft.setCursor(0, 110);
  tft.print(&timeinfo, "%m %d %Y %H:%M:%S");
   
  Serial.println();
}

void checkButton(){
  // check for button press
  if ( digitalRead(TRIGGER_PIN) == LOW ) {
    // poor mans debounce/press-hold, code not ideal for production
    delay(50);
    if( digitalRead(TRIGGER_PIN) == LOW ){
      Serial.println("Button Pressed");
      // still holding button for 3000 ms, reset settings, code not ideaa for production
      delay(3000); // reset delay hold
      if( digitalRead(TRIGGER_PIN) == LOW ){
        Serial.println("Button Held");
        Serial.println("Erasing Config, restarting");
        wm.resetSettings();
        ESP.restart();
      }
      
      // start portal w delay
      Serial.println("Starting config portal");
      wm.setConfigPortalTimeout(120);
      
      if (!wm.startConfigPortal("OnDemandAP","password")) {
        Serial.println("failed to connect or hit timeout");
        delay(3000);
        // ESP.restart();
      } else {
        //if you get here you have connected to the WiFi
        Serial.println("connected...yeey :)");
      }
    }
  }
}


String getParam(String name){
  //read parameter from server, for customhmtl input
  String value;
  if(wm.server->hasArg(name)) {
    value = wm.server->arg(name);
  }
  return value;
}

void saveParamCallback(){
  Serial.println("[CALLBACK] saveParamCallback fired");
  Serial.println("PARAM customfieldid = " + getParam("customfieldid"));
}

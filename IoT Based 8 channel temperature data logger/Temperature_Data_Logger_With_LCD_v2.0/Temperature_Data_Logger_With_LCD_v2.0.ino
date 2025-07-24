#include <Adafruit_MAX31856.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define CLK_PIN      18
#define DI_PIN       23
#define DO_PIN       19

#define FAULT_PIN    34
#define DRDY_PIN     35

// CS Pins for each MAX31856
#define CS_PIN_1     32
#define CS_PIN_2     33
#define CS_PIN_3     25
#define CS_PIN_4     5
#define CS_PIN_5     2
#define CS_PIN_6     14
#define CS_PIN_7     12
#define CS_PIN_8     13

#define LCD_ADDRESS  0x27

// Create MAX31856 objects for each thermocouple

// Use software SPI: CS, DI, DO, CLK
Adafruit_MAX31856 maxthermo1(CS_PIN_1, DI_PIN, DO_PIN, CLK_PIN);
Adafruit_MAX31856 maxthermo2(CS_PIN_2, DI_PIN, DO_PIN, CLK_PIN);
Adafruit_MAX31856 maxthermo3(CS_PIN_3, DI_PIN, DO_PIN, CLK_PIN);
Adafruit_MAX31856 maxthermo4(CS_PIN_4, DI_PIN, DO_PIN, CLK_PIN);
Adafruit_MAX31856 maxthermo5(CS_PIN_5, DI_PIN, DO_PIN, CLK_PIN);
Adafruit_MAX31856 maxthermo6(CS_PIN_6, DI_PIN, DO_PIN, CLK_PIN);
Adafruit_MAX31856 maxthermo7(CS_PIN_7, DI_PIN, DO_PIN, CLK_PIN);
Adafruit_MAX31856 maxthermo8(CS_PIN_8, DI_PIN, DO_PIN, CLK_PIN);

// OR use hardware SPI, just pass in the CS pin
// Adafruit_MAX31856 maxthermo1(CS_PIN_1);
// Adafruit_MAX31856 maxthermo2(CS_PIN_2);
// Adafruit_MAX31856 maxthermo3(CS_PIN_3);
// Adafruit_MAX31856 maxthermo4(CS_PIN_4);
// Adafruit_MAX31856 maxthermo5(CS_PIN_5);
// Adafruit_MAX31856 maxthermo6(CS_PIN_6);
// Adafruit_MAX31856 maxthermo7(CS_PIN_7);
// Adafruit_MAX31856 maxthermo8(CS_PIN_8);


// Initialize the LCD at I2C address 0x27 with 16 columns and 2 rows
LiquidCrystal_I2C lcd(LCD_ADDRESS, 16, 2);


void setup() {
  Serial.begin(115200);

  pinMode(FAULT_PIN, INPUT_PULLUP);
  pinMode(DRDY_PIN, INPUT_PULLUP);

  
  // Initialize CS pins as output
  pinMode(CS_PIN_1, OUTPUT);
  pinMode(CS_PIN_2, OUTPUT);
  pinMode(CS_PIN_3, OUTPUT);
  pinMode(CS_PIN_4, OUTPUT);
  pinMode(CS_PIN_5, OUTPUT);
  pinMode(CS_PIN_6, OUTPUT);
  pinMode(CS_PIN_7, OUTPUT);
  pinMode(CS_PIN_8, OUTPUT);

  // Set CS pins to LOW
  digitalWrite(CS_PIN_1, HIGH);
  digitalWrite(CS_PIN_2, HIGH);
  digitalWrite(CS_PIN_3, HIGH);
  digitalWrite(CS_PIN_4, HIGH);
  digitalWrite(CS_PIN_5, HIGH);
  digitalWrite(CS_PIN_6, HIGH);
  digitalWrite(CS_PIN_7, HIGH);
  digitalWrite(CS_PIN_8, HIGH);

  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.print("Initializing...");

  delay(1000);

  // Initialize each MAX31856 and set thermocouple type
  initializeMAX31856(maxthermo1, "Thermocouple 1");  delay(10);
  initializeMAX31856(maxthermo2, "Thermocouple 2");  delay(10);
  initializeMAX31856(maxthermo3, "Thermocouple 3");  delay(10);
  initializeMAX31856(maxthermo4, "Thermocouple 4");  delay(10);
  initializeMAX31856(maxthermo5, "Thermocouple 5");  delay(10);
  initializeMAX31856(maxthermo6, "Thermocouple 6");  delay(10);
  initializeMAX31856(maxthermo7, "Thermocouple 7");  delay(10);
  initializeMAX31856(maxthermo8, "Thermocouple 8");  delay(10);
  
  lcd.clear();
  lcd.print("Thermocouples OK");
  delay(1000);
}

void loop() {
  readTemperature(maxthermo1, "T1");
  readTemperature(maxthermo2, "T2");
  readTemperature(maxthermo3, "T3");
  readTemperature(maxthermo4, "T4");
  readTemperature(maxthermo5, "T5");
  readTemperature(maxthermo6, "T6");
  readTemperature(maxthermo7, "T7");
  readTemperature(maxthermo8, "T8");

  Serial.println("");

  delay(1000); // Wait 1 second before the next reading
}

















void initializeMAX31856(Adafruit_MAX31856 &thermo, const char* name) {
  if (!thermo.begin()) {
    Serial.print("Could not initialize ");
    Serial.print(name);
    Serial.println("! Check wiring.");
    lcd.clear();
    lcd.print(name);
    lcd.setCursor(0, 1);
    lcd.print("Init Failed!");
  }

  thermo.setThermocoupleType(MAX31856_TCTYPE_K); // Set to Type K thermocouple

  Serial.print(name);
  Serial.print(" thermocouple type: ");
  switch (thermo.getThermocoupleType()) {
    case MAX31856_TCTYPE_B: Serial.println("B Type"); break;
    case MAX31856_TCTYPE_E: Serial.println("E Type"); break;
    case MAX31856_TCTYPE_J: Serial.println("J Type"); break;
    case MAX31856_TCTYPE_K: Serial.println("K Type"); break;
    case MAX31856_TCTYPE_N: Serial.println("N Type"); break;
    case MAX31856_TCTYPE_R: Serial.println("R Type"); break;
    case MAX31856_TCTYPE_S: Serial.println("S Type"); break;
    case MAX31856_TCTYPE_T: Serial.println("T Type"); break;
    default: Serial.println("Unknown Type"); break;
  }
  delay(100);
}





void readTemperature(Adafruit_MAX31856 &thermo, const char* name) {
  // unsigned long startMillis = millis();
  // unsigned long timeout = 1000; // Timeout in milliseconds

  //  // Wait until DRDY pin goes low or timeout occurs
  // while (digitalRead(DRDY_PIN)) {
  //   if (millis() - startMillis >= timeout) {
  //     Serial.print(name);
  //     Serial.println(" - DRDY timeout. Skipping reading.");
  //     lcd.clear();
  //     lcd.print(name);
  //     lcd.setCursor(0, 1);
  //     lcd.print("DRDY Timeout");
  //     return; // Exit the function if DRDY does not go low
  //   }
  // }

  uint8_t thermo_Fault = 0;

  // First, attempt to read temperature and then check for fault
  thermo.readThermocoupleTemperature();

  // Check for faults
  thermo_Fault = thermo.readFault();
  if (thermo_Fault) {
    Serial.print(name);
    Serial.print(" - Fault detected! Code: ");
    Serial.println(thermo_Fault, HEX);

    lcd.clear();
    lcd.print(name);
    lcd.setCursor(0, 1);
    lcd.print("Fault Code: ");
    lcd.print(thermo_Fault, HEX);

    if (thermo_Fault & MAX31856_FAULT_CJRANGE) Serial.println("Cold junction range fault");
    if (thermo_Fault & MAX31856_FAULT_TCRANGE) Serial.println("Thermocouple range fault");
    if (thermo_Fault & MAX31856_FAULT_CJHIGH)  Serial.println("Cold junction high fault");
    if (thermo_Fault & MAX31856_FAULT_CJLOW)   Serial.println("Cold junction low fault");
    if (thermo_Fault & MAX31856_FAULT_TCHIGH)  Serial.println("Thermocouple high fault");
    if (thermo_Fault & MAX31856_FAULT_TCLOW)   Serial.println("Thermocouple low fault");
    if (thermo_Fault & MAX31856_FAULT_OVUV)    Serial.println("Over/Under voltage fault");
    if (thermo_Fault & MAX31856_FAULT_OPEN)    Serial.println("Thermocouple open fault");

    // Reinitialize the sensor to clear the fault
    // initializeMAX31856(thermo, name);
    thermo.begin();
    return; // Exit the function after reinitializing
  } else {
    // If no fault, read the temperature
    double temperature = thermo.readThermocoupleTemperature();
    Serial.print(name);
    Serial.print(" - Thermocouple Temperature : ");
    Serial.print(temperature);
    Serial.println(" °C");

    lcd.clear();
    lcd.print(name);
    lcd.setCursor(0, 1);
    lcd.print(temperature);
    lcd.print(" C");

    delay(500); // Delay to allow display to be readable
  }
}
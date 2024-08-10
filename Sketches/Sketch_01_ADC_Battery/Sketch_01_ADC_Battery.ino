/**********************************************************************
  Filename    : ADC_Battery
  Description : Use the ADC of the esp32s3 to detect the battery voltage.
  Auther      : www.freenove.com
  Modification: 2023/02/22
**********************************************************************/
#define PIN_ANALOG_IN  19
void setup() {
  Serial.begin(115200);
}

void loop() {
  int adcVal = analogRead(PIN_ANALOG_IN);      //Gets the raw adc value.
  double voltage = adcVal / 4095.0 * 3.3;      //Convert to the voltage value at the detection point.
  double battery = voltage * 4.0;              //There is only 1/4 battery voltage at the detection point.
  Serial.printf("ADC Val: %d, \t Voltage: %.2fV, \t Battery: %.2fV\r\n", adcVal, voltage, battery);
  delay(200);
}

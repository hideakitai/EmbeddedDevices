#include "MAX17049.h"

MAX17049::MAX17049<2> pwr_mgmt;

void setup()
{
    delay(3000);
    Serial.begin(115200);
    Wire.begin();
}

void loop()
{
    Serial.print("VCELL ADC : ");
    Serial.println(pwr_mgmt.adc());
    Serial.print("VCELL V   : ");
    Serial.println(pwr_mgmt.voltage());
    Serial.print("VCELL SOC : ");
    Serial.print(pwr_mgmt.percent());
    Serial.println(" \%");
    Serial.print("VCELL SOC : ");
    Serial.print(pwr_mgmt.accuratePercent());
    Serial.println(" \%");
    Serial.println();
    delay(1000);
}

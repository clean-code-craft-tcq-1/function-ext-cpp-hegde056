#include <assert.h>
#include <iostream>
#include <string.h>
#include <vector>
using namespace std;

typedef enum {
    ENGLISH,
    DEUTSCH
} LanguageType;

std::vector<std::string> BatteryParamNamesInEnglish{"Temperature" , "SOC" , "Charge Rate" };
std::vector<std::string> BatteryParamNamesInDuetsch{ "Temperatur" , "Ladezustand" , "Laderate" };
std::vector<std::vector<std::string>> BatteryParamNamesMasterList{ BatteryParamNamesInEnglish , BatteryParamNamesInDuetsch };


enum BatteryParam{
    Temperature,
    SOC , 
    ChargeRate,
    NUM_OF_ELEM
};


typedef struct  {
    float min;
    float max;
    float thresholdPercentage;
}BatteryParamInfoType;

BatteryParamInfoType TemperatureInfo =  { 0.0f,  45.0f, 5.0f };
BatteryParamInfoType SocInfo =          { 20.0f, 80.0f, 5.0f };
BatteryParamInfoType ChargingRateInfo = { 0.0f,  0.8f,  5.0f };

std::vector<BatteryParamInfoType> BatteryParamInfo = { TemperatureInfo , SocInfo , ChargingRateInfo };


class BMS {
    
private :
    bool BatteryStatusIndicator = true; 
    float m_temperature , m_soc, m_chargeRate;
    LanguageType m_language; 
public : 
    BMS() {}



    bool IsBatteryHealthOk()
    {
        checkParamInWorkingRange(m_temperature, BatteryParam::Temperature);
        checkParamInWorkingRange(m_soc, BatteryParam::SOC);
        checkParamInWorkingRange(m_chargeRate, BatteryParam::ChargeRate);
        return BatteryStatusIndicator;
    }

    void checkParamInWorkingRange(float paramVal, int paramId)
    {
        if (paramVal < BatteryParamInfo[paramId].min)
        {
            BatteryStatusIndicator = false;
            cout << BatteryParamNamesMasterList[m_language][paramId] << "LOW Breach!!!"<< endl; 
        }
        else if (paramVal > BatteryParamInfo[paramId].max)
        {
            BatteryStatusIndicator = false;
            cout << BatteryParamNamesMasterList[m_language][paramId] << "HIGH Breach!!!" << endl;
        }
        else
        {
            BatteryStatusIndicator = true;
            checkParamAtWarningLevels()
        }
    }


};

int main() {
  assert(batteryIsOk(25, 70, 0.7) == true);
  assert(batteryIsOk(50, 85, 0) == false);
}

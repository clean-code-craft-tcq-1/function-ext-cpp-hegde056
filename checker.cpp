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


std::vector<std::string> DisplayAlertInEnglish{ "Low Breach" , "High Breach" , "Low Warning" , "High Warning" , "Normal" };
std::vector<std::string> DisplayAlertInDuetsch{ "Niedriges Niveau durchbrochen" , "Hoch Niveau durchbrochen" , "Niedrige Warnung","Hoch Warnung" , "Normal" };
std::vector<std::vector<std::string>> DisplayAlertMasterList{ DisplayAlertInEnglish , DisplayAlertInDuetsch };

typedef enum {
    LOW_BREACH,
    HIGH_BREACH,
    LOW_WARNING,
    HIGH_WARNING,
    NORMAL
}AlertType;



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
    float m_temperature , m_soc, m_chargeRate = 0.00;
    LanguageType m_language ; 
public : 
    BMS() 
    {

    }


    void checkParamAtWarningLevels(float paramVal, int paramId)
    {
        if (paramVal < (BatteryParamInfo[paramId].min + BatteryParamInfo[paramId].min * (BatteryParamInfo[paramId].thresholdPercentage / 100.00f)))
        {
            cout << BatteryParamNamesMasterList[m_language][paramId] << " <--> " << DisplayAlertMasterList[m_language][AlertType::LOW_WARNING] << endl;
        }
        else if (paramVal > (BatteryParamInfo[paramId].max - BatteryParamInfo[paramId].max * (BatteryParamInfo[paramId].thresholdPercentage / 100.00f)))
        {
            cout << BatteryParamNamesMasterList[m_language][paramId] << " <--> " << DisplayAlertMasterList[m_language][AlertType::HIGH_WARNING] << endl;
        }
        else
        {
            cout << BatteryParamNamesMasterList[m_language][paramId] << " <--> " << DisplayAlertMasterList[m_language][AlertType::NORMAL] << endl;
        }

    }

    void checkParamInWorkingRange(float paramVal, int paramId)
    {
        if (paramVal < BatteryParamInfo[paramId].min)
        {
            BatteryStatusIndicator = false;
            cout << BatteryParamNamesMasterList[m_language][paramId] << " <--> " << DisplayAlertMasterList[m_language][AlertType::LOW_BREACH] << endl;
        }
        else if (paramVal > BatteryParamInfo[paramId].max)
        {
            BatteryStatusIndicator = false;
            cout << BatteryParamNamesMasterList[m_language][paramId] << " <--> " << DisplayAlertMasterList[m_language][AlertType::HIGH_BREACH] << endl;
        }
        else
        {
            checkParamAtWarningLevels(paramVal, paramId);
        }
    }

    bool IsBatteryHealthOk(float temperature, float soc, float chargeRate, LanguageType language = LanguageType::ENGLISH)
    {
        m_temperature = temperature;
        m_soc = soc;
        m_chargeRate = chargeRate;
        m_language = language;

        checkParamInWorkingRange(m_temperature, BatteryParam::Temperature);
        checkParamInWorkingRange(m_soc, BatteryParam::SOC);
        checkParamInWorkingRange(m_chargeRate, BatteryParam::ChargeRate);
        return BatteryStatusIndicator;
    }



};

int main() {

    BMS* bmsObj = new BMS();

    assert(bmsObj->IsBatteryHealthOk(25, 70, 0.7, ENGLISH) == true);
    assert(bmsObj->IsBatteryHealthOk(50, 85, 0,DEUTSCH) == false);
}

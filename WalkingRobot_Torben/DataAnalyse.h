

#ifndef DATAANALYSE_H__
#define DATAANALYSE_H__


#include <Vector.h>
#define DATA_HYST_LOW ( 10 )

#define DATA_HYST_HIGH ( 20 )
#define DATA_ELEMENTS (3)
#define DATA_MIN_CROP_SIZE (3)
#define DATA_ELEMENT_CROP ((DATA_ELEMENTS < DATA_MIN_CROP_SIZE) ? 0 : 1)
#define DATA_ELEMENT_CROPPED_SIZE ( DATA_ELEMENTS - 2 * DATA_ELEMENT_CROP )
class DataAnalyse 
{
  
  public:
  enum 
  {
    
  };
  typedef struct
  {
    uint32_t distance;
    uint8_t hyst;
  }ValidData_t;
  
  DataAnalyse() : m_HighestValueInDataSet(0)
  {
    m_Data.setStorage(m_EmptyArray);
  }
  ~DataAnalyse() 
  {
  }
  uint32_t getDataResult(uint32_t type = 0)
  {
    uint32_t rv = (0 == type) ? m_Result.hyst : m_Result.distance;
    return rv;
  }
  Vector<uint32_t> CopyIntVec(Vector<uint32_t> *data,int32_t offset)
  {
     Vector<uint32_t> rv;
     uint32_t value;
     rv.setStorage(m_EmptyArray2);
     size_t dataLen = data->size();
     
     
    //Serial.println("CopyIntVec DataLen: "+String(dataLen));
    for(size_t i = offset; i < dataLen; i++)
    {
      value = data->at(i);
      m_HighestValueInDataSet = (value > m_HighestValueInDataSet) ? value : m_HighestValueInDataSet;
      rv.push_back(value);
    }
    return rv;
  }
  void PrintIntVec(Vector<uint32_t> *data)
  {
    #ifdef DEBUG
     size_t dataLen = data->size();
     Serial.print("[");
     for(size_t i = 0; i < dataLen; i++)
     {
        Serial.print(String(data->at(i)) + ",");
     }
     Serial.println("]");
     #endif
  }
  
  Vector<uint32_t> SortIntVec(Vector<uint32_t> *data,bool dir)
  {
    Vector<uint32_t> rv;
    size_t dataLen = data->size();
    rv.setStorage(m_EmptyArray);
    for(size_t i = 1; i <  m_HighestValueInDataSet+1; i++)
    {
      for(size_t j = 0; j <  dataLen; j++)
      {
        if(data->at(j) == i)
        {
         // Serial.println("Data :DD " + String(data->at(j)));
           rv.push_back(i);
        }
      }
    }
    return rv;
  }
  void EvaluateData(Vector<uint32_t> *data)
  {
    uint32_t val = 0;
    int32_t counter = 0;
    for(size_t i = DATA_ELEMENT_CROP; i < data->size() - DATA_ELEMENT_CROP; i++)
    {
      val += data->at(i);
      counter++;
    }
    val = ( counter < 2 ) ? ( (0 == val) ? data->at(0) : val ) : (val/counter);
    m_Result.distance = val; // saved generated data to storage
    Serial.println("Distance after analyse: " + String(val));
    if(val < DATA_HYST_LOW)
    {
      m_Result.hyst = 1;
    }
    else if (val > DATA_HYST_HIGH)
    {
      m_Result.hyst = 2;
    }
    
    
  }
  void sortData(Vector<uint32_t> *data)
  {
    m_WorkingData = CopyIntVec(data,0);
    
    uint32_t sizeNew = m_WorkingData.size();
    PrintIntVec(data);
    m_WorkingData = SortIntVec(&m_WorkingData,false);
    PrintIntVec(&m_WorkingData);
    uint32_t sizeOld = data->size();
   // Serial.print("Value in working data: " + String(m_WorkingData.at(sizeNew-1)));
   // Serial.println("old: " + String(sizeOld) + " New: " + String(sizeNew));
  }
  void ProceedValue(int32_t val)
  {
    uint32_t rv = 0;
    //Serial.println("\nValue raw: " + String(val));
    if(m_Data.size() > ( DATA_ELEMENTS - 1 )  )
    {
      m_Data = CopyIntVec(&m_Data,1); // shift 
    }
    m_Data.push_back(val);
    sortData(&m_Data);
    EvaluateData(&m_Data);
    
  }
private:
  Vector<uint32_t> m_Data;
  Vector<uint32_t> m_WorkingData;
  ValidData_t m_Result;
  uint32_t m_HighestValueInDataSet;
  uint32_t m_EmptyArray[DATA_ELEMENTS];
  uint32_t m_EmptyArray2[DATA_ELEMENTS];
  
};
#endif

void newSurvey(fs::FS &fs)
{
  String fileName = "/surveys/Survey";
  fileName.concat(surveyNum);
  fileName.concat(".csv");
  
  File newSurveyFile = fs.open(fileName, FILE_WRITE);
  if (newSurveyFile)
  {
    currentLogFilePath = newSurveyFile.path();
    currentLogFileName = newSurveyFile.name();

    Serial.print("Current Log Path: ");
    Serial.println(newSurveyFile.path());
  }
  else 
    Serial.println("Error creating file");
  newSurveyFile.close();
  surveyNum++;
  preferences.putInt(surveyNumKey, surveyNum);

}

bool logPoint(fs::FS &fs, String path, uint16_t luxVal, double latittude, double longitude){
    
    bool retVal;
    
    Serial.printf("Appending to file: %s\n", path);

    File file = fs.open(path, FILE_APPEND);
    if(!file){
        Serial.println("Failed to open file for appending");
        return false;
    }
    if(file.print((footcandles ? (luxVal/10.764) : luxVal)) && file.print(", ") && 
        file.print(latittude, 8) && file.print(", ") &&
        file.print(longitude, 8) && file.println()){
        Serial.println("Data Point Logged");
        retVal = true;
    } else {
        Serial.println("Append failed");
        retVal = false;
    }
    file.close();

    return retVal;
}

void deleteSurvey(int sel){

  File file;
  
  for (int i = 1; i < sel; i++)
  {
    file = sdroot.openNextFile();
  }
  
  SD.remove(file.path());
  M5.Lcd.clear();
  M5.Lcd.setTextSize(2);
  M5.Lcd.println(file.name());
  M5.Lcd.println("Deleted");
}
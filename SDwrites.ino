void newSurvey(fs::FS &fs)
{
  String fileName = "/surveys/";
  
  // fileName.concat(incoming_p.month);
  // fileName.concat(incoming_p.day);
  // fileName.concat(incoming_p.year);
  // fileName.concat("_");
  fileName.concat(incoming_p.hour);
  fileName.concat(incoming_p.minute);
  fileName.concat(".csv");
  File newSurveyFile = fs.open(fileName, FILE_WRITE, true);
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
}

void writeFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Writing file: %s\n", path);

    File file = fs.open(path, FILE_WRITE);
    if(!file){
        Serial.println("Failed to open file for writing");
        return;
    }
    if(file.print(message)){
        Serial.println("File written");
    } else {
        Serial.println("Write failed");
    }
    file.close();
}

bool logPoint(fs::FS &fs, String path, uint16_t luxVal, double latittude, double longitude){
    
    bool retVal;
    
    Serial.printf("Appending to file: %s\n", path);

    File file = fs.open(path, FILE_APPEND);
    if(!file){
        Serial.println("Failed to open file for appending");
        return false;
    }
    if(file.print(luxVal) && file.print(", ") && 
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
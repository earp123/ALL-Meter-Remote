static void updateMenu(int index)
{
  
  M5.Lcd.setCursor(0,0);
  M5.Lcd.setTextSize(3);
  

  if (index == 1) M5.Lcd.setTextColor(BLACK, WHITE);//Highlight the current selection
  else            M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.println("TIME ZONE SET");

  if (index == 2) M5.Lcd.setTextColor(BLACK, WHITE);//Highlight the current selection
  else            M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.println("FS MENU");

  if (index == 3) M5.Lcd.setTextColor(BLACK, WHITE);//Highlight the current selection
  else            M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.println("VEML7700 MENU");

  if (index == 4) M5.Lcd.setTextColor(BLACK, WHITE);//Highlight the current selection
  else            M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.println("RX MENU");

  if (index == 5) M5.Lcd.setTextColor(BLACK, WHITE);//Highlight the current selection
  else            M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.println("UNITS");

  if (index == 6) M5.Lcd.setTextColor(BLACK, WHITE);//Highlight the current selection
  else            M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.println("<BACK");

}

void update_unit(int index)
{
  M5.Lcd.setCursor(0,0);
  M5.Lcd.setTextSize(3);
  

  if (index == 1) M5.Lcd.setTextColor(BLACK, WHITE);//Highlight the current selection
  else            M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.println("Fc");

  if (index == 2) M5.Lcd.setTextColor(BLACK, WHITE);//Highlight the current selection
  else            M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.println("Lux");
}

void unit_select()
{
  
  int umenu_idx = 1;
  update_unit(umenu_idx);

  int display_time = millis();
  while((millis() - display_time) < (MENU_TIMEOUT_S*1000))
  {
    switch(butn){
      case ABUTN:
        butn = NONE;
        if(umenu_idx == 1) umenu_idx++;
        else umenu_idx--;
        update_unit(umenu_idx);
        display_time = millis();
        break;

      case BBUTN:
        butn = NONE;
        if(umenu_idx == 1) umenu_idx++;
        else umenu_idx--;
        update_unit(umenu_idx);
        display_time = millis();
        break;

      case CBUTN:
        butn = NONE;
        if(umenu_idx == 1) footcandles = true;
        else footcandles = false;
        mainMenu();
        break;
      case NONE:
        //fall through
      default: break;
    }
    delay(300);
  }
}

void updateTimeZoneSelect()
{
  M5.Lcd.setCursor(0,0);
  M5.Lcd.setTextSize(3);
  M5.Lcd.setTextColor(WHITE, BLACK);

  M5.Lcd.print("GMT     ");if(GMToffset >= 0) M5.Lcd.print("+"); M5.Lcd.print(GMToffset); M5.Lcd.println(" ");
}

void timeZoneSelect()
{
  M5.Lcd.setCursor(0,0);
  M5.Lcd.setTextSize(3);
  M5.Lcd.setTextColor(WHITE, BLACK);

  M5.Lcd.print("GMT     ");if(GMToffset >= 0) M5.Lcd.print("+"); M5.Lcd.println(GMToffset); M5.Lcd.println(" ");

  int display_time = millis();
  while((millis() - display_time) < (MENU_TIMEOUT_S*1000))
  {
    switch(butn){
      case ABUTN:
        butn = NONE;
        GMToffset--;
        updateTimeZoneSelect();
        display_time = millis();
        break;

      case BBUTN:
        butn = NONE;
        GMToffset++;
        updateTimeZoneSelect();
        display_time = millis();
        break;

      case CBUTN:
        butn = NONE;
        preferences.putInt(GMToffsetKey, GMToffset);
        mainMenu();
        break;
      case NONE:
        //fall through
      default: break;
    }
    delay(300);
  }
  M5.Lcd.clear();
}

void mainMenu()
{
  int menu_idx = 1;

  updateMenu(menu_idx);

  int display_time = millis();
  while((millis() - display_time) < (MENU_TIMEOUT_S*1000))
  {
    switch(butn){
      case ABUTN:
        butn = NONE;
        menu_idx++;
        if (menu_idx>6) menu_idx = 1;
        M5.Display.clear();
        updateMenu(menu_idx);
        display_time = millis();
        break;

      case BBUTN:
        butn = NONE;
        menu_idx--;
        if (menu_idx<1) menu_idx = 6;
        updateMenu(menu_idx);
        display_time = millis();
        break;

      case CBUTN:
        butn = NONE;
        if (menu_idx == 6){//BACK
          M5.Lcd.clear();
          mainDisplay();
        }
        else if (menu_idx == 5){//UNITS
          M5.Lcd.clear();
          unit_select();
          
        }
        else if (menu_idx == 4){//RX MENU
          //M5.Lcd.clear();
          //TODO Maya
          
        }
        else if (menu_idx == 3){//VEML7700 
          //M5.Lcd.clear();
          //TODO Maya
          //menuVEML();
        }
        else if (menu_idx == 2){//FS MENU
          M5.Lcd.clear();
          FSmenu();          
          
        }
        else if (menu_idx == 1){//TIME ZONE MENU
          M5.Lcd.clear();
          GMToffset = preferences.getChar(GMToffsetKey);
          timeZoneSelect();
        }

        break;
      case NONE:
        //fall through
      default: break;
    }
    delay(300);
  }
  M5.Lcd.clear();
  
}

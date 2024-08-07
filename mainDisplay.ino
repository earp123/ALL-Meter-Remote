#define MAIN_DISPLAY_TIMEOUT_S 45
#define RADIO_TIMEOUT 20

void init_label(int x_pos, int y_pos, int fg_color, int bg_color, float text_size, String show_text)
{
  M5.Lcd.setCursor(x_pos, y_pos);
  M5.Lcd.setTextSize(text_size);
  M5.Lcd.setTextColor(fg_color, bg_color);
  M5.Lcd.print(show_text);

}

static void updateMainDisplay()
{
  //receiver Battery
  init_label(10, 0, (connected ? BLACK : WHITE), (connected ? GREEN : BLACK), 3, "  ");
  init_label(12, 2, (connected ? BLACK : WHITE), (connected ? GREEN : BLACK), 2.5, "Rx");
  String rx_batt = "";
  rx_batt.concat(incoming_p.rxBatt);
  rx_batt.concat("% ");

  init_label(60, 2, GREEN, BLACK, 2.5, rx_batt);

  //Time
  String display_time = "";
  int d_hour = incoming_p.hour + GMToffset;
  if (d_hour < 0) d_hour += 24;
  else if (d_hour < 10) display_time.concat(" ");
  display_time += d_hour;
  
  display_time += ":";
  if (incoming_p.minute < 10) display_time.concat("0");
  display_time += incoming_p.minute; 
  init_label(225, 0, RED, BLACK, 3, display_time);

  //SIV
  String display_siv = "SIV:";
  display_siv += incoming_p.satsInView;
  display_siv += " ";//covers up the last digit if we go <10
  init_label(10, 50, (incoming_p.satsInView > 20) ? GREEN : (incoming_p.satsInView > 0) ? WHITE : RED, BLACK, 3, display_siv);

  //Horizontal Accuracy
  String display_hza = "Acc:";
  if (incoming_p.horizAcc > 99) 
    init_label(10, 80, WHITE, BLACK, 3, "--             ");
  else if (incoming_p.horizAcc > 0.3)
  {
    display_hza += incoming_p.horizAcc;
    display_hza += "m    ";
    init_label(10, 80, WHITE, BLACK, 3, display_hza);
  }
  else
  {
    display_hza += incoming_p.horizAcc;
    display_hza += "m";
    init_label(10, 80, GREEN, BLACK, 3, display_hza);
  }
  
  //Lat, Long
  /*
  String display_lat = "LAT:";
  init_label(10, 80, WHITE, BLACK, 2, display_lat);
  M5.Lcd.print(incoming_p.latit, 8);//just use the print settings from the above init Fn

  String display_long = "LONG:";
  init_label(10, 100, WHITE, BLACK, 2, display_long);
  M5.Lcd.print(incoming_p.longit, 8);//just use the print settings from the above init Fn
  */

  //Last Lux
  String display_lux = "Last: ";
  if (incoming_p.lux < 65535)
  {
    if(footcandles)
    {
      float fc = incoming_p.lux / 10.764;
      display_lux += fc;
      display_lux += " Fc  ";
    }
    else
    {
      display_lux += incoming_p.lux;
      display_lux += " Lux  ";
    }
    
  }
  else display_lux += "Unstable";
  init_label(10, 120, MAGENTA, BLACK, 3, display_lux);

  String batteryLevel = "Remote:";
  batteryLevel += M5.Power.getBatteryLevel();
  batteryLevel += "%    ";
  init_label( 10, 155, CYAN, BLACK, 2, batteryLevel);

  //Current File Name
  init_label(10, 180, WHITE, BLACK, 2, currentLogFileName);

  //Button Graphics
  init_label( 30, 216, BLACK, BLUE, 3, "    ");
  init_label(125, 216, BLACK, BLUE, 3, "    ");
  init_label(220, 216, BLACK, BLUE, 3, "    ");

  //Button Labels
  init_label(35, 220, BLACK, BLUE, 2.5, "READ");
  init_label(140, 220, BLACK, BLUE, 2.5, "LOG");
  init_label(225, 220, BLACK, BLUE, 2.5, "MENU");

   
}

void mainDisplay()
{
  M5.Lcd.setBrightness(80);
  updateMainDisplay();
  butn = NONE;

  int display_time = millis();
  while((millis() - display_time) < (MAIN_DISPLAY_TIMEOUT_S*1000))
  {
    switch(butn){
      case ABUTN:
        //MEASURE
        butn = NONE;
        if (connected)
        {
          command_p.cmd = LUX_READ;
          result = esp_now_send(rxMAC, (uint8_t*) &command_p, sizeof(command_p));
          if(result != ESP_OK)
          {
            Serial.println("ESPNOW Error");
          }
          incoming_p.read_done = false;
          init_label(10, 120, YELLOW, BLACK, 3, "                ");
          delay(300); //wait for it to get there
          if(incoming_p.cmd_recvd)
          {
            int spinner = 0;
            int veml_time = millis();
            bool f_timeout = false;
            while(!incoming_p.read_done && !f_timeout)
            {
              if ((millis() - veml_time) < (RADIO_TIMEOUT*1000))
              {
                //Rx is reading the VEML
                if(spinner == 0) init_label(10, 120, YELLOW, BLACK, 3, ".");
                else M5.Lcd.print(".");
                spinner++;
                if (spinner >=6)
                {
                  spinner = 0;
                  init_label(10, 120, YELLOW, BLACK, 3, "          ");
                } 
                delay(350);
              }
              else
              {
                f_timeout=true;
                init_label(10, 120, YELLOW, BLACK, 3, "Timed out.");
              }
            }
          }
          else Serial.println("Command wasn't received.");
        }
        
        display_time = millis();
        command_p.cmd = NO_CMD;
        break;

      case BBUTN:
        //MEASURE and LOG
        butn = NONE;
        if (connected)
        {
          command_p.cmd = LUX_READ;
          result = esp_now_send(rxMAC, (uint8_t*) &command_p, sizeof(command_p));
          incoming_p.read_done = false;
          init_label(10, 120, YELLOW, BLACK, 3, "                ");
        
          delay(300); //wait for it to get there
          if(incoming_p.cmd_recvd)
          {
            int spinner = 0;
            int veml_time = millis();
            bool f_timeout = false;
            while(!incoming_p.read_done && !f_timeout)
            {
              if ((millis() - veml_time) < (RADIO_TIMEOUT*1000))
              {
                //Rx is reading the VEML
                if(spinner == 0) init_label(10, 120, YELLOW, BLACK, 3, ".");
                else M5.Lcd.print(".");
                spinner++;
                if (spinner >=6)
                {
                  spinner = 0;
                  init_label(10, 120, YELLOW, BLACK, 3, "          ");
                } 
                delay(350);
              }
              else
              {
                f_timeout=true;
                init_label(10, 120, YELLOW, BLACK, 3, "Timed out.");
              } 
              
            }
            if (incoming_p.lux < 65535)
            {
              if (logPoint(SD, currentLogFilePath, incoming_p.lux, incoming_p.latit, incoming_p.longit))
              {
                Serial.println("File written to");
              }
              else
              {
                Serial.println("Failed to write");
              }
            }
            else Serial.println("Measurement unstable - not logged");
          }
          else Serial.println("Command wasn't received.");
        }
        display_time = millis();
        command_p.cmd = NO_CMD;
        break;

      case CBUTN:
        butn = NONE;
        M5.Lcd.clear();
        mainMenu();
        break;
      case NONE:
        //fall through
      default: updateMainDisplay();
    }

    delay(500);
    lastPacket_s++;
    if (lastPacket_s > 100) connected = false; //connected timeout = delay
    else                  connected = true;
  }
  M5.Lcd.clear();
}
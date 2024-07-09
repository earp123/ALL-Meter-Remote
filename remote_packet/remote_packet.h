struct remote_packet {
  float lux = 45;
  uint8_t satsInView = 23;
  float horizAcc = 0.02;
  
  //use 8 decminals
  double latit  = 45.12345678;
  double longit = 23.87654321;
  
  //from Rx RTC
  int hour = 12;
  int minute = 34;
  int month = 12;
  int day = 29;
  int year = 2024;
  
  int rxBatt = 9;
  
  bool read_done = true;
  bool cmd_recvd = false;
    
};
enum rx_command {
	LUX_READ, NO_CMD, PWR_OFF, VEML_PARAM
};

struct rx_packet {
	
	rx_command cmd = NO_CMD;
	
	uint8_t data[32] = {0};
};
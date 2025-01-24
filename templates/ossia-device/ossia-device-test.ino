
#include "ossia_embedded_api.hpp"
#include "impl-ossia.hpp"
#include "utility.hpp"
#include "constants.hpp"

#include <WiFi.h>

void setup() {
  Serial.setTimeout(10);
  Serial.begin(9600);
  
  { Initialize _("Wi-Fi");
  // Wi-Fi
  WiFi.begin(wifi_ssid, wifi_pass);
  int wifi_retries_count = 40;
  while(WiFi.status() != WL_CONNECTED)
  {
    delay(300);
    if(wifi_retries_count-- == 0) 
    {
      _.error();
      return;
    }
  }
  }
  
  Serial.println(WiFi.localIP());

  ossia_init_board();
}


void loop() {

ossia_read_pins();
ossia_read_osc();

// ossia_process();

// do custom processing here.

ossia_write_pins();
ossia_write_osc();

}

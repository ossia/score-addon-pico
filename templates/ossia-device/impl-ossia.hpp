#pragma once
#include "ossia_embedded_api.hpp"
#include "constants.hpp"
#include "utility.hpp"

#include <soc/adc_channel.h>

#include <OSCBundle.h>
#include <WiFiUdp.h>
#include <DacESP32.h>
  
WiFiUDP ossia_udp_socket;
OSCBundle ossia_osc_bundle;

// Global variables
DacESP32 ossia_dac_1(DAC_CHANNEL_1);

struct ossia_data_model
{
float v_led1_A;
bool v_pot1_SIG;

} ossia_model;

void ossia_init_board() {

  // UDP
  { Initialize _("UDP"); 
  ossia_udp_socket.begin(local_osc_udp_port);
  }

  // Per-device init
  
}

void ossia_read_osc() {
  OSCMessage inmsg;
  int size = ossia_udp_socket.parsePacket();
  if(size <= 0)
    return;

  while(size--)
    inmsg.fill(ossia_udp_socket.read());

  if(inmsg.hasError())
    return;

  inmsg.dispatch("/led1/A", [](OSCMessage& msg) { ossia_model.v_led1_A = msg.getFloat(0); });

}

void ossia_read_pins() {
  ossia_model.v_pot1_SIG = digitalRead(0);

}

void ossia_write_osc() {
  ossia_osc_bundle.add("/pot1/SIG").add(ossia_model.v_pot1_SIG);


  ossia_udp_socket.beginPacket(remote_osc_udp_host, remote_osc_udp_port);
  ossia_osc_bundle.send(ossia_udp_socket);
  ossia_udp_socket.endPacket();
  //Udp.flush();
  ossia_osc_bundle.empty();
}

void ossia_write_pins() {
  ossia_dac_1.outputVoltage(ossia_model.v_led1_A * 3.3f);

}

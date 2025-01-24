#pragma once
#include "ossia_embedded_api.hpp"
#include "constants.hpp"
#include "utility.hpp"
#include <WiFiUdp.h>
#include <DacESP32.h>
 

DacESP32 ossia_dac(DAC_CHANNEL_1);

WiFiUDP ossia_udp_socket;
OSCBundle ossia_osc_bundle;

struct ossia_data_model
{
    // Read
    float sensor{};

    // Write
    float led{};
} ossia_model;

void ossia_init_board()
{
  // UDP
  { Initialize _("UDP"); 
  ossia_udp_socket.begin(local_osc_udp_port);
  }
}

void ossia_read_pins()
{
  model.sensor = analogRead(36) / 4095.f;
}

void ossia_write_pins()
{
  ossia_dac.outputVoltage(ossia_model.led * 3.3f);
}

void ossia_process()
{
  model.led = model.sensor;
}

void ossia_read_osc()
{
  OSCMessage inmsg;
  int size = ossia_udp_socket.parsePacket();
  if(size <= 0)
    return;

  while(size--)
    inmsg.fill(ossia_udp_socket.read());

  if(inmsg.hasError())
    return;

#define read_variable(addr, var) \
  inmsg.dispatch(addr, [](OSCMessage& msg) { var = get_float(msg);  });

  read_variable("/led", ossia_model.led);
}


void ossia_write_osc()
{
  ossia_osc_bundle.add("/sensor").add(ossia_model.sensor);
  ossia_udp_socket.beginPacket(remote_osc_udp_host, remote_osc_udp_port);
  ossia_osc_bundle.send(ossia_udp_socket);
  ossia_udp_socket.endPacket();
  //Udp.flush();
  ossia_osc_bundle.empty();
}

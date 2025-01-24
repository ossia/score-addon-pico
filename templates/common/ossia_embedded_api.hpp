#pragma once

extern "C" {
void ossia_init_board();

void ossia_read_pins();
void ossia_write_pins();

void ossia_read_net();
void ossia_write_net();

void ossia_run_graph();
}

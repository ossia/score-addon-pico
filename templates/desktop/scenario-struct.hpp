std::vector<ossia::token_request> process_exec_table[4] = { };
struct scenario_t {
  std::shared_ptr<ossia::scenario> itv_0_sc_1 = std::make_shared<ossia::scenario>();
  const std::shared_ptr<ossia::time_sync>& itv_0_sc_1_ts_0 = itv_0_sc_1->get_start_time_sync();
  std::shared_ptr<ossia::time_sync> itv_0_sc_1_ts_1 = std::make_shared<ossia::time_sync>();
  std::shared_ptr<ossia::time_sync> itv_0_sc_1_ts_2 = std::make_shared<ossia::time_sync>();
  std::shared_ptr<ossia::time_sync> itv_0_sc_1_ts_3 = std::make_shared<ossia::time_sync>();
  std::shared_ptr<ossia::time_event> itv_0_sc_1_ev_0 = std::make_shared<ossia::time_event>(ossia::time_event::exec_callback{}, *itv_0_sc_1_ts_0, ossia::expressions::make_expression_true());
  std::shared_ptr<ossia::time_event> itv_0_sc_1_ev_1 = std::make_shared<ossia::time_event>(ossia::time_event::exec_callback{}, *itv_0_sc_1_ts_1, ossia::expressions::make_expression_true());
  std::shared_ptr<ossia::time_event> itv_0_sc_1_ev_2 = std::make_shared<ossia::time_event>(ossia::time_event::exec_callback{}, *itv_0_sc_1_ts_2, ossia::expressions::make_expression_true());
  std::shared_ptr<ossia::time_event> itv_0_sc_1_ev_3 = std::make_shared<ossia::time_event>(ossia::time_event::exec_callback{}, *itv_0_sc_1_ts_3, ossia::expressions::make_expression_true());
  std::shared_ptr<ossia::time_interval> itv_0_sc_1_itv_1 = ossia::time_interval::create({}, *itv_0_sc_1_ev_0, *itv_0_sc_1_ev_1, 7973223416_tv, 7973223416_tv, 7973223416_tv);
  std::shared_ptr<ossia::node_process_raw> itv_0_sc_1_itv_1_Gradient_1 = std::make_shared<ossia::node_process_raw>(process_exec_table[0]);
  std::shared_ptr<ossia::node_process_raw> itv_0_sc_1_itv_1_avnd_pattern_apply_2 = std::make_shared<ossia::node_process_raw>(process_exec_table[1]);
  std::shared_ptr<ossia::time_interval> itv_0_sc_1_itv_2 = ossia::time_interval::create({}, *itv_0_sc_1_ev_0, *itv_0_sc_1_ev_2, 8210849957_tv, 0_tv, 4611686018427387903_tv);
  std::shared_ptr<ossia::node_process_raw> itv_0_sc_1_itv_2_p_1 = std::make_shared<ossia::node_process_raw>(process_exec_table[2]);
  std::shared_ptr<ossia::node_process_raw> itv_0_sc_1_itv_2_p_2 = std::make_shared<ossia::node_process_raw>(process_exec_table[3]);
  std::shared_ptr<ossia::time_interval> itv_0_sc_1_itv_3 = ossia::time_interval::create({}, *itv_0_sc_1_ev_0, *itv_0_sc_1_ev_3, 3175200000_tv, 3175200000_tv, 3175200000_tv);

  scenario_t() {
    itv_0_sc_1_ts_1->set_expression(ossia::expressions::make_expression_true());
    itv_0_sc_1->add_time_sync(itv_0_sc_1_ts_1);
    itv_0_sc_1_ts_2->set_expression(ossia::expressions::make_expression_true());
    itv_0_sc_1->add_time_sync(itv_0_sc_1_ts_2);
    itv_0_sc_1_ts_3->set_expression(ossia::expressions::make_expression_true());
    itv_0_sc_1->add_time_sync(itv_0_sc_1_ts_3);
    itv_0_sc_1_ts_0->insert(itv_0_sc_1_ts_0->get_time_events().end(), itv_0_sc_1_ev_0);
    itv_0_sc_1_ts_1->insert(itv_0_sc_1_ts_1->get_time_events().end(), itv_0_sc_1_ev_1);
    itv_0_sc_1_ts_2->insert(itv_0_sc_1_ts_2->get_time_events().end(), itv_0_sc_1_ev_2);
    itv_0_sc_1_ts_3->insert(itv_0_sc_1_ts_3->get_time_events().end(), itv_0_sc_1_ev_3);
    itv_0_sc_1->add_time_interval(itv_0_sc_1_itv_1);
    itv_0_sc_1_itv_1->add_time_process(itv_0_sc_1_itv_1_Gradient_1);
    itv_0_sc_1_itv_1->add_time_process(itv_0_sc_1_itv_1_avnd_pattern_apply_2);
    itv_0_sc_1->add_time_interval(itv_0_sc_1_itv_2);
    itv_0_sc_1_itv_2->add_time_process(itv_0_sc_1_itv_2_p_1);
    itv_0_sc_1_itv_2->add_time_process(itv_0_sc_1_itv_2_p_2);
    itv_0_sc_1->add_time_interval(itv_0_sc_1_itv_3);

  }
} scenario;
struct graph_t {
  ao::ColorAutomation itv_0_sc_1_itv_1_Gradient_1 = {
    .inputs = { .curve = { .value = { { { 0.0117096018735363, { 0, 0, 0 } }, { 0.8, { 100, -5.3357444, 5.433189 } },  } } } }
  };
  avnd_tools::PatternUnfolder itv_0_sc_1_itv_1_avnd_pattern_apply_2 = {
    .inputs = { .Pattern = { 0 },  }
  };
  LFO itv_0_sc_1_itv_2_p_1 = {
    .inputs = { .freq = { 1 }, .ampl = { 0 }, .offset = { 0.9722222 }, .jitter = { 0 }, .phase = { 0 }, .waveform = { ao::LFO::Inputs::Waveform::enum_type(0) },  }, .setup = { .rate = 1e6 }
  };
  LFO itv_0_sc_1_itv_2_p_2 = {
    .inputs = { .freq = { 1 }, .ampl = { 0.5 }, .offset = { 0.5 }, .jitter = { 0 }, .phase = { 0 }, .waveform = { ao::LFO::Inputs::Waveform::enum_type(0) },  }, .setup = { .rate = 1e6 }
  };

  graph_t() {

  }
  void execute() {
    {
        // Process: itv_0_sc_1_itv_1_Gradient_1 (N8Gradient12ProcessModelE)
        for(const auto& g_tick : process_exec_table[0]) {
            bool cable_read = false;
            itv_0_sc_1_itv_1_Gradient_1( { .frames = g_tick.frames, .relative_position = g_tick.relative_position } );
            { do_device_write<"OSC:/0/led/color">(itv_0_sc_1_itv_1_Gradient_1.outputs.value.value); }
        }
        process_exec_table[0].clear();
    }
    {
        // Process: itv_0_sc_1_itv_1_avnd_pattern_apply_2 (N4oscr12ProcessModelIN10avnd_tools15PatternUnfolderEEE)
        for(const auto& g_tick : process_exec_table[1]) {
            bool cable_read = false;
            { if (!process_exec_table[0].empty()) { value_adapt((avnd::input_introspection<avnd_tools::PatternUnfolder>::field<0>(itv_0_sc_1_itv_1_avnd_pattern_apply_2.inputs).value), itv_0_sc_1_itv_1_Gradient_1.outputs.value.value); cable_read = true; } }
            avnd_call(itv_0_sc_1_itv_1_avnd_pattern_apply_2);
        }
        process_exec_table[1].clear();
    }
    {
        // Process: itv_0_sc_1_itv_2_p_1 (N7Control14ControlProcessIN5Nodes3LFO2v24NodeENS_10is_controlEEE)
        for(const auto& g_tick : process_exec_table[2]) {
            bool cable_read = false;
            avnd_call(itv_0_sc_1_itv_2_p_1);
            { do_device_write<"OSC:/*/motor/*/speed">((avnd::output_introspection<LFO>::field<0>(itv_0_sc_1_itv_2_p_1.outputs).value)); }
        }
        process_exec_table[2].clear();
    }
    {
        // Process: itv_0_sc_1_itv_2_p_2 (N7Control14ControlProcessIN5Nodes3LFO2v24NodeENS_10is_controlEEE)
        for(const auto& g_tick : process_exec_table[3]) {
            bool cable_read = false;
            avnd_call(itv_0_sc_1_itv_2_p_2);
            { do_device_write<"OSC:/*/led/bright">((avnd::output_introspection<LFO>::field<0>(itv_0_sc_1_itv_2_p_2.outputs).value)); }
        }
        process_exec_table[3].clear();
    }

  }
} graph;

#pragma once

#include "ScoreHelpers.hpp"

#include <Audio.h>

#include <Wire.h>

struct synth
{
  // GUItool: begin automatically generated code
  AudioSynthWaveformDc lfoenvelope;      // xy=589.047534942627,966.6665487289429
  AudioSynthWaveform lfo;                // xy=677.4761581420898,1172.523769378662
  AudioMixer4 mixer1;                    // xy=720.9047355651855,1027.666706085205
  AudioAnalyzePeak peak1;                // xy=949.0476112365723,890.8095207214355
  AudioSynthWaveform voice4a;            // xy=1206.6190299987793,925.6666355133057
  AudioSynthWaveform voice4b;            // xy=1207.6190299987793,963.6666355133057
  AudioSynthNoiseWhite voice4n;          // xy=1207.6190299987793,998.6666355133057
  AudioSynthWaveform voice3b;            // xy=1228.6190299987793,715.6666355133057
  AudioSynthNoiseWhite voice3n;          // xy=1228.6190299987793,750.6666355133057
  AudioSynthWaveform voice3a;            // xy=1233.6190299987793,672.6666355133057
  AudioSynthWaveform voice1b;            // xy=1258.6190299987793,249.66663551330566
  AudioSynthNoiseWhite voice1n;          // xy=1261.6190299987793,293.66663551330566
  AudioSynthWaveform voice2b;            // xy=1261.6190299987793,483.66663551330566
  AudioSynthNoiseWhite voice2n;          // xy=1261.6190299987793,518.6666355133057
  AudioSynthWaveform voice1a;            // xy=1263.6190299987793,206.66663551330566
  AudioSynthWaveform voice2a;            // xy=1263.6190299987793,445.66663551330566
  AudioSynthWaveformDc voice4filterenv;  // xy=1335.6190299987793,1109.6666355133057
  AudioSynthWaveformDc voice4env;        // xy=1349.6190299987793,1048.6666355133057
  AudioMixer4 voice4mix;                 // xy=1352.6190299987793,983.6666355133057
  AudioSynthWaveformDc voice3filterenv;  // xy=1356.6190299987793,861.6666355133057
  AudioSynthWaveformDc voice3env;        // xy=1370.6190299987793,800.6666355133057
  AudioMixer4 voice3mix;                 // xy=1373.6190299987793,735.6666355133057
  AudioSynthWaveformDc voice1filterenv;  // xy=1387.6190299987793,385.66663551330566
  AudioSynthWaveformDc voice2filterenv;  // xy=1389.6190299987793,629.6666355133057
  AudioMixer4 voice1mix;                 // xy=1403.6190299987793,269.66663551330566
  AudioSynthWaveformDc voice2env;        // xy=1403.6190299987793,568.6666355133057
  AudioSynthWaveformDc voice1env;        // xy=1404.6190299987793,334.66663551330566
  AudioMixer4 voice2mix;                 // xy=1406.6190299987793,503.66663551330566
  AudioEffectMultiply voice4multiply;    // xy=1516.6190299987793,1021.6666355133057
  AudioMixer4 voice4filtermodmixer;      // xy=1526.6190299987793,1137.6666355133057
  AudioEffectMultiply voice3multiply;    // xy=1537.6190299987793,773.6666355133057
  AudioMixer4 voice3filtermodmixer;      // xy=1554.6190299987793,883.6666355133057
  AudioEffectMultiply voice1multiply;    // xy=1567.6190299987793,307.66663551330566
  AudioEffectMultiply voice2multiply;    // xy=1570.6190299987793,541.6666355133057
  AudioMixer4 voice2filtermodmixer;      // xy=1580.6190299987793,657.6666355133057
  AudioMixer4 voice1filtermodmixer;      // xy=1584.6190299987793,417.66663551330566
  AudioFilterStateVariable voice4filter; // xy=1699.6190299987793,1044.6666355133057
  AudioFilterStateVariable voice3filter; // xy=1717.6190299987793,817.6666355133057
  AudioFilterStateVariable voice2filter; // xy=1753.6190299987793,564.6666355133057
  AudioFilterStateVariable voice1filter; // xy=1770.6190299987793,359.66663551330566
  AudioMixer4 first4premix;              // xy=2178.6190299987793,1210.6666355133057
  AudioFilterStateVariable delayFilter;  // xy=2627.6190299987793,1404.6666355133057
  AudioMixer4 mainOutMixer;              // xy=2698.6190299987793,1287.6666355133057
  AudioEffectDelay delay1;               // xy=2756.6190299987793,1599.6666355133057
  AudioOutputI2S i2s1;                   // xy=2924.6190299987793,1285.6666355133057
  AudioConnection patchCord1{lfoenvelope, 0, mixer1, 0};
  AudioConnection patchCord2{lfo, 0, voice1filtermodmixer, 1};
  AudioConnection patchCord3{lfo, 0, voice2filtermodmixer, 1};
  AudioConnection patchCord4{lfo, 0, voice3filtermodmixer, 1};
  AudioConnection patchCord5{lfo, 0, voice4filtermodmixer, 1};
  AudioConnection patchCord10{lfo, 0, mixer1, 1};
  AudioConnection patchCord11{mixer1, peak1};
  AudioConnection patchCord15{voice4a, 0, voice4mix, 0};
  AudioConnection patchCord16{voice4b, 0, voice4mix, 1};
  AudioConnection patchCord17{voice4n, 0, voice4mix, 2};
  AudioConnection patchCord27{voice3b, 0, voice3mix, 1};
  AudioConnection patchCord28{voice3n, 0, voice3mix, 2};
  AudioConnection patchCord29{voice3a, 0, voice3mix, 0};
  AudioConnection patchCord30{voice1b, 0, voice1mix, 1};
  AudioConnection patchCord31{voice1n, 0, voice1mix, 2};
  AudioConnection patchCord32{voice2b, 0, voice2mix, 1};
  AudioConnection patchCord33{voice2n, 0, voice2mix, 3};
  AudioConnection patchCord34{voice1a, 0, voice1mix, 0};
  AudioConnection patchCord35{voice2a, 0, voice2mix, 0};
  AudioConnection patchCord39{voice4filterenv, 0, voice4filtermodmixer, 0};
  AudioConnection patchCord42{voice4env, 0, voice4multiply, 1};
  AudioConnection patchCord45{voice4mix, 0, voice4multiply, 0};
  AudioConnection patchCord47{voice3filterenv, 0, voice3filtermodmixer, 0};
  AudioConnection patchCord52{voice3env, 0, voice3multiply, 1};
  AudioConnection patchCord53{voice3mix, 0, voice3multiply, 0};
  AudioConnection patchCord54{voice1filterenv, 0, voice1filtermodmixer, 0};
  AudioConnection patchCord55{voice2filterenv, 0, voice2filtermodmixer, 0};
  AudioConnection patchCord56{voice1mix, 0, voice1multiply, 0};
  AudioConnection patchCord57{voice2env, 0, voice2multiply, 1};
  AudioConnection patchCord58{voice1env, 0, voice1multiply, 1};
  AudioConnection patchCord59{voice2mix, 0, voice2multiply, 0};
  AudioConnection patchCord62{voice4multiply, 0, voice4filter, 0};
  AudioConnection patchCord64{voice4filtermodmixer, 0, voice4filter, 1};
  AudioConnection patchCord68{voice3multiply, 0, voice3filter, 0};
  AudioConnection patchCord71{voice3filtermodmixer, 0, voice3filter, 1};
  AudioConnection patchCord72{voice1multiply, 0, voice1filter, 0};
  AudioConnection patchCord73{voice2multiply, 0, voice2filter, 0};
  AudioConnection patchCord74{voice2filtermodmixer, 0, voice2filter, 1};
  AudioConnection patchCord75{voice1filtermodmixer, 0, voice1filter, 1};
  AudioConnection patchCord78{voice4filter, 0, first4premix, 3};
  AudioConnection patchCord81{voice3filter, 0, first4premix, 2};
  AudioConnection patchCord82{voice2filter, 0, first4premix, 1};
  AudioConnection patchCord83{voice1filter, 0, first4premix, 0};
  AudioConnection patchCord85{first4premix, 0, mainOutMixer, 0};
  AudioConnection patchCord86{delayFilter, 0, mainOutMixer, 3};
  AudioConnection patchCord87{mainOutMixer, 0, i2s1, 0};
  AudioConnection patchCord88{mainOutMixer, 0, i2s1, 1};
  AudioConnection patchCord89{mainOutMixer, delay1};
  AudioConnection patchCord90{delay1, 0, delayFilter, 0};
  AudioControlSGTL5000 sgtl5000_1; // xy=2661.6190299987793,1054.6666355133057
  // GUItool: end automatically generated code

  synth()
  {

    AudioMemory(20);
    Serial.begin(115200);
    sgtl5000_1.enable();
    sgtl5000_1.volume(.7);

    // mix
    first4premix.gain(0, .25);
    first4premix.gain(1, .25);
    first4premix.gain(2, .25);
    first4premix.gain(3, .25);

    // Voice 1
    voice1a.begin(.3, 440, WAVEFORM_SQUARE);
    voice1b.begin(.3, 440, WAVEFORM_SAWTOOTH);
    // Voice 2
    voice2a.begin(.3, 440, WAVEFORM_SQUARE);
    voice2b.begin(.3, 440, WAVEFORM_SAWTOOTH);
    // Voice 3
    voice3a.begin(.3, 440, WAVEFORM_SQUARE);
    voice3b.begin(.3, 440, WAVEFORM_SAWTOOTH);
    // Voice 4
    voice4a.begin(.3, 440, WAVEFORM_SQUARE);
    voice4b.begin(.3, 440, WAVEFORM_SAWTOOTH);

    delayFilter.frequency(3000);
    delayFilter.resonance(1);
    delay1.delay(0, 0);
    mainOutMixer.gain(3, 0);

    // LFO
    lfo.begin(1, 3, WAVEFORM_SINE);

    mainOutMixer.gain(0, .5);
    lfoenvelope.amplitude(1);
  }
  void heh()
  {

    for(;;)
      for(int i = 0; i < 8; i++)
      {
        // audio startup
        //  if(i < 4)
        {
          voice1env.amplitude(.05, 1);
          voice1a.frequency(rand() % 100 + 200);
          voice1b.frequency(rand() % 100 + 400);
          voice1n.amplitude(rand() % 100 / 100.f);
          delay(200);
          voice1env.amplitude(0, 0);
        }
      }
  }
};

namespace Synthimi
{
// Let's define a generic port for the waveform chooser
struct Waveform
{
  halp_meta(columns, 5)
  enum enum_type
  {
    Sine,
    Square,
    Triangle,
    Sawtooth,
    Noise
  } value;
  enum widget
  {
    enumeration,
    list,
    combobox
  };

  struct range
  {
    std::string_view values[5]{"Sine", "Square", "Tri", "Saw", "Noise"};
    std::string_view pixmaps[10]{
        ":/icons/wave_sin_off.png",      ":/icons/wave_sin_on.png",
        ":/icons/wave_square_off.png",   ":/icons/wave_square_on.png",
        ":/icons/wave_triangle_off.png", ":/icons/wave_triangle_on.png",
        ":/icons/wave_saw_off.png",      ":/icons/wave_saw_on.png",
        ":/icons/wave_noise1_off.png",   ":/icons/wave_noise1_on.png"};
    enum_type init = Sine;
  };

  operator enum_type&() noexcept { return value; }
  operator enum_type() const noexcept { return value; }
  auto& operator=(enum_type t) noexcept
  {
    value = t;
    return *this;
  }
};

inline auto m2f(double m)
{
  return (440.0 * std::exp2((m - 69.0) / 12.0));
}

class Synthimi
{
public:
  halp_meta(name, "Synthimi")
  halp_meta(category, "Audio/Synth")
  halp_meta(author, "Jean-Michaël Celerier")
  halp_meta(
      description,
      "A basic synthesizer. Modeled after the meows of the developer's cat, Shashimi.")
  halp_meta(manual_url, "https://ossia.io/score-docs/processes/synthimi.html")
  halp_meta(c_name, "synthimi")
  halp_meta(uuid, "d4008ff6-73b9-4575-80a4-60e3da095db7")

  struct ins
  {
    halp::midi_bus<"In"> midi;

    // Here we'll define our parameters - very simply, and then we'll try to refactor
    // that afterwards :-)
    halp::knob_f32<"Osc 1 Amp.", halp::range{0., 1., 1.}> osc0_amp;
    struct : Waveform
    {
      halp_meta(name, "Osc 1 Wave")
    } osc0_waveform;
    struct : halp::knob_f32<"Osc 1 Pitch", halp::range{-12, 12, 0.}>
    {
    } osc0_pitch;
    struct : halp::knob_i32<"Osc 1 Oct", halp::irange{-5, 5, 0}>
    {
    } osc0_oct;

    halp::knob_f32<"Osc 2 Amp.", halp::range{0., 1., 1.}> osc1_amp;
    struct : Waveform
    {
      halp_meta(name, "Osc 2 Wave")
    } osc1_waveform;
    struct : halp::knob_f32<"Osc 2 Pitch", halp::range{-12, 12, 0.}>
    {
    } osc1_pitch;
    struct : halp::knob_i32<"Osc 2 Oct", halp::irange{-5, 5, 0}>
    {
    } osc1_oct;

    halp::knob_f32<"Osc 3 Amp.", halp::range{0., 1., 1.}> osc2_amp;
    struct : Waveform
    {
      halp_meta(name, "Osc 3 Wave")
    } osc2_waveform;
    struct : halp::knob_f32<"Osc 3 Pitch", halp::range{-12, 12, 0.}>
    {
    } osc2_pitch;
    struct : halp::knob_i32<"Osc 3 Oct", halp::irange{-5, 5, 0}>
    {
    } osc2_oct;

    halp::knob_f32<"Osc 4 Amp.", halp::range{0., 1., 1.}> osc3_amp;
    struct : Waveform
    {
      halp_meta(name, "Osc 4 Wave")
    } osc3_waveform;
    struct : halp::knob_f32<"Osc 4 Pitch", halp::range{-12, 12, 0.}>
    {
    } osc3_pitch;
    struct : halp::knob_i32<"Osc 4 Oct", halp::irange{-5, 5, 0}>
    {
    } osc3_oct;

    halp::knob_f32<"Amp. Attack", halp::range{0., 1., 0.1}> amp_attack;
    halp::knob_f32<"Amp. Decay", halp::range{0., 1., 0.1}> amp_decay;
    halp::knob_f32<"Amp. Sustain", halp::range{0., 1., 0.5}> amp_sustain;
    halp::knob_f32<"Amp. Release", halp::range{0., 1., 0.2}> amp_release;

    struct
    {
      halp__enum_combobox("Type", LPF, LPF, HPF)
    } filt_type;
    halp::knob_f32<"Cutoff", halp::range{20., 20000., 2000.}> filt_cutoff;
    halp::knob_f32<"Reso", halp::range{0., 0., 1.}> filt_res;
    halp::knob_f32<"Flt. Attack", halp::range{0., 1., 0.1}> filt_attack;
    halp::knob_f32<"Flt. Decay", halp::range{0., 1., 0.1}> filt_decay;
    halp::knob_f32<"Flt. Sustain", halp::range{0., 1., 0.5}> filt_sustain;
    halp::knob_f32<"Flt. Release", halp::range{0., 1., 0.2}> filt_release;

    struct
    {
      halp__enum_combobox("Polyphony", Poly, Mono, Poly)
    } poly_mode;
    halp::knob_f32<"Porta", halp::range{0., 1., 0.}> portamento;

    halp::toggle<"Filter", halp::toggle_setup{true}> filt_env;
    halp::knob_f32<"Drive", halp::range{0., 1., 0.}> drive;

    struct
    {
      halp__enum_combobox("Modmatrix", S, S, C, CSP, CST, CSC)
    } matrix;
    halp::knob_i32<"Unison", halp::irange{0, 16, 0}> unison;
  } inputs;

  struct
  {
  } outputs;

  synth the_synth;
  halp::setup settings;
  void prepare(halp::setup info) { settings = info; }
  void operator()(int frames)
  {
    for(auto& mess : inputs.midi.midi_messages)
    {
      switch(mess.bytes[0])
      {
        case 144: {
          float pitch = 440.f * std::exp2((mess.bytes[1] - 69) / 12.);

          the_synth.voice1a.frequency(pitch);
          the_synth.voice1b.frequency(pitch);
          the_synth.voice1env.amplitude(0.5, 1);
          break;
        }
        case 128:
          the_synth.voice1env.amplitude(0.0, 1);
          break;
      }

      // voice1env.amplitude(.05, 1);
      // voice1a.frequency(rand() % 100 + 200);
      // voice1b.frequency(rand() % 100 + 400);
      // voice1n.amplitude(rand() % 100 / 100.f);
      // delay(200);
      // voice1env.amplitude(0, 0);
    }
  }
};
}

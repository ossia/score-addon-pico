#include "score_addon_pico.hpp"

#include <score/plugins/FactorySetup.hpp>

#include <Avnd/Factories.hpp>
#include <Pico/Pico.hpp>

// clang-format off
#include <Avnd/Factories.hpp>
#include "include.pico.cpp"
// clang-format on

#include <score_plugin_engine.hpp>
/**
 * This file instantiates the classes that are provided by this plug-in.
 */

/**
 * 1/ Execution filter that finds all the nodes that talk to
 *    an embedded device explicitly.. Maybe we can just look inside
 *    a specific interval
 *
 * 2/ Compile the nodes as avendish object patch
 *
 * 3/ Establish a communication channel e.g. through wifi
 *
 * 4/ Flash the patch
 *
 * 5/ In the "normal" score exec graph, capture the inputs of the nodes
 *    that are at the "border" between score and the device, e.g.
 *    they get their input from something that cannot be executed on a pico
 *    or from some external input.
 *
 * 6/ Blast them through CBOR or some specific encoding
 *
 * Note: we can do the same for inputs!
 *
 * if there's e.g.
 *
 * sensor > calibrator > filter > mapper > sqrt > lfo
 *
 * we can try to cut at the "sqrt" stage before the LFO.
 * goal: limit as much as possible the data transmission rate
 *
 *
 * - Processes should have "affinity" ? dpeneding on if they blast data or not basically
 * => if they blast *to an output GPIO* we want them running on the embedded
 * => if they blast *from an input GPIO* we want them running on score (or do we? depends on the
 *    reading rate no?)
 *
 * - We need to save the .hpp of each avnd process?
 *
 * For the paper: do it manually
 * - Asssign a tag to the objects
 * - "target device"
 *
 * NOTE: we could also do this with WASM :') compile the objects as wasm blobs and send them
 * to a webpage device!!!!!
 *
 * => add the ability to assign a device to groups (they act like a client? or not? should work
 *    even without a network context)
 *
 * - A device can have multiple things happening in parallel, e.g. 2 automations
 *   which run each on a GPIO: we need some sort of scheduler on the board
 *   => freertos tasks for independent subgraphs
 *
 */
score_addon_pico::score_addon_pico() = default;
score_addon_pico::~score_addon_pico() = default;

std::vector<std::unique_ptr<score::InterfaceBase>> score_addon_pico::factories(
    const score::ApplicationContext& ctx,
    const score::InterfaceKey& key) const
{
  std::vector<std::unique_ptr<score::InterfaceBase>> fx;
  all_custom_factories(fx, ctx, key);

  return fx;
}

std::vector<score::PluginKey> score_addon_pico::required() const
{
  return {score_plugin_engine::static_key()};
}

#include <score/plugins/PluginInstances.hpp>
SCORE_EXPORT_PLUGIN(score_addon_pico)

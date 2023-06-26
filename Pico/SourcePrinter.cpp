#include "SourcePrinter.hpp"

namespace Pico
{

std::string defaultIncludesGraph()
{
  return R"_(
#include <cstdio>
#include <cmath>
#include <cstring>
#include <cstdlib>
#include <cinttypes>
#include <avnd/common/export.hpp>
#include <avnd/introspection/input.hpp>
#include <avnd/introspection/messages.hpp>
#include <avnd/introspection/output.hpp>
#include <avnd/introspection/port.hpp>
#include <avnd/wrappers/avnd.hpp>
#include <avnd/wrappers/controls.hpp>
#include <avnd/wrappers/metadatas.hpp>
#include <avnd/../../examples/Advanced/Utilities/Math.hpp>
#include <ExprtkMapper.hpp>


)_";
}

}

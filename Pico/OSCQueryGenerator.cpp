#include "OSCQueryGenerator.hpp"

#include <Device/Protocol/DeviceInterface.hpp>

#include <ossia/network/generic/generic_device.hpp>
#include <ossia/network/oscquery/detail/json_writer.hpp>

namespace Pico
{
static void
oscquery_generate_namespace_rec(ossia::net::generic_device& odev, const Device::Node& n)
{
  auto settings
      = Device::FullAddressSettings::make<Device::FullAddressSettings::as_parent>(
          n.get<Device::AddressSettings>(), Device::address(*n.parent()));

  ossia::net::node_base* node = Device::createNodeFromPath(settings.address.path, odev);
  if(node && settings.value.v)
  {
    if(auto p = node->create_parameter(settings.value.get_type()))
    {
      auto& addr = *p;
      SCORE_ASSERT(settings.ioType);
      addr.set_access(*settings.ioType);
      addr.set_bounding(settings.clipMode);
      addr.set_repetition_filter(ossia::repetition_filter(settings.repetitionFilter));
      addr.set_value_type(settings.value.get_type());
      addr.set_value(settings.value);
      addr.set_domain(settings.domain);
      addr.set_unit(settings.unit);
      addr.get_node().set_extended_attributes(settings.extendedAttributes);
    }
  }

  for(const auto& child : n)
    oscquery_generate_namespace_rec(odev, child);
}

QString oscquery_generate_namespace(const ::Device::Node& root)
{
  // 1. Load the data model into an ossia device
  ossia::net::generic_device odev;
  for(const auto& child : root)
    oscquery_generate_namespace_rec(odev, child);

  // 2. Use libossia's oscquery generation
  auto str = ossia::oscquery::json_writer::query_namespace(odev.get_root_node());

  QString res = QString(R"!(#pragma once
static constexpr std::string_view oscquery_namespace()
{
  using namespace std::literals;
  return R"_(%1)_"sv;
}
)!")
                    .arg(QString::fromUtf8(str.GetString(), str.GetLength()));
  return res;
}
}

/*
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (C) 2022, KNS Group LLC (YADRO).
 */
#pragma once

#include "base.hpp"

namespace dbus
{
namespace property
{

constexpr auto interface = "org.freedesktop.DBus.Properties";

/** @brief Get a property. */
template <typename Property>
inline Property get(sdbusplus::bus::bus& bus, const std::string& service,
                    const std::string& path, const std::string& interface,
                    const std::string& property)
{
    auto value = dbus::callMethodAndRead<std::variant<Property>>(
        bus, service, path, dbus::property::interface, "Get", interface,
        property);
    return std::get<Property>(value);
}

template <typename Property>
inline void set(sdbusplus::bus::bus& bus, const std::string& service,
                const std::string& path, const std::string& interface,
                const std::string& name, const Property& value)
{
    dbus::callMethod(bus, service, path, dbus::property::interface, "Set",
                     interface, name, std::variant<Property>(value));
}

/** @brief Get all properties. */
template <typename... Types>
inline auto getAll(sdbusplus::bus::bus& bus, const std::string& service,
                   const std::string& path, const std::string& interface = {})
{
    using Value = std::variant<Types...>;
    using Name = std::string;
    using PropertiesMap = std::map<Name, Value>;

    return dbus::callMethodAndRead<PropertiesMap>(
        bus, service, path, dbus::property::interface, "GetAll", interface);
}

} // namespace property
} // namespace dbus

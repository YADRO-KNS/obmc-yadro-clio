/*
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (C) 2022, KNS Group LLC (YADRO).
 */
#pragma once

#include "utils/dbus.hpp"

namespace srvctl
{

struct Service
{
    using PropertyDef =
        std::tuple<std::string, std::string, std::string, std::string>;

    std::string summary;                 //!< Service shor description
    std::vector<PropertyDef> properties; //!< List of D-Bus properties to
                                         //!< manage state

    /**
     * @brief Get actual status of service
     *
     * @param bus - D-Bus connection
     *
     * @return true if service is active and false otherwise
     */
    bool status(sdbusplus::bus::bus& bus) const
    {
        for (const auto& [service, path, iface, name] : properties)
        {
            if (!dbus::property::get<bool>(bus, service, path, iface, name))
            {
                return false;
            }
        }
        return true;
    }

    /**
     * @brief Attepmt to change the actual status of service
     *
     * @param bus       - D-Bus connection
     * @param newStatus - Required state
     */
    void status(sdbusplus::bus::bus& bus, bool newStatus) const
    {
        for (const auto& [service, path, iface, name] : properties)
        {
            dbus::property::set(bus, service, path, iface, name, newStatus);
        }
    }
};

struct CaseInsensitiveComparator
{
    bool operator()(const std::string& a, const std::string& b) const noexcept
    {
        return ::strcasecmp(a.c_str(), b.c_str()) < 0;
    }
};

using ServiceDefinitions =
    std::map<std::string, Service, CaseInsensitiveComparator>;

/**
 * @brief This is a map of manageable service definitions and their aliases.
 */
extern ServiceDefinitions serviceDefinitions;

} // namespace srvctl

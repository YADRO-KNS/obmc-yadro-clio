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

    std::string summary;                 //!< Service short description
    std::vector<PropertyDef> properties; //!< List of D-Bus properties to
                                         //!< manage the state

    /**
     * @brief Get actual state of service
     *
     * @param bus - D-Bus connection
     *
     * @return true if service is active and false otherwise
     */
    bool getState(sdbusplus::bus::bus& bus) const
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
     * @brief Attepmt to change the actual state of service
     *
     * @param bus       - D-Bus connection
     * @param newState - Required state
     */
    void setState(sdbusplus::bus::bus& bus, bool newState) const
    {
        for (const auto& [service, path, iface, name] : properties)
        {
            dbus::property::set(bus, service, path, iface, name, newState);
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

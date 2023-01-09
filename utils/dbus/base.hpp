/*
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (C) 2022, KNS Group LLC (YADRO).
 */
#pragma once

#include <sdbusplus/bus.hpp>
#include <sdbusplus/message.hpp>

namespace dbus
{

/** @brief Invoke a method. */
template <typename... Args>
inline auto callMethod(sdbusplus::bus::bus& bus, const std::string& serviceName,
                       const std::string& objectPath,
                       const std::string& interface, const std::string& method,
                       Args&&... args)
{
    auto request = bus.new_method_call(serviceName.c_str(), objectPath.c_str(),
                                       interface.c_str(), method.c_str());
    request.append(std::forward<Args>(args)...);
    return bus.call(request);
}

/** @brief Invoke a method and read response. */
template <typename Result, typename... Args>
inline Result callMethodAndRead(sdbusplus::bus::bus& bus,
                                const std::string& serviceName,
                                const std::string& objectPath,
                                const std::string& interface,
                                const std::string& method, Args&&... args)
{
    Result responce;
    callMethod<Args...>(bus, serviceName, objectPath, interface, method,
                        std::forward<Args>(args)...)
        .read(responce);
    return responce;
}

} // namespace dbus

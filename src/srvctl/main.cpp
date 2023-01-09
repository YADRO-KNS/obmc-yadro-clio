/*
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (C) 2022, KNS Group LLC (YADRO).
 */
#include "config.h"

#include "srvctl.hpp"
#include "utils/confirm.hpp"

#include <fmt/printf.h>

#include <CLI/CLI.hpp>

#include <stdexcept>

static void setState(sdbusplus::bus::bus& bus, const std::string& service,
                     bool state, bool interactive)
{
    auto it = srvctl::serviceDefinitions.find(service);
    if (it == srvctl::serviceDefinitions.end())
    {
        throw std::runtime_error(
            fmt::format("Service {} is invalid or unsupported", service));
    }

    const auto strState = state ? "started" : "stopped";

    if (interactive)
    {
        auto title = fmt::format("Service {} will be {}.", service, strState);
        if (!state && it->first == "ssh")
        {
            title += "\nWARNING: You won't be able to log back in via SSH!";
        }

        if (!confirm(title.c_str()))
        {
            return;
        }
    }

    fmt::print("Setting service {} to {} state... ", service, strState);
    try
    {
        it->second.setState(bus, state);
        fmt::print("done\n");
        fmt::print("The service will be {} in about 20 seconds.\n", strState);
    }
    catch (const std::exception&)
    {
        fmt::print("fail\n");
        throw;
    }
}

static void showList(sdbusplus::bus::bus& bus)
{
    static constexpr auto formatStr = " {:8s} {:48s} {}\n";
    fmt::print(formatStr, "SERVICE", "DESCRIPTION", "STATE");
    for (const auto& [name, service] : srvctl::serviceDefinitions)
    {
        try
        {
            fmt::print(formatStr, name, service.summary,
                       service.getState(bus) ? "on" : "off");
        }
        catch (const sdbusplus::exception::SdBusError&)
        {
            fmt::print(formatStr, name, service.summary, "N/A");
        }
    }
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    CLI::App app("");
    auto bus = sdbusplus::bus::new_default();

#ifdef REMOTE_HOST_SUPPORT
    app.add_option_function<std::string>(
        "-H,--host", [&](const std::string& hostname) {
            sd_bus* b = nullptr;
            sd_bus_open_system_remote(&b, hostname.c_str());
            bus = sdbusplus::bus::bus(b, std::false_type());
        });
#endif

    auto listCmd = app.add_subcommand(
        "list", "List available services and their actual state");
    listCmd->callback([&]() { showList(bus); });

    std::string service;
    bool yes = false;

    auto enableCmd =
        app.add_subcommand("enable", "Enable a particular service");
    enableCmd->add_option("SERVICE", service, "Service name")->required();
    enableCmd->add_flag("-y,--yes", yes, "Don't ask user for confirmation");
    enableCmd->callback([&]() { setState(bus, service, true, !yes); });

    auto disableCmd =
        app.add_subcommand("disable", "Disable a particular service");
    disableCmd->add_option("SERVICE", service, "Service name")->required();
    disableCmd->add_flag("-y,--yes", yes, "Don't ask user for confirmation");
    disableCmd->callback([&]() { setState(bus, service, false, !yes); });

    app.require_subcommand(1);
    try
    {
        CLI11_PARSE(app, argc, argv);
    }
    catch (const std::exception& e)
    {
        fmt::print(stderr, "ERROR: {}\n", e.what());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

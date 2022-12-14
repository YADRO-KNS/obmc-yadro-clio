/*
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (C) 2022, KNS Group LLC (YADRO).
 */
#include "config.h"

#include "srvctl.hpp"

#include <fmt/printf.h>
#include <getopt.h>

#include <cstring>
#include <stdexcept>

enum class Command
{
    list,
    enable,
    disable,
};

static void setState(sdbusplus::bus::bus& bus, const char* service, bool state)
{
    auto it = srvctl::serviceDefinitions.find(service);
    if (it == srvctl::serviceDefinitions.end())
    {
        throw std::runtime_error(
            fmt::format("Service {} is invalid or unsupported", service));
    }

    fmt::print("Setting service {} to {}... ", service, state);
    try
    {
        it->second.status(bus, true);
        fmt::print("done\n");
    }
    catch (const std::exception&)
    {
        fmt::print("fail\n");
        throw;
    }
}

static void showList(sdbusplus::bus::bus& bus)
{
    for (const auto& [name, service] : srvctl::serviceDefinitions)
    {
        try
        {
            fmt::print(" {:8s} {:48s} {}\n", name, service.summary,
                       service.status(bus));
        }
        catch (const sdbusplus::exception::SdBusError&)
        {
            fmt::print(" {:8s} {:48s} N/A\n", name, service.summary);
        }
    }
}

static void printUsage(const char* binaryPath)
{
    const char* app = basename(binaryPath);
    fmt::print(R"(
Usage: {} [OPTIONS] [COMMANDS [ARGS]]

OPTIONS:
 -h, --help         show this help message and exit

COMMANDS:
 list               show list of managing services and their actual state
 enable SERVICE     enable and start specified service
 disable SERVICE    disable and stop specified service
)",
               app);
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    fmt::print("YADRO OpenBMC Service manager version {}\n", PROJECT_VERSION);

    const struct option opts[] = {
        // clang-format off
        { "help",   no_argument,        0, 'h' },
#ifdef REMOTE_HOST_SUPPORT
        { "host",   required_argument,  0, 'H' },
#endif

        { 0,        0,                  0,  0  }
        // clang-format on
    };

    opterr = 0;
    int optVal;
#ifdef REMOTE_HOST_SUPPORT
    const char* hostname = nullptr;
#endif

    while ((optVal = getopt_long(argc, argv,
                                 "h"
#ifdef REMOTE_HOST_SUPPORT
                                 "H:"
#endif
                                 ,
                                 opts, nullptr)) != -1)
    {
        switch (optVal)
        {
            case 'h':
                printUsage(argv[0]);
                return EXIT_SUCCESS;

#ifdef REMOTE_HOST_SUPPORT
            case 'H':
                hostname = optarg;
                break;
#endif

            default:
                fmt::print(stderr, "ERROR: Invalid option: {}\n",
                           argv[optind - 1]);
                printUsage(argv[0]);
                return EXIT_FAILURE;
        }
    }

    auto command = Command::list;

    if (optind < argc)
    {
        if (0 == strcasecmp(argv[optind], "enable"))
        {
            command = Command::enable;
        }
        else if (0 == strcasecmp(argv[optind], "disable"))
        {
            command = Command::disable;
        }
        else if (0 != strcasecmp(argv[optind], "list"))
        {
            fmt::print(stderr, "ERROR: {} is invalid command\n", argv[optind]);
            printUsage(argv[0]);
            return EXIT_FAILURE;
        }

        if ((command == Command::enable || command == Command::disable) &&
            (optind + 1 >= argc))
        {
            fmt::print(stderr, "ERROR: Service name required\n");
            printUsage(argv[0]);
            return EXIT_FAILURE;
        }
    }

    auto bus = sdbusplus::bus::new_default();

#ifdef REMOTE_HOST_SUPPORT
    if (hostname)
    {
        sd_bus* b = nullptr;
        sd_bus_open_system_remote(&b, hostname);
        bus = sdbusplus::bus::bus(b, std::false_type());
    }
#endif

    try
    {
        switch (command)
        {
            case Command::enable:
                setState(bus, argv[optind + 1], true);
                break;

            case Command::disable:
                setState(bus, argv[optind + 1], false);
                break;

            default:
                showList(bus);
                break;
        }
    }
    catch (const std::exception& e)
    {
        fmt::print(stderr, "ERROR: {}\n", e.what());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

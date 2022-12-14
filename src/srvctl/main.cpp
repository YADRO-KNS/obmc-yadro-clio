/*
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (C) 2022, KNS Group LLC (YADRO).
 */
#include "config.h"

#include <fmt/printf.h>

#include <CLI/CLI.hpp>

#include <stdexcept>

static void setState(const std::string& /*service*/, bool /*state*/)
{
    throw std::runtime_error("Not implemented yet");
}

static void showList()
{
    throw std::runtime_error("Not implemented yet");
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    CLI::App app("");

    auto listCmd = app.add_subcommand(
        "list", "List available services and their actual state");
    listCmd->callback(showList);

    std::string service;

    auto enableCmd =
        app.add_subcommand("enable", "Enable a particular service");
    enableCmd->add_option("SERVICE", service, "Service name")->required();
    enableCmd->callback([&]() { setState(service, true); });

    auto disableCmd =
        app.add_subcommand("disable", "Disable a particular service");
    disableCmd->add_option("SERVICE", service, "Service name")->required();
    disableCmd->callback([&]() { setState(service, false); });

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

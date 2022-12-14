/*
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (C) 2022, KNS Group LLC (YADRO).
 */
#include "config.h"

#include <getopt.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stdexcept>

enum class Command
{
    list,
    enable,
    disable,
};

static void setState(const char* /*service*/, bool /*state*/)
{
    throw std::runtime_error("Not implemented yet");
}

static void showList()
{
    throw std::runtime_error("Not implemented yet");
}

static void printUsage(const char* binaryPath)
{
    const char* app = basename(binaryPath);
    printf(R"(
Usage: %s [OPTIONS] [COMMANDS [ARGS]]

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
    printf("YADRO OpenBMC Service manager version %s\n", PROJECT_VERSION);

    const struct option opts[] = {
        // clang-format off
        { "help",   no_argument,        0, 'h' },

        { 0,        0,                  0,  0  }
        // clang-format on
    };

    opterr = 0;
    int optVal;
    while ((optVal = getopt_long(argc, argv, "h", opts, nullptr)) != -1)
    {
        switch (optVal)
        {
            case 'h':
                printUsage(argv[0]);
                return EXIT_SUCCESS;

            default:
                fprintf(stderr, "ERROR: Invalid option: %s\n",
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
            fprintf(stderr, "ERROR: %s is invalid command\n", argv[optind]);
            printUsage(argv[0]);
            return EXIT_FAILURE;
        }

        if ((command == Command::enable || command == Command::disable) &&
            (optind + 1 >= argc))
        {
            fprintf(stderr, "ERROR: Service name required\n");
            printUsage(argv[0]);
            return EXIT_FAILURE;
        }
    }

    try
    {
        switch (command)
        {
            case Command::enable:
                setState(argv[optind + 1], true);
                break;

            case Command::disable:
                setState(argv[optind + 1], false);
                break;

            default:
                showList();
                break;
        }
    }
    catch (const std::exception& e)
    {
        fprintf(stderr, "ERROR: %s\n", e.what());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

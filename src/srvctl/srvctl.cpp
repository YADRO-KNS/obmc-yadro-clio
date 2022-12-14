/*
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (C) 2022, KNS Group LLC (YADRO).
 */

#include "srvctl.hpp"

namespace srvctl
{

/** NOTE: This map was written by knowledges extracted from `bmcweb`
 *        (https://github.com/openbmc/bmcweb) and `service-control-manager`
 *        (https://github.com/openbmc/service-config-manager)
 *
 *        I dislike the such hard code, but this is a quick solution.
 *        In further it should be generating at compile time or even better
 *        retrieving via d-bus in run-time. But this requires a corresponding
 *        implementation at service-manager side.
 */
ServiceDefinitions serviceDefinitions = {
    {"SSH",
     {
         "SSH for BMC CLI shell",
         {
             {"xyz.openbmc_project.Control.Service.Manager",
              "/xyz/openbmc_project/control/service/dropbear",
              "xyz.openbmc_project.Control.Service.Attributes", "Running"},
             {"xyz.openbmc_project.Control.Service.Manager",
              "/xyz/openbmc_project/control/service/dropbear",
              "xyz.openbmc_project.Control.Service.Attributes", "Enabled"},
         },
     }},

    {"IPMI",
     {
         "Network IPMI (out-of-band IPMI)",
         {
             {"xyz.openbmc_project.Control.Service.Manager",
              "/xyz/openbmc_project/control/service/"
              "phosphor_2dipmi_2dnet_40eth0",
              "xyz.openbmc_project.Control.Service.Attributes", "Running"},
             {"xyz.openbmc_project.Control.Service.Manager",
              "/xyz/openbmc_project/control/service/"
              "phosphor_2dipmi_2dnet_40eth0",
              "xyz.openbmc_project.Control.Service.Attributes", "Enabled"},
#ifdef WITH_ETH1
             {"xyz.openbmc_project.Control.Service.Manager",
              "/xyz/openbmc_project/control/service/"
              "phosphor_2dipmi_2dnet_40eth1",
              "xyz.openbmc_project.Control.Service.Attributes", "Running"},
             {"xyz.openbmc_project.Control.Service.Manager",
              "/xyz/openbmc_project/control/service/"
              "phosphor_2dipmi_2dnet_40eth1",
              "xyz.openbmc_project.Control.Service.Attributes", "Enabled"},
#endif
         },
     }},

    {"SOL",
     {
         "SSH for SOL",
         {
             {"xyz.openbmc_project.Control.Service.Manager",
              "/xyz/openbmc_project/control/service/obmc_2dconsole_2dssh",
              "xyz.openbmc_project.Control.Service.Attributes", "Running"},
             {"xyz.openbmc_project.Control.Service.Manager",
              "/xyz/openbmc_project/control/service/obmc_2dconsole_2dssh",
              "xyz.openbmc_project.Control.Service.Attributes", "Enabled"},
         },
     }},

    {"KVMIP",
     {
         "KVMIP",
         {
             {"xyz.openbmc_project.Control.Service.Manager",
              "/xyz/openbmc_project/control/service/start_2dipkvm",
              "xyz.openbmc_project.Control.Service.Attributes", "Running"},
             {"xyz.openbmc_project.Control.Service.Manager",
              "/xyz/openbmc_project/control/service/start_2dipkvm",
              "xyz.openbmc_project.Control.Service.Attributes", "Enabled"},
         },
     }},

    {"IPMISOL",
     {
         "IPMI for SOL",
         {
             {"xyz.openbmc_project.Settings",
              "/xyz/openbmc_project/ipmi/sol/eth0",
              "xyz.openbmc_project.Ipmi.SOL", "Enable"},
#ifdef WITH_ETH1
             {"xyz.openbmc_project.Settings",
              "/xyz/openbmc_project/ipmi/sol/eth1",
              "xyz.openbmc_project.Ipmi.SOL", "Enable"},
#endif
         },
     }},

    {"SNMP",
     {
         "SNMP",
         {
             {"xyz.openbmc_project.Control.Service.Manager",
              "/xyz/openbmc_project/control/service/snmpd",
              "xyz.openbmc_project.Control.Service.Attributes", "Running"},
             {"xyz.openbmc_project.Control.Service.Manager",
              "/xyz/openbmc_project/control/service/snmpd",
              "xyz.openbmc_project.Control.Service.Attributes", "Enabled"},
         },
     }},

    {"HTTPS",
     {
         "WebUI/RedFish",
         {
             {"xyz.openbmc_project.Control.Service.Manager",
              "/xyz/openbmc_project/control/service/lighttpd",
              "xyz.openbmc_project.Control.Service.Attributes", "Running"},
             {"xyz.openbmc_project.Control.Service.Manager",
              "/xyz/openbmc_project/control/service/lighttpd",
              "xyz.openbmc_project.Control.Service.Attributes", "Enabled"},
         },
     }},
};

} // namespace srvctl

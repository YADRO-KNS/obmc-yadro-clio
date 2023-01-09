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
 *        In future it must be generated at compile time or even better
 *        retrieved via d-bus in run-time. But this requires a corresponding
 *        implementation at service-manager side.
 */
ServiceDefinitions serviceDefinitions = {
    {"ssh",
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

    {"ipmi",
     {
         "Out-of-band IPMI over LAN",
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

    {"sol-ssh",
     {
         "Serial-over-LAN via direct SSH connection",
         {
             {"xyz.openbmc_project.Control.Service.Manager",
              "/xyz/openbmc_project/control/service/obmc_2dconsole_2dssh",
              "xyz.openbmc_project.Control.Service.Attributes", "Running"},
             {"xyz.openbmc_project.Control.Service.Manager",
              "/xyz/openbmc_project/control/service/obmc_2dconsole_2dssh",
              "xyz.openbmc_project.Control.Service.Attributes", "Enabled"},
         },
     }},

    {"kvm",
     {
         "Keyboard-Video-Mouse over IP",
         {
             {"xyz.openbmc_project.Control.Service.Manager",
              "/xyz/openbmc_project/control/service/start_2dipkvm",
              "xyz.openbmc_project.Control.Service.Attributes", "Running"},
             {"xyz.openbmc_project.Control.Service.Manager",
              "/xyz/openbmc_project/control/service/start_2dipkvm",
              "xyz.openbmc_project.Control.Service.Attributes", "Enabled"},
         },
     }},

    {"sol-ipmi",
     {
         "Serial-over-LAN via IPMI",
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

    {"snmp",
     {
         "Simple Network Management Protocol",
         {
             {"xyz.openbmc_project.Control.Service.Manager",
              "/xyz/openbmc_project/control/service/snmpd",
              "xyz.openbmc_project.Control.Service.Attributes", "Running"},
             {"xyz.openbmc_project.Control.Service.Manager",
              "/xyz/openbmc_project/control/service/snmpd",
              "xyz.openbmc_project.Control.Service.Attributes", "Enabled"},
         },
     }},

    {"https",
     {
         "WebUI/RedFish via HTTPS",
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

/*
   Copyright (C) 2017, The XPerience Project (Klozz Jesus AKA TeamMEX)
   Copyright (c) 2014, The Linux Foundation. All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are
   met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following
      disclaimer in the documentation and/or other materials provided
      with the distribution.
    * Neither the name of The Linux Foundation nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
   WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
   ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
   BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
   BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
   WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
   OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
   IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdlib.h>

#include "vendor_init.h"
#include "property_service.h"
#include "log.h"
#include "util.h"
#include <sys/sysinfo.h>

//Take care about 3gb ram
int is3GB()
{
    struct sysinfo sys;
    sysinfo(&sys);
    return sys.totalram > 2048ull * 1024 * 1024;
}

void vendor_load_properties()
{

    char gb[PROP_VALUE_MAX];
    char ds[PROP_VALUE_MAX];

    std::string platform = property_get("ro.board.platform");
    if (platform != ANDROID_TARGET)
        return;

    std::string radio = property_get("ro.boot.radio");
    std::string device = property_get("ro.boot.device");
    std::string sku = property_get("ro.boot.hardware.sku");
    property_set("ro.product.model", sku.c_str());


    if (is3GB()) {
        property_set("dalvik.vm.heapstartsize", "8m");
        property_set("dalvik.vm.heapgrowthlimit", "192m");
        property_set("dalvik.vm.heapsize", "512m");
        property_set("dalvik.vm.heaptargetutilization", "0.75");
        property_set("dalvik.vm.heapminfree", "512k");
        property_set("dalvik.vm.heapmaxfree", "8m");
        sprintf(gb, "_3gb");
    } else {
        property_set("dalvik.vm.heapstartsize", "8m");
        property_set("dalvik.vm.heapgrowthlimit", "96m");
        property_set("dalvik.vm.heapsize", "256m");
        property_set("dalvik.vm.heaptargetutilization", "0.75");
        property_set("dalvik.vm.heapminfree", "2m");
        property_set("dalvik.vm.heapmaxfree", "8m");
        gb[0] = 0;
    }

   //dual sim
   std::string msim = property_get("ro.boot.dualsim");
   property_set("ro.hw.dualsim", msim.c_str());

    if (property_get("ro.telephony.default_network").empty()) {
        property_set("ro.telephony.default_network", "9");
    }

    if (msim == "true")
    {
        property_set("persist.radio.multisim.config", "dsds");
        property_set("ro.telephony.ril.config", "simactivation");
        sprintf(ds, "_ds");
    }

   //rmt storage
    property_set("ro.hw.device", device.c_str());
    property_set("ro.hw.radio", radio.c_str());

}

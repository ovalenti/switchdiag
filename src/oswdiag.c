#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "atheros.h"
#include "log.h"
#include "mdio.h"

#define STRINGIFY(x) #x
#define AS_STRING(x) STRINGIFY(x)

#define DFLT_LOG_LEVEL 2
int log_dyn_level = DFLT_LOG_LEVEL;

static void usage(int argc, char** argv)
{
    fprintf(stderr,
            "Usage: %s [-d <dbg_lvl>] [-i <intf>] { <ath_reg_or_name>  | -m "
            "<phy> <reg> }\n"
            "\t-d <dbg_lvl>\tset log level. [" AS_STRING(
                DFLT_LOG_LEVEL) "]\n"
                                "\t-i <intf>\tset the switch interface "
                                "name. [eth0]\n",
            argv[0]);
}

static int mii_show_content(const char* param_phy, const char* param_reg)
{
    uint8_t phy;
    uint8_t reg;
    uint16_t val;
    int res;

    TRACE_ENTER;

    phy = strtoul(param_phy, NULL, 0);
    reg = strtoul(param_reg, NULL, 0);

    LOG_DEBUG("Reading MII register: <phy=0x%02x> <reg=0x%02x>\n",
              (unsigned int)phy, (unsigned int)reg);

    res = mdio_read(phy, reg, &val);

    if (res == 0)
        printf("0x%02x 0x%02x : 0x%04x\n", (unsigned int)phy, (unsigned int)reg,
               (unsigned int)val);

    TRACE_EXIT;

    return res;
}

int main(int argc, char** argv)
{
    int c;
    char* intf_name = "eth0";
    int mii_mode    = 0;

    TRACE_ENTER;

    while ((c = getopt(argc, argv, "d:i:hm")) != -1) {
        switch (c) {
        case 'd':
            log_dyn_level = atoi(optarg);
            LOG_DEBUG("Log level set to %d\n", log_dyn_level);
            break;
        case 'i':
            intf_name = optarg;
            LOG_DEBUG("Interface set to %s\n", intf_name);
            break;
        case 'm':
            mii_mode = 1;
            break;
        case 'h':
        case '?':
        default:
            usage(argc, argv);
            break;
        }
    }

    LOG_VERB("Using interface %s\n", intf_name);

    mdio_init(intf_name);

    if (mii_mode) {
        if (optind < argc + 1) {
            mii_show_content(argv[optind], argv[optind + 1]);
        } else {
            LOG_ERR("required parameters: <phy> <register>\n");
            usage(argc, argv);
        }
    } else {
        if (optind < argc) {
            ath_show_content(argv[optind]);
        } else {
            LOG_ERR("missing register address or name");
            usage(argc, argv);
        }
    }
    mdio_fini();

    TRACE_EXIT;
    return 0;
}

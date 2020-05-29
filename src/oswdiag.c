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
            "Usage: %s [-d <dbg_lvl>] [-i <intf>] \n"
            "\t-d <dbg_lvl>\tset log level. [" AS_STRING(
                DFLT_LOG_LEVEL) "]\n"
                                "\t-i <intf>\tset the switch interface "
                                "name. [eth0]\n",
            argv[0]);
}

int main(int argc, char** argv)
{
    int c;
    char* intf_name = "eth0";

    TRACE_ENTER;

    while ((c = getopt(argc, argv, "d:i:h")) != -1) {
        switch (c) {
        case 'd':
            log_dyn_level = atoi(optarg);
            LOG_DEBUG("Log level set to %d\n", log_dyn_level);
            break;
        case 'i':
            intf_name = optarg;
            LOG_DEBUG("Interface set to %s\n", intf_name);
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

    if (optind < argc)
        ath_show_content(argv[optind]);

    mdio_fini();

    TRACE_EXIT;
    return 0;
}

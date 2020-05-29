#include "mdio.h"
#include "log.h"

#include <errno.h>
#include <linux/mii.h>
#include <linux/sockios.h>
#include <net/if.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>

static int fd;
static struct ifreq ifreq = { 0 };

int mdio_init(const char* intf_name)
{
    TRACE_ENTER;

    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0) {
        LOG_ERR("socket() failed: %s\n", strerror(errno));
        return -1;
    }
    LOG_DEBUG("socket fd: %d\n", fd);

    strncpy(ifreq.ifr_name, intf_name, IFNAMSIZ);

    TRACE_EXIT;
    return 0;
}

void mdio_fini(void)
{
    TRACE_ENTER;

    if (fd >= 0)
        close(fd);

    fd = -1;

    TRACE_EXIT;
}

int mdio_read(uint8_t phy, uint8_t reg, uint16_t* val)
{
    struct mii_ioctl_data* mii = (struct mii_ioctl_data*)&ifreq.ifr_data;

    TRACE_ENTER;
    LOG_DEBUG("mdio_read(phy=0x%x, reg=0x%x)\n", (unsigned int)phy,
              (unsigned int)reg);

    mii->phy_id  = phy;
    mii->reg_num = reg;

    if (ioctl(fd, SIOCGMIIREG, &ifreq) < 0) {
        LOG_ERR("SIOCGMIIREG failure: %s\n", strerror(errno));
        TRACE_EXIT;
        return -1;
    }

    *val = mii->val_out;

    LOG_DEBUG("mdio_read -> %x\n", (unsigned int)*val);

    TRACE_EXIT;
    return 0;
}

int mdio_write(uint8_t phy, uint8_t reg, uint16_t val)
{
    struct mii_ioctl_data* mii = (struct mii_ioctl_data*)&ifreq.ifr_data;

    TRACE_ENTER;
    LOG_DEBUG("mdio_write(phy=0x%x, reg=0x%x, val=0x%x)\n", (unsigned int)phy,
              (unsigned int)reg, (unsigned int)val);

    mii->phy_id  = phy;
    mii->reg_num = reg;
    mii->val_in  = val;

    if (ioctl(fd, SIOCSMIIREG, &ifreq) < 0) {
        LOG_ERR("SIOCGMIIREG failure: %s\n", strerror(errno));
        TRACE_EXIT;
        return -1;
    }

    TRACE_EXIT;
    return 0;
}

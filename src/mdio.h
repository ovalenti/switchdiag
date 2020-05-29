
#ifndef MDIO_H_
#define MDIO_H_

#include <stdint.h>

int mdio_init(const char* intf_name);
void mdio_fini(void);

int mdio_read(uint8_t phy, uint8_t reg, uint16_t* val);
int mdio_write(uint8_t phy, uint8_t reg, uint16_t val);

#endif /* MDIO_H_ */


#ifndef ATHEROS_H_
#define ATHEROS_H_

#include <stdint.h>

void ath_read(uint16_t reg, uint32_t* val);
int ath_show_content(const char* name_or_address);

#endif /* ATHEROS_H_ */

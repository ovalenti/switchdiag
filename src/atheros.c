#include <stdlib.h>
#include <string.h>

#include "atheros.h"

#include "log.h"
#include "mdio.h"

// doc p15 2.3
#define PHY_COMPAT (0b00 << 3)
#define PHY_PAGESEL (0b11 << 3)
#define PHY_ACCESS (0b10 << 3)

void ath_split_address(uint16_t reg, uint16_t* offset_low, uint8_t* offset_high,
                       uint8_t* page)
{
    TRACE_ENTER;

    reg >>= 1;
    *offset_low = reg & 0x1e;

    reg >>= 5;
    *offset_high = reg & 0x7;

    reg >>= 3;
    *page = reg & 0x1ff;

    TRACE_EXIT;
}

void ath_switch_page(uint8_t page)
{
    TRACE_ENTER;

    LOG_DEBUG("switch to page 0x%x\n", (unsigned int)page);
    mdio_write(PHY_PAGESEL, 0, page);

    TRACE_EXIT;
}

void ath_read(uint16_t reg, uint32_t* val)
{
    uint16_t offset_low;
    uint8_t offset_high;
    uint8_t page;
    uint16_t word;

    TRACE_ENTER;

    ath_split_address(reg, &offset_low, &offset_high, &page);
    ath_switch_page(page);

    mdio_read(PHY_ACCESS | offset_high, offset_low + 1, &word);
    *val = word;
    (*val) <<= 16;
    mdio_read(PHY_ACCESS | offset_high, offset_low, &word);
    *val |= word;

    TRACE_EXIT;
}

static const char* ath_chip_name(uint32_t value)
{
    switch (value) {
    case 1:
        return "AR8216";
    case 3:
        return "AR8236";
    case 16:
        return "AR8316";
    case 18:
        return "AR8327";
    case 19:
        return "AR8337";
    default:
        return "unknown";
    }
}

static const char* ath_invert_bit[] = { "not inverted", "inverted" };

const struct known_register {
    const char* name;
    uint16_t offset;
    struct register_field {
        const char* name;
        uint8_t bit_offset;
        uint8_t bit_width;
        const char** symbolic_values;
        const char* (*symbolic_fn)(uint32_t value);
    } breakout[16];
} known_registers[] = {
    { .name   = "Mask Control Register",
      .offset = 0x0000,
      .breakout =
          { { .name = "REV_ID", .bit_offset = 0, .bit_width = 8, 0 },
            { .name        = "DEVICE_ID",
              .bit_offset  = 8,
              .bit_width   = 8,
              .symbolic_fn = ath_chip_name },
            { .name = "LOAD_EEPROM", .bit_offset = 16, .bit_width = 1, 0 },
            { .name = "SOFT_RET", .bit_offset = 31, .bit_width = 1, 0 },
            { 0 } } },
    { .name   = "PORT0 PAD MODE CTRL",
      .offset = 0x0004,
      .breakout =
          { { .name            = "MAC0_MAC_MII_RXCLK_SEL",
              .bit_offset      = 0,
              .bit_width       = 1,
              .symbolic_values = ath_invert_bit,
              0 },
            { .name            = "MAC0_MAC_MII_TXCLK_SEL",
              .bit_offset      = 1,
              .bit_width       = 1,
              .symbolic_values = ath_invert_bit,
              0 },
            { .name = "MAC0_MAC_MII_EN", .bit_offset = 2, .bit_width = 1, 0 },
            { .name = "MAC0_200M_EN", .bit_offset = 7, .bit_width = 1, 0 },
            { .name            = "MAC0_PHY_MII_RXCLK_SEL",
              .bit_offset      = 8,
              .bit_width       = 1,
              .symbolic_values = ath_invert_bit,
              0 },
            { .name            = "MAC0_PHY_MII_TXCLK_SEL",
              .bit_offset      = 9,
              .bit_width       = 1,
              .symbolic_values = ath_invert_bit,
              0 },
            { .name = "MAC0_PHY_MII_EN", .bit_offset = 10, .bit_width = 1, 0 },
            { .name            = "Mac0_phy_mii_pipe_rxclk_sel",
              .bit_offset      = 11,
              .bit_width       = 1,
              .symbolic_values = ath_invert_bit,
              0 },
            { .name = "MAC0_RMII_EN", .bit_offset = 17, .bit_width = 1, 0 },
            { .name            = "MAC0_RMII_RXCLK_SEL",
              .bit_offset      = 18,
              .bit_width       = 1,
              .symbolic_values = ath_invert_bit,
              0 },
            { .name            = "MAC0_RMII_TXCLK_SEL",
              .bit_offset      = 19,
              .bit_width       = 1,
              .symbolic_values = ath_invert_bit,
              0 },
            { 0 } } },
    { .name   = "PORT5 PAD MODE CTRL",
      .offset = 0x0008,
      .breakout =
          { { .name = "MAC5_200M_EN", .bit_offset = 7, .bit_width = 1, 0 },
            { .name            = "MAC5_PHY_MII_RXCLK_SEL",
              .bit_offset      = 8,
              .bit_width       = 1,
              .symbolic_values = ath_invert_bit,
              0 },
            { .name            = "MAC5_PHY_MII_TXCLK_SEL",
              .bit_offset      = 9,
              .bit_width       = 1,
              .symbolic_values = ath_invert_bit,
              0 },
            { .name            = "MAC5_PHY_MII_EN",
              .bit_offset      = 10,
              .bit_width       = 1,
              .symbolic_values = ath_invert_bit,
              0 },
            { .name            = "Mac5_phy_mii_pipe_rxclk_sel",
              .bit_offset      = 11,
              .bit_width       = 1,
              .symbolic_values = ath_invert_bit,
              0 },
            { .name = "MAC5_RMII_EN", .bit_offset = 17, .bit_width = 1, 0 },
            { .name            = "MAC5_RMII_RXCLK_SEL",
              .bit_offset      = 18,
              .bit_width       = 1,
              .symbolic_values = ath_invert_bit,
              0 },
            { .name            = "MAC5_RMII_TXCLK_SEL",
              .bit_offset      = 19,
              .bit_width       = 1,
              .symbolic_values = ath_invert_bit,
              0 },
            { .name = "PHY4_MII_EN", .bit_offset = 28, .bit_width = 1, 0 },
            { .name = "PHY4_RMII_EN", .bit_offset = 29, .bit_width = 1, 0 },
            { 0 } } },
    { .name   = "Global MAC Address Register 1",
      .offset = 0x0020,
      .breakout =
          { { .name = "MAC_ADDR_BYTE4", .bit_offset = 8, .bit_width = 8, 0 },
            { .name = "MAC_ADDR_BYTE5", .bit_offset = 0, .bit_width = 8, 0 },
            { 0 } } },
    { .name   = "Global MAC Address Register 2",
      .offset = 0x0020,
      .breakout =
          { { .name = "MAC_ADDR_BYTE0", .bit_offset = 24, .bit_width = 8, 0 },
            { .name = "MAC_ADDR_BYTE1", .bit_offset = 16, .bit_width = 8, 0 },
            { .name = "MAC_ADDR_BYTE2", .bit_offset = 8, .bit_width = 8, 0 },
            { .name = "MAC_ADDR_BYTE3", .bit_offset = 0, .bit_width = 8, 0 },

            { 0 } } },
    { 0 }
};

#define MASK(width) ((1 << (width)) - 1)

static void ath_show_details(const struct known_register* reg, uint32_t value)
{
    const struct register_field* field;
    fprintf(stdout, "%s\n", reg->name);

    field = reg->breakout;
    while (field->name) {
        const char* symbolic_value = "";
        unsigned int field_value =
            (value >> field->bit_offset) & MASK(field->bit_width);

        if (field->symbolic_values != NULL)
            symbolic_value = field->symbolic_values[field_value];
        else if (field->symbolic_fn != NULL)
            symbolic_value = field->symbolic_fn(field_value);

        fprintf(stdout, "  %s\t[%d:%d]\t= 0x%lx\t%u %s\n", field->name,
                (int)field->bit_offset,
                (int)(field->bit_offset + field->bit_width - 1),
                (unsigned long)field_value, (unsigned int)field_value,
                symbolic_value);

        field++;
    }
}

struct search_criteria {
    int by_name;
    const char* name;
    int by_offset;
    uint16_t offset;
};

static const struct known_register*
ath_find_register(struct search_criteria* criteria)
{
    const struct known_register* reg = known_registers;

    while (reg->name) {
        if ((criteria->by_name && 0 == strcmp(criteria->name, reg->name))
            || (criteria->by_offset && criteria->offset == reg->offset))
            return reg;
        reg++;
    }
    return NULL;
}

int ath_show_content(const char* name_or_address)
{
    struct search_criteria search_criteria = { .by_name = 1,
                                               .name    = name_or_address,
                                               0 };
    char* end;
    uint16_t offset;
    uint32_t value;
    const struct known_register* reg;

    TRACE_ENTER;

    offset = strtoul(name_or_address, &end, 0);
    if (end != name_or_address) {
        LOG_DEBUG("Searching by address 0x%04x\n", (unsigned int)offset);
        search_criteria.by_offset = 1;
        search_criteria.offset    = offset;
    }

    reg = ath_find_register(&search_criteria);

    if (reg != NULL) {
        LOG_VERB("Found corresponding known register '%s'@0x%04x\n", reg->name,
                 (unsigned int)reg->offset);
        offset = reg->offset;
    } else if (!search_criteria.by_offset) {
        LOG_ERR("Invalid address and no matching register name: %s\n",
                name_or_address);
        TRACE_EXIT;
        return -1;
    }

    ath_read(offset, &value);

    fprintf(stdout, "0x%04x: 0x%08lx\n", (unsigned int)offset,
            (unsigned long)value);

    if (reg != NULL)
        ath_show_details(reg, value);

    TRACE_EXIT;
    return 0;
}

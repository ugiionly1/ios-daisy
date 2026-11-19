
#ifndef _PEXPERT_ARM64_DAISY_GICV2_H
#define _PEXPERT_ARM64_DAISY_GICV2_H

/* Qualcomm MSM8953 Daisy: ARM GICv2 */

/* Distributor registers */
#define DAISY_GICD_CTLR       0x0000
#define DAISY_GICD_TYPER      0x0004
#define DAISY_GICD_IGROUPR          0x0080

#define DAISY_GICD_CTLR_ENABLEGRP0  (1U << 0)
#define DAISY_GICD_CTLR_ENABLEGRP1  (1U << 1)
#define DAISY_GICD_ISENABLER  0x0100
#define DAISY_GICD_ICENABLER  0x0180
#define DAISY_GICD_IPRIORITYR 0x0400
#define DAISY_GICD_ITARGETSR  0x0800
#define DAISY_GICD_ICFGR      0x0C00

/* CPU interface registers */
#define DAISY_GICC_CTLR       0x0000
#define DAISY_GICC_PMR        0x0004
#define DAISY_GICC_BPR        0x0008
#define DAISY_GICC_IAR        0x000C
#define DAISY_GICC_EOIR       0x0010

#define DAISY_GIC_SPURIOUS_IRQ 1023

/* Daisy GICv2 accessors, implemented in pe_identify_machine.c */
unsigned int daisy_gicv2_acknowledge(void);
void daisy_gicv2_end_interrupt(unsigned int acknowledge_value);

#endif /* _PEXPERT_ARM64_DAISY_GICV2_H */

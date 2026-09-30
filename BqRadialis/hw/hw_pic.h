// Made by Berkay

#ifndef HW_PIC_H
#define HW_PIC_H

#define HW_PIC_MASTER_CMD           0x20
#define HW_PIC_MASTER_DATA          0x21
#define HW_PIC_SLAVE_CMD            0xA0
#define HW_PIC_SLAVE_DATA           0xA1

#define HW_PIC_ICW1_INIT            0x11

#define HW_PIC_ICW2_MASTER_OFFSET   0x20
#define HW_PIC_ICW2_SLAVE_OFFSET    0x28

#define HW_PIC_ICW3_MASTER_CASCADE  0x04
#define HW_PIC_ICW3_SLAVE_CASCADE   0x02

#define HW_PIC_ICW4_8086            0x01

#define HW_PIC_OCW2_EOI             0x20
#define HW_PIC_OCW3_READ_ISR        0x0B

#define HW_PIC_MASK_ALL             0xFF
#define HW_PIC_IRQ_CASCADE          2
#define HW_PIC_MAX_IRQ              16

#endif

// Made by Berkay

#include "hw_private.h"

void HwPicLoad(void)
{
    HwIoOut8(HW_PIC_MASTER_CMD, HW_PIC_ICW1_INIT);
    HwIoWait();
    HwIoOut8(HW_PIC_SLAVE_CMD, HW_PIC_ICW1_INIT);
    HwIoWait();

    HwIoOut8(HW_PIC_MASTER_DATA, HW_PIC_ICW2_MASTER_OFFSET);
    HwIoWait();
    HwIoOut8(HW_PIC_SLAVE_DATA, HW_PIC_ICW2_SLAVE_OFFSET);
    HwIoWait();

    HwIoOut8(HW_PIC_MASTER_DATA, HW_PIC_ICW3_MASTER_CASCADE);
    HwIoWait();
    HwIoOut8(HW_PIC_SLAVE_DATA, HW_PIC_ICW3_SLAVE_CASCADE);
    HwIoWait();

    HwIoOut8(HW_PIC_MASTER_DATA, HW_PIC_ICW4_8086);
    HwIoWait();
    HwIoOut8(HW_PIC_SLAVE_DATA, HW_PIC_ICW4_8086);
    HwIoWait();

    HwIoOut8(HW_PIC_MASTER_DATA, HW_PIC_MASK_ALL);
    HwIoWait();
    HwIoOut8(HW_PIC_SLAVE_DATA, HW_PIC_MASK_ALL);
    HwIoWait();
}

void HwPicMaskIrq(UInt8 irq)
{
    if (irq >= HW_PIC_MAX_IRQ)
    {
        return;
    }

    UInt16 port;
    UInt8  line;

    if (irq < 8)
    {
        port = HW_PIC_MASTER_DATA;
        line = irq;
    }
    else
    {
        port = HW_PIC_SLAVE_DATA;
        line = (UInt8)(irq - 8);
    }

    UInt8 mask = HwIoIn8(port);
    mask |= (UInt8)(1U << line);
    HwIoOut8(port, mask);
    HwIoWait();
}

void HwPicUnmaskIrq(UInt8 irq)
{
    if (irq >= HW_PIC_MAX_IRQ)
    {
        return;
    }

    if (irq >= 8)
    {
        UInt8 master_mask = HwIoIn8(HW_PIC_MASTER_DATA);
        master_mask &= (UInt8)~(1U << HW_PIC_IRQ_CASCADE);
        HwIoOut8(HW_PIC_MASTER_DATA, master_mask);
        HwIoWait();

        UInt8 slave_line = (UInt8)(irq - 8);
        UInt8 slave_mask = HwIoIn8(HW_PIC_SLAVE_DATA);
        slave_mask &= (UInt8)~(1U << slave_line);
        HwIoOut8(HW_PIC_SLAVE_DATA, slave_mask);
        HwIoWait();
    }
    else
    {
        UInt8 master_mask = HwIoIn8(HW_PIC_MASTER_DATA);
        master_mask &= (UInt8)~(1U << irq);
        HwIoOut8(HW_PIC_MASTER_DATA, master_mask);
        HwIoWait();
    }
}

void HwPicSendEoi(UInt8 irq)
{
    if (irq >= HW_PIC_MAX_IRQ)
    {
        return;
    }

    if (irq == 7)
    {
        HwIoOut8(HW_PIC_MASTER_CMD, HW_PIC_OCW3_READ_ISR);
        HwIoWait();

        if ((HwIoIn8(HW_PIC_MASTER_CMD) & 0x80U) == 0U)
        {
            return;
        }
    }
    else if (irq == 15)
    {
        HwIoOut8(HW_PIC_SLAVE_CMD, HW_PIC_OCW3_READ_ISR);
        HwIoWait();

        if ((HwIoIn8(HW_PIC_SLAVE_CMD) & 0x80U) == 0U)
        {
            HwIoOut8(HW_PIC_MASTER_CMD, HW_PIC_OCW2_EOI);
            return;
        }
    }

    if (irq >= 8)
    {
        HwIoOut8(HW_PIC_SLAVE_CMD, HW_PIC_OCW2_EOI);
    }

    HwIoOut8(HW_PIC_MASTER_CMD, HW_PIC_OCW2_EOI);
}

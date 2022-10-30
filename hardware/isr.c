#include "isr.h"
#include "idt.h"
#include "../lib/logging.h"

// exception interrupts
static __attribute__((interrupt)) void isr0(interrupt_frame *frame) { exception_isr(0, 0); }
static __attribute__((interrupt)) void isr1(interrupt_frame *frame) { exception_isr(1, 0); }
static __attribute__((interrupt)) void isr2(interrupt_frame *frame) { exception_isr(2, 0); }
static __attribute__((interrupt)) void isr3(interrupt_frame *frame) { exception_isr(3, 0); }
static __attribute__((interrupt)) void isr4(interrupt_frame *frame) { exception_isr(4, 0); }
static __attribute__((interrupt)) void isr5(interrupt_frame *frame) { exception_isr(5, 0); }
static __attribute__((interrupt)) void isr6(interrupt_frame *frame) { exception_isr(6, 0); }
static __attribute__((interrupt)) void isr7(interrupt_frame *frame) { exception_isr(7, 0); }
static __attribute__((interrupt)) void isr8(interrupt_frame *frame) { exception_isr(8, 0); }
static __attribute__((interrupt)) void isr9(interrupt_frame *frame) { exception_isr(9, 0); }
static __attribute__((interrupt)) void isr10(interrupt_frame *frame) { exception_isr(10, 0); }
static __attribute__((interrupt)) void isr11(interrupt_frame *frame) { exception_isr(11, 0); }
static __attribute__((interrupt)) void isr12(interrupt_frame *frame) { exception_isr(12, 0); }
static __attribute__((interrupt)) void isr13(interrupt_frame *frame, uint32_t error_code) { exception_isr(13, error_code); }
static __attribute__((interrupt)) void isr14(interrupt_frame *frame) { exception_isr(14, 0); }
static __attribute__((interrupt)) void isr15(interrupt_frame *frame) { exception_isr(15, 0); }
static __attribute__((interrupt)) void isr16(interrupt_frame *frame) { exception_isr(16, 0); }
static __attribute__((interrupt)) void isr17(interrupt_frame *frame) { exception_isr(17, 0); }
static __attribute__((interrupt)) void isr18(interrupt_frame *frame) { exception_isr(18, 0); }
static __attribute__((interrupt)) void isr19(interrupt_frame *frame) { exception_isr(19, 0); }
static __attribute__((interrupt)) void isr20(interrupt_frame *frame) { exception_isr(20, 0); }
static __attribute__((interrupt)) void isr21(interrupt_frame *frame) { exception_isr(21, 0); }
static __attribute__((interrupt)) void isr22(interrupt_frame *frame) { exception_isr(22, 0); }
static __attribute__((interrupt)) void isr23(interrupt_frame *frame) { exception_isr(23, 0); }
static __attribute__((interrupt)) void isr24(interrupt_frame *frame) { exception_isr(24, 0); }
static __attribute__((interrupt)) void isr25(interrupt_frame *frame) { exception_isr(25, 0); }
static __attribute__((interrupt)) void isr26(interrupt_frame *frame) { exception_isr(26, 0); }
static __attribute__((interrupt)) void isr27(interrupt_frame *frame) { exception_isr(27, 0); }
static __attribute__((interrupt)) void isr28(interrupt_frame *frame) { exception_isr(28, 0); }
static __attribute__((interrupt)) void isr29(interrupt_frame *frame) { exception_isr(29, 0); }
static __attribute__((interrupt)) void isr30(interrupt_frame *frame) { exception_isr(30, 0); }
static __attribute__((interrupt)) void isr31(interrupt_frame *frame) { exception_isr(31, 0); }

// interrupt requests (irqs)
static __attribute__((interrupt)) void isr32(interrupt_frame *frame) { silent_isr(32); } // timer
static __attribute__((interrupt)) void isr33(interrupt_frame *frame) { echo_isr(33); }
static __attribute__((interrupt)) void isr34(interrupt_frame *frame) { echo_isr(34); }
static __attribute__((interrupt)) void isr35(interrupt_frame *frame) { echo_isr(35); }
static __attribute__((interrupt)) void isr36(interrupt_frame *frame) { echo_isr(36); }
static __attribute__((interrupt)) void isr37(interrupt_frame *frame) { echo_isr(37); }
static __attribute__((interrupt)) void isr38(interrupt_frame *frame) { echo_isr(38); }
static __attribute__((interrupt)) void isr39(interrupt_frame *frame) { echo_isr(39); }
static __attribute__((interrupt)) void isr40(interrupt_frame *frame) { echo_isr(40); }
static __attribute__((interrupt)) void isr41(interrupt_frame *frame) { echo_isr(41); }
static __attribute__((interrupt)) void isr42(interrupt_frame *frame) { echo_isr(42); }
static __attribute__((interrupt)) void isr43(interrupt_frame *frame) { echo_isr(43); }
static __attribute__((interrupt)) void isr44(interrupt_frame *frame) { echo_isr(44); }
static __attribute__((interrupt)) void isr45(interrupt_frame *frame) { echo_isr(45); }
static __attribute__((interrupt)) void isr46(interrupt_frame *frame) { echo_isr(46); }
static __attribute__((interrupt)) void isr47(interrupt_frame *frame) { echo_isr(47); }

void init_idt()
{
    INFO("remapping the pic");
    remap_pic();

    INFO("registering default ISRs");
    register_isr(0, (void *)isr0);
    register_isr(2, (void *)isr2);
    register_isr(1, (void *)isr1);
    register_isr(3, (void *)isr3);
    register_isr(4, (void *)isr4);
    register_isr(5, (void *)isr5);
    register_isr(6, (void *)isr6);
    register_isr(7, (void *)isr7);
    register_isr(8, (void *)isr8);
    register_isr(9, (void *)isr9);
    register_isr(10, (void *)isr10);
    register_isr(11, (void *)isr11);
    register_isr(12, (void *)isr12);
    register_isr(13, (void *)isr13);
    register_isr(14, (void *)isr14);
    register_isr(15, (void *)isr15);
    register_isr(16, (void *)isr16);
    register_isr(17, (void *)isr17);
    register_isr(18, (void *)isr18);
    register_isr(19, (void *)isr19);
    register_isr(20, (void *)isr20);
    register_isr(21, (void *)isr21);
    register_isr(22, (void *)isr22);
    register_isr(23, (void *)isr23);
    register_isr(24, (void *)isr24);
    register_isr(25, (void *)isr25);
    register_isr(26, (void *)isr26);
    register_isr(27, (void *)isr27);
    register_isr(28, (void *)isr28);
    register_isr(29, (void *)isr29);
    register_isr(30, (void *)isr30);
    register_isr(31, (void *)isr31);

    register_isr(32, (void *)isr32);
    register_isr(33, (void *)isr33);
    register_isr(34, (void *)isr34);
    register_isr(35, (void *)isr35);
    register_isr(36, (void *)isr36);
    register_isr(37, (void *)isr37);
    register_isr(38, (void *)isr38);
    register_isr(39, (void *)isr39);
    register_isr(40, (void *)isr40);
    register_isr(41, (void *)isr41);
    register_isr(42, (void *)isr42);
    register_isr(43, (void *)isr43);
    register_isr(44, (void *)isr44);
    register_isr(45, (void *)isr45);
    register_isr(46, (void *)isr46);
    register_isr(47, (void *)isr47);

    INFO("loading idtr");
    load_idtr();
}
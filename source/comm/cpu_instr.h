#ifndef CPU_INSTR_H
#define CPU_INSTR_H

#include "types.h"

/**
 * 二进制的方式从指定的 I/O端口 中读取数据
 * 
 */
static inline uint8_t inb (uint16_t port) {
    uint8_t rv;

    // inb al, dx
    __asm__ __volatile__ ("inb %[p], %[v]":[v]"=a"(rv) : [p]"d"(port));
    return rv;
}

/**
 * inb 和 in 指令的功能一样都是从 I/O端口 中读取数据
 * "in"指令使用DX寄存器来指定I/O端口地址，而"inb"指令使用立即数来指定端口地址。
 */ 
static inline uint16_t inw (uint16_t port) {
    uint16_t rv;

    // in al, dx
    __asm__ __volatile__ ("in %1, %0" : "=a"(rv) : "dN"(port));
    return rv;
}

/**
 * 二进制的方式将数据传给指定的 I/O端口
 * IO端口是计算机与外部设备的接口，通过IO端口来控制外部设备，IO端口出厂自带，或者说是厂商之间有协定端口的功能
 *  @port 端口
 *  @data 数据
 */
static inline void outb (uint16_t port, uint8_t data) {
    // outb al, dx
    __asm__ __volatile__ ("outb %[v], %[p]"::[p]"d"(port), [v]"a"(data));
}

// inline修饰：内联函数 -> 将代码直接插入到调用的位置上
/**
 * cli：将IF标志位设置为0，关闭中断
 */ 
static inline void cli (void) {
    __asm__ __volatile__ ("cli");
}

/**
 * IF标记为1表示允许中断请求
 * sti：将IF标志位设置位1，开启中断
 */ 
static inline void sti (void) {
    __asm__ __volatile__ ("sti");
}

/**
 * 加载 GDT 表，开启保护模式
 */ 
static inline void lgdt (uint32_t start, uint32_t size) {
    struct {
        uint16_t limit;         // 长度
        uint16_t start15_0;     // 低16位
        uint16_t start31_16;    // 高6位
    } gdt;
    
    gdt.start31_16 = start >> 16;
    gdt.start15_0 = start & 0XFFFF;
    gdt.limit = size - 1;

    /*
        lgdt：
            加载全局描述符，将GDT表的地址加载到GDTR寄存器中，
            用于告诉处理器GDT表的位置和大小.
            GDTR寄存器：48位的寄存器，数据包括基地址（低32位）和限制（大小，低16位）
     */
    __asm__ __volatile__ ("lgdt %[g]"::[g]"m"(gdt));
}

/*
    读取处理器的控制寄存器 CR0 的值，并将其返回
    cr0：控制寄存器 CR0 是x86体系结构中的一个特殊寄存器，
         包含了一系列的控制标志位，用于控制处理器的各种行为和特性，
         通过它可以确定处理器是否启用了分页机制、是否启用了浮点数协处理器等。
 */
static inline uint16_t read_cr0 (void) {
    uint32_t cr0;
    __asm__ __volatile__ ("mov %%cr0, %[v]":[v]"=r"(cr0));
    return cr0;
}

/**
 * 改写 CR0 的值
 */ 
static inline uint16_t write_cr0 (uint32_t v) {
    uint32_t cr0;
    __asm__ __volatile__ ("mov %[v], %%cr0"::[v]"r"(v));
    return cr0;
}

/**
 * 远跳转操作，切换到指定的代码段
 *  @selector 选择子
 *  @offset 偏移量
 */
static inline void far_jump(uint32_t selector, uint32_t offset) {
    uint32_t addr[] = { offset, selector };
    __asm__ __volatile__ ("ljmpl *(%[a])" :: [a]"r"(addr));
}

#endif
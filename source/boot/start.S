/**
 * 自己动手写操作系统
 *
 * 系统引导部分，启动时由硬件加载运行，然后完成对二级引导程序loader的加载
 * 该部分程序存储于磁盘的第1个扇区，在计算机启动时将会由BIOS加载到0x7c00处
 * 之后，将由BIOS跳转至0x7c00处开始运行
 * 
 * 作者：李述铜
 * 联系邮箱: 527676163@qq.com
 */
	#include "boot.h"

  	// 16位代码，务必加上
  	.code16
 	.text
	.global _start 
	.extern boot_entry
_start:
	// 重置数据段寄存器
	mov $0, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs
	mov %ax, %ss

	// 使用0x7c00之前的空间作栈，大约有30KB的RAM，足够boot和loader使用 (自定义)
	mov $_start, %esp

	/*
	 	内联显示字符 -> ax=ah+al(ah高八位，al低八位)
		 	ah：前景色
			al：显示字符，只能是单个字符，因为它的内存大小是8位
	 */
	mov $0xe, %ah
	mov $'O', %al
	int $0x10
	mov $0xe, %ah
	mov $'K', %al
	int $0x10

read_loader:
	// 加载loader，读取磁盘内容
	mov 0x8000, %bx		// 读取到指定内存地址
	mov $0x2, %cx		// ch：磁道号，cl：起始扇区号
	mov $0x2, %ah		// ah：读取的扇区数量
	mov $64, %al		// al：读取的扇区数量，暂时设置成 32KB
	mov $0x0080, %dx	// dh：磁头号，dl：驱动器号 ox80（磁盘1）
	int $0x13		
	jc read_loader		// 出现异常反复执行

	// 跳入到c代码中运行
	jmp boot_entry

	jmp .
	
	// 引导结束段
	.section boot_end, "ax"
boot_sig: .byte 0x55, 0xaa

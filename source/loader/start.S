/**
	加载
 */
  	// 16位代码，务必加上
  	.code16
 	.text
	.global _start 
	.extern loader_entry
_start:
	jmp loader_entry

	.code32						// 生成32位的代码
	.text						// 表示下面放置的是代码
	// 全局声明
	.global protect_mode_entry
	.extern load_kernel

protect_mode_entry:		// 入口标号
	// 重置寄存器，目前寄存器是16位的，我们需要将他们改为32位模式
	mov $16, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs
	mov %ax, %ss

	jmp $8, $load_kernel

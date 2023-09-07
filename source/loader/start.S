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
	# 重置寄存器，目前寄存器是16位的，我们需要将他们改为32位模式
	mov $16, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs
	mov %ax, %ss

	# jmp $selector, $addr：直接跳转命令，它会直接跳转到selector指定代码段中执行，addr表示在代码段中的位置
	# jmp $addr：相对跳转指令，它会根据当前指令的位置计算相对与该指令的偏移量，使用该偏移量进行跳转
	jmp $8, $load_kernel	# 跳转到为 8 的代码段中的 $load_kernel位置执行



/**
 * 加载
 */
#include "loader.h"

/**
 * 使用 LBA48模式 读取磁盘
 */
static void read_disk(uint32_t sector, uint32_t sector_count, uint8_t *buf) {
	/*
		x86架构使用 0x1F6端口 来选择磁盘类型
		0xE0 表示使用 LBA（Logical Block Addressing）模式
	 */
    outb(0x1F6, (uint8_t) (0xE0));

	/*
		磁盘控制器只能接收8位的值，需要将填写的值设置位8位的
			1.sector_count >> 8 
	 */
	outb(0x1F2, (uint8_t) (sector_count >> 8));
    outb(0x1F3, (uint8_t) (sector >> 24));		// LBA参数的24~31位
    outb(0x1F4, (uint8_t) (0));					// LBA参数的32~39位
    outb(0x1F5, (uint8_t) (0));					// LBA参数的40~47位

    outb(0x1F2, (uint8_t) (sector_count));
	outb(0x1F3, (uint8_t) (sector));			// LBA参数的0~7位
	outb(0x1F4, (uint8_t) (sector >> 8));		// LBA参数的8~15位
	outb(0x1F5, (uint8_t) (sector >> 16));		// LBA参数的16~23位

	outb(0x1F7, (uint8_t) 0x24);

	// 转换成16位的，因为 inb 每次读取两个字节
	uint16_t *data_buf = (uint16_t*) buf;
	// 读取指定扇区数的数据到缓冲区中
	while (sector_count-- > 0) {
		/*
		 	每次扇区读之前都要检查，等待数据就绪
			inb(0x1F7) 读取磁盘的状态位，& 0x88 != 0x8 就是还未准备好
		 */
		while ((inb(0x1F7) & 0x88) != 0x8) {}

		/*
		 	读取并将数据写入到缓存中
			循环 inw(0x1F0) 每次读取两个字节数据放入到缓冲区中，所以循环要除以2。一块扇区512字节
		 */
		for (int i = 0; i < SECTOR_SIZE / 2; i++) {
			*data_buf++ = inw(0x1F0);
		}
	}
     
}

void load_kernel (void) {
    // 读取的扇区数尽可能大一些，kernel.elf文件可能会很大
    read_disk(100, 500, (uint8_t *) SYS_KERNEL_LOAD_ADDR);
    // 跳转到内核中
	((void (*)(void)) SYS_KERNEL_LOAD_ADDR)();
}




   


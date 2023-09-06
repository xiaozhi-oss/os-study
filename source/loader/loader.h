/**
 * 加载
 */
#ifndef LOADER_H
#define LOADER_H

#include "comm/boot_info.h"
#include "comm/types.h"

// 内存检测信息结构
typedef struct SMAP_entry {
    uint32_t BaseL; // base address uint64_t
    uint32_t BaseH;
    uint32_t LengthL; // length uint64_t
    uint32_t LengthH;
    uint32_t Type; // entry Type
    uint32_t ACPI; // extended
}__attribute__((packed)) SMAP_entry_t;  
/* 
    __attribute__((packed)) 
        用于告诉编译器不要在结构体的成员之间插入任何填充字节，
        以确保结构体在内存中是以紧凑的形式存储的。
*/
#endif

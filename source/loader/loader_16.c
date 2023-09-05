/**
 * 加载
 */
__asm__(".code16gcc");

static void show_msg (const char * msg) {
    char c;

    while ((c = *msg++) != '\0') {
        asm __volatile__ (
            "mov $0xe, %%ah\n\t"
	        "mov %[ch], %%al\n\t"
	        "int $0x10"
            :
            :[ch]"r"(c)
        );
    }
    
}

void loader_entry (void) {
    show_msg("....loading....\n\r");
    for (;;) {}
}


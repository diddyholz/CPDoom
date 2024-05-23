ENTRY(_start);
STARTUP(crt0.o);

SECTIONS {
    text_address = 0x8C052800;
    . = text_address;
	.text : AT(text_address) {
		*(.text*)
		*(.rodata*)
    }
    
    _actualbssstart = .;
    PROVIDE(actualbssstart = .);
    
	.bss : {
		*(.bss*)
        *(COMMON)
	}
    
    _actualbssend = .;
    PROVIDE(actualbssend = .);

	start_address = 0x8CFE2000;
	. = start_address;
    .bootstrap.start : AT(start_address) {
        *(.bootstrap.start*)
    }
    
	info_address = 0x8CFE2010;
	. = info_address;
	.hollyhock_name : {
		KEEP(*(.hollyhock_name))
	}
	.hollyhock_description : {
		KEEP(*(.hollyhock_description))
	}
	.hollyhock_author : {
		KEEP(*(.hollyhock_author))
	}
	.hollyhock_version : {
		KEEP(*(.hollyhock_version))
	}

    .bootstrap : {
        *(.bootstrap.text*)
        *(.bootstrap.data*)
    }

	.data : {
		*(.data*)
	}
    
    _edata = .;
    PROVIDE(edata = .);
    
    _end = .;
    PROVIDE(end = .);

	.eh_frame : {
		*(.eh_frame*)
	}
  
    x_mem_addr = 0xE5007000;
    . = x_mem_addr;

    .oc_mem.x : {
        *(.oc_mem.x.data*)
    }
}

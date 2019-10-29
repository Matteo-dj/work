MEMORY
{
    ram : ORIGIN = 0x70000000, LENGTH = 0x8000000
}
SECTIONS
{
    .debug_aranges 0 : { *(.debug_aranges) } .debug_pubnames 0 : { *(.debug_pubnames) } .debug_info 0 : { *(.debug_info) } .debug_abbrev 0 : { *(.debug_abbrev) } .debug_line 0 : { *(.debug_line) } .debug_frame 0 : { *(.debug_frame) } .debug_str 0 : { *(.debug_str) } .debug_loc 0 : { *(.debug_loc) } .debug_macinfo 0 : { *(.debug_macinfo) } .note.arm.ident 0 : { KEEP (*(.note.arm.ident)) } /DISCARD/ 0 : { *(.fini_array*) }
    .rom_vectors (0x70000000 + 0x8000) : { __rom_vectors_vma = ABSOLUTE(.); . = .; KEEP (*(.vectors)) } > ram __rom_vectors_lma = LOADADDR(.rom_vectors);
}

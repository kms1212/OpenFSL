OUTPUT_ARCH(i386)
OUTPUT_FORMAT("elf32-i386", "elf32-i386", "elf32-i386")

ENTRY(_start)
SEARCH_DIR("../buildutils/cross/i686-elf/lib");

SECTIONS{
	PROVIDE (__executable_start = SEGMENT_START("text-segment", 0x08048000)); . = SEGMENT_START("text-segment", 0x08048000) + SIZEOF_HEADERS;

	.text 0x8000 : {
		*(.text .stub .text.* .gnu.linkonce.t.*)
		*(.gnu.warning)
	} =0x90909090

	.rodata : { *(.rodata .rodata.* .gnu.linkonce.r.*) }
	.rodata1 : { *(.rodata1) }

	. = ALIGN(512);

	.data : {
		*(.data .data.* .gnu.linkonce.d.*)
		SORT(CONSTRUCTORS)
	}
	.data1 : { *(.data1) }

	__bss_start = .;
	.bss : {
		*(.dynbss)
		*(.bss .bss.* .gnu.linkonce.b.*)
		*(COMMON)
		. = ALIGN(. != 0 ? 32 / 8 : 1);
	}
	. = ALIGN(32 / 8);
	. = ALIGN(32 / 8);
	_end = .; PROVIDE(end = .);

	.interp : { *(.interp) }
	.note.gnu.build-id : { *(.note.gnu.build-id) }
	.hash : { *(.hash) }
	.gnu.hash : { *(.gnu.hash) }
	.dynsym : { *(.dynsym) }
	.dynstr : { *(.dynstr) }
	.gnu.version : { *(.gnu.version) }
	.gnu.version_d : { *(.gnu.version_d) }
	.gnu.version_r : { *(.gnu.version_r) }
	.rel.init : { *(.rel.init) }
	.rela.init : { *(.rela.init) }
	.rel.text : { *(.rel.text .rel.text.* .rel.gnu.linkonce.t.*) }
	.rela.text : { *(.rela.text .rela.text.* .rela.gnu.linkonce.t.*) }
	.rel.fini : { *(.rel.fini) }
	.rela.fini : { *(.rela.fini) }
	.rel.rodata : { *(.rel.rodata .rel.rodata.* .rel.gnu.linkonce.r.*) }
	.rela.rodata : { *(.rela.rodata .rela.rodata.* .rela.gnu.linkonce.r.*) }
	.rel.data.rel.ro : { *(.rel.data.rel.ro .rel.data.rel.ro.* .rel.gnu.linkonce.d.rel.ro.*) }
	.rela.data.rel.ro : { *(.rela.data.rel.ro .rela.data.rel.ro.* .rela.gnu.linkonce.d.rel.ro.*) }
	.rel.data : { *(.rel.data .rel.data.* .rel.gnu.linkonce.d.*) }
	.rela.data : { *(.rela.data .rela.data.* .rela.gnu.linkonce.d.*) }
	.rel.tdata : { *(.rel.tdata .rel.tdata.* .rel.gnu.linkonce.td.*) }
	.rela.tdata : { *(.rela.tdata .rela.tdata.* .rela.gnu.linkonce.td.*) }
	.rel.tbss : { *(.rel.tbss .rel.tbss.* .rel.gnu.linkonce.tb.*) }
	.rela.tbss : { *(.rela.tbss .rela.tbss.* .rela.gnu.linkonce.tb.*) }
	.rel.ctors : { *(.rel.ctors) }
	.rela.ctors : { *(.rela.ctors) }
	.rel.dtors : { *(.rel.dtors) }
	.rela.dtors : { *(.rela.dtors) }
	.rel.got : { *(.rel.got) }
	.rela.got : { *(.rela.got) }
	.rel.bss : { *(.rel.bss .rel.bss.* .rel.gnu.linkonce.b.*) }
	.rela.bss : { *(.rela.bss .rela.bss.* .rela.gnu.linkonce.b.*) }
	.rel.ifunc : { *(.rel.ifunc) }
	.rel.plt : {
		*(.rel.plt)
		PROVIDE_HIDDEN(__rel_iplt_start = .);
		*(.rel.ipit)
		PROVIDE_HIDDEN(__rel_iplt_end = .);
	}
	.rela.plt : {
		*(.rela.plt)
		PROVIDE_HIDDEN(__rela_iplt_start = .);
		*(.rela.ipit)
		PROVIDE_HIDDEN(__rela_iplt_end = .);
	}
	.init : {
		KEEP(*(.init))
	} =0x90909090
	.plt : { *(.plt) }
	.fini : {
		KEEP( *(.fini) )
	} =0x90909090
	PROVIDE(__etext = .);
	PROVIDE(_etext = .);
	PROVIDE(etext = .);
	.preinit_array : {
		PROVIDE_HIDDEN(__preinit_array_start = .);
		KEEP(*(.preinit_array))
		PROVIDE_HIDDEN(__preinit_array_end = .);
	}
	.init_array : {
		PROVIDE_HIDDEN(__init_array_start = .);
		KEEP(*(SORT_BY_INIT_PRIORITY(.init_array.*) SORT_BY_INIT_PRIORITY(.ctors.*)))
		KEEP(*(.init_array EXCLUDE_FILE(*crtbegin.o *crtbegin?.o *crtend.o *crtend?.o) .ctors))
		PROVIDE_HIDDEN (__init_array_end = .);
	}
	.fini_array : {
		PROVIDE_HIDDEN(__fini_array_start = .);
		KEEP(*(SORT_BY_INIT_PRIORITY(.fini_array.*) SORT_BY_INIT_PRIORITY(.dtors.*)))
		KEEP(*(.fini_array EXCLUDE_FILE(*crtbegin.o *crtbegin?.o *crtend.o *crtend?.o) .dtors))
		PROVIDE_HIDDEN(__fini_array_end = .);
	}
	_edata = .; PROVIDE (edata = .);
	.tdata : { *(.tdata .tdata.* .gnu.linkonce.td.*) }
	.tbss : { *(.tbss .tbss.* .gnu.linkonce.tb.*) *(.tcommon) }
	.ctors : {
		KEEP(*crtbegin.o(.ctors))
		KEEP(*crtbegin?.o(.ctors))
		KEEP(*(EXCLUDE_FILE(*crtend.o *crtend?.o) .ctors))
		KEEP(*(SORT(.ctors.*)))
		KEEP(*(.ctors))
	}
	.dtors : {
		KEEP(*crtbegin.o(.dtors))
		KEEP(*crtbegin?.o(.dtors))
		KEEP(*(EXCLUDE_FILE(*crtend.o *crtend?.o) .dtors))
		KEEP(*(SORT(.dtors.*)))
		KEEP(*(.dtors))
	}
	.jcr : { KEEP(*(.jcr)) }
	.data.rel.ro : { *(.data.rel.ro.local* .gnu.linkonce.d.rel.ro.local.*) *(.data.rel.ro .data.rel.ro.* .gnu.linkonce.d.rel.ro.*) }
	.dynamic : { *(.dynamic) }
	.got : { *(.got) }
	.got.plt : { *(.got.plt) }

	.eh_frame_hdr : { *(.eh_frame_hdr) }
	.eh_frame : ONLY_IF_RO { KEEP (*(.eh_frame)) }
	.gcc_except_table : ONLY_IF_RO { *(.gcc_except_table .gcc_except_table.*) }
	.eh_frame : ONLY_IF_RW { KEEP (*(.eh_frame)) }
	.gcc_except_table : ONLY_IF_RW { *(.gcc_except_table .gcc_except_table.*) }
	.stab 0 : { *(.stab) }
	.stabstr 0 : { *(.stabstr) }
	.stab.excl 0 : { *(.stab.excl) }
	.stab.exclstr 0 : { *(.stab.exclstr) }
	.stab 0 : { *(.stab.index) }
	.stab.index 0 : { *(.stab.index) }
	.stab.indexstr 0 : { *(.stab.indexstr) }
	.comment 0 : { *(.comment) }
	.gnu.build.attributes : { *(.gnu.build.attributes .gnu.build.attributes.*) }
	.debug 0 : { *(.debug) }
	.line 0 : { *(.line) }	
	.debug_srcinfo 0 : { *(.debug_srcinfo) }
	.debug_sfnames 0 : { *(.debug_sfnames) }
	.debug_aranges 0 : { *(.debug_aranges) }
	.debug_pubnames 0 : { *(.debug_pubnames) }
	.debug_info 0 : { *(.debug_info .gnu.linkonce.wi.*) }
	.debug_abbrev 0 : { *(.debug_abbrev) }
	.debug_line 0 : { *(.debug_line .debug.line.* .debug_line_end) }
	.debug_frame 0 : { *(.debug_frame) }
	.debug_str 0 : { *(.debug_str) }
	.debug_loc 0 : { *(.debug_loc) }
	.debug_macinfo 0 : { *(.debug_macinfo) }
	.debug_weaknames 0 : { *(.debug_weaknames) }
	.debug_funcnames 0 : { *(.debug_funcnames) }
	.debug_typenames 0 : { *(.debug_typenames) }
	.debug_varnames 0 : { *(.debug_varnames) }
	.debug_pubtypes 0 : { *(.debug_pubtypes) }
	.debug_ranges 0 : { *(.debug_ranges) }
	.debug_macro 0 : { *(.debug_macro) }
	.debug_addr 0 : { *(.debug_addr) }
	.gnu.attributes 0 : { KEEP(*(.gnu,attributes)) }
	/DISCARD/ : { *(.note.GNU-stack) *(.gnu_debuglink) *(.gnu.lto_*) }
}

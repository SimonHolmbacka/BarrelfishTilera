/* Hypervisor call vector addresses; see <hv/hypervisor.h> */
.section ".hvglue","x"
.align 8
.org 0x20
.global hv_init
.type hv_init,function
hv_init:
.size hv_init,32
.org 0x40
.global hv_install_context
.type hv_install_context,function
hv_install_context:
.size hv_install_context,32
.org 0x60
.global hv_sysconf
.type hv_sysconf,function
hv_sysconf:
.size hv_sysconf,32
.org 0x80
.global hv_get_rtc
.type hv_get_rtc,function
hv_get_rtc:
.size hv_get_rtc,32
.org 0xa0
.global hv_set_rtc
.type hv_set_rtc,function
hv_set_rtc:
.size hv_set_rtc,32
.org 0xc0
.global hv_flush_asid
.type hv_flush_asid,function
hv_flush_asid:
.size hv_flush_asid,32
.org 0xe0
.global hv_flush_page
.type hv_flush_page,function
hv_flush_page:
.size hv_flush_page,32
.org 0x100
.global hv_flush_pages
.type hv_flush_pages,function
hv_flush_pages:
.size hv_flush_pages,32
.org 0x120
.global hv_restart
.type hv_restart,function
hv_restart:
.size hv_restart,32
.org 0x140
.global hv_halt
.type hv_halt,function
hv_halt:
.size hv_halt,32
.org 0x160
.global hv_power_off
.type hv_power_off,function
hv_power_off:
.size hv_power_off,32
.org 0x180
.global hv_inquire_physical
.type hv_inquire_physical,function
hv_inquire_physical:
.size hv_inquire_physical,32
.org 0x1a0
.global hv_inquire_memory_controller
.type hv_inquire_memory_controller,function
hv_inquire_memory_controller:
.size hv_inquire_memory_controller,32
.org 0x1c0
.global hv_inquire_virtual
.type hv_inquire_virtual,function
hv_inquire_virtual:
.size hv_inquire_virtual,32
.org 0x1e0
.global hv_inquire_asid
.type hv_inquire_asid,function
hv_inquire_asid:
.size hv_inquire_asid,32
.org 0x200
.global hv_nanosleep
.type hv_nanosleep,function
hv_nanosleep:
.size hv_nanosleep,32
.org 0x220
.global hv_console_read_if_ready
.type hv_console_read_if_ready,function
hv_console_read_if_ready:
.size hv_console_read_if_ready,32
.org 0x240
.global hv_console_write
.type hv_console_write,function
hv_console_write:
.size hv_console_write,32
.org 0x260
.global hv_downcall_dispatch
.type hv_downcall_dispatch,function
hv_downcall_dispatch:
.size hv_downcall_dispatch,32
.org 0x280
.global hv_inquire_topology
.type hv_inquire_topology,function
hv_inquire_topology:
.size hv_inquire_topology,32
.org 0x2a0
.global hv_fs_findfile
.type hv_fs_findfile,function
hv_fs_findfile:
.size hv_fs_findfile,32
.org 0x2c0
.global hv_fs_fstat
.type hv_fs_fstat,function
hv_fs_fstat:
.size hv_fs_fstat,32
.org 0x2e0
.global hv_fs_pread
.type hv_fs_pread,function
hv_fs_pread:
.size hv_fs_pread,32
.org 0x300
.global hv_physaddr_read64
.type hv_physaddr_read64,function
hv_physaddr_read64:
.size hv_physaddr_read64,32
.org 0x320
.global hv_physaddr_write64
.type hv_physaddr_write64,function
hv_physaddr_write64:
.size hv_physaddr_write64,32
.org 0x340
.global hv_get_command_line
.type hv_get_command_line,function
hv_get_command_line:
.size hv_get_command_line,32
.org 0x360
.global hv_set_caching
.type hv_set_caching,function
hv_set_caching:
.size hv_set_caching,32
.org 0x380
.global hv_bzero_page
.type hv_bzero_page,function
hv_bzero_page:
.size hv_bzero_page,32
.org 0x3a0
.global hv_register_message_state
.type hv_register_message_state,function
hv_register_message_state:
.size hv_register_message_state,32
.org 0x3c0
.global hv_send_message
.type hv_send_message,function
hv_send_message:
.size hv_send_message,32
.org 0x3e0
.global hv_receive_message
.type hv_receive_message,function
hv_receive_message:
.size hv_receive_message,32
.org 0x400
.global hv_inquire_context
.type hv_inquire_context,function
hv_inquire_context:
.size hv_inquire_context,32
.org 0x420
.global hv_start_all_tiles
.type hv_start_all_tiles,function
hv_start_all_tiles:
.size hv_start_all_tiles,32
.org 0x440
.global hv_dev_open
.type hv_dev_open,function
hv_dev_open:
.size hv_dev_open,32
.org 0x460
.global hv_dev_close
.type hv_dev_close,function
hv_dev_close:
.size hv_dev_close,32
.org 0x480
.global hv_dev_pread
.type hv_dev_pread,function
hv_dev_pread:
.size hv_dev_pread,32
.org 0x4a0
.global hv_dev_pwrite
.type hv_dev_pwrite,function
hv_dev_pwrite:
.size hv_dev_pwrite,32
.org 0x4c0
.global hv_dev_poll
.type hv_dev_poll,function
hv_dev_poll:
.size hv_dev_poll,32
.org 0x4e0
.global hv_dev_poll_cancel
.type hv_dev_poll_cancel,function
hv_dev_poll_cancel:
.size hv_dev_poll_cancel,32
.org 0x500
.global hv_dev_preada
.type hv_dev_preada,function
hv_dev_preada:
.size hv_dev_preada,32
.org 0x520
.global hv_dev_pwritea
.type hv_dev_pwritea,function
hv_dev_pwritea:
.size hv_dev_pwritea,32
.org 0x540
.global hv_flush_remote
.type hv_flush_remote,function
hv_flush_remote:
.size hv_flush_remote,32
.org 0x560
.global hv_console_putc
.type hv_console_putc,function
hv_console_putc:
.size hv_console_putc,32
.org 0x580
.global hv_inquire_tiles
.type hv_inquire_tiles,function
hv_inquire_tiles:
.size hv_inquire_tiles,32
.org 0x5a0
.global hv_confstr
.type hv_confstr,function
hv_confstr:
.size hv_confstr,32
.org 0x5c0
.global hv_reexec
.type hv_reexec,function
hv_reexec:
.size hv_reexec,32
.org 0x5e0
.global hv_set_command_line
.type hv_set_command_line,function
hv_set_command_line:
.size hv_set_command_line,32
.org 0x600
.global hv_clear_intr
.type hv_clear_intr,function
hv_clear_intr:
.size hv_clear_intr,32
.org 0x620
.global hv_enable_intr
.type hv_enable_intr,function
hv_enable_intr:
.size hv_enable_intr,32
.org 0x640
.global hv_disable_intr
.type hv_disable_intr,function
hv_disable_intr:
.size hv_disable_intr,32
.org 0x660
.global hv_raise_intr
.type hv_raise_intr,function
hv_raise_intr:
.size hv_raise_intr,32
.org 0x680
.global hv_trigger_ipi
.type hv_trigger_ipi,function
hv_trigger_ipi:
.size hv_trigger_ipi,32
.org 0x6a0
.global hv_store_mapping
.type hv_store_mapping,function
hv_store_mapping:
.size hv_store_mapping,32
.org 0x6c0
.global hv_inquire_realpa
.type hv_inquire_realpa,function
hv_inquire_realpa:
.size hv_inquire_realpa,32
.org 0x6e0
.global hv_flush_all
.type hv_flush_all,function
hv_flush_all:
.size hv_flush_all,32
.org 0x700
.global hv_get_ipi_pte
.type hv_get_ipi_pte,function
hv_get_ipi_pte:
.size hv_get_ipi_pte,32
.org 0x720
.global hv_glue_internals
.type hv_glue_internals,function
hv_glue_internals:
.size hv_glue_internals,30944

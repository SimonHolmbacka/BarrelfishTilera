OUTPUT_ARCH(tile)
ENTRY(start_kernel)
PHDRS
{
 intr_pl1 PT_LOAD;
 text PT_LOAD;
 data PT_LOAD;
}
SECTIONS
{
 .intr_pl1 (0xFD000000) :
 {
  *(.intr_pl1)
 } :intr_pl1 =0
 .hvglue (((0xFD000000) + ((0x10000)))) (NOLOAD) :
 {
  *(.hvglue)
 } :data
 .text (0xFD030000) :
 {
  *(.text)
 } :text
 _end_text = .;
 .rodata (0x00F00000) :
 {
  *(.rodata)
 } :data
 .data :
 {
  *(.data)
 } :data
 _end_data = .;
}

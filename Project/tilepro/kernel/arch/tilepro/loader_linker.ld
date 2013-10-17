OUTPUT_ARCH(tile)
ENTRY(_start)
PHDRS
{
 text PT_LOAD;
 data PT_LOAD;
}
SECTIONS
{
 .hvglue (((0xFD000000) + ((0x10000)))) (NOLOAD) : AT ((0x10000))
 {
  *(.hvglue)
 } :data
 .text (0xFD020000) : AT (0x00020000)
 {
  *(.text)
 } :text
 _end_text = .;
 .rodata (0x00A00000) : AT (0x01A00000)
 {
  *(.rodata)
 } :data
 .data :
 {
  *(.data)
 } :data
 _end_data = .;
}

MEMORY
{
PAGE 0:    /* Program Memory (Initialized Sections) */
   RAMGS0123           : origin = 0x00C000, length = 0x004000
   FLASH_BEGIN     	   : origin = 0x080000, length = 0x000002
   FLASH_A             : origin = 0x080002, length = 0x001FFE
   FLASH_BCDEFGHIJKLMN : origin = 0x082000, length = 0x03E000
   RESET           	   : origin = 0x3FFFC0, length = 0x000002

PAGE 1:    /* Data Memory (Uninitialized Sections) */
   RAMM01              : origin = 0x000000, length = 0x000800
   RAMLS012345         : origin = 0x008000, length = 0x003000
   RAMD01              : origin = 0x00B000, length = 0x001000
   RAMGS456789ABCDEF   : origin = 0x010000, length = 0x00C000
}

SECTIONS
{
   .text               : > RAMGS0123,                   PAGE = 0
   .cinit              : > RAMGS0123,                   PAGE = 0
   .econst             : > RAMGS0123,                   PAGE = 0
   .switch             : > RAMGS0123,                   PAGE = 0
   .reset              : > RESET,                       PAGE = 0, TYPE = DSECT  /* Not using the .reset section */

   .ebss               : > RAMGS456789ABCDEF,           PAGE = 1
   .stack              : > RAMM01,                      PAGE = 1
   .esysmem            : > RAMLS012345,                 PAGE = 1
}

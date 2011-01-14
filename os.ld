SECTIONS
{
    . = 0x10000;
    .data
    {
        * (.data);
    }
    .text
    {
        test.o (.text);
        * (.text);
    }
    .bss
    {
        * (.bss);
    }
}

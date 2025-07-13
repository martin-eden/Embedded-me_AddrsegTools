# What

(2024-05/2024-06, 2024-09/2024-10, 2024-12)

Arduino library describing memory segment.


## Design

Memory segment is a range of memory addresses.

```C++
struct TMemorySegment
{
  union
  {
    TUint_2 Addr = 0;
    // "Bytes" provides array access to memory at "Addr"
    TUint_1 * Bytes;
  };
  TUint_2 Size = 0;
};
```

This structure is our type for variable-length data in RAM.

### `.Bytes`

`Bytes` field is a sort of compiler's hack. It tells compiler that
given address is pointer to byte. And C++ is always happy to treat
anything as array.

For example let's divide value in register R3 by 2 (just for example):

```C++
TMemorySegment Registers;
Resisters.Addr = 0;
Registers.Size = 32;

Registers.Bytes[3] = Registers.Bytes[3] / 2;
```

This works while segment describes span in RAM. (Not in flash or in EEPROM
for example.)


## Uses

Actually this module became like new base type and widely used
by my other libraries.


## Reference output

```
[me_MemorySegment] Okay, we are here.
  FromAsciiz
    Addr ( 00372 ) Size ( 00003 )
    Contents ( ABC )
    Byte-Contents ( 065 066 067 )
  FromAddrSize
    Addr ( 00093 ) Size ( 00002 )
    Contents (  )
    Byte-Contents ( 243 008 )
  ( Reserve CopyMemTo Release )
    Reserve
      Addr ( 00553 ) Size ( 00004 )
      Contents (  )
      Byte-Contents ( 000 000 000 000 )
    CopyMemTo
      Addr ( 00553 ) Size ( 00004 )
      Contents ( DATA )
      Byte-Contents ( 068 065 084 065 )
    Release
      Addr ( 00000 ) Size ( 00000 )
      Contents (  )
      Failed to setup iterator.
[me_MemorySegment] Done.
```

## Code

* [Example][Example]
* [Interface][Interface]
* [Implementation][Implementation]
* [Design notes][Design notes]


## Requirements

  * arduino-cli
  * bash


## Install/remove

Easy way is to clone [GetLibs][GetLibs] repo and run it's code.


## See also

* [My other embedded C++ libraries][Embedded]
* [My other repositories][Repos]

[Example]: examples/me_MemorySegment/me_MemorySegment.ino
[Interface]: src/me_MemorySegment.h
[Implementation]: src/me_MemorySegment.cpp
[Design notes]: extras/Design%20notes.txt

[me_ManagedMemory]: https://github.com/martin-eden/Embedded-me_ManagedMemory
[me_List]: https://github.com/martin-eden/Embedded-me_List
[me_Menu]: https://github.com/martin-eden/Embedded-me_Menu
[me_RgbStripeConsole]: https://github.com/martin-eden/Embedded-me_RgbStripeConsole

[GetLibs]: https://github.com/martin-eden/Embedded-Framework-GetLibs

[Embedded]: https://github.com/martin-eden/Embedded_Crafts/tree/master/Parts
[Repos]: https://github.com/martin-eden/contents

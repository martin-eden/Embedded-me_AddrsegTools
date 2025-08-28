## What

(2024, 2025)

Tools for address segment.


## Design

Structure for variable-length data is defined in `[me_BaseTypes]` and is

```C++
struct TAddressSegment
{
  TAddress Addr = 0;
  TUint_2 Size = 0;
};
```

Typically this record describes data is in RAM, but it may also be
in Flash or EEPROM.


## History note

This is the first important module. It was written before all other
modules. Original scope was like "isolate array pointer functions".
Currently scope is like "tools for address span record".

So most of functions here are out of scope. We're working on this
by thinking what to move and where and do we really need that stuff.

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

* [Interface][Interface]
* [Example][Example]
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


[Interface]: src/me_MemorySegment.h
[Example]: examples/me_MemorySegment/me_MemorySegment.ino
[Implementation]: src/me_MemorySegment.cpp
[Design notes]: extras/Design%20notes.txt

[me_ManagedMemory]: https://github.com/martin-eden/Embedded-me_ManagedMemory
[me_List]: https://github.com/martin-eden/Embedded-me_List
[me_Menu]: https://github.com/martin-eden/Embedded-me_Menu
[me_RgbStripeConsole]: https://github.com/martin-eden/Embedded-me_RgbStripeConsole

[GetLibs]: https://github.com/martin-eden/Embedded-Framework-GetLibs

[Embedded]: https://github.com/martin-eden/Embedded_Crafts/tree/master/Parts
[Repos]: https://github.com/martin-eden/contents

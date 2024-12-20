// Memory segment structure

/*
  Author: Martin Eden
  Last mod.: 2024-12-15
*/

/*
  Data structures is the base substrate. Algorithms grow on them
  like plant species.
*/

/*
  Central part of this module is memory segment definition.

  It describes 1-D segment of memory units.

  It's pointer with size information.

  It's base datatype for all variable-length data.
*/

#pragma once

#include <me_BaseTypes.h>

namespace me_MemorySegment
{
  /*
    Memory span with byte granularity
  */
  struct TMemorySegment
  {
    union
    {
      TAddress Addr = 0;
      // "Bytes" provides array access to memory at "Addr"
      TUnit * Bytes;
    };
    TUint_2 Size = 0;
  };

  /*
    Segment iterator

    Main use it to avoid writing for's. For writing while's instead.

    Initialize it with some memory segment. Then call GetNext()
    to get address. Until GetNext() fails.
  */
  class TSegmentIterator
  {
    private:
      TAddress CurrentAddr;
      TAddress MaxAddr;

    public:
      TBool Init(TMemorySegment Segment);

      TBool GetNext(TAddress * Address);
  };

  namespace Freetown
  {
    // Reserve block of memory. Zero after allocation
    TBool Reserve(
      me_MemorySegment::TMemorySegment * MemSeg,
      TUint_2 Size
    );

    // Release block of memory. Zero before release
    void Release(
      me_MemorySegment::TMemorySegment * MemSeg
    );

    // Describe ASCIIZ structure as memory segment
    TMemorySegment FromAsciiz(TAsciiz Asciiz);

    // Represent address and size args as record
    TMemorySegment FromAddrSize(TAddress Addr, TUint_2 Size);

    // Return true if segments intersect
    TBool Intersects(TMemorySegment A, TMemorySegment B);

    // Return true if segment A is inside segment B
    TBool IsInside(TMemorySegment A, TMemorySegment B);

    // Compare for equality
    TBool AreEqual(TMemorySegment A, TMemorySegment B);

    // Fill memory span with zero byte
    void ZeroMem(
      me_MemorySegment::TMemorySegment MemSeg
    );

    // Copy data to another segment
    TBool CopyMemTo(
      me_MemorySegment::TMemorySegment Dest,
      me_MemorySegment::TMemorySegment Src
    );
  }
}

/*
  2024-05 # # # # #
  2024-06 # # # #
  2024-09 #
  2024-10 # # # # #
  2024-12-12 [>] Imported TSegmentIterator
  2024-12-15
*/

// Memory segment structure

/*
  Author: Martin Eden
  Last mod.: 2024-12-12
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

    Main use it to avoid writing for's. For writing while's.

    Provide it with memory segment and unit getter function
    (boolean function receives address and sets unit argument).
    Then call GetNext() while it succeeds.
  */
  class TSegmentIterator
  {
    private:
      TUint_2 CurrentAddr;
      TUint_2 MaxAddr;
      TResponsiveMethod Getter;

    public:
      void Init(
        me_MemorySegment::TMemorySegment Segment,
        TResponsiveMethod ArgGetter
      );

      TBool GetNext(TUnit * Value);
  };

  namespace Freetown
  {
    // [Debug] Print fields and data to stdout
    void PrintWrappings(TMemorySegment MemSeg);

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
    TMemorySegment FromAddrSize(TUint_2 Addr, TUint_2 Size);

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

    // Get byte from memory segment. Getter for iterator
    TBool UnitGetter(
      TAddress DestUnit,
      TAddress SrcUnit
    );
  }
}

/*
  2024-05-17
  2024-05-23 GetByte
  2024-05-25 PrintWrappings, PrintMem, CopyMemTo, Spawn, Kill
  2024-05-27 ReserveChunk, ReleaseChunk, ZeroMem
  2024-05-30 CloneFrom
  2024-06-02 < CloneFrom
  2024-06-04
  2024-06-07 IsEqualTo
  2024-06-08 IsEqualTo.Asciiz
  2024-09-15 Zeroing before alloc/free is part of contract now
  2024-10-05 Freetown. Moved memory-changing code to [me_ManagedMemory]
  2024-10-11 IsInside
  2024-10-14 FromAddrSize
  2024-10-18 [>] Import Freetown from [me_ManagedMemory]
  2024-10-23 [-] Removed [me_MemoryPoint], <TMemorySegment_Bits>
  2024-12-12 [>] Imported TSegmentIterator
*/

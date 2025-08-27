// Memory segment structure

/*
  Author: Martin Eden
  Last mod.: 2025-08-27
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
#include <me_BaseInterfaces.h>

namespace me_MemorySegment
{
  /*
    Memory span with byte granularity
  */
  struct TMemorySegment
  {
    TAddress Addr = 0;
    TUint_2 Size = 0;
  };

  /*
    Unary functions for memory segment
  */

  // Validity check
  TBool IsValid(TMemorySegment Seg);

  // Invalidation
  void Invalidate(TMemorySegment * Seg);

  // Get last address helper
  TAddress GetEndAddr(TMemorySegment Seg);

  /*
    Binary functions for memory segments
  */

  // Same size check
  TBool IsSameSize(TMemorySegment A, TMemorySegment B);

  // Fields equality check
  TBool IsSameRec(TMemorySegment A, TMemorySegment B);

  // Compatibility check
  TBool AreCompatible(TMemorySegment A, TMemorySegment B);

  // Data equality check
  TBool AreEqual(TMemorySegment A, TMemorySegment B);

  // Return true if segments intersect
  TBool Intersects(TMemorySegment A, TMemorySegment B);

  // Return true if segment A is inside segment B
  TBool IsInside(TMemorySegment A, TMemorySegment B);

  /*
    Address segment iterator
  */
  class TSegmentIterator : public TAddressIterator
  {
    public:
      TBool Init(TMemorySegment Segment);
  };

  namespace Freetown
  {
    // Fill memory span with zero byte
    void ZeroMem(TMemorySegment MemSeg);

    // Copy data to another segment
    void CopyMemTo(TMemorySegment Dest, TMemorySegment Src);

    // Reserve block of memory. Zeroes after allocation
    TBool Reserve(TMemorySegment * MemSeg, TUint_2 Size);

    // Release block of memory. Zeroes before release
    void Release(TMemorySegment * MemSeg);

    // Describe ASCIIZ structure as memory segment
    TMemorySegment FromAsciiz(TAsciiz Asciiz);

    // Represent address and size args as record
    TMemorySegment FromAddrSize(TAddress Addr, TUint_2 Size);
  }
}

/*
  Macro to embed ASCIIZ into program memory segment

  Used as

    Console.PrintProgmem(AsProgmemSeg("Hello"));
*/
#define AsProgmemSeg(Asciiz) \
  ( \
    __extension__ \
    ( \
      { \
        static const TUint_1 ProgmemData[] [[gnu::progmem]] = (Asciiz); \
        \
        me_MemorySegment::TMemorySegment ProgmemSeg = \
          { \
            .Addr = (TAddress) ProgmemData, \
            .Size = sizeof(ProgmemData) - 1 \
          }; \
        ProgmemSeg; \
      } \
    ) \
  )

/*
  2024 # # # # # # # # # # # # # # # # #
  2025-08-22
  2025-08-24
  2025-08-25
  2025-08-26
  2025-08-27
*/

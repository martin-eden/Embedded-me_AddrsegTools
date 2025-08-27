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
    Unary functions for memory segment
  */

  // Validity check
  TBool IsValid(TAddressSegment Seg);

  // Invalidation
  void Invalidate(TAddressSegment * Seg);

  // Get last address helper
  TAddress GetEndAddr(TAddressSegment Seg);

  /*
    Binary functions for memory segments
  */

  // Same size check
  TBool IsSameSize(TAddressSegment A, TAddressSegment B);

  // Fields equality check
  TBool IsSameRec(TAddressSegment A, TAddressSegment B);

  // Compatibility check
  TBool AreCompatible(TAddressSegment A, TAddressSegment B);

  // Data equality check
  TBool AreEqual(TAddressSegment A, TAddressSegment B);

  // Return true if segments intersect
  TBool Intersects(TAddressSegment A, TAddressSegment B);

  // Return true if segment A is inside segment B
  TBool IsInside(TAddressSegment A, TAddressSegment B);

  /*
    Address segment iterator
  */
  class TSegmentIterator : public TAddressIterator
  {
    public:
      TBool Init(TAddressSegment Segment);
  };

  namespace Freetown
  {
    // Fill memory span with zero byte
    void ZeroMem(TAddressSegment MemSeg);

    // Copy data to another segment
    void CopyMemTo(TAddressSegment Dest, TAddressSegment Src);

    // Reserve block of memory. Zeroes after allocation
    TBool Reserve(TAddressSegment * MemSeg, TUint_2 Size);

    // Release block of memory. Zeroes before release
    void Release(TAddressSegment * MemSeg);

    // Describe ASCIIZ structure as memory segment
    TAddressSegment FromAsciiz(TAsciiz Asciiz);

    // Represent address and size args as record
    TAddressSegment FromAddrSize(TAddress Addr, TUint_2 Size);
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
        TAddressSegment ProgmemSeg = \
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

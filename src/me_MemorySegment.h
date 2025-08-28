// Memory segment structure

/*
  Author: Martin Eden
  Last mod.: 2025-08-29
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
    Unary functions for address segment
  */

  // Validity check
  TBool IsValid(TAddressSegment AddrSeg);

  // Invalidation
  void Invalidate(TAddressSegment * AddrSeg);

  /*
    Binary functions for address segments
  */

  // Data equality check
  TBool AreEqual(TAddressSegment A, TAddressSegment B);

  // Return true if segment A is inside segment B
  TBool IsInside(TAddressSegment A, TAddressSegment B);

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
  }
}

/*
  2024 # # # # # # # # # # # # # # # # #
  2025-08-22
  2025-08-24
  2025-08-25
  2025-08-26
  2025-08-27
  2025-08-28
  2025-08-29
*/

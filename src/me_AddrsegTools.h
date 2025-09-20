// Utility functions for address segment

/*
  Author: Martin Eden
  Last mod.: 2025-09-20
*/

#pragma once

#include <me_BaseTypes.h>

namespace me_AddrsegTools
{
  // Validity check
  TBool IsValid(TAddressSegment AddrSeg);

  // Invalidation
  void Invalidate(TAddressSegment * AddrSeg);

  // Nesting check
  TBool IsInside(TAddressSegment InnerSeg, TAddressSegment OuterSeg);

  // (Side selection

  // Drop left part, keep address
  TBool ChopLeftFrom(TAddressSegment * AddrSeg, TAddress Addr);

  // Drop left part, exclude address
  TBool ChopLeftAt(TAddressSegment * AddrSeg, TAddress Addr);

  // Drop right part, keep address
  TBool ChopRightFrom(TAddressSegment * AddrSeg, TAddress Addr);

  // Drop right part, exclude address
  TBool ChopRightAt(TAddressSegment * AddrSeg, TAddress Addr);

  // )

  // Create segment from start and end addresses
  TBool SegFromAddrs(TAddressSegment *, TAddress Start, TAddress Stop);
}

/*
  2024 # # # # # # # # # # # # # # # # #
  2025 # # # # # # # # #
  2025-09-03
  2025-09-20
*/

// Utility functions for address segment

/*
  Author: Martin Eden
  Last mod.: 2025-08-30
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
}

/*
  2024 # # # # # # # # # # # # # # # # #
  2025 # # # # # # #
  2025-08-30
*/

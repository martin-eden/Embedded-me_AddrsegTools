// Utility functions for address segment

/*
  Author: Martin Eden
  Last mod.: 2025-08-30
*/

#include <me_AddrsegTools.h>

#include <me_BaseTypes.h>

using namespace me_AddrsegTools;

/*
  [Internal] Check that we can move address by N units

  Another descent madness because C don't do range checks.
*/
TBool CanAdvance(
  TAddress Address,
  TSize NumUnits
)
{
  TAddress MaxNewAddress = TAddress_Max - NumUnits;

  return (Address <= MaxNewAddress);
}

/*
  Check that segment is valid

  It means it has non-zero size and ends before max address.
*/
TBool me_AddrsegTools::IsValid(
  TAddressSegment AddrSeg
)
{
  return
    (AddrSeg.Size != 0) &&
    CanAdvance(AddrSeg.Addr, AddrSeg.Size - 1);
}

/*
  Invalidate segment
*/
void me_AddrsegTools::Invalidate(
  TAddressSegment * Seg
)
{
  Seg->Addr = 0;
  Seg->Size = 0;
}

/*
  [Internal] Get end address

  Segment MUST be valid
*/
TAddress GetEndAddr(
  TAddressSegment Seg
)
{
  return (Seg.Addr + Seg.Size - 1);
}

/*
  Check for nesting
*/
TBool me_AddrsegTools::IsInside(
  TAddressSegment InnerSeg,
  TAddressSegment OuterSeg
)
{
  if (!IsValid(InnerSeg))
    return false;

  if (!IsValid(OuterSeg))
    return false;

  if (InnerSeg.Addr < OuterSeg.Addr)
    return false;

  if (GetEndAddr(InnerSeg) > GetEndAddr(OuterSeg))
    return false;

  return true;
}

/*
  2024 # # # # # # # # # # #
  2025-08-07
  2025-08-29
  2025-08-30
*/

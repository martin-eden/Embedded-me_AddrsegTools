// Utility functions for address segment

/*
  Author: Martin Eden
  Last mod.: 2025-09-02
*/

#include <me_AddrsegTools.h>

#include <me_BaseTypes.h>

using namespace me_AddrsegTools;

/*
  [Internal] Check that we can move address by N units forward without overflow

  Another descent madness because C don't do range checks.
*/
TBool CanAdd(
  TAddress Address,
  TSize NumUnits
)
{
  TAddress MaxNewAddress = TAddress_Max - NumUnits;

  return (Address <= MaxNewAddress);
}

/*
  [Internal] Check we can move address N units backward
*/
TBool CanSubtract(
  TAddress Address,
  TSize NumUnits
)
{
  return (Address >= NumUnits);
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
  [Internal] Calculate size from start and end addresses

  No input checks!
*/
TSize CalcSizeFromAddrs(
  TAddress StartAddr,
  TAddress EndAddr
)
{
  return (EndAddr - StartAddr + 1);
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
    CanAdd(AddrSeg.Addr, AddrSeg.Size - 1);
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
  Chop left, include address
*/
TBool me_AddrsegTools::ChopLeftFrom(
  TAddressSegment * AddrSeg,
  TAddress CuttingAddr
)
{
  if (!IsValid(*AddrSeg))
    return false;

  if (CuttingAddr < AddrSeg->Addr)
    return false;

  if (CuttingAddr > GetEndAddr(*AddrSeg))
    return false;

  AddrSeg->Size = CalcSizeFromAddrs(CuttingAddr, GetEndAddr(*AddrSeg));
  AddrSeg->Addr = CuttingAddr;

  return true;
}

/*
  Chop left, exclude address
*/
TBool me_AddrsegTools::ChopLeftAt(
  TAddressSegment * AddrSeg,
  TAddress CuttingAddr
)
{
  if (!ChopLeftFrom(AddrSeg, CuttingAddr))
    return false;

  if (!CanAdd(AddrSeg->Addr, 1))
    return false;

  AddrSeg->Size = AddrSeg->Size - 1;
  AddrSeg->Addr = AddrSeg->Addr + 1;

  return IsValid(*AddrSeg);
}

/*
  Chop right, include address
*/
TBool me_AddrsegTools::ChopRightFrom(
  TAddressSegment * AddrSeg,
  TAddress CuttingAddr
)
{
  if (!IsValid(*AddrSeg))
    return false;

  if (CuttingAddr < AddrSeg->Addr)
    return false;

  if (CuttingAddr > GetEndAddr(*AddrSeg))
    return false;

  AddrSeg->Size = CalcSizeFromAddrs(AddrSeg->Addr, CuttingAddr);

  return true;
}

/*
  Chop right, exclude address
*/
TBool me_AddrsegTools::ChopRightAt(
  TAddressSegment * AddrSeg,
  TAddress CuttingAddr
)
{
  if (!ChopRightFrom(AddrSeg, CuttingAddr))
    return false;

  if (!CanSubtract(AddrSeg->Addr, 1))
    return false;

  AddrSeg->Size = AddrSeg->Size - 1;

  return IsValid(*AddrSeg);
}

/*
  2024 # # # # # # # # # # #
  2025 # # #
  2025-09-02
*/

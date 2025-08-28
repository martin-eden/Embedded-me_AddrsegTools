// Memory segment companion functions

/*
  Author: Martin Eden
  Last mod.: 2025-08-29
*/

#include <me_MemorySegment.h>

#include <me_BaseTypes.h>
#include <me_StreamTools.h>
#include <me_WorkMemory.h>
#include <me_Asciiz.h>

using namespace me_MemorySegment;

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
TBool me_MemorySegment::IsValid(
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
void me_MemorySegment::Invalidate(
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
  Compare for data equality

  We do not provide additional checks for intersection.

  These two segments will be considered equal:

    ~~~
    ABABA
      ~~~
*/
TBool me_MemorySegment::AreEqual(
  TAddressSegment A_Seg,
  TAddressSegment B_Seg
)
{
  me_WorkMemory::TInputStream A_Stream;
  me_WorkMemory::TInputStream B_Stream;

  if (!A_Stream.Init(A_Seg))
    return false;

  if (!B_Stream.Init(B_Seg))
    return false;

  return me_StreamTools::StreamsAreEqual(&A_Stream, &B_Stream);
}

/*
  Check for belonging

  Return true if segment A is inside segment B.

  Empty segment doesn't belong to anything
*/
TBool me_MemorySegment::IsInside(
  TAddressSegment A,
  TAddressSegment B
)
{
  if (!IsValid(A))
    return false;

  if (!IsValid(B))
    return false;

  if (A.Addr < B.Addr)
    return false;

  if (GetEndAddr(A) > GetEndAddr(B))
    return false;

  return true;
}

/*
  Represent ASCII structure as memory segment

  Zero byte is not counted.
*/
TAddressSegment me_MemorySegment::FromAsciiz(
  TAsciiz Asciiz
)
{
  TAddressSegment Result;

  Result.Addr = (TAddress) Asciiz;
  me_Asciiz::GetLength_Workmem(&Result.Size, Asciiz);

  return Result;
}

/*
  2024 # # # # # # # # # # #
  2025-08-07
*/

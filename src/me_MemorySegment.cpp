// Memory segment companion functions

/*
  Author: Martin Eden
  Last mod.: 2025-08-27
*/

#include <me_MemorySegment.h>

#include <me_BaseTypes.h>
#include <me_Streams.h>
#include <me_WorkMemory.h>

using namespace me_MemorySegment;

/*
  Check for existence

  Segments with zero size are not valid.
*/
TBool me_MemorySegment::IsValid(
  TAddressSegment Seg
)
{
  return (Seg.Size != 0);
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
  Get end address. Segment MUST be valid
*/
TAddress me_MemorySegment::GetEndAddr(
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

  return me_Streams::StreamsAreEqual(&A_Stream, &B_Stream);
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

// ( Segment iterator

/*
  Setup segment iterator
*/
TBool TSegmentIterator::Init(
  TAddressSegment Segment
)
{
  if (!IsValid(Segment))
    return false;

  return TAddressIterator::Init(Segment.Addr, GetEndAddr(Segment));
}

// )

/*
  2024 # # # # # # # # # # #
  2025-08-07
*/

// Memory segment companion functions

/*
  Author: Martin Eden
  Last mod.: 2024-12-17
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
  TMemorySegment Seg
)
{
  return (Seg.Size != 0);
}

/*
  Invalidate segment
*/
void me_MemorySegment::Invalidate(
  TMemorySegment * Seg
)
{
  Seg->Addr = 0;
  Seg->Size = 0;
}

/*
  Get end address. Segment MUST be valid
*/
TAddress me_MemorySegment::GetEndAddr(
  TMemorySegment Seg
)
{
  return (Seg.Addr + Seg.Size - 1);
}

/*
  Check that segments are valid and have same size
*/
TBool me_MemorySegment::IsSameSize(
  TMemorySegment A,
  TMemorySegment B
)
{
  return
    IsValid(A) &&
    IsValid(B) &&
    (A.Size == B.Size);
}

/*
  Check that record fields are same
*/
TBool me_MemorySegment::IsSameRec(
  TMemorySegment A,
  TMemorySegment B
)
{
  return
    (A.Size == B.Size) &&
    (A.Addr == B.Addr);
}

/*
  Compatibility check

  Segments are compatible if they are same size and do no intersect.
*/
TBool me_MemorySegment::AreCompatible(
  TMemorySegment A,
  TMemorySegment B
)
{
  if (!IsValid(A))
    return false;

  if (!IsValid(B))
    return false;

  if (A.Size != B.Size)
    return false;

  if (Intersects(A, B))
    return false;

  return true;
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
  TMemorySegment A_Seg,
  TMemorySegment B_Seg
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
  Return true if segments intersect
*/
TBool me_MemorySegment::Intersects(
  TMemorySegment A,
  TMemorySegment B
)
{
  if (!IsValid(A))
    return false;

  if (!IsValid(B))
    return false;

  // A starts before B
  if (A.Addr < B.Addr)
  {
    if (GetEndAddr(A) < B.Addr)
      return false;
  }
  // A starts after B
  else
  {
    if (GetEndAddr(B) < A.Addr)
      return false;
  }

  return true;
}

/*
  Check for belonging

  Return true if segment A is inside segment B.

  Empty segment doesn't belong to anything
*/
TBool me_MemorySegment::IsInside(
  TMemorySegment A,
  TMemorySegment B
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
  TMemorySegment Segment
)
{
  if (!IsValid(Segment))
    return false;

  CurrentAddr = Segment.Addr;
  MaxAddr = GetEndAddr(Segment);

  return true;
}

/*
  Get next address from iterator

  On fail it returns false.

  It fails when address is beyond segment.
*/
TBool TSegmentIterator::GetNextAddr(
  TAddress * Address
)
{
  if (CurrentAddr > MaxAddr)
    return false;

  *Address = CurrentAddr;

  ++CurrentAddr;

  return true;
}

// )

/*
  2024 # # # # # # # # # # #
*/

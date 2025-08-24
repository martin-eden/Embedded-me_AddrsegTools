// Memory segment companion functions

/*
  Author: Martin Eden
  Last mod.: 2024-12-17
*/

#include <me_MemorySegment.h>

#include <me_BaseTypes.h>

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
  Get end address
*/
TAddress me_MemorySegment::GetEndAddr(
  TMemorySegment Seg
)
{
  return (Seg.Addr + Seg.Size - 1);
}

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
TBool TSegmentIterator::GetNext(
  TAddress * Address
)
{
  if (CurrentAddr > MaxAddr)
    return false;

  *Address = CurrentAddr;

  ++CurrentAddr;

  return true;
}

/*
  2024 # # # # # # # # # # #
*/

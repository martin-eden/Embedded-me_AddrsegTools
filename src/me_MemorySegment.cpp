// Memory segment companion functions

/*
  Author: Martin Eden
  Last mod.: 2024-12-17
*/

#include <me_MemorySegment.h>

#include <me_BaseTypes.h>

#include <Arduino.h> // malloc(), free(), min()
#include <me_Asciiz.h>

using namespace me_MemorySegment;

/*
  Setup segment iterator
*/
TBool TSegmentIterator::Init(
  TMemorySegment Segment
)
{
  if ((Segment.Addr == 0) && (Segment.Size == 0))
    return false;

  CurrentAddr = Segment.Addr;
  MaxAddr = CurrentAddr + Segment.Size - 1;

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
  Reserve block of memory

  Zeroes data after allocation.
*/
TBool Freetown::Reserve(
  TMemorySegment * MemSeg,
  TUint_2 Size
)
{
  // Zero size? Job done!
  if (Size == 0)
    return true;

  TAddress MallocAddr = (TUint_2) malloc(Size);

  if (MallocAddr == 0)
    return false;

  MemSeg->Addr = MallocAddr;
  MemSeg->Size = Size;

  ZeroMem(*MemSeg);

  return true;
}

/*
  Release block of memory

  Zeroes data before release.
*/
void Freetown::Release(
  TMemorySegment * MemSeg
)
{
  // Zero size? Job done!
  if (MemSeg->Size == 0)
  {
    MemSeg->Addr = 0;
    return;
  }

  ZeroMem(*MemSeg);

  free((void *) MemSeg->Addr);

  MemSeg->Addr = 0;
  MemSeg->Size = 0;
}

/*
  Cast ASCII structure to memory segment

  Zero byte is not counted.
*/
TMemorySegment Freetown::FromAsciiz(
  TAsciiz Asciiz
)
{
  using
    me_Asciiz::GetLength_Workmem;

  TMemorySegment Result;

  Result.Addr = (TAddress) Asciiz;
  GetLength_Workmem(&Result.Size, Asciiz);

  return Result;
}

/*
  Represent address and size args as record
*/
TMemorySegment Freetown::FromAddrSize(
  TAddress Addr,
  TUint_2 Size
)
{
  TMemorySegment Result;

  Result.Addr = Addr;
  Result.Size = Size;

  return Result;
}

/*
  Return true if segments intersect

  Empty segment don't intersect anything.
*/
TBool Freetown::Intersects(
  TMemorySegment A,
  TMemorySegment B
)
{
  // Empty segment don't intersect anything. Even same empty segment.
  if ((A.Size == 0) || (B.Size == 0))
    return false;

  TAddress A_Start = A.Addr;
  TAddress B_Start = B.Addr;

  if (A_Start < B_Start)
  {
    TUint_2 A_Stop = A_Start + A.Size - 1;
    if (A_Stop >= B_Start)
      return true;
  }
  else // if (A_Start => B_Start)
  {
    TUint_2 B_Stop = B_Start + B.Size - 1;
    if (B_Stop >= A_Start)
      return true;
  }

  return false;
}

/*
  Check for belonging

  Return true if segment A is inside segment B.

  Empty segment doesn't belong to anything
*/
TBool Freetown::IsInside(
  TMemorySegment A,
  TMemorySegment B
)
{
  // Empty segments belongs to noone. Even to the same empty segment
  if ((A.Size == 0) || (B.Size == 0))
    return false;

  TAddress A_Start = A.Addr;
  TAddress B_Start = B.Addr;

  if (!(A_Start >= B_Start))
    return false;

  TAddress A_Stop = A_Start + A.Size - 1;
  TAddress B_Stop = B_Start + B.Size - 1;

  if (!(A_Stop <= B_Stop))
    return false;

  return true;
}

/*
  Compare for equality

  If trivial check (equal spans) is passed, we'll compare data.

  Segments may intersect, we don't care.
*/
TBool Freetown::AreEqual(
  TMemorySegment A,
  TMemorySegment B
)
{
  // No equality for different sizes
  if (A.Size != B.Size)
    return false;

  // Equality for same span
  if (A.Addr == B.Addr)
    return true;

  // Data comparison
  for (TUint_2 Offset = 0; Offset < A.Size; ++Offset)
    if (A.Bytes[Offset] != B.Bytes[Offset])
      return false;

  return true;
}

/*
  Fill memory span with zero byte
*/
void Freetown::ZeroMem(
  TMemorySegment MemSeg
)
{
  for (TUint_2 Offset = 0; Offset < MemSeg.Size; ++Offset)
    MemSeg.Bytes[Offset] = 0;
}

/*
  Copy data to another segment

  If destination segment is larger than source, we don't touch
  tail data.

  If segments do intersect, we return false. We see no practical
  cases for data copy between intersecting segments.
*/
TBool Freetown::CopyMemTo(
  TMemorySegment Dest,
  TMemorySegment Src
)
{
  if (Freetown::Intersects(Src, Dest))
    return false;

  TUint_2 MinSize = min(Src.Size, Dest.Size);

  // Zero size? Job done!
  if (MinSize == 0)
    return true;

  for (TUint_2 Offset = 0; Offset < MinSize; ++Offset)
    Dest.Bytes[Offset] = Src.Bytes[Offset];

  return true;
}

/*
  2024-05 # # # #
  2024-06 #
  2024-10 # # # # #
  2024-12-15
*/

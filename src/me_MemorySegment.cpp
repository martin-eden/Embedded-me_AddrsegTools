// Memory segment companion functions

/*
  Author: Martin Eden
  Last mod.: 2024-12-15
*/

#include "me_MemorySegment.h"

#include <me_BaseTypes.h>

#include <Arduino.h> // malloc(), free(), min()
#include <string.h> // strlen() for FromAsciiz()

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
  Reserve block of memory. Zero data after allocation

  Opportunistically calls custom memory manager.
  Uses stock malloc() as fallback option.
*/
TBool Freetown::Reserve(
  TMemorySegment * MemSeg,
  TUint_2 Size
)
{
  // Zero size? Job done!
  if (Size == 0)
    return true;

  TUint_2 MallocAddr = (TUint_2) malloc(Size);

  if (MallocAddr == 0)
  {
    /* [Debug]
    printf_P(PSTR("Failed to reserve %u bytes.\n"), Size);
    //*/
    return false;
  }

  MemSeg->Addr = MallocAddr;
  MemSeg->Size = Size;

  // Zero memory (contract)
  ZeroMem(*MemSeg);

  /* [Debug]
  printf_P(PSTR("Reserve ( Addr %05u Size %05u )\n"), MemSeg->Addr, MemSeg->Size);
  //*/

  return true;
}

/*
  Release block of memory. Zero before release

  Opportunistically calls memory manager if it is alive.
  Uses free() as fallback option.
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

  /* [Debug]
  printf_P(PSTR("Release ( Addr %05u Size %05u )\n"), MemSeg->Addr, MemSeg->Size);
  //*/

  // Zero memory (optional)
  ZeroMem(*MemSeg);

  free((void *) MemSeg->Addr);

  MemSeg->Addr = 0;
  MemSeg->Size = 0;
}

/*
  Cast ASCII structure to memory segment

  What is called "string" in C is a pointer to zero-terminated
  sequence of bytes.

  This function describes span of that sequence.

  Zero byte is not counted.
*/
TMemorySegment Freetown::FromAsciiz(
  TAsciiz Asciiz
)
{
  TMemorySegment Result;

  Result.Addr = (TUint_2) Asciiz;
  Result.Size = strlen(Asciiz);

  return Result;
}

/*
  Represent address and size args as record
*/
TMemorySegment Freetown::FromAddrSize(
  TUint_2 Addr,
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

  TUint_2 A_Start = A.Addr;
  TUint_2 B_Start = B.Addr;

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

  TUint_2 A_Start = A.Addr;
  TUint_2 B_Start = B.Addr;

  if (!(A_Start >= B_Start))
    return false;

  TUint_2 A_Stop = A_Start + A.Size - 1;
  TUint_2 B_Stop = B_Start + B.Size - 1;

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
  2024-05 ####
  2024-06 #
  2024-10 #####
  2024-12-15
*/

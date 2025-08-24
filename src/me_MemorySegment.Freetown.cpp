// Core functions for memory segment

/*
  Author: Martin Eden
  Last mod.: 2025-08-24
*/

#include <me_MemorySegment.h>

#include <me_BaseTypes.h>
#include <me_Asciiz.h>
#include <stdlib.h> // malloc(), free()

using namespace me_MemorySegment;

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
  Check that record fields are same
*/
TBool Freetown::IsSameRec(
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
TBool Freetown::AreCompatible(
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

  Segments must not intersect.
*/
TBool Freetown::AreEqual(
  TMemorySegment A,
  TMemorySegment B
)
{
  if (!IsValid(A))
    return false;

  if (!IsValid(B))
    return false;

  if (IsSameRec(A, B))
    return true;

  if (!AreCompatible(A, B))
    return false;

  // Assert: Segments are same size and do not intersect

  // Compare data
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
  Reserve block of memory

  Zeroes data after allocation.
*/
TBool Freetown::Reserve(
  TMemorySegment * MemSeg,
  TUint_2 Size
)
{
  TAddress MallocAddr;

  if (Size == 0)
    return false;

  MallocAddr = (TAddress) malloc(Size);

  if (MallocAddr == 0)
    return false;

  *MemSeg = FromAddrSize(MallocAddr, Size);

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
  if (!IsValid(*MemSeg))
    return;

  ZeroMem(*MemSeg);

  free((void *) MemSeg->Addr);

  Invalidate(MemSeg);
}

/*
  Represent ASCII structure as memory segment

  Zero byte is not counted.
*/
TMemorySegment Freetown::FromAsciiz(
  TAsciiz Asciiz
)
{
  TMemorySegment Result;

  Result.Addr = (TAddress) Asciiz;
  me_Asciiz::GetLength_Workmem(&Result.Size, Asciiz);

  return Result;
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
  Return true if segments intersect
*/
TBool Freetown::Intersects(
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
  Copy data to another segment

  Segments must be compatible (same size and do no intersect).
*/
TBool Freetown::CopyMemTo(
  TMemorySegment Dest,
  TMemorySegment Src
)
{
  if (!IsValid(Src))
    return false;

  if (!IsValid(Dest))
    return false;

  if (!AreCompatible(Src, Dest))
    return false;

  for (TUint_2 Offset = 0; Offset < Src.Size; ++Offset)
    Dest.Bytes[Offset] = Src.Bytes[Offset];

  return true;
}

/*
  2024 # # # # # # # # # # #
  2025-08-24
*/

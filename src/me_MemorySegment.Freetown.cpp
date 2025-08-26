// Core functions for memory segment

/*
  Author: Martin Eden
  Last mod.: 2025-08-26
*/

#include <me_MemorySegment.h>

#include <me_BaseTypes.h>
#include <me_Asciiz.h>
#include <me_Streams.h>
#include <me_WorkMemory.h>

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
  Copy data to another segment
*/
void Freetown::CopyMemTo(
  TMemorySegment Dest,
  TMemorySegment Src
)
{
  me_WorkMemory::TInputStream InputStream;
  me_WorkMemory::TOutputStream OutputStream;

  if (!InputStream.Init(Src))
    return;

  if (!OutputStream.Init(Dest))
    return;

  me_Streams::CopyStreamTo(&InputStream, &OutputStream);
}

/*
  Fill memory span with zero byte
*/
void Freetown::ZeroMem(
  TMemorySegment MemSeg
)
{
  me_Streams::TZeroesInputStream ZeroesInputStream;
  me_WorkMemory::TOutputStream OutputStream;

  if (!OutputStream.Init(MemSeg))
    return;

  me_Streams::CopyStreamTo(&ZeroesInputStream, &OutputStream);
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
  2024 # # # # # # # # # # #
  2025-08-24
  2025-08-26
*/

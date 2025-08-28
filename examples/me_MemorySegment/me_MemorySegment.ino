// [me_MemorySegment] test/demo

/*
  Author: Martin Eden
  Last mod.: 2025-08-29
*/

#include <me_MemorySegment.h>

#include <me_BaseTypes.h>
#include <me_Console.h>
#include <me_WorkMemory.h>

void PrintWrappings(
  TAddressSegment AddrSeg
)
{
  Console.Write("Addr (");
  Console.Print(AddrSeg.Addr);
  Console.Write(")");

  Console.Write(" ");

  Console.Write("Size (");
  Console.Print(AddrSeg.Size);
  Console.Write(")");

  Console.EndLine();
}

void PrintRawContents(
  TAddressSegment AddrSeg
)
{
  Console.Write("Contents ( ");
  Console.Write(AddrSeg);
  Console.Write(" )");

  Console.EndLine();
}

/*
  Print contents using byte iterator
*/
void PrintByteContents(
  TAddressSegment AddrSeg
)
{
  TAddressIterator Rator;
  TAddress Addr;
  TUnit Unit;

  if (!Rator.Init(AddrSeg))
  {
    Console.Print("Failed to setup iterator");
    return;
  }

  Console.Write("Byte-Contents (");

  while (Rator.GetNextAddr(&Addr))
  {
    if (me_WorkMemory::GetByteFrom(&Unit, Addr))
      Console.Print(Unit);
  }

  Console.Write(" )");

  Console.EndLine();
}

void PrintSegmentDetails(
  TAsciiz Header,
  TAddressSegment AddrSeg
)
{
  Console.Print(Header);

  Console.Indent();

  PrintWrappings(AddrSeg);
  PrintRawContents(AddrSeg);
  PrintByteContents(AddrSeg);

  Console.Unindent();
}

void TestFixedSegment()
{
  TAddressSegment AddrSeg;

  /*
    Construct memory segment record
  */

  // Stack pointer is two bytes at 93
  AddrSeg = { .Addr = 93, .Size = 2 };

  PrintSegmentDetails("Stack pointer", AddrSeg);

  // Last addressable byte, describable but not existing
  AddrSeg = { .Addr = TAddress_Max, .Size = 1};

  PrintSegmentDetails("Last addressable byte", AddrSeg);
}

void TestAsciiz()
{
  TAddressSegment AddrSeg;

  /*
    FromAsciiz(): Treat ASCIIZ as memory segment

    Returned segment does not include zero byte.
  */
  AddrSeg = me_MemorySegment::Freetown::FromAsciiz("ABC");

  PrintSegmentDetails("FromAsciiz", AddrSeg);
}

void TestMemoryAllocator()
{
  TAddressSegment SourceData =
    me_MemorySegment::Freetown::FromAsciiz("DATA");
  TAddressSegment DestData;

  Console.Print("( Reserve CopyMemTo Release )");
  Console.Indent();

  /*
    Reserve(): Allocate memory and zero data
  */
  if (!me_MemorySegment::Freetown::Reserve(&DestData, SourceData.Size))
  {
    Console.Print("No memory for temporary data");
    return;
  }

  PrintSegmentDetails("Reserve", DestData);

  /*
    CopyMemTo(): Copy data to another segment
  */
  me_MemorySegment::Freetown::CopyMemTo(DestData, SourceData);

  PrintSegmentDetails("CopyMemTo", DestData);

  /*
    Release(): Deallocate memory and zero data
  */
  me_MemorySegment::Freetown::Release(&DestData);

  PrintSegmentDetails("Release", DestData);

  Console.Unindent();
}

void RunTests()
{
  TestAsciiz();
  TestFixedSegment();
  TestMemoryAllocator();
}

void setup()
{
  Console.Init();

  Console.Print("( [me_MemorySegment] Test");
  Console.Indent();
  RunTests();
  Console.Unindent();
  Console.Print(") Done");
}

void loop()
{
}

/*
  2024-05 # # # # # # # # # # #
  2025-08-28
*/

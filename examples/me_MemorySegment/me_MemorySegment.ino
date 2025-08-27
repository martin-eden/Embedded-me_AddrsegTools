// [me_MemorySegment] test/demo

/*
  Author: Martin Eden
  Last mod.: 2025-08-26
*/

/*
  Demonstration of base functionality of memory segment:

  * Create structure (FromAsciiz, FromAddrSize)
  * Access (iterator)
  * Copy (CopyMemTo)
  * Manage memory (Reserve, Release)

  See RunTest().
*/

#include <me_MemorySegment.h>

#include <me_BaseTypes.h>
#include <me_Console.h>

void setup()
{
  Console.Init();

  Console.Print("[me_MemorySegment] Okay, we are here.");
  Console.Indent();
  RunTest();
  Console.Unindent();
  Console.Print("[me_MemorySegment] Done.");
}

void loop()
{
}

// --

void PrintRawContents(
  TAddressSegment AddrSeg
)
{
  Console.Write("Contents ( ");
  Console.Write(AddrSeg);
  Console.Write(" )");

  Console.EndLine();
}

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

/*
  Print contents using byte iterator
*/
void PrintByteContents(
  TAddressSegment AddrSeg
)
{
  me_MemorySegment::TSegmentIterator Rator;
  TAddress Addr;
  TUnit Unit;

  if (!Rator.Init(AddrSeg))
  {
    Console.Print("Failed to setup iterator.");
    return;
  }

  Console.Write("Byte-Contents (");

  while (Rator.GetNextAddr(&Addr))
  {
    Unit = *(TUnit *) Addr;
    Console.Print(Unit);
  }

  Console.Write(")");

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

void RunTest()
{
  TAddressSegment AddrSeg;

  {
    using
      me_MemorySegment::Freetown::FromAsciiz;

    /*
      FromAsciiz(): Treat ASCIIZ as memory segment

      Note that function return segment not including zero byte.
    */
    AddrSeg = FromAsciiz("ABC");

    PrintSegmentDetails("FromAsciiz", AddrSeg);
  }

  {
    using
      me_MemorySegment::Freetown::FromAddrSize;

    /*
      FromAddrSize(): Construct memory segment
    */
    // Stack pointer is at bytes 93 and 94
    AddrSeg = FromAddrSize(93, 2);

    PrintSegmentDetails("FromAddrSize", AddrSeg);
  }

  {
    using
      me_MemorySegment::Freetown::FromAsciiz,
      me_MemorySegment::Freetown::Reserve,
      me_MemorySegment::Freetown::CopyMemTo,
      me_MemorySegment::Freetown::Release;

    TAddressSegment SourceData = FromAsciiz("DATA");
    TAddressSegment DestData;

    Console.Print("( Reserve CopyMemTo Release )");
    Console.Indent();

    /*
      Reserve(): Allocate memory and zero data
    */
    if (!Reserve(&DestData, SourceData.Size))
    {
      Console.Print("No memory for temporary data.");
      return;
    }

    PrintSegmentDetails("Reserve", DestData);

    /*
      CopyMemTo(): Copy data to another segment
    */
    CopyMemTo(DestData, SourceData);

    PrintSegmentDetails("CopyMemTo", DestData);

    /*
      Release(): Deallocate memory and zero data
    */
    Release(&DestData);

    PrintSegmentDetails("Release", DestData);

    Console.Unindent();
  }
}

/*
  2024-05 # # # #
  2024-06 #
  2024-10 # # # #
  2024-12-12
  2024-12-15
*/

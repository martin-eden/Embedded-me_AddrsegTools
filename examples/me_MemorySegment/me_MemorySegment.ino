// [me_MemorySegment] test/demo

/*
  Author: Martin Eden
  Last mod.: 2024-12-20
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
#include <me_Uart.h>
#include <me_Console.h>

void setup()
{
  me_Uart::Init(me_Uart::Speed_115k_Bps);

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
  me_MemorySegment::TMemorySegment Memseg
)
{
  Console.Write("Contents ( ");
  Console.Write(Memseg);
  Console.Write(" )");

  Console.EndLine();
}

void PrintWrappings(
  me_MemorySegment::TMemorySegment Memseg
)
{
  Console.Write("Addr (");
  Console.Print(Memseg.Addr);
  Console.Write(")");

  Console.Write(" ");

  Console.Write("Size (");
  Console.Print(Memseg.Size);
  Console.Write(")");

  Console.EndLine();
}

/*
  Print contents using byte iterator
*/
void PrintByteContents(
  me_MemorySegment::TMemorySegment Memseg
)
{
  me_MemorySegment::TSegmentIterator Rator;
  TAddress Addr;
  TUnit Unit;

  if (!Rator.Init(Memseg))
  {
    Console.Print("Failed to setup iterator.");
    return;
  }

  Console.Write("Byte-Contents (");

  while (Rator.GetNext(&Addr))
  {
    Unit = *(TUnit *) Addr;
    Console.Print(Unit);
  }

  Console.Write(")");

  Console.EndLine();
}

void PrintSegmentDetails(
  TAsciiz Header,
  me_MemorySegment::TMemorySegment Memseg
)
{
  Console.Print(Header);

  Console.Indent();

  PrintWrappings(Memseg);
  PrintRawContents(Memseg);
  PrintByteContents(Memseg);

  Console.Unindent();
}

void RunTest()
{
  using
    me_MemorySegment::TMemorySegment;

  TMemorySegment Memseg;

  {
    using
      me_MemorySegment::Freetown::FromAsciiz;

    /*
      FromAsciiz(): Treat ASCIIZ as memory segment

      Note that function return segment not including zero byte.
    */
    Memseg = FromAsciiz("ABC");

    PrintSegmentDetails("FromAsciiz", Memseg);
  }

  {
    using
      me_MemorySegment::Freetown::FromAddrSize;

    /*
      FromAddrSize(): Construct memory segment
    */
    // Stack pointer is at bytes 93 and 94
    Memseg = FromAddrSize(93, 2);

    PrintSegmentDetails("FromAddrSize", Memseg);
  }

  {
    using
      me_MemorySegment::Freetown::FromAsciiz,
      me_MemorySegment::Freetown::Reserve,
      me_MemorySegment::Freetown::CopyMemTo,
      me_MemorySegment::Freetown::Release;

    TMemorySegment SourceData = FromAsciiz("DATA");
    TMemorySegment DestData;

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
    if (!CopyMemTo(DestData, SourceData))
    {
      Console.Print("Failed to copy data.");
      return;
    }

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

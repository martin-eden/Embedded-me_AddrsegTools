// [me_AddrsegTools] test

/*
  Author: Martin Eden
  Last mod.: 2025-09-03
*/

#include <me_AddrsegTools.h>

#include <me_BaseTypes.h>
#include <me_Console.h>
#include <me_WorkMemory.h>

void PrintAddress(
  TAsciiz Annotation,
  TAddress Addr
)
{
  Console.Write(Annotation);
  Console.Print(Addr);
}

void PrintWrappings(
  TAddressSegment AddrSeg
)
{
  Console.Write("( ");
  PrintAddress("Addr", AddrSeg.Addr);
  PrintAddress("Size", AddrSeg.Size);
  Console.Write(")");
  Console.EndLine();
}

void PrintSegmentDetails(
  TAsciiz Annotation,
  TAddressSegment AddrSeg
)
{
  Console.Print(Annotation);

  Console.Indent();

  PrintWrappings(AddrSeg);

  Console.Unindent();
}

void PrintEmptyLine()
{
  Console.Print("");
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
  PrintEmptyLine();
}

void TestIsValid()
{
  TAddressSegment ValidAddrSeg = TAddressSegment_Max;
  TAddressSegment InvalidAddrSeg = { .Addr = 10, .Size = TSize_Max };

  PrintSegmentDetails("Valid segment", ValidAddrSeg);
  PrintSegmentDetails("Invalid segment", InvalidAddrSeg);

  Console.Write("IsValid(): ");

  if (
    me_AddrsegTools::IsValid(ValidAddrSeg) &&
    !me_AddrsegTools::IsValid(InvalidAddrSeg)
  )
    Console.Write("Passed");
  else
    Console.Write("Failed");

  PrintEmptyLine();
}

void TestInvalidate()
{
  TAddressSegment AddrSeg = { .Addr = 100, .Size = 10 };

  PrintSegmentDetails("Valid segment", AddrSeg);

  me_AddrsegTools::Invalidate(&AddrSeg);

  PrintSegmentDetails("Invalid segment", AddrSeg);

  Console.Write("Invalidate(): ");

  if (me_AddrsegTools::IsValid(AddrSeg))
    Console.Write("Failed");
  else
    Console.Write("Passed");

  PrintEmptyLine();
}

void TestIsInside()
{
  TAddressSegment OuterSeg = { .Addr = 0, .Size = 10 };
  TAddressSegment InnerSeg = { .Addr = 2, .Size = 8 };

  PrintSegmentDetails("Inner segment", InnerSeg);
  PrintSegmentDetails("Outer segment", OuterSeg);

  Console.Write("IsInside(): ");

  if (!me_AddrsegTools::IsInside(InnerSeg, OuterSeg))
    Console.Write("Failed");
  else
    Console.Write("Passed");

  PrintEmptyLine();
}

void TestChops()
{
  TAddressSegment OrigSeg = { .Addr = 101, .Size = 4 };
  TAddressSegment ChoppedSeg;
  TAddress CutAddr;

  PrintSegmentDetails("Original segment", OrigSeg);

  CutAddr = 101;

  Console.Write("Cutting address");
  Console.Print(CutAddr);

  {
    ChoppedSeg = OrigSeg;

    if (!me_AddrsegTools::ChopLeftFrom(&ChoppedSeg, CutAddr))
      Console.Print("ChopLeftFrom() failed");

    PrintSegmentDetails("Left-chopped segment (keep cut)", ChoppedSeg);
  }
  {
    ChoppedSeg = OrigSeg;

    if (!me_AddrsegTools::ChopLeftAt(&ChoppedSeg, CutAddr))
      Console.Print("ChopLeftAt() failed");

    PrintSegmentDetails("Left-chopped segment (exclude cut)", ChoppedSeg);
  }
  {
    ChoppedSeg = OrigSeg;

    if (!me_AddrsegTools::ChopRightFrom(&ChoppedSeg, CutAddr))
      Console.Print("ChopRightFrom() failed");

    PrintSegmentDetails("Right-chopped segment (keep cut)", ChoppedSeg);
  }
  {
    ChoppedSeg = OrigSeg;

    if (!me_AddrsegTools::ChopRightAt(&ChoppedSeg, CutAddr))
      Console.Print("ChopRightAt() failed");

    PrintSegmentDetails("Right-chopped segment (exclude cut)", ChoppedSeg);
  }

  PrintEmptyLine();
}

/*
  Test segment creation from two addresses
*/
void TestFromAddrs()
{
  TAddressSegment AddrSeg;
  TAddress StartAddr;
  TAddress EndAddr;

  StartAddr = 101;
  EndAddr = 102;

  Console.Write("Start address");
  Console.Print(StartAddr);

  Console.Write("End address");
  Console.Print(EndAddr);

  if (!me_AddrsegTools::FillSegFromAddrs(&AddrSeg, StartAddr, EndAddr))
    Console.Print("FillSegFromAddrs() failed");

  PrintSegmentDetails("Segment created by two addresses", AddrSeg);

  PrintEmptyLine();
}

void RunTests()
{
  TestFixedSegment();
  TestIsValid();
  TestInvalidate();
  TestIsInside();
  TestChops();
  TestFromAddrs();
}

void setup()
{
  Console.Init();

  Console.Print("( [me_AddrsegTools] Test");
  Console.Indent();
  RunTests();
  Console.Unindent();
  Console.Print(") Done");
}

void loop()
{
}

/*
  2024 # # # # # # # # # # #
  2025 # # # #
  2025-09-03
*/

// [me_AddrsegTools] test

/*
  Author: Martin Eden
  Last mod.: 2025-08-29
*/

#include <me_AddrsegTools.h>

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

void PrintSegmentDetails(
  TAsciiz Header,
  TAddressSegment AddrSeg
)
{
  Console.Print(Header);

  Console.Indent();

  PrintWrappings(AddrSeg);

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
  Console.Print("");
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
    Console.Write("Ok");
  else
    Console.Write("Fail");

  Console.EndLine();
  Console.Print("");
}

void TestInvalidate()
{
  TAddressSegment AddrSeg = { .Addr = 100, .Size = 10 };

  PrintSegmentDetails("Valid segment", AddrSeg);

  me_AddrsegTools::Invalidate(&AddrSeg);

  PrintSegmentDetails("Invalid segment", AddrSeg);

  Console.Write("Invalidate(): ");

  if (me_AddrsegTools::IsValid(AddrSeg))
    Console.Write("Fail");
  else
    Console.Write("Ok");

  Console.EndLine();
  Console.Print("");
}

void TestIsInside()
{
  TAddressSegment OuterSeg = { .Addr = 0, .Size = 10 };
  TAddressSegment InnerSeg = { .Addr = 2, .Size = 8 };

  PrintSegmentDetails("Inner segment", InnerSeg);
  PrintSegmentDetails("Outer segment", OuterSeg);

  Console.Write("IsInside(): ");

  if (!me_AddrsegTools::IsInside(InnerSeg, OuterSeg))
    Console.Write("Fail");
  else
    Console.Write("Ok");

  Console.EndLine();
  Console.Print("");
}

void RunTests()
{
  TestFixedSegment();
  TestIsValid();
  TestInvalidate();
  TestIsInside();
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
  2024-05 # # # # # # # # # # #
  2025-08-28
  2025-08-29
  2025-08-30
*/

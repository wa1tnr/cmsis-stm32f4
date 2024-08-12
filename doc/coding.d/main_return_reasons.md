### int main() returns an int

#### Sat 10 Aug 23:14:17 UTC 2024

int main() in main.c can indeed exit.

  return:
  - -1 or any negative - used to flag error condx
  - 0 - exit clean (no errors to report)

  why use it?
  - halt processor with code after the call to main();
  - other

  Students ask about halting the processor but 'we' usually tell them 'no'.

  Essentially, in Arduino IDE C++ framework, the `setup()` function exits the same way `int main()` does (here) except the caller then hands program flow to a loop (which the end user acccesses by constructing a function named `loop()` - which can be empty).

  `void setup()` can (of course) trap program flow, if there is a reason (or a preference?) to avoid ever calling `loop()` (which must be present, called or not).

end.

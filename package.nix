{
  lib,
  stdenv,
  cmake,
  boost,
  help2man,
  taglib_1,
  enableTests ? true
}:
stdenv.mkDerivation {
  pname = "musicmove";
  version = "unstable";

  src = lib.sourceByRegex ./. [
    "^cmake.*"
    "^ext.*"
    "^src.*"
    "^testdata.*"
    "LICENSE"
    "CMakeLists.txt"
  ];

  # Required at compile time.
  nativeBuildInputs = [
    cmake
    help2man
  ];

  # Required at run time.
  buildInputs = [
    boost
    taglib_1
  ];

  # Required for tests.
  checkInputs = [
    boost
  ];

  doCheck = enableTests;
  cmakeFlags = lib.optional (!enableTests) "-DTESTING=OFF";
}

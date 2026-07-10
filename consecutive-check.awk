# Skip irrelevant files.
!/Version automatically set by release.yml/ {
  next;
}

{
  print("Line found:");
  print;
  line_found = 1;
}

# Match major.
MAJOR > 0 && $0 ~ "VERSION "MAJOR-1"[.]"MINOR"[.]"PATCH"[^0-9]" {
  print("Matched major");
  matched = 1;
}

# Match minor.
MINOR > 0 && $0 ~ "VERSION "MAJOR"[.]"MINOR-1"[.]"PATCH"[^0-9]" {
  print("Matched minor");
  matched = 1;
}

# Match patch.
PATCH > 0 && $0 ~ "VERSION "MAJOR"[.]"MINOR"[.]"PATCH-1"[^0-9]" {
  print("Matched minor");
  matched = 1;
}

END {
  if (!line_found) {
    print("ERROR: Line not found in CMakeLists.txt");
    exit(1);
  }
  if (!matched) {
    printf("ERROR: Version %d.%d.%d is not consecutive\n", MAJOR, MINOR, PATCH);
    exit(1);
  }
}

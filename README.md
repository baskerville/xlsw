# Description
Outputs the list of the children window informations for the given window identifiers (use the root window if none are given).

# Synopsis
    xlsw [-h|-r] [WID ...]

# Options
- `-h` — Print the synopsis to standard output and exit.
- `-r` — Recurse into subwindows.

# Format
Each line holds the following informations:
    WID  FLAGS  CLASS/INSTANCE  TITLE

# Flags
- `o` — Override redirect.
- `u` — Unmapped/unviewable.
- `i` — Input only.

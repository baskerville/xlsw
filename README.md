# Description
Outputs the list of the children window informations for the given window identifiers (use the root window if none are given).

# Synopsis
    xlsw [-h|-v|-r] [WID ...]

# Options
- `-h` — Print the synopsis to standard output and exit.
- `-v` — Print the version to standard output and exit.
- `-r` — Recurse into subwindows.

# Format
Each line holds the following informations:

    WID  FLAGS  CLASS/INSTANCE  TITLE

# Flags
- `u` — Unmapped/unviewable.
- `i` — Input only.
- `o` — Override redirect.

# Example Output

    0x0400335  ---  URxvt/urxvt  terminal ~/bin
      0x040033A  ---  NA           NA
        0x060013F  -io  NA           NA
    0x0600004  u--  NA           NA
    0x1200001  u--  NA           xcompmgr
    0x0E00005  ---  dzen/dzen2   dzen title
    0x0C00001  ui-  NA           NA

adboard
===

ncurses-based command line tool to send input events to android tv
devices using adb.

Supports the following inputs:

  - UP/DOWN/LEFT/RIGHT to send dPad keys
  - ENTER/0x0A to send the enter key
  - ESC to send the back button
  - HOME to send the home key
  - BACKSPACE to send backspace
  - "t" to send arbitrary ASCII text

Tested on a Nexus Player running Android O with root. Seems to work
OK for miscellaneous experiments without reaching for the remote. No
plans to extend very far as the input command is quite slow due to
spinning up a new JVM every time.

A better way to do this for fast I/O is the more heavyweight option
some existing tools have of registering a USB IME and sending everything
directly over that. However, that needs setup on the device and alters
the filesystem, whereas this is completely host-side, both of which are
useful traits in a lot of development cases.

# ImageSort
GUI application that sorts images by date using EXIF data.

### Features
- Sorts images into subdirectories based on date taken
- Options for month, day, minute, hour, and second.
- Can copy or move files
- Optionally sorts non-EXIF file formats by date modified into a separate folder
- Written in C++ with Qt5
- 64bit
- There is also a (slower) C# version with a WPF GUI: [ImageSort-CSharp](https://github.com/benkostr/ImageSort-CSharp).

### Dependencies
- Qt5
- C++14 for the ``<filesystem>`` header

### Building
#### Windows
<ol>
<li>Download and install <a href="https://www.qt.io/download-open-source/">Qt5</a>.
<li>Setup Qt (QtCreator) build kit to your specifications.
<li>Open .pro files with QtCreator.
<li>Compile Sorter (shared library).
<li>Compile ImageSort (executable).
<li>Copy <I>Qt5Core.dll</I>, <I>Qt5Gui.dll</I>, <I>Qt5Widgets.dll</I>, and <I>platforms/qwindows.dll</I> to binary output location.
  <ul>
  <li>These files are found within your Qt binary folder.
  <li>There are also debug versions of these files.
  </ul>
<li> Run ImageSort.exe
</ol>

### To Do
- Comment code
- Port to Linux
- Optimize
  - Minimize type conversions
  - Minimize dynamic arrays
  - Do profiling with many files

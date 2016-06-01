ImageSort
Sorts image files into subirectories based on date taken. This requires EXIF
data to be within the image file, which is usually found within jpeg's. The
user can optionally choose to sort non-EXIF file formats by date modified, and
into a separate folder.

License terms are outlined in the LICENSE file.

##### Inputs: #####

Image Source Directory:
    Specify the directory that contains the image files you wish to be sorted.

Target Directory:
    Specify the directory you wish to sort your image files into.
    Subdirectories will be created here based on dates (month, day, hour, etc.).

Sort non-EXIF file formats:
    Choose to sort files without EXIF information by date modified.

Sort into separate folder:
    Sort files without EXIF information into a separate subdirectory, titled
    "Non-EXIF".

Sort Files by...:
    Choose to sort files by year, month, day, hour, minute, and second. Year is
    not an option. If day is selected but not month, then directories will be
    created based off of the day of the year.

Move or Copy Files:
    Choose to move files or copy files from the source directory into the
    sorted target directory.
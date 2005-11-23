#!/bin/sh -e

for ext in png PNG jpg JPG jpeg JPEG ; do
  for f in `find -name "*.$ext"` ; do
    dest=`basename $f .$ext`.eps
    echo "$f -> $dest"
    convert $f $dest
  done
done
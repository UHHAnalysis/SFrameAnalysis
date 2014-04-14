#!/bin/sh

export dir=$1
export name=$2

echo ' hi starting to fill xml'

for i in $( lcg-ls $1 | grep $2 );
do
  echo '<In FileName="dcap://dcache-cms-dcap.desy.de/'$1'/'$i'" Lumi="0.0"/>' >> $2.xml
done

echo 'xml should be ready'
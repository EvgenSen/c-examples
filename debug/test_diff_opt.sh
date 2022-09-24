#!/bin/bash

apps="array_sort_g1
      array_sort_g1_strip
      array_sort_g2
      array_sort_g2_strip
      array_sort_g3
      array_sort_g3_strip
      array_sort_no_flags
      array_sort_no_flags_strip
      array_sort_O0
      array_sort_O0_strip
      array_sort_O1
      array_sort_O1_strip
      array_sort_O2
      array_sort_O2_strip
      array_sort_O3
      array_sort_O3_strip
      array_sort_Ofast
      array_sort_Ofast_strip
      array_sort_Os
      array_sort_Os_strip"

echo "Start test apps with different optimization"
for a in $apps; do
    echo -e "\n\nCheck $a"
    time ./$a
done

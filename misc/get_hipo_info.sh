#!/usr/bin/bash

totev=0

# loop on all the arguments (single file or multiple files, bash wildcards allowed)
# and print something (1 line per file)

for file in "$@" ; do

    # just to have a readable name
    shortname=$(echo "$file" | awk '{c=split($1,a,"/");print a[c]}')

    # get the number of events from the last field of the "events" line in the command output
    events=$(hipo-utils -info "$file" | awk '{if($0~/events/)print $NF}')

    # sum of all the events
    totev=$(echo $totev + $events|bc)

    echo $shortname $events $totev
done

#!/bin/bash
start="$(date +'%s.%N')"
$@
echo "Time: $(date +"%s.%N - ${start}" | bc)" >&2
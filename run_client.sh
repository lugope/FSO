#!/bin/bash

tab="--tab"
cmd="bash -c './cliente';bash"
foo=""

for i in 1 2 ... n; do
	foo+=($tab -e "$cmd")
done

gnome-terminal "${foo[@]}"

exit 0

#!/bin/sh

## variables
pwd="$(dirname $(readlink -f $0))"

echo ----------------------------------------
echo "$pwd"
echo ----------------------------------------

"$pwd/pack.sh" "false"

if [ -f "$pwd/setup.run" ]; then
    rm -f "$pwd/setup.run"
fi


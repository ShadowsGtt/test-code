#!/bin/bash
while read LINE
do
    echo "[one line of file:]$LINE"
done < test.txt

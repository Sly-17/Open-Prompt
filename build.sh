#! /bin/bash

echo $(gcc -o promptgpt ./.src/prompt.c -lcurl)

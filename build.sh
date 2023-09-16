#! /bin/bash

echo $(gcc -o promptgpt ./.src/prompt.c -lcurl)
echo "Build Successful !!  Prompt using the following command : ./promptgpt \"<Your prompt>\"";
echo "Occasional Segmentation Faults may occur, but just try re-running the command in such cases";
echo "Do not overwhelm the server with too many prompts";

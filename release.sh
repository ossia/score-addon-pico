#!/bin/bash
rm -rf release
mkdir -p release

cp -rf Pico *.{hpp,cpp,txt,json} LICENSE release/

mv release score-addon-pico
7z a score-addon-pico.zip score-addon-pico

#!/bin/bash
g++ -o build/file_crawler file_crawler.cpp -lncurses -lnoise -lssl -lcrypto -lSDL2 -lSDL2_mixer -Wno-deprecated-declarations -g

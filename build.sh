#!/bin/bash
g++ -o build/file_crawler file_crawler.cpp -lncurses -lnoise -lssl -lcrypto -Wno-deprecated-declarations

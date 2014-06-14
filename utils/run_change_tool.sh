#!/bin/bash

# Just run the install tool to fix the library paths
install_name_tool -change libpsmoveapi.3.dylib lib/libpsmoveapi.3.dylib $1
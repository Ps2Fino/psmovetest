#!/bin/bash

# Just run the install tool to fix the library paths
install_name_tool -change libpsmoveapi.3.dylib /Users/danielfinnegan/Documents/UBISS-Summer-School-2014/psmoveapi_3.0.0_osx108/opt/psmoveapi_3.0.0/lib/libpsmoveapi.3.dylib test
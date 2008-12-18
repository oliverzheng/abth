CMAKE_MINIMUM_REQUIRED (VERSION 2.6)

ADD_EXECUTABLE (msnpabth
    main.cpp
    msnpabth.hpp
	msnpabth.cpp
)

TARGET_LINK_LIBRARIES (msnpabth msnp packetwrapper)

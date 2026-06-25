#pragma once

#ifdef __clang__
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wempty-body"
        #include "stb_image.h"
    #pragma clang diagnostic pop
#else
    #include "stb_image.h"
#endif
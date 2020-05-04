//==============================================================================
// Smart Eye Embedded Tracking SDK
// Copyright (c) 2017-2019 Smart Eye AB. All rights reserved.
//
// This software is provided as a toolkit for developing head and eye tracking
// applications. The software is provided "as is" without warranty of any kind.
// Unauthorized copying of this file, via any medium is strictly prohibited.
//==============================================================================

//==============================================================================
// error.h
//==============================================================================
#ifndef ERROR_H
#define ERROR_H

#ifdef __cplusplus
extern "C" {
#endif

#define ensure(statement, ...) \
  do                           \
  {                            \
    if (!(statement))          \
    {                          \
      fatal(__VA_ARGS__);      \
    }                          \
  } while (0)

void fatal(const char* format, ...);

#ifdef __cplusplus
}
#endif

#endif  // ERROR_H

//==============================================================================
// Smart Eye Embedded Tracking SDK
// Copyright (c) 2017-2019 Smart Eye AB. All rights reserved.
//
// This software is provided as a toolkit for developing head and eye tracking
// applications. The software is provided "as is" without warranty of any kind.
// Unauthorized copying of this file, via any medium is strictly prohibited.
//==============================================================================

//==============================================================================
// error.cpp
//==============================================================================

#include "error.h"

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_MESSAGE_SIZE 1000

#ifdef _MSC_VER
#pragma warning(disable : 4068)
#endif

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-nonliteral"
#endif

void fatal(const char* format, ...)
{
  static const char header[] = "ERROR: ";
  char errorMessage[MAX_MESSAGE_SIZE];
  va_list argptr;
  va_start(argptr, format);
  sprintf(errorMessage, "%s", header);
  vsnprintf(errorMessage + strlen(header), MAX_MESSAGE_SIZE - strlen(header), format, argptr);
  va_end(argptr);
  if (errno != 0)
  {
    perror(errorMessage);
  }
  else
  {
    fprintf(stderr, "%s\n", errorMessage);
  }
  exit(EXIT_FAILURE);
}

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

/* ----------------------------------------------------------------------------
  Copyright (c) 2021, Microsoft Research, Daan Leijen
  This is free software; you can redistribute it and/or modify it
  under the terms of the MIT License. A copy of the license can be
  found in the "LICENSE" file at the root of this distribution.
-----------------------------------------------------------------------------*/
#include "internal/gstack.h"

//-----------------------------------------------------------------------
// Prompt chain
//-----------------------------------------------------------------------

// get the current gstack; used for on-demand-paging in gstack_mmap/gstack_win
mp_gstack_t* mp_gstack_current(void) {
  // mp_prompt_t* top = mp_prompt_top();
  // return (top != NULL ? top->gstack : NULL);
  return NULL;
}

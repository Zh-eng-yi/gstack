/* ----------------------------------------------------------------------------
  Copyright (c) 2021, Microsoft Research, Daan Leijen
  This is free software; you can redistribute it and/or modify it
  under the terms of the MIT License. A copy of the license can be
  found in the "LICENSE" file at the root of this distribution.
-----------------------------------------------------------------------------*/
#pragma once
#ifndef MP_MPROMPT_H
#define MP_MPROMPT_H 

//------------------------------------------------------
// Compiler specific attributes
//------------------------------------------------------
#if defined(_MSC_VER) || defined(__MINGW32__)
#if !defined(MP_SHARED_LIB)
#define mp_decl_export      
#elif defined(MP_SHARED_LIB_EXPORT)
#define mp_decl_export      __declspec(dllexport)
#else
#define mp_decl_export      __declspec(dllimport)
#endif
#elif defined(__GNUC__) // includes clang and icc      
#define mp_decl_export      __attribute__((visibility("default")))
#else
#define mp_decl_export      
#endif


//---------------------------------------------------------------------------
// Initialization
//---------------------------------------------------------------------------
#include <stddef.h>
#include <stdbool.h>

// Configuration settings
typedef struct mp_config_s {
  bool      gpool_enable;         // enable gpools for in-process reuse of stack memory (besides the thread-local cache)
  bool      stack_grow_fast;      // grow stacks by doubling (to up to 1MiB at a time) instead of per-page
  bool      stack_use_overcommit; // use overcommit on systems that support this (Linux only) -- disables gpools and fast stack growing.
  bool      stack_reset_decommits;// instead of resetting memory in a gpool, use a full decommit in instead.
  ptrdiff_t gpool_max_size;       // maximum virtual size per gpool (256 GiB)
  ptrdiff_t stack_max_size;       // maximum virtual size of a gstack (8 MiB)
  ptrdiff_t stack_exn_guaranteed; // guaranteed extra stack space available during exception unwinding (Windows only) (16 KiB)
  ptrdiff_t stack_initial_commit; // initial commit size of a gstack (OS page size, 4 KiB)
  ptrdiff_t stack_gap_size;       // virtual no-access gap between stacks for security (64 KiB)
  ptrdiff_t stack_cache_count;    // count of gstacks to keep in a thread-local cache (4)  
} mp_config_t;

// Initialize with `config`; use NULL for default settings.
// Call at most once from the main thread before using any other functions. 
// Use as: `mp_config_t config = mp_config_default(); config.<setting> = <N>; mp_init(&config);`.
mp_decl_export mp_config_t mp_config_default(void);  // default configuration for this platform


#endif

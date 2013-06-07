#pragma once

#include "rd_types.h"
#include "rd_trace.h"
#include "rd_string.h"
#include "rd_heapalloc.h"

#include <string>
#include <queue>


#ifndef NAMESPACE_BEGINE
#define NAMESPACE_BEGINE(X) namespace X {
#define NAMESPACE_END };
#endif


#define SAFE_DELETE(p)		{ if (p) delete p; p = NULL; }
#define SAFE_DELETEARRY(p)	{ if (p) delete [] p; p = NULL; }
#define SAFE_FREE(p)		{ if (p) rd_free(p); p = NULL; }
#define SAFE_RELEASE(p)		{ if (p) p->Release(); p = NULL; }


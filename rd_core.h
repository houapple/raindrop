#pragma once

#include "rd_types.h"
#include "rd_trace.h"
#include "rd_string.h"


#include <string>



#ifndef NAMESPACE_BEGINE
#define NAMESPACE_BEGINE(X) namespace X {
#define NAMESPACE_END };
#endif


#define SAFE_DELETE(p)		{ if (p) delete p; p = NULL; }
#define SAFE_DELETEARRY(p)	{ if (p) delete [] p; p = NULL; }
#define SAFE_RELEASE(p)		{ if (p) p->Release(); p = NULL; }


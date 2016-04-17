%module upnp
%include "std_string.i"
%include "std_vector.i"
%{
/* Includes the header in the wrapper code */
#include "../src/include/Omm/Upnp.h"
using namespace Omm;
%}

/* Parse the header file to generate wrappers */
%include "../src/include/Omm/Upnp.h"

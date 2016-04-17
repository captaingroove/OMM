%module upnpav
%include "std_string.i"
%include "std_vector.i"
%ignore time; // ambiguity between time_t and Poco::Time
%{
/* Includes the header in the wrapper code */
#include "../src/include/Omm/UpnpAv.h"
#include "../src/include/Omm/UpnpAvCtlObject.h"
#include "../src/include/Omm/UpnpAvCtlRenderer.h"
#include "../src/include/Omm/UpnpAvCtlServer.h"
#include "../src/include/Omm/UpnpAvObject.h"
#include "../src/include/Omm/UpnpAvRenderer.h"
#include "../src/include/Omm/UpnpAvServer.h"

using namespace Omm;
using namespace Omm::Av;
%}

/* Parse the header file to generate wrappers */
%include "../src/include/Omm/UpnpAv.h"
%include "../src/include/Omm/UpnpAvCtlObject.h"
%include "../src/include/Omm/UpnpAvCtlRenderer.h"
%include "../src/include/Omm/UpnpAvCtlServer.h"
%include "../src/include/Omm/UpnpAvObject.h"
%include "../src/include/Omm/UpnpAvRenderer.h"
%include "../src/include/Omm/UpnpAvServer.h"

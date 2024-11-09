//*****************************************************************************
//
//      Class:          manual_reset_happening
//      Author:         Kenny Kerr
//      Date created:   10 April 2004
//      Description:    Notifies one or more waiting threads that an event has
//                      occurred.
//
//*****************************************************************************
#include "framework.h"


manual_reset_happening::manual_reset_happening(char * sz, bool bInitiallyOwn) :
   ::event(sz, bInitiallyOwn, true)
{

}




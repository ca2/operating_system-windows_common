//*****************************************************************************
//
//      Class:          manual_reset_happening
//      Author:         Kenny Kerr
//      Date created:   10 April 2004
//      Description:    Notifies one or more waiting threads that an happening has
//                      occurred.
//
//*****************************************************************************
#include "platform.h"


manual_reset_happening::manual_reset_happening(::i8 * sz, bool bInitiallyOwn) :
   ::happening(sz, bInitiallyOwn, true)
{

}




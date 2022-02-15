#!/bin/sh
# 	$Log: ScriptLinuxNett.sh,v $
# 	Revision 1.1.1.1  2006/05/03 11:35:11  ht
# 	SynoTerm - Environnement d'acquisition de relations sémantiques entre termes
# 	
# 	Revision 1.3  2001/05/20 14:55:28  thierry
# 	*** empty log message ***
#
# 	Revision 1.2  2001/05/01 19:03:41  thierry
# 	*** empty log message ***
#	

chmod 644 VarSem.h
sed -e "s/\/\*#include <sys\/ddi\.h>/#include <sys\/ddi\.h>/" VarSem.h > VarSem.h.old
sed -e "s/#include <sys\/filio\.h>\*\//#include <sys\/filio\.h>/" VarSem.h.old > VarSem.h
chmod 444 VarSem.h


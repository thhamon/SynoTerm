
#!/bin/sh

sed -e "s/#include <sys\/ddi\.h>/\/\*#include <sys\/ddi\.h>/" VarSem.h > VarSem.h.old
sed -e "s/#include <sys\/filio\.h>/#include <sys\/filio\.h>\*\//" VarSem.h.old > VarSem.h


#!/usr/local/bin/perl
# 	$Log: ITUnet.sh,v $
# 	Revision 1.1.1.1  2006/05/03 11:35:11  ht
# 	SynoTerm - Environnement d'acquisition de relations sémantiques entre termes
# 	
# 	Revision 1.3  2001/05/20 14:54:56  thierry
# 	*** empty log message ***
#
# 	Revision 1.2  2001/05/01 19:04:42  thierry
# 	*** empty log message ***
#	

while($ligne = <STDIN>) {
    if ($ligne =~ /^#[^#]+# GNM/) {
    	$ligne =~ s/# GNM/\n# GNM/g;
    }
    print $ligne;
}

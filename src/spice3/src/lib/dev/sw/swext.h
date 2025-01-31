/**********
Copyright 1990 Regents of the University of California.  All rights reserved.
Author: 1985 Gordon M. Jacobs
**********/

extern int SWacLoad(GENmodel*,CKTcircuit*);
extern int SWask(CKTcircuit*,GENinstance*,int,IFvalue*,IFvalue*);
extern int SWdelete(GENmodel*,IFuid,GENinstance**);
extern void SWdestroy(GENmodel**);
extern int SWload(GENmodel*,CKTcircuit*);
extern int SWmAsk(CKTcircuit*,GENmodel*,int,IFvalue*);
extern int SWmDelete(GENmodel**,IFuid,GENmodel*);
extern int SWmParam(int,IFvalue*,GENmodel*);
extern int SWparam(int,IFvalue*,GENinstance*,IFvalue*);
extern int SWpzLoad(GENmodel*,CKTcircuit*,SPcomplex*);
extern int SWsetup(SMPmatrix*,GENmodel*,CKTcircuit*,int*);
extern int SWnoise(int,int,GENmodel*,CKTcircuit*,Ndata*,double*);


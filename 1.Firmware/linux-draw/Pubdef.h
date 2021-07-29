#ifndef NULL
#define NULL 0
#endif

#ifndef null
#define null 0
#endif

#ifndef ROUND4
#define ROUND4(x) (((x) + 3) &~(3))
#endif

#ifndef MAX
#define MAX(a,b) (a>b?a:b)
#endif

#ifndef MAX3
#define MAX3(a,b,c) MAX(MAX(a,b),c)
#endif

#ifndef MIN
#define MIN(a,b) (a<b?a:b)
#endif

#ifndef MIN3
#define MIN3(a,b,c) MIN(MIN(a,b),c)
#endif

#ifndef ExchangeVal
#define ExchangeVal(a,b) a^=b,b^=a,a^=b
#endif

#ifndef SAFE_DELETES
#define SAFE_DELETES(p) if(p){delete []p; p=NULL;}
#endif

#include <iostream>
using namespace std;

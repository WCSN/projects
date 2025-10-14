#ifndef _NFPARSER_H_
#define _NFPARSER_H_

#include <string>
#include <fstream>

// Mobitech inc.
#include <mt/common/XmlConfig.h>
#include <mt/utils/System.h>
#include <mt/logger/LoggerFactory.h>
#include <mt/logger/Logable.h>
#include <mt/tools/ReloadableConfig.h>
#include <mt/tools/Application.h>


#include "wcsocket.h"
#include "netflowprc.h"
#include "conf.h"
#include "filter.h"

#define MAX_COUNT 128
#define DAEMON_FAILED -101

extern char *ver;
extern char tmplineff[MAX_COUNT][512];
extern int Nmax;

class NfparserApp: public mt::Logable
{
public:
        WC_Socket *ws;

        void Init(void);
        void Close(void);
        int Run(void);
        NfparserApp(void);
        ~NfparserApp();
private:
        int DataProcess(void);
        void Daemonized(void);
};

extern NetflowPr *nfpcsr;

#endif  /* _NFPARSER_H_*/

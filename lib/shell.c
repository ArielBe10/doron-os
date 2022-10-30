#include "shell.h"

#define prompt ">>> "

#include "screen.h"
#include "terminal.h"
#include "logging.h"
#include "string.h"

static char osname[] =
    "    ////      ////     /////     ////     ///   // \n"
    "   //  //   //   //   //   //  //   //   ////  //  \n"
    "  //   //  //    //  //   //  //    //  // /////   \n"
    " //   //   //   //  //////    //   //  //   ///    \n"
    "/////       ///    //  ///     ///    //    //     \n";

static char logo[] =
    "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@n;;;\"\"^^^\"\"\"\";;;::-6@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
    "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@t:;\"\";;;:-----------(@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
    "@@@@@@@@@@@@@@@@@@@@@@@@@@B;-~::::::-----'--------:`_F@@@@@@@@@@@@@@@@@@@@@@@@@@"
    "@@@@@@@@@@@@@@@@@@@@@@@%`_`'\":::::::::---------..,.,`_`@@@@@@@@@@@@@@@@@@@@@@@@@"
    "@@@@@@@@@@@@@@@@@@@@@@:`,-:::-:::::::-::-----,..'```,`~@@@@@@@@@@@@@@@@@@@@@@@@@"
    "@@@@@@@@@@@@@@@@@@?,@?-::::--:::;:::------'.,,````,,-(f@@@@@@@@@@@@@@@@@@@@@@@@@"
    "@@@@@@@@@@@@@@@@@@@~?$::;;;\"\";;::;;:::--.',,,'.'--:!yT}:LD@@@@@@@@@@@@@@@@@@@@@@"
    "@@@@@@@@@&&0D#$@@@@>:R&5;:;\"\";\"\"\";;;;;::::::::!L;-:.:T5~T@@@@@@@@@@@@@@@@@@@&0Dm"
    "@@0$w5uFlTLLLLFwN@@#@@@@@@s5Fc!~\"\"\"\";;:---;!([?L<`,\"nSDwT0@@@@@@@@@@@@@@@@@@@@@@"
    "uL[?5$@@@@@@@@@@@@@@@S__________________________(5(L55$&no00%$$$0M0@@@@@@@@@@@@@"
    "@@@@@@@@@@@@@@@@@@@@@@@__________T@F____________~3Y[z@@q?c0@@@@@@@@@@@@BHAmD%&@@"
    "0&@@@@@@@@@@@@@@@@@@@@@@:________R@@m\"__________[gpDB>z&@@@@@@@@@@@@@@@@@@@@@@@@"
    "wPA3B@@@@@@@@@@@@@@@@@@@@@@+-,`>@@@@0@PHo~-,`'>g%%$&@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
    "pgU9F%Ss$@@@@@@@@@@@@@@@@@@@@@@@@@&3?n8@@@@@0$$OOUb@@@@@@@@@@@@@@@@@@@@@@@@@@@DN"
    "9dqgqgBuSgqn$wD%@@@@@@@@@@@@@@@@@A8@@DU3sd0@@#m6OB9@@@@@@@@@@@@@@@@@@@@@@@@@$T$M"
    "5f0H3k&mZ6P6O#mDdA8DB@@@@@@@@@@@&#&$Ak5upZqq5Z6Pd50$R@@@@@@@@@@@@@@@@DD8%MNBP$0D"
    "Lq@#ku&mf9USmwbD@0RM$O@9k&#$@5N@@@@@@@@@&DqLTlz?yk@$F%9@@@@@@@@HbHNOd$$%MBBB6DAD"
    "@@@OT+$DuuRodoFd&#ZO&Od[5N$[&@9#k$B&dqgFl^LT[F5TPoTlPFog5?+6mFgB&&#ZdD$$%B$$Z$oR"
    "@@@@S!m@yY~Lu;m$@&6T}B6!R@B}q@#:A@@@BF+@B8$?n@#UYdHHdu9P5TA$NwkBM06}o$&B$DB9o8tg"
    "@@@@9z@@ST<AF~90&@d(Z@&[q$S!9Hd<TZ@&M?o@&&@86!T$&@8wl?#[T!g9gs?R@@R[c9@&$6$ZwOO@"
    "@@@@@$@@$YO@qF5NM0&@0N8kH@@TndN&&obNO@@&BgB&NHdN&$OdR65$#DZ6$MN&&0N&+lR&&q6s6$@@";

void init_shell(void)
{
    init_terminal();

    println(osname);
    INFO("init shell");

    char input_str[0x10000];
    while (1)
    {
        print(prompt);
        input(input_str, SINGLE_LINE);

        print(input_str);
    }
}
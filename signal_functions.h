#define _BSD_SOURCE
#include <signal.h>
#define _GUN_SOURCE
extern const char *const sys_siglist[];

void psignal(int sig, const char *msg);
int sigemptstyset(sigset_t *set);

int sigandset(sigset_t *set, sigset_t *left, sigset_t *right);
int sigorset(sigset_t *set, sigset_t *left, sigset_t *right);

int sigemptstyset(sigset_t *set);

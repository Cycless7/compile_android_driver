#ifndef PARADISE_SAFE_SIGNAL_H
#define PARADISE_SAFE_SIGNAL_H

#include "paradise_common.h"

int paradise_safe_signal_init(void);
void paradise_safe_signal_cleanup(void);

int paradise_add_unsafe_region(pid_t session, uid_t uid, uintptr_t start, size_t num_page);
int paradise_del_unsafe_region(pid_t pid);

#endif // PARADISE_SAFE_SIGNAL_H

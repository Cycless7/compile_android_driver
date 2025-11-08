#ifndef PARADISE_BINDPROC_H
#define PARADISE_BINDPROC_H

#include <linux/socket.h>

int do_bind_proc(struct socket* sock, void __user* arg);

#endif // PARADISE_BINDPROC_H
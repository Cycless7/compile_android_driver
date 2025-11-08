#include "paradise_sock.h"
#include <asm/pgalloc.h>
#include <asm/pgtable-hwdef.h>
#include "paradise_ioctl.h"
#include "paradise_protocol.h"
#include "paradise_utils.h"

#include "paradise_safe_signal.h"

static int paradise_release(struct socket* sock) {
    paradise_info("release paradise sock\n");

    struct sock* sk = sock->sk;
    if (!sk) {
        return 0;
    }

    struct paradise_sock* ws = (struct paradise_sock*)sk;
    ws->version = 0;

    if (ws->session) {
        paradise_del_unsafe_region(ws->session);
        ws->session = 0;
    }

    if (ws->used_pages) {
        for (int i = 0; i < ws->used_pages->size; ++i) {
            struct page* page = (typeof(page))arraylist_get(ws->used_pages, i);
            if (page) {
                __free_page(page);
            }
        }
        paradise_info("free %lu used pages\n", ws->used_pages->size);
        arraylist_destroy(ws->used_pages);
    }

    sock_orphan(sk);
    sock_put(sk);
    return 0;
}

static int paradise_ioctl(struct socket* sock, unsigned int cmd, unsigned long arg) {
    void __user* argp = (void __user*)arg;

    int i;
    for (i = 0; i < ARRAY_SIZE(ioctl_handlers); i++) {
        if (cmd == ioctl_handlers[i].cmd) {
            if (ioctl_handlers[i].handler == NULL) {
                continue;
            }
            return ioctl_handlers[i].handler(sock, argp);
        }
    }

    paradise_warn("unsupported ioctl command: %u\n", cmd);
    return -ENOTTY;
}

static __poll_t paradise_poll(struct file* file, struct socket* sock, struct poll_table_struct* wait) { return 0; }

static int paradise_setsockopt(struct socket* sock, int level, int optname, sockptr_t optval, unsigned int optlen) {
#if defined(BUILD_HIDE_SIGNAL)
    if (optname == SOCK_OPT_SET_MODULE_VISIBLE) {
        if (optval.user != NULL) {
            show_module();
        } else {
            hide_module();
        }
        return 0;
    }
#endif

    return -ENOPROTOOPT;
}

static int paradise_getsockopt(struct socket* sock, int level, int optname, char __user* optval, int __user* optlen) {
    return 0;
}

static int paradise_bind(struct socket* sock, struct sockaddr* saddr, int len) { return -EOPNOTSUPP; }

static int paradise_connect(struct socket* sock, struct sockaddr* saddr, int len, int flags) { return -EOPNOTSUPP; }

// int (*)(struct socket *, struct sockaddr *, int)' with an expression of type 'int (struct socket *, struct  sockaddr *, int *, int)
#if  defined(MAGIC_PARADISE_GETNAME)
static int paradise_getname(struct socket* sock, struct sockaddr* saddr, int* len, int peer) { return -EOPNOTSUPP; }
#else
static int paradise_getname(struct socket* sock, struct sockaddr* saddr, int peer) { return -EOPNOTSUPP; }
#endif

static int paradise_recvmsg(struct socket* sock, struct msghdr* m, size_t len, int flags) { return -EOPNOTSUPP; }

static int paradise_sendmsg(struct socket* sock, struct msghdr* m, size_t len) { return -EOPNOTSUPP; }

static int paradise_socketpair(struct socket *sock1, struct socket *sock2)
{
	return -EOPNOTSUPP;
}

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(6, 12, 0))
static int paradise_accept(struct socket *sock, struct socket *newsock,
		   struct proto_accept_arg *arg)
{
	return -EOPNOTSUPP;
}
#else
static int paradise_accept(struct socket *sock, struct socket *newsock, int flags,
		   bool kern)
{
	return -EOPNOTSUPP;
}
#endif

static int paradise_listen(struct socket *sock, int backlog)
{
	return -EOPNOTSUPP;
}

static int paradise_shutdown(struct socket *sock, int how)
{
	return -EOPNOTSUPP;
}

static int paradise_mmap(struct file *file, struct socket *sock, struct vm_area_struct *vma)
{
	/* Mirror missing mmap method error code */
	return -ENODEV;
}

struct proto_ops paradise_proto_ops = {
    .family = PF_DECnet,
    .owner = THIS_MODULE,
    .release = paradise_release,
    .bind = paradise_bind,
    .connect = paradise_connect,
    .socketpair = paradise_socketpair,
    .accept = paradise_accept,
    .getname = paradise_getname,
    .poll = paradise_poll,
    .ioctl = paradise_ioctl,
    .listen = paradise_listen,
    .shutdown = paradise_shutdown,
    .setsockopt = paradise_setsockopt,
    .getsockopt = paradise_getsockopt,
    .sendmsg = paradise_sendmsg,
    .recvmsg = paradise_recvmsg,
    .mmap = paradise_mmap,
};

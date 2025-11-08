#include "paradise_protocol.h"

#include <asm-generic/errno.h>

#include "paradise_common.h"

#include <net/sock.h>

#include "paradise_ioctl.h"
#include "paradise_sock.h"

static int free_family = AF_DECnet;

struct proto paradise_proto = {
    .name = "NFC_LLCP",
    .owner = THIS_MODULE,
    .obj_size = sizeof(struct paradise_sock),
};

static int register_free_family(void) {
    int err = 0, i = 0;

    for (i = 0; i < ARRAY_SIZE(ioctl_handlers); i++) {
        paradise_info("registered ioctl command: %u\n", ioctl_handlers[i].cmd);
    }

    for (int family = free_family; family < NPROTO; family++) {
        paradise_family_ops.family = family;
        err = sock_register(&paradise_family_ops);
        if (err)
            continue;
        free_family = family;
        paradise_proto_ops.family = free_family;
        paradise_info("find free proto_family: %d\n", paradise_proto_ops.family);
        return 0;
    }

    paradise_err("can't find any free proto_family!\n");
    return err;
}

int paradise_proto_init(void) {
    int err = proto_register(&paradise_proto, 1);
    if (err)
        goto out;

    err = register_free_family();
    if (err)
        goto out_proto;

    return 0;

    sock_unregister(free_family);
out_proto:
    proto_unregister(&paradise_proto);
out:
    return err;
}

void paradise_proto_cleanup(void) {
    sock_unregister(free_family);
    proto_unregister(&paradise_proto);
}

static int paradise_sock_create(struct net* net, struct socket* sock, int protocol, int kern) {
    if (!capable(CAP_NET_BIND_SERVICE)) {
        return -EACCES;
    }

    uid_t caller_uid = *(uid_t*)&current_cred()->uid;
    if (caller_uid != 0) {
        paradise_warn("only root can create paradise socket!\n");
        return -EAFNOSUPPORT;
    }

    if (sock->type != SOCK_RAW) {
        paradise_warn("socket must be SOCK_RAW!\n");
        return -ENOKEY;
    }

    sock->state = SS_UNCONNECTED;
    struct sock* sk = sk_alloc(net, PF_INET, GFP_KERNEL, &paradise_proto, kern);
    if (!sk) {
        paradise_warn("sk_alloc failed!\n");
        return -ENOBUFS;
    }

    paradise_family_ops.family = free_family;
    sock->ops = &paradise_proto_ops;
    sock_init_data(sock, sk);

    struct paradise_sock* ws = (struct paradise_sock*)sk;
    ws->version = 1;
    ws->session = current->pid;
    ws->used_pages = arraylist_create(4);

    return 0;
}

struct net_proto_family paradise_family_ops = {
    .family = PF_DECnet,
    .create = paradise_sock_create,
    .owner = THIS_MODULE,
};

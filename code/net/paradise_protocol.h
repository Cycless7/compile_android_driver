#ifndef PARADISE_PROTOCOL_H
#define PARADISE_PROTOCOL_H

#include "paradise_common.h"

int paradise_proto_init(void);
void paradise_proto_cleanup(void);

extern struct proto paradise_proto;
extern struct net_proto_family paradise_family_ops;

#endif /* PARADISE_PROTOCOL_H */

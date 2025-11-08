#include <asm/tlbflush.h>
#include <asm/unistd.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/init_task.h>
#include <linux/kernel.h>
#include <linux/kprobes.h>
#include <linux/list.h>
#include <linux/module.h>
#include <linux/syscalls.h>
#include <linux/types.h>
#include "paradise_common.h"
#include "paradise_kallsyms.h"
#include "paradise_protocol.h"
#include "paradise_safe_signal.h"
#include "paradise_sock.h"
#include "paradise_utils.h"
#include "hijack_arm64.h"

static int __init paradise_init(void) {
    int ret;
    paradise_info("helo!\n");

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(6, 1, 0))
    ret = disable_kprobe_blacklist();
    if (ret) {
        paradise_err("disable_kprobe_blacklist failed: %d\n", ret);
        return ret;
    }
#endif

    ret = init_arch();
    if (ret) {
        paradise_err("init_arch failed: %d\n", ret);
        return ret;
    }

    ret = paradise_proto_init();
    if (ret) {
        paradise_err("paradise_socket_init failed: %d\n", ret);
        goto out;
    }

#if defined(BUILD_HIDE_SIGNAL)
    ret = paradise_safe_signal_init();
    if (ret) {
        paradise_err("paradise_safe_signal_init failed: %d\n", ret);
        goto clean_sig;
    }
#endif


#if defined(HIDE_SELF_MODULE)
    hide_module();
#endif

#if defined(BUILD_NO_CFI)
    paradise_info("NO_CFI is enabled, patched: %d\n", cfi_bypass());
#endif

    return 0;

#if defined(BUILD_HIDE_SIGNAL)
clean_sig:
    paradise_proto_cleanup();
#endif


out:
    return ret;
}

static void __exit paradise_exit(void) {
    paradise_info("bye!\n");
    paradise_proto_cleanup();
#if defined(BUILD_HIDE_SIGNAL)
    paradise_safe_signal_cleanup();
#endif
}

module_init(paradise_init);
module_exit(paradise_exit);

MODULE_LICENSE("GPL");

MODULE_IMPORT_NS(DMA_BUF);

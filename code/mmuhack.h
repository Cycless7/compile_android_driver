#ifndef PARADISE_MMUHACK_H
#define PARADISE_MMUHACK_H

#include <asm/pgtable.h>

pte_t *page_from_virt_kernel(unsigned long addr);

pte_t *page_from_virt_user(struct mm_struct * mm, unsigned long addr);

int protect_rodata_memory(unsigned nr);

int unprotect_rodata_memory(unsigned nr);

#endif //PARADISE_MMUHACK_H

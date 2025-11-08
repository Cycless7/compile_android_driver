obj-m := android-paradise.o

android-paradise-y := \
    src/core/paradise.o \
    src/net/paradise_sock.o \
    src/net/paradise_protocol.o \
    src/utils/paradise_utils.o \
    src/ioctl/paradise_ioctl.o \
    src/mm/paradise_page_walk.o \
    src/mm/paradise_proc_dmabuf.o \
    src/hook/paradise_safe_signal.o \
    src/proc/paradise_proc.o \
    src/inlinehook/hijack_arm64.o \
    src/utils/karray_list.o \
    src/mm/paradise_bindproc.o

src := $(if $(filter /%,$(src)),$(src),$(srctree)/$(src))

KDIR := $(KDIR)
MDIR := $(realpath $(dir $(abspath $(lastword $(MAKEFILE_LIST)))))

$(info -- KDIR: $(KDIR))
$(info -- MDIR: $(MDIR))
$(info -- PARADISE_SRC_DIR: $(src))
$(info -- PARADISE_OBJ_DIR: $(obj))

ccflags-y += -I$(src)/src/core -I$(src)/src/net -I$(src)/src/ioctl -I$(src)/src/mm
ccflags-y += -I$(src)/src/inlinehook -I$(src)/src/hook -I$(src)/src/proc -I$(src)/src/utils

ccflags-y += -Wno-implicit-function-declaration -Wno-strict-prototypes -Wno-int-conversion -Wno-gcc-compat
ccflags-y += -Wno-declaration-after-statement -Wno-unused-function -Wno-unused-variable

# 编译时启用 隐藏模块功能
ccflags-y += -DHIDE_SELF_MODULE
# 编译时启用 PTE_MAPPING 功能
# ccflags-y += -DBUILD_PTE_MAPPING
# 编译时启用 HIDE_SIGNAL 功能
ccflags-y += -DBUILD_HIDE_SIGNAL
# ccflags-y += -DPTE_WALK
# 编译时启用 NO_CFI 功能
ccflags-y += -DBUILD_NO_CFI

all:
	make -C $(KDIR) M=$(MDIR) modules

clean:
	make -C $(KDIR) M=$(MDIR) clean
    
compdb:
	python3 $(MDIR)/.vscode/generate_compdb.py -O $(KDIR) $(MDIR)

.PHONY: all clean
obj-m := android-paradise.o

android-paradise-y := \
    paradise.o \
    paradise_sock.o \
    paradise_protocol.o \
    paradise_utils.o \
    paradise_ioctl.o \
    paradise_page_walk.o \
    paradise_proc_dmabuf.o \
    paradise_safe_signal.o \
    paradise_proc.o \
    hijack_arm64.o \
    karray_list.o \
    paradise_bindproc.o

src := $(if $(filter /%,$(src)),$(src),$(srctree)/$(src))

KDIR := $(KDIR)
MDIR := $(realpath $(dir $(abspath $(lastword $(MAKEFILE_LIST)))))

$(info -- KDIR: $(KDIR))
$(info -- MDIR: $(MDIR))
$(info -- PARADISE_SRC_DIR: $(src))
$(info -- PARADISE_OBJ_DIR: $(obj))

ccflags-y += -I$(src)

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

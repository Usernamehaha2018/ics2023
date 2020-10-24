#include <proc.h>
#include <elf.h>

#ifdef __LP64__
# define Elf_Ehdr Elf64_Ehdr
# define Elf_Phdr Elf64_Phdr
#else
# define Elf_Ehdr Elf32_Ehdr
# define Elf_Phdr Elf32_Phdr
#endif

size_t ramdisk_read(void *buf, size_t offset, size_t len);

size_t get_ramdisk_size();

static uintptr_t loader(PCB *pcb, const char *filename) {
  void* buf1 =  (void *)0x3000000;
  void* buf2 =  (void *)0x3001000;
  void* buf3 =  (void *)0x3005000;
  void* buf4 =  (void *)0x30062a0;
  Elf_Ehdr ret_buf;
  ramdisk_read(&ret_buf, 0, sizeof(Elf_Ehdr));
  ramdisk_read(buf1, 0, 0xd4);
  ramdisk_read(buf2, 0x1000, 0x3b93);
  ramdisk_read(buf3, 0x5000, 0x28c);
  ramdisk_read(buf4, 0x52a0, 0x918);
  return ret_buf.e_entry;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %u", entry);
  ((void(*)())entry) ();
}


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
  Elf64_Ehdr buf;
  ramdisk_read(&buf, 0, sizeof(Elf_Ehdr));
  printf("%u,\n",buf.e_phnum);
  Elf_Phdr phdr[buf.e_phnum];
  for(int i = 0; i<buf.e_phnum;i++){
    ramdisk_read(&phdr[i], buf.e_phoff+i*sizeof(Elf_Phdr), sizeof(Elf_Phdr));
  }
  for(int i = 0; i<buf.e_phnum;i++){
    printf("%u,\n");
    printf("%u,\n",phdr[i].p_offset);
  }
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


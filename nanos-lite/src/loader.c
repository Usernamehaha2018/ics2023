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
  void* buf =  (void *)0x3000000;
  Elf_Ehdr ret_buf;
  Elf_Phdr p_buf;
  ramdisk_read(&ret_buf, 0, sizeof(Elf_Ehdr));
  ramdisk_read(&p_buf, 0, sizeof(Elf_Phdr));
  printf("%u,\n",p_buf.p_offset);
  printf("%u,\n",p_buf.p_vaddr);
  printf("%u,\n",p_buf.p_paddr);
  printf("%u,\n",p_buf.p_filesz);
  printf("%u,\n",p_buf.p_memsz);
printf("\n\n");
  size_t  len = get_ramdisk_size();
  ramdisk_read(buf, 0, len);
  return ret_buf.e_entry;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %u", entry);
  ((void(*)())entry) ();
}


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
size_t ramdisk_write(const void *buf, size_t offset, size_t len);
size_t get_ramdisk_size();

static uintptr_t loader(PCB *pcb, const char *filename) {  
  Elf_Ehdr ret_buf;
  ramdisk_read(&ret_buf, 0, sizeof(Elf_Ehdr));
  Elf_Phdr phdr[ret_buf.e_phnum];
  for(int i = 0; i<ret_buf.e_phnum;i++){
    ramdisk_read(&phdr[i], ret_buf.e_phoff+i*sizeof(Elf_Phdr), sizeof(Elf_Phdr));
    ramdisk_read((void*)phdr[i].p_vaddr, phdr[i].p_offset, phdr[i].p_memsz);
    printf("%u,%u,%u\n",phdr[i].p_vaddr,phdr[i].p_offset,phdr[i].p_memsz);
  }
  
  return ret_buf.e_entry;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %u", entry);
  ((void(*)())entry) ();
}


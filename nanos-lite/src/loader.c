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
int fs_open(const char *pathname, int flags, int mode);
size_t fs_read(int fd, void *buf, size_t len);
size_t fs_write(int fd, const void *buf, size_t len);
size_t fs_lseek(int fd, size_t offset, int whence);
int fs_close(int fd);

static uintptr_t loader(PCB *pcb, const char *filename) {  
  int fd = fs_open(filename, 0, 0);
  Elf_Ehdr ret_buf;
  fs_read(fd, &ret_buf, sizeof(Elf_Ehdr));
  Elf_Phdr phdr[ret_buf.e_phnum];
  for(int i = 0; i<ret_buf.e_phnum;i++){
    fs_read(fd, &phdr[i], sizeof(Elf_Phdr));
  }
  for(int i = 0; i<ret_buf.e_phnum;i++){
    if (phdr[i].p_type == PT_LOAD){
      fs_lseek(fd, phdr[i].p_offset, 0);
      fs_read(fd, (void* )(phdr[i].p_vaddr), phdr[i].p_memsz);
      if (phdr[i].p_memsz > phdr[i].p_filesz) {
        memset((void* )(phdr[i].p_vaddr + phdr[i].p_filesz), 0, phdr[i].p_memsz - phdr[i].p_filesz);
      }
    }
  }
  
  return ret_buf.e_entry;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %u", entry);
  ((void(*)())entry) ();
}


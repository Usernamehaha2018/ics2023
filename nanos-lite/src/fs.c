#include <fs.h>

typedef size_t (*ReadFn) (void *buf, size_t offset, size_t len);
typedef size_t (*WriteFn) (const void *buf, size_t offset, size_t len);

typedef struct {
  char *name;
  size_t size;
  size_t disk_offset;
  ReadFn read;
  WriteFn write;  
  size_t open_offset;
} Finfo;

enum {FD_STDIN, FD_STDOUT, FD_STDERR, FD_FB, FD_EVENT, FD_DISP};
//enum {SEEK_SET, SEEK_CUR, SEEK_END};

size_t ramdisk_read(void *buf, size_t offset, size_t len);
size_t ramdisk_write(const void *buf, size_t offset, size_t len);
void __am_gpu_config(AM_GPU_CONFIG_T *cfg);


size_t invalid_read(void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

size_t invalid_write(const void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

size_t sys_write_standard(const void *buf, size_t offset, size_t len) {
  for (int i=0; i<len ;i++)
      putch(((char *)buf)[i]);
  return len;
}

size_t events_read(void *buf, size_t offset, size_t len);
size_t dispinfo_read(void *buf, size_t offset, size_t len);
size_t fb_write(const void *buf, size_t offset, size_t len);

/* This is the information about all files in disk. */
static Finfo file_table[] __attribute__((used)) = {
  [FD_STDIN]  = {"stdin", 0, 0, invalid_read, invalid_write},
  [FD_STDOUT] = {"stdout", 0, 0, invalid_read, sys_write_standard},
  [FD_STDERR] = {"stderr", 0, 0, invalid_read, sys_write_standard},
  [FD_FB]     = {"/dev/fb", 0, 0, invalid_read, invalid_write},
  [FD_EVENT]  = {"/dev/events", 0, 0, events_read, invalid_write},
  [FD_DISP]   = {"/proc/dispinfo", 128, 0,  dispinfo_read, invalid_write},
#include "files.h"
};


int fs_open(const char *pathname, int flags, int mode){
  printf("%s\n",pathname);
  for (int i=0; i<sizeof(file_table)/sizeof(file_table)[0]; i++) {
    if (strcmp(file_table[i].name, pathname) == 0){
      file_table[i].open_offset = 0;
      return i;
    }
  }  
  assert(0);
  return -1;
}

int fs_close(int fd, void *buf, size_t len){
 return 0;
}


size_t fs_write(int fd, const void *buf, size_t len){
  switch (fd){
    case FD_STDOUT:
    case FD_STDERR:
    case FD_EVENT:
    case FD_DISP:
    file_table[fd].write(buf, 0, len); break;
    default: 
      if (file_table[fd].open_offset + len > file_table[fd].size)
        len = file_table[fd].size - file_table[fd].open_offset;
      ramdisk_write(buf, file_table[fd].disk_offset+file_table[fd].open_offset, len);
      file_table[fd].open_offset += len;
      break;
  }
 return len;
}

size_t fs_read(int fd, void *buf, size_t len){
  switch (fd)
  {
    case FD_STDIN: 
    case FD_STDOUT: 
    case FD_STDERR: 
    case FD_EVENT:
    case FD_DISP:
    return file_table[fd].read(buf, 0, len);
    default:
      if (file_table[fd].open_offset + len > file_table[fd].size)
        len = file_table[fd].size - file_table[fd].open_offset;
      ramdisk_read(buf, file_table[fd].disk_offset+file_table[fd].open_offset, len);
      file_table[fd].open_offset += len;
      break;
  }
  return len;
}

size_t fs_lseek(int fd, size_t offset, int whence){
  size_t ret_val = -1;
  switch (whence)
  {
    case SEEK_SET: 
    if (offset >= 0 && offset <= file_table[fd].size) {
      file_table[fd].open_offset = offset;
      ret_val = file_table[fd].open_offset;
    }
    break;
    case SEEK_CUR:
    if (offset >= 0 && offset + file_table[fd].open_offset <= file_table[fd].size) {
      file_table[fd].open_offset += offset;
      ret_val = file_table[fd].open_offset;
    }
      break;
    case SEEK_END:
      file_table[fd].open_offset = file_table[fd].size + offset;
      ret_val = file_table[fd].open_offset;
      break;
  default:
    break;
  }
  return ret_val;
}

int fs_stat(int fd){
  return file_table[fd].size;
}

void init_fs() {
  // TODO: initialize the size of /dev/fb
  AM_GPU_CONFIG_T cfg;
  __am_gpu_config(&cfg);
  file_table[FD_FB].size = cfg.width * cfg.height * 4;
}

#include <proc.h>

#define MAX_NR_PROC 4

static PCB pcb[MAX_NR_PROC] __attribute__((used)) = {};
static PCB pcb_boot = {};
PCB *current = NULL;

void naive_uload(PCB *pcb, const char *filename);

void switch_boot_pcb() {
  current = &pcb_boot;
}

void hello_fun(void *arg) {
  int j = 1;
  while (1) {
    Log("Hello World from Nanos-lite with arg '%p' for the %dth time!", (uintptr_t)arg, j);
    j ++;
    yield();
  }
}

// {"/bin/menu", 75380, 28407139},
// {"/bin/event-test", 34328, 28482519},
// {"/bin/nslider", 56180, 28516847},
// {"/bin/dummy", 28840, 28573027},
// {"/bin/pal", 393260, 28601867},
// {"/bin/file-test", 47072, 28995127},
// {"/bin/bmp-test", 39348, 29042199},
// {"/bin/time-test", 34328, 29081547},
// {"/bin/nterm", 81700, 29115875},
// {"/bin/hello", 33596, 29197575},
// {"/bin/bird", 140100, 29231171},

void init_proc() {
  switch_boot_pcb();

  Log("Initializing processes...");

  naive_uload(NULL, "/bin/coremark");

}

Context* schedule(Context *prev) {
  return NULL;
}

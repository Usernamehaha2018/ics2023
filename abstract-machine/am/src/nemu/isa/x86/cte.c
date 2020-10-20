#include <am.h>
#include <x86.h>
#include <klib.h>

#define NR_IRQ         256     // IDT size
#define SEG_KCODE      1
#define SEG_KDATA      2

static Context* (*user_handler)(Event, Context*) = NULL;

void __am_irq0();
void __am_vecsys();
void __am_vectrap();
void __am_vecnull();


Context* __am_irq_handle(Context *c) {
  if (user_handler) {
    Event ev = {0};
// eax     53      83
// ecx     132fd7  1257431
// edx     3f8     1016
// ebx     133f70  1261424
// esp     133f50  1261392
// ebp     133f68  1261416
// esi     4c066120        1275486496
// edi     28ed23cc        686629836
    // printf("%d\n",c->irq);
    // printf("%d\n",c->ess);
    // printf("%d\n",c->eax);
    // printf("%d\n",c->ecx);
    // printf("%d\n",c->edx);
    // printf("%d\n",c->ebx);
    // printf("%d\n",c->esp);
    // printf("%d\n",c->ebp);
    // printf("%d\n",c->esi);
    // printf("%d\n",c->ebp);
    // printf("%d\n",c->eflags);
    // printf("%d\n",c->cs);
    // printf("%d\n",c->eip);

    switch (c->irq) {
      case 0x81: ev.event = EVENT_YIELD; break;
      default: ev.event = EVENT_ERROR; break;
    }
    c = user_handler(ev, c);
    assert(c != NULL);
  }

  return c;
}

bool cte_init(Context*(*handler)(Event, Context*)) {
  static GateDesc32 idt[NR_IRQ];

  // initialize IDT
  for (unsigned int i = 0; i < NR_IRQ; i ++) {
    idt[i]  = GATE32(STS_TG, KSEL(SEG_KCODE), __am_vecnull, DPL_KERN);
  }

  // ----------------------- interrupts ----------------------------
  idt[32]   = GATE32(STS_IG, KSEL(SEG_KCODE), __am_irq0,    DPL_KERN);
  // ---------------------- system call ----------------------------
  idt[0x80] = GATE32(STS_TG, KSEL(SEG_KCODE), __am_vecsys,  DPL_USER);
  idt[0x81] = GATE32(STS_TG, KSEL(SEG_KCODE), __am_vectrap, DPL_KERN);

  set_idt(idt, sizeof(idt));

  // register event handler
  user_handler = handler;

  return true;
}


Context* kcontext(Area kstack, void (*entry)(void *), void *arg) {
  return NULL;
}

void yield() {
  asm volatile("int $0x81");
}

bool ienabled() {
  return false;
}

void iset(bool enable) {
}

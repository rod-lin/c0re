#ifndef _PUB_X86_H_
#define _PUB_X86_H_

#include "pub/com.h"

// TODO: why do we need this?? (required in printfmt)
#define do_div(n, base) ({                                   \
        unsigned long __upper, __low, __high, __mod, __base; \
        __base = (base);                                     \
        asm("" : "=a" (__low), "=d" (__high) : "A" (n));     \
        __upper = __high;                                    \
        if (__high != 0) {                                   \
            __upper = __high % __base;                       \
            __high = __high / __base;                        \
        }                                                    \
        asm("divl %2" : "=a" (__low), "=d" (__mod)           \
            : "rm" (__base), "0" (__low), "1" (__upper));    \
        asm("" : "=A" (n) : "a" (__low), "d" (__high));      \
        __mod;                                               \
    })

C0RE_INLINE uint8_t inb(uint16_t port);
C0RE_INLINE void insl(uint32_t port, void *addr, int cnt);
C0RE_INLINE void outb(uint16_t port, uint8_t data);
C0RE_INLINE void outw(uint16_t port, uint16_t data);
C0RE_INLINE void outsl(uint32_t port, const void *addr, int cnt);
C0RE_INLINE uint32_t read_ebp(void);

/* argument used for LGDT, LLDT(not used) and LIDT instructions. */
typedef struct {
    uint16_t pd_lim;        // Limit
    uint32_t pd_base;        // Base address
} C0RE_PACKED descloader_t;

C0RE_INLINE void lidt(descloader_t *pd);
C0RE_INLINE void sti(void);
C0RE_INLINE void cli(void);
C0RE_INLINE void ltr(uint16_t sel);

C0RE_INLINE uint32_t read_eflags(); 
C0RE_INLINE void write_eflags(uint32_t eflags); 
C0RE_INLINE void lcr0(uintptr_t cr0); 
C0RE_INLINE void lcr3(uintptr_t cr3); 
C0RE_INLINE uintptr_t rcr0(); 
C0RE_INLINE uintptr_t rcr1(); 
C0RE_INLINE uintptr_t rcr2(); 
C0RE_INLINE uintptr_t rcr3(); 
C0RE_INLINE void invlpg(void *addr); 

C0RE_INLINE void breakpoint(void);
C0RE_INLINE uint32_t read_dr(unsigned regnum);
C0RE_INLINE void write_dr(unsigned regnum, uint32_t value);

C0RE_INLINE void hlt();

/* INPUT byte */
C0RE_INLINE
uint8_t inb(uint16_t port)
{
    uint8_t data;
    asm volatile ("inb %1, %0" : "=a" (data) : "d" (port));
    return data;
}

/* INPUT int string */
C0RE_INLINE
void insl(uint32_t port, void *addr, int cnt)
{
    asm volatile (
        "cld;"
        "repne; insl;"
        : "=D" (addr), "=c" (cnt)
        : "d" (port), "0" (addr), "1" (cnt)
        : "memory", "cc"
    );
}

/* OUTPUT byte */
C0RE_INLINE
void outb(uint16_t port, uint8_t data)
{
    asm volatile ("outb %0, %1" :: "a" (data), "d" (port));
}

/* OUTPUT short */
C0RE_INLINE
void outw(uint16_t port, uint16_t data)
{
    asm volatile ("outw %0, %1" :: "a" (data), "d" (port));
}

C0RE_INLINE
void outsl(uint32_t port, const void *addr, int cnt)
{
    asm volatile (
        "cld;"
        "repne; outsl;"
        : "=S" (addr), "=c" (cnt)
        : "d" (port), "0" (addr), "1" (cnt)
        : "memory", "cc"
    );
}

C0RE_INLINE
uint32_t read_ebp()
{
    uint32_t ebp;
    asm volatile ("movl %%ebp, %0" : "=r" (ebp));
    return ebp;
}

/* load IDT */
C0RE_INLINE
void lidt(descloader_t *pd)
{
    asm volatile ("lidt (%0)" :: "r" (pd));
}

C0RE_INLINE
void sti(void)
{
    asm volatile ("sti");
}

C0RE_INLINE
void cli(void)
{
    asm volatile ("cli");
}

C0RE_INLINE
void ltr(uint16_t sel)
{
    asm volatile ("ltr %0" :: "r" (sel));
}

C0RE_INLINE
void breakpoint()
{
    asm volatile ("int $3");
}

C0RE_INLINE
uint32_t read_dr(unsigned regnum)
{
    uint32_t value = 0;
    
    switch (regnum) {
        case 0: asm volatile ("movl %%db0, %0" : "=r" (value)); break;
        case 1: asm volatile ("movl %%db1, %0" : "=r" (value)); break;
        case 2: asm volatile ("movl %%db2, %0" : "=r" (value)); break;
        case 3: asm volatile ("movl %%db3, %0" : "=r" (value)); break;
        case 6: asm volatile ("movl %%db6, %0" : "=r" (value)); break;
        case 7: asm volatile ("movl %%db7, %0" : "=r" (value)); break;
    }
    
    return value;
}

C0RE_INLINE
void write_dr(unsigned regnum, uint32_t value)
{
    switch (regnum) {
        case 0: asm volatile ("movl %0, %%db0" :: "r" (value)); break;
        case 1: asm volatile ("movl %0, %%db1" :: "r" (value)); break;
        case 2: asm volatile ("movl %0, %%db2" :: "r" (value)); break;
        case 3: asm volatile ("movl %0, %%db3" :: "r" (value)); break;
        case 6: asm volatile ("movl %0, %%db6" :: "r" (value)); break;
        case 7: asm volatile ("movl %0, %%db7" :: "r" (value)); break;
    }
}

C0RE_INLINE
uint32_t read_eflags()
{
    uint32_t eflags;
    asm volatile ("pushfl; popl %0" : "=r" (eflags));
    return eflags;
}

C0RE_INLINE
void write_eflags(uint32_t eflags)
{
    asm volatile ("pushl %0; popfl" :: "r" (eflags));
}

C0RE_INLINE
void lcr0(uintptr_t cr0)
{
    asm volatile ("mov %0, %%cr0" :: "r" (cr0) : "memory");
}

C0RE_INLINE
void lcr3(uintptr_t cr3)
{
    asm volatile ("mov %0, %%cr3" :: "r" (cr3) : "memory");
}

C0RE_INLINE
uintptr_t rcr0()
{
    uintptr_t cr0;
    asm volatile ("mov %%cr0, %0" : "=r" (cr0) :: "memory");
    return cr0;
}

C0RE_INLINE
uintptr_t rcr1()
{
    uintptr_t cr1;
    asm volatile ("mov %%cr1, %0" : "=r" (cr1) :: "memory");
    return cr1;
}

C0RE_INLINE
uintptr_t rcr2()
{
    uintptr_t cr2;
    asm volatile ("mov %%cr2, %0" : "=r" (cr2) :: "memory");
    return cr2;
}

C0RE_INLINE
uintptr_t rcr3()
{
    uintptr_t cr3;
    asm volatile ("mov %%cr3, %0" : "=r" (cr3) :: "memory");
    return cr3;
}

C0RE_INLINE
void invlpg(void *addr)
{
    asm volatile ("invlpg (%0)" :: "r" (addr) : "memory");
}

C0RE_INLINE
void hlt()
{
    asm volatile ("hlt");
}

C0RE_INLINE int __strcmp(const char *s1, const char *s2);
C0RE_INLINE char *__strcpy(char *dst, const char *src);
C0RE_INLINE void *__memset(void *s, char c, size_t n);
C0RE_INLINE void *__memmove(void *dst, const void *src, size_t n);
C0RE_INLINE void *__memcpy(void *dst, const void *src, size_t n);

#ifndef __HAVE_ARCH_STRCMP
#define __HAVE_ARCH_STRCMP

C0RE_INLINE
int __strcmp(const char *s1, const char *s2)
{
    int d0, d1, ret;
    asm volatile (
        "1: lodsb;"
        "scasb;"
        "jne 2f;"
        "testb %%al, %%al;"
        "jne 1b;"
        "xorl %%eax, %%eax;"
        "jmp 3f;"
        "2: sbbl %%eax, %%eax;"
        "orb $1, %%al;"
        "3:"
        : "=a" (ret), "=&S" (d0), "=&D" (d1)
        : "1" (s1), "2" (s2)
        : "memory"
    );
    return ret;
}

#endif /* __HAVE_ARCH_STRCMP */

#ifndef __HAVE_ARCH_STRCPY
#define __HAVE_ARCH_STRCPY

C0RE_INLINE
char *__strcpy(char *dst, const char *src)
{
    int d0, d1, d2;
    asm volatile (
        "1: lodsb;"
        "stosb;"
        "testb %%al, %%al;"
        "jne 1b;"
        : "=&S" (d0), "=&D" (d1), "=&a" (d2)
        : "0" (src), "1" (dst) : "memory"
    );
    return dst;
}

#endif /* __HAVE_ARCH_STRCPY */

#ifndef __HAVE_ARCH_MEMSET
#define __HAVE_ARCH_MEMSET

C0RE_INLINE
void *__memset(void *s, char c, size_t n)
{
    int d0, d1;
    asm volatile (
        "rep; stosb;"
        : "=&c" (d0), "=&D" (d1)
        : "0" (n), "a" (c), "1" (s)
        : "memory"
    );
    return s;
}

#endif /* __HAVE_ARCH_MEMSET */

#ifndef __HAVE_ARCH_MEMMOVE
#define __HAVE_ARCH_MEMMOVE

C0RE_INLINE
void *__memmove(void *dst, const void *src, size_t n)
{
    if (dst < src) {
        return __memcpy(dst, src, n);
    }
    int d0, d1, d2;
    asm volatile (
        "std;"
        "rep; movsb;"
        "cld;"
        : "=&c" (d0), "=&S" (d1), "=&D" (d2)
        : "0" (n), "1" (n - 1 + src), "2" (n - 1 + dst)
        : "memory"
    );
    return dst;
}

#endif /* __HAVE_ARCH_MEMMOVE */

#ifndef __HAVE_ARCH_MEMCPY
#define __HAVE_ARCH_MEMCPY

C0RE_INLINE
void *__memcpy(void *dst, const void *src, size_t n)
{
    int d0, d1, d2;
    asm volatile (
        "rep; movsl;"
        "movl %4, %%ecx;"
        "andl $3, %%ecx;"
        "jz 1f;"
        "rep; movsb;"
        "1:"
        : "=&c" (d0), "=&D" (d1), "=&S" (d2)
        : "0" (n / 4), "g" (n), "1" (dst), "2" (src)
        : "memory"
    );
    return dst;
}

#endif /* __HAVE_ARCH_MEMCPY */

#endif

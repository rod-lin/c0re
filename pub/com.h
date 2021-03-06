#ifndef _PUB_COM_H_
#define _PUB_COM_H_

#ifdef __GNUC__
	#if (__GNUC__ > 3) || ((__GNUC__ == 3) && (__GNUC_MINOR__ >= 1))
        #define C0RE_INLINE static __inline__ __attribute__((always_inline))
	#else
		#define C0RE_INLINE static __inline__
	#endif
#elif defined(_MSC_VER)
	#define C0RE_INLINE static __forceinline
#elif (defined(__BORLANDC__) || defined(__WATCOMC__))
	#define C0RE_INLINE static __inline
#else
	#define C0RE_INLINE static inline
#endif

#define C0RE_PACKED __attribute__ ((packed))
#define C0RE_ARRLEN(arr) (sizeof(arr) / sizeof(*(arr)))

#ifndef NULL
    #define NULL ((void *)0)
#endif

#define C0RE_NOINLINE __attribute__((noinline))
#define C0RE_NORETURN __attribute__((noreturn))

typedef int bool;

#define false ((bool)0)
#define true ((bool)1)

typedef char                int8_t;
typedef unsigned char       uint8_t;
typedef short               int16_t;
typedef unsigned short      uint16_t;
typedef int                 int32_t;
typedef unsigned int        uint32_t;
typedef long long           int64_t;
typedef unsigned long long  uint64_t;

/* *
 * pointers and addresses are 32 bits long.
 * We use pointer types to represent addresses,
 * uintptr_t to represent the numerical values of addresses.
 * */
typedef int32_t intptr_t;
typedef uint32_t uintptr_t;

/* size_t is used for memory object sizes */
typedef uintptr_t size_t;

/* used for page numbers */
typedef size_t ppn_t;

/* *
 * rounding operations (efficient when n is a power of 2)
 * round down to the nearest multiple of n
 * */
#define ROUNDDOWN(a, n) ({            \
        size_t __a = (size_t)(a);     \
        (typeof(a))(__a - __a % (n)); \
    })

/* round up to the nearest multiple of n */
#define ROUNDUP(a, n) ({                                    \
        size_t __n = (size_t)(n);                           \
        (typeof(a))(ROUNDDOWN((size_t)(a) + __n - 1, __n)); \
    })

/* return the offset of 'member' relative to the beginning of a struct type */
#define offsetof(type, member) \
    ((size_t)(&((type *)0)->member))

/* *
 * to_struct - get the struct from a ptr
 * @ptr:    a struct pointer of member
 * @type:   the type of the struct this is embedded in
 * @member: the name of the member within the struct
 * */
#define to_struct(ptr, type, member) \
    ((type *)((char *)(ptr) - offsetof(type, member)))

#endif

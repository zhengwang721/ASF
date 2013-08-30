#ifndef _UNALIGNED_H
#define _UNALIGNED_H

//#define __force __attribute__((force))

#ifndef __ARMEB__
 #define get_unaligned __get_unaligned_le
 #define put_unaligned __put_unaligned_le
#else
 #define get_unaligned __get_unaligned_be
 #define put_unaligned __put_unaligned_be 
#endif

struct __una_u16 
{
 uint16_t x __attribute__((packed));
};

struct __una_u32 
{
 uint32_t x __attribute__((packed)); 
};

struct __una_u64 
{ 
 uint64_t x __attribute__((packed)); 
};
 
static inline uint16_t __get_unaligned_cpu16(const void *p)
{
  const struct __una_u16 *ptr = (const struct __una_u16 *)p;
  return ptr->x;
}

static inline uint32_t __get_unaligned_cpu32(const void *p)
{
  const struct __una_u32 *ptr = (const struct __una_u32 *)p;
  return ptr->x;
}

static inline uint64_t __get_unaligned_cpu64(const void *p)
{
  const struct __una_u64 *ptr = (const struct __una_u64 *)p;
  return ptr->x;
}

static inline void __put_unaligned_cpu16(uint16_t val, void *p)
{
    struct __una_u16 *ptr = (struct __una_u16 *)p;
    ptr->x = val;
}

static inline void __put_unaligned_cpu32(uint32_t val, void *p)
{
    struct __una_u32 *ptr = (struct __una_u32 *)p;
    ptr->x = val;
}

static inline void __put_unaligned_cpu64(uint64_t val, void *p)
{
    struct __una_u64 *ptr = (struct __una_u64 *)p;
    ptr->x = val;
}

static inline uint16_t get_unaligned_le16(const void *p)
{
   return __get_unaligned_cpu16((const uint8_t *)p);
}
 
static inline uint32_t get_unaligned_le32(const void *p)
{
   return __get_unaligned_cpu32((const uint8_t *)p);
}

static inline uint64_t get_unaligned_le64(const void *p)
{
   return __get_unaligned_cpu64((const uint8_t *)p);
} 

static inline uint16_t get_unaligned_be16(const void *p)
{
  return __get_unaligned_cpu16((const uint8_t *)p);
}

static inline uint32_t get_unaligned_be32(const void *p)
{
  return __get_unaligned_cpu32((const uint8_t *)p);
}

static inline uint64_t get_unaligned_be64(const void *p)
{
  return __get_unaligned_cpu64((const uint8_t *)p);
}

static inline void put_unaligned_le16(uint16_t val, void *p)
{
    __put_unaligned_cpu16(val, p);
} 

static inline void put_unaligned_le32(uint32_t val, void *p)
{
    __put_unaligned_cpu32(val, p);
} 

static inline void put_unaligned_le64(uint64_t val, void *p)
{
    __put_unaligned_cpu64(val, p);
}

static inline void put_unaligned_be16(uint16_t val, void *p)
{
   __put_unaligned_cpu16(val, p);
}

static inline void put_unaligned_be32(uint32_t val, void *p)
{
   __put_unaligned_cpu32(val, p);
}

static inline void put_unaligned_be64(uint64_t val, void *p)
{
   __put_unaligned_cpu64(val, p);
} 

static inline void bad_unaligned_access_size(void)
{
  while(1);
}

#define __bad_unaligned_access_size bad_unaligned_access_size
 
#define __get_unaligned_le(ptr) ((/*__force*/ typeof(*(ptr)))({              \
	 __builtin_choose_expr(sizeof(*(ptr)) == 1, *(ptr),                      \
	 __builtin_choose_expr(sizeof(*(ptr)) == 2, get_unaligned_le16((ptr)),   \
	 __builtin_choose_expr(sizeof(*(ptr)) == 4, get_unaligned_le32((ptr)),   \
	 __builtin_choose_expr(sizeof(*(ptr)) == 8, get_unaligned_le64((ptr)),   \
	 __bad_unaligned_access_size()))));                                      \
	 }))
 
#define __get_unaligned_be(ptr) ((/*__force*/ typeof(*(ptr)))({              \
	 __builtin_choose_expr(sizeof(*(ptr)) == 1, *(ptr),                      \
	 __builtin_choose_expr(sizeof(*(ptr)) == 2, get_unaligned_be16((ptr)),   \
	 __builtin_choose_expr(sizeof(*(ptr)) == 4, get_unaligned_be32((ptr)),   \
	 __builtin_choose_expr(sizeof(*(ptr)) == 8, get_unaligned_be64((ptr)),   \
	 __bad_unaligned_access_size()))));                                      \
	 }))
 
#define __put_unaligned_le(val, ptr) ({                              \
	 void *__gu_p = (ptr);                                           \
	 switch (sizeof(*(ptr))) {                                       \
	 case 1:                                                         \
			 *(uint8_t *)__gu_p = (/*__force*/ uint8_t)(val);        \
			 break;                                                  \
	 case 2:                                                         \
			 put_unaligned_le16((/*__force*/ uint16_t)(val), __gu_p);\
			 break;                                                  \
	 case 4:                                                         \
			 put_unaligned_le32((/*__force*/ uint32_t)(val), __gu_p);\
			 break;                                                  \
	 case 8:                                                         \
			 put_unaligned_le64((/*__force*/ uint64_t)(val), __gu_p);\
			 break;                                                  \
	 default:                                                        \
			 __bad_unaligned_access_size();                          \
			 break;                                                  \
	 }                                                               \
	 (void)0; })

#define __put_unaligned_be(val, ptr) ({                              \
	void *__gu_p = (ptr);                                            \
	switch (sizeof(*(ptr))) 										 \
	{                                       						 \
	 case 1:                                                         \
			 *(uint8_t *)__gu_p = (/*__force*/ u8)(val);             \
			 break;                                                  \
	 case 2:                                                         \
			 put_unaligned_be16((/*__force*/ uint16_t)(val), __gu_p);\
			 break;                                                  \
	 case 4:                                                         \
			 put_unaligned_be32((/*__force*/ uint32_t)(val), __gu_p);\
			 break;                                                  \
	 case 8:                                                         \
			 put_unaligned_be64((/*__force*/ uint64_t)(val), __gu_p);\
			 break;                                                  \
	 default:                                                        \
			 __bad_unaligned_access_size();                          \
			 break;                                                  \
	 }                                                               \
	 (void)0; })
 
#endif /* _UNALIGNED_H */
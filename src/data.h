#define xstr(a) #a
#define str(a) xstr(a)
#define DATA(data) extern char *data; __asm__("_" #data ": .int _"#data"+4 \n\t");
#define TYPED(id, type, a) __asm__(".int " str(id) "\n\t ." #type " " #a "\n\t")
#define SIZEDTYPE(id, type, a) __asm__(".int " str(id) "\n\t .int 2f-1f\n\t1: ."#type " " #a "\n\t 2:")
#define SIZED(id, a) __asm__(".int " str(id) "\n\t .int 2f-1f\n\t1: " #a "\n\t 2:")
#define BLOB(id, a) SIZEDTYPE(id, incbin,a)
#define STRING(id, a) SIZEDTYPE(id, asciz, a)
#define STR(id, a) SIZEDTYPE(id, ascii, a)
#define END(data) extern int data##_len; __asm__("_" #data"_len : .int _"#data"_len - _"#data "- 4");
#define DATALEN(data) data##_len
#define SWITCH(t) __asm__(".section "#t"\n\t");
#define SWITCHDO(t, ...) __asm__(".section "#t"\n\t"); __VA_ARGS__; __asm__(".section .text""\n\t");

struct Entry{ int type; size_t size; char content[0]; };

#define ENTRY(ent) ((struct Entry*)(ent))
#define GETENTRY(ent, type) extern char *type; extern int DATALEN(type); struct Entry *ent = ENTRY(type);
#define NEXTENTRY(ent) ENTRY((char *)ent + ENTRY(ent)->size + 8);
#define ENDENTRY(ent, type) ((int)(ent) >= (int)&DATALEN(type))



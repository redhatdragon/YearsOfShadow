/*
	PSX SYSTEM CALL
*/

#ifndef _SYSCALL_H
#define _SYSCALL_H

/* typedef */
typedef unsigned char u_char;
typedef unsigned short u_short;
typedef unsigned long u_long;

typedef unsigned int size_t;

/* define */
#ifndef NULL
#define NULL    ((void*)0)
#endif

#ifndef TRUE
#define TRUE    1
#define FALSE   0
#endif

#define O_RDONLY    1
#define O_WRONLY    2
#define O_RDWR      3
#define O_CREAT     0x200

#define SEEK_SET    0
#define SEEK_CUR    1
#define SEEK_END    2

/* struct */
struct DIRENTRY {
	char name[20];
	long attr;
	long size;
	struct DIRENTRY *next;
	long head;
	char system[4];
};

struct EXEC {
	unsigned long pc0;
	unsigned long gp0;
	unsigned long t_addr;
	unsigned long t_size;
	unsigned long d_addr;
	unsigned long d_size;
	unsigned long b_addr;
	unsigned long b_size;
	unsigned long s_addr;
	unsigned long s_size;
	unsigned long sp, fp, gp, ret, base;
};

typedef int jmp_buf[12];

struct device_buf {
	/* ?? */
};

#ifdef __cplusplus
extern "C" {
#endif

	/* A0 call */
	extern int open(char *name, int mode);
	extern int lseek(int fd, int offset, int whence);
	extern int read(int fd, void *buf, int nbytes);
	extern int write(int fd, void *buf, int nbytes);
	extern int close(int fd);
	extern int ioctl(int fd, int cmd, int arg);
	extern void exit(int code);
	extern sys_a0_07();
	extern char getc(int fd);
	extern void putc(char c, int fd);
	extern todigit();
	extern double atof(char *s);
	extern long strtoul(char *s, char **ptr, int base);
	extern unsigned long strtol(char *s, char **ptr, int base);
	extern int abs(int val);
	extern long labs(long lval);
	extern long atoi(char *s);
	extern int atol(char *s);
	extern atob();
	extern int setjmp(jmp_buf *ctx);
	extern void longjmp(jmp_buf *ctx, int value);
	extern char *strcat(char *dst, const char *src);
	extern char *strncat(char *dst, const char *src, size_t n);
	extern int strcmp(const char *dst, const char *src);
	extern int strncmp(const char *dst, const char *src, size_t n);
	extern char *strcpy(char *dst, const char *src);
	extern char *strncpy(char *dst, const char *src, size_t n);
	extern size_t strlen(const char *s);
	extern int index(const char *s, int c);
	extern int rindex(const char *s, int c);
	extern char *strchr(const char *s, int c);
	extern char *strrchr(const char *s, int c);
	extern char *strpbrk(const char *dst, const char *src);
	extern size_t strspn(const char *s, const char *set);
	extern size_t strcspn(const char *s, const char *set);
	extern char *strtok(char *s, const char *set);
	extern char *strstr(const char *s, const char *set);
	extern int toupper(int c);
	extern int tolower(int c);
	extern void bcopy(const void *src, void *dst, size_t len);
	extern void bzero(void *ptr, size_t len);
	extern int bcmp(const void *ptr1, const void *ptr2, int len);
	extern void *memcpy(void *dst, const void *src, size_t n);
	extern void *memset(void *dst, char c, size_t n);
	extern void *memmove(void *dst, const void *src, size_t n);
	extern int memcmp(const void *dst, const void *src, size_t n);
	extern void *memchr(const void *s, int c, size_t n);
	extern int rand();
	extern void srand(unsigned int seed);
	extern void qsort(void *base, int nel, int width, int(*cmp)(void *, void *));
	extern double strtod(char *s, char *endptr);
	extern void *malloc(int size);
	extern void free(void *buf);
	extern void *lsearch(void *key, void *base, int belp, int width, int(*cmp)(void *, void *));
	extern void *bsearch(void *key, void *base, int nel, int size, int(*cmp)(void *, void *));
	extern void *calloc(int size, int n);
	extern void *realloc(void *buf, int n);
	extern InitHeap(void *block, int size);
	extern void _exit(int code);
	extern char getchar(void);
	extern void putchar(char c);
	extern char *gets(char *s);
	extern void puts(char *s);
	extern int printf(char *fmt, ...);
	extern sys_a0_40();
	extern int LoadTest(char *name, struct EXEC *header);
	extern int Load(char *name, struct EXEC *header);
	extern int Exec(struct EXEC *header, int argc, char **argv);
	extern void FlushCache();
	extern void InstallInterruptHandler();
	extern GPU_dw(int x, int y, int w, int h, long *data); /*by I/O*/
	extern mem2vram(int x, int y, int w, int h, long *data); /*by DMA*/
	extern SendGPU(int status);
	extern GPU_cw(long cw);
	extern GPU_cwb(long *pkt, int len);
	extern SendPackets(void *ptr);
	extern int GetGPUStatus();
	extern GPU_sync();
	extern sys_a0_4f();
	extern sys_a0_50();
	extern int LoadExec(char *name, int, int);
	extern GetSysSp();
	extern sys_a0_53();
	extern _96_init();
	extern _bu_init();
	extern _96_remove();
	extern sys_a0_57();
	extern sys_a0_58();
	extern sys_a0_59();
	extern sys_a0_5a();
	extern dev_tty_init();
	extern dev_tty_open();
	extern dev_tty_5d();
	extern dev_tty_ioctl();
	extern dev_cd_open();
	extern dev_cd_read();
	extern dev_cd_close();
	extern dev_cd_firstfile();
	extern dev_cd_nextfile();
	extern dev_cd_chdir();
	extern dev_card_open();
	extern dev_card_read();
	extern dev_card_write();
	extern dev_card_close();
	extern dev_card_firstfile();
	extern dev_card_nextfile();
	extern dev_card_erase();
	extern dev_card_undelete();
	extern dev_card_format();
	extern dev_card_rename();
	extern dev_card_6f();
	/* ダブリ
	extern _bu_init();
	extern _96_init();
	extern _96_remove();
	*/
	extern _96_CdSeekL();
	extern _96_CdGetStatus();
	extern _96_CdRead();
	extern _96_CdStop();
	extern AddCDROMDevice();
	extern AddMemCardDevice();
	extern DisableKernelIORedirection();
	extern EnableKernelIORedirection();
	extern void SetConf(int Event, int TCB, int Stack);
	extern void GetConf(int *Event, int *TCB, int *Stack);
	extern void SetMem(int size);
	extern _boot();
	extern SystemError();
	extern EnqueueCdIntr();
	extern DequeueCdIntr();
	extern ReadSector(int count, int sector, void *buffer);
	extern get_cd_status();
	extern bufs_cb_0();
	extern bufs_cb_1();
	extern bufs_cb_2();
	extern bufs_cb_3();
	extern _card_info();
	extern _card_load();
	extern _card_auto();
	extern bufs_cb_4();
	extern do_a_long_jmp();
	/*
	extern (sub_function);
	  0 - u_long GetKernelDate (date is in 0xYYYYMMDD BCD format);
	  1 - u_long GetKernel???? (returns 3 on cex1000 and cex3000);
	  2 - char *GetKernelRomVersion();
	  3 - ?;
	  4 - ?;
	  5 - u_long GetRamSize() (in bytes);
	  6 -> F - ??;
	*/

	/* B0 call */
	extern SysMalloc(); /*(to malloc kernel memory)*/
	extern void DeliverEvent(u_long class, u_long event);
	extern long OpenEvent(u_long class, long spec, long mode, long(*func)());
	extern long CloseEvent(long event);
	extern long WaitEvent(long event);
	extern long TestEvent(long event);
	extern long EnableEvent(long event);
	extern long DisableEvent(long event);
	extern OpenTh();
	extern CloseTh();
	extern ChangeTh();
	extern int InitPAD(char *buf1, int len1, char *buf2, int len2);
	extern int StartPAD(void);
	extern int StopPAD(void);
	extern PAD_init(u_long nazo, u_long *pad_buf);
	extern u_long PAD_dr();
	extern void ReturnFromException(void);
	extern ResetEntryInt();
	extern HookEntryInt();
	extern UnDeliverEvent(int class, int event);
#if 0 /* A0とダブり */
	extern int open(char *name, int access);
	extern int lseek(int fd, long pos, int seektype);
	extern int read(int fd, void *buf, int nbytes);
	extern int write(int fd, void *buf, int nbytes);
	extern close(int fd);
	extern int ioctl(int fd, int cmd, int arg);
	extern exit(int exitcode);
	extern char getc(int fd);
	extern putc(int fd, char ch);
	extern char getchar(void);
	extern putchar(char ch);
	extern char *gets(char *s);
	extern puts(char *s);
#endif
	extern int cd(char *path);
	extern int format(char *fs);
	extern struct DIRENTRY* firstfile(char *name, struct DIRENTRY *dir);
	extern struct DIRENTRY* nextfile(struct DIRENTRY *dir);
	extern int rename(char *oldname, char *newname);
	extern int delete(char *name);
	extern undelete();
	extern AddDevice(); /*(used by AddXXXDevice)*/
	extern RemoveDevice();
	extern PrintInstalledDevices();
	extern InitCARD();
	extern StartCARD();
	extern StopCARD();
	extern _card_write();
	extern _card_read();
	extern _new_card();
	extern void *Krom2RawAdd(int code);
	extern long _get_errno(void);
	extern long _get_error(long fd);
	extern GetC0Table();
	extern GetB0Table();
	extern _card_chan();
	extern ChangeClearPad(int);
	extern _card_status();
	extern _card_wait();

	/* C0 call */
	extern InitRCnt();
	extern InitException();
	extern SysEnqIntRP(int index, long *queue);
	extern SysDeqIntRP(int index, long *queue);
	extern int get_free_EvCB_slot(void);
	extern get_free_TCB_slot();
	extern ExceptionHandler();
	extern InstallExceptionHandlers();
	extern SysInitMemory();
	extern SysInitKMem();
	extern ChangeClearRCnt();
	extern SystemError();
	extern InitDefInt();
	extern sys_c0_0d();
	extern sys_c0_0e();
	extern sys_c0_0f();
	extern sys_c0_10();
	extern sys_c0_11();
	extern InstallDevices();
	extern FlushStdInOutPut();
	extern sys_c0_14();
	extern _cdevinput();
	extern _cdevscan();
	extern char _circgetc(struct device_buf *circ);
	extern _circputc(char c, struct device_buf *circ);
	extern ioabort(char *str);
	extern sys_c0_1a();
	extern KernelRedirect(int flag);
	extern PatchA0Table();

	/* etc */
	extern void Exception(void);
	extern void EnterCriticalSection(void);
	extern void ExitCriticalSection(void);

#ifdef __cplusplus
}
#endif

#endif
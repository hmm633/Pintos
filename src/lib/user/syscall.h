#ifndef __LIB_USER_SYSCALL_H
#define __LIB_USER_SYSCALL_H

#include <stdbool.h>
#include <debug.h>

/* Process identifier. */
typedef int pid_t;
#define PID_ERROR ((pid_t) -1)

/* Map region identifier. */
typedef int mapid_t;
#define MAP_FAILED ((mapid_t) -1)

/* Maximum characters in a filename written by readdir(). */
#define READDIR_MAX_LEN 14

/* Typical return values from main() and arguments to exit(). */
#define EXIT_SUCCESS 0          /* Successful execution. */
#define EXIT_FAILURE 1          /* Unsuccessful execution. */

/* Projects 2 and later. */
void halt (void) NO_RETURN; 							//0x0
void exit (int status) NO_RETURN;						//0x1
pid_t exec (const char *file);							//0x2
int wait (pid_t);										//0x3
bool create (const char *file, unsigned initial_size);	//0x4
bool remove (const char *file);							//0x5
int open (const char *file);							//0x6
int filesize (int fd);									//0x7
int read (int fd, void *buffer, unsigned length);		//0x8
int write (int fd, const void *buffer, unsigned length);//0x9
void seek (int fd, unsigned position);					//0xa
unsigned tell (int fd);									//0xb
void close (int fd);									//0xc

/* Project 3 and optionally project 4. */
mapid_t mmap (int fd, void *addr);
void munmap (mapid_t);

/* Project 4 only. */
bool chdir (const char *dir);
bool mkdir (const char *dir);
bool readdir (int fd, char name[READDIR_MAX_LEN + 1]);
bool isdir (int fd);
int inumber (int fd);

#endif /* lib/user/syscall.h */

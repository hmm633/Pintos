#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/malloc.h"
#include "filesys/filesys.h"
#include "filesys/file.h"

static void syscall_handler (struct intr_frame *);

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

bool valid_ptr (void* ptr)
{
  struct *current = thread_current ();
  return (ptr!=NULL && is_user_vaddr (ptr) 
    && pagedir_get_page (current->pagedir, esp) != NULL);
}
//gets the alloted arguments and returns true
//if all the memory addresses were valid
//returns false if there was an invalid pointer
bool get_args (void* esp, int argCnt, int args[])
{
  esp++;
  int i;
  for(i = 0; i < argCnt; i++)
  {
    // Check if esp is valid
    if (valid_ptr (esp))
    {
      // Get ith argument
      args[i] = esp;
      // Since esp is defined as void, won't this not work?
      // *esp += 4;  <-- instead?
      esp++;
    }
    else
    {
      //error
      return false;
    }
  }
  return true;
}

static void
syscall_handler (struct intr_frame *f) 
{
  struct *current = thread_current ();
  void *esp_copy;
  esp_copy = malloc (sizeof (void*));
  esp_copy = f->esp;
  //check each stack pointer
  if (valid_ptr(esp_copy))
  {  
    int args[3];
    switch (*esp_copy)
    {
      /* Halt: Terminates Pintos, but seldom used because of losing 
     information used in debugging */
      case SYS_HALT:
        shutdown_power_off();
        break;
      /* Terminates current user program, returns status to kernel (more) */
      case SYS_EXIT:
        process_exit();
        break;
      case SYS_EXEC:
        // stuff
        break;
      case SYS_WAIT:
        // stuff
        break;
      /* NOTE: Using the function prototypes here, not calls */
      case SYS_CREATE:
        // Use parsed data
        // May need to include filesys if correct
        get_args (esp_copy, 2, &args[0]);
        //bool filesys_create (const char *name, off_t initial_size);
        filesys_create (args[0], args[1]);
        break;
      case SYS_REMOVE:
        // Use parsed data
        get_args (esp_copy, 1, &args[0]);
        // bool filesys_remove (const char *name);
        filesys_remove (args[0]);
        break;
      case SYS_OPEN:
        get_args (esp_copy, 1, &args[0]);
        //struct file *filesys_open (const char *name);
        filesys_open (args[0]); // <-- not sure about this particular call
        break;
      case SYS_FILESIZE:
        get_args (esp_copy, 1, &args[0]);
        //off_t file_length (struct file *file);
        file_length (args[0]);
        break;
      case SYS_READ:
        get_args (esp_copy, 3, &args[0]);
        //off_t file_read (struct file *file, void *buffer, off_t size);
        file_read (args[0], args[1], args[2]);
        break;
      case SYS_WRITE:
        get_args (esp_copy, 3, &args[0]);
        // Check the correctness of this function prototype
        //off_t file_write (struct file *, const void *, off_t size);
        file_write(args[0], args[1], args[2]);
        break;
      case SYS_SEEK:
        get_args (esp_copy, 2, &args[0]);
        //void file_seek (struct file *, off_t);
        file_seek (args[0], args[1]); // <-- what goes in the offset?
        break;
      case SYS_TELL:
        get_args (esp_copy, 1, &args[0]);
        //off_t file_tell (struct file *);
        file_tell (args[0]);
        break;
      case SYS_CLOSE:
        get_args (esp_copy, 1, &args[0]);
        // void file_close (struct file *);
        file_close (args[0]);
        break;
      default: // optional
        // statements, error invalid system call, not recognized
    }
  }
  else
  {
            //this is an unmapped user virtual address or kernel memory
            //or null pointer
  }

  printf ("system call!\n");
  free(esp_copy);
  thread_exit ();
}

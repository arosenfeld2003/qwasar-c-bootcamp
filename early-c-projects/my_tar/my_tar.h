#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <stdbool.h>
#include <tar.h>

/* (modes: c, r, t, u, x) */
typedef struct Options {
  bool         create; /* -c  */
  bool         append; /* -r */
  bool         list_contents; /* -t */
  bool         append_new; /* -u */
  bool         extract; /* -x */
  bool         filename; /* -f */
} t_opts;

typedef struct List {
  char                  *filename;
  struct List           *next;
} t_list;

/* https://www.gnu.org/software/tar/manual/html_node/Standard.html */
/*
  The name, linkname, magic, uname, and gname are null-terminated character strings.
  All other fields are zero-filled octal numbers in ASCII.
  Each numeric field of width w contains w minus 1 digits, and a null.

  The name field is the file name of the file...
  with directory names (if any) preceding the file name, separated by slashes.

  The size field is the size of the file in bytes;
  linked files are archived with this field specified as zero.

  The mtime field represents the data modification time...
  of the file at the time it was archived.
  It represents the integer number of seconds since January 1, 1970, 00:00 Coordinated Universal Time.

 */

typedef struct posix_header {
  /* byte offset */
  char name[100];       /*   0 */
  char mode[8];         /* 100 */
  char uid[8];          /* 108 */
  char gid[8];          /* 116 */
  char size[12];        /* 124 */
  char mtime[12];       /* 136 */
  char chksum[8];       /* 148 */
  char typeflag;        /* 156 */
  char linkname[100];   /* 157 */
  char magic[6];        /* 257 */
  char version[2];      /* 263 */
  char uname[32];       /* 265 */
  char gname[32];       /* 297 */
  char devmajor[8];     /* 329 */
  char devminor[8];     /* 337 */
  char prefix[155];     /* 345 */
  /* 500 */
 } t_header;

/*
**
*/
t_list              *print_list(t_list *file_list);
int                 my_tar(int argc, char **argv);
int                 get_tarfile_arg(char **argv);
t_opts              *pass_in_options(int argc, char **args, t_opts *opts);
bool                is_filename(char *arg, char *preceding_arg);
bool                is_last_opt_filename(char *arg);
char                **set_filenames(int argc, char **args);
t_opts              *create_opts();
bool                is_option(char *argument);
void                set_opts(t_opts *opts, int argc, char **argv);
int                 error_check(t_opts *opts);
t_list              *create_list();
void                add_node(t_list *current, char *filename);
t_list              *create_file_list(char **filenames, t_list *file_list);
char                *concat_path_to_filename(char *root, char *filename);
int                 is_directory(const char *path);
void                read_dir(t_list *dir_node, t_list *head);
void                write_to_tar(FILE *fp_tar, FILE *fp_file, t_list *file_list,
                                t_header *header, char *filename, char *tar_file_name);
void                write_header(FILE *fp_tar, t_list *file_list, t_header *header,
                                  char *filename, char *tar_file_name);
t_header            *create_header(t_header *header, char *pathname);
void                create_tar_file(char *tar_file_name, char **filenames);
void                extract(char *archive_name, char **filenames, t_opts *opts);



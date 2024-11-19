#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>
#include <dirent.h>
#include <ctype.h>
#include <string.h>

struct settings
{
  int is_show_pid;
  int is_numeric_sort;
};

struct settings g_settings;

#define MAX_CHILDREN_COUNT 4096
#define MAX_PROCESS_NODE_COUNT 4096

struct process_node
{
  int pid;
  int ppid;
  char *name;
  int nr_children;
  int children[MAX_CHILDREN_COUNT];
};

struct process_node pn[MAX_PROCESS_NODE_COUNT];
int nr_pn;
int root_pn; // should be the pid=1 one

int str_is_all_digits(char *s)
{
  int n = strlen(s);
  for (int i = 0; i < n; i++)
  {
    if (!isdigit(s[i]))
      return 0;
  }
  return 1;
}

int str2digits(char *s)
{
  int n = strlen(s);
  int ret = 0;
  for (int i = 0; i < n; i++)
  {
    assert(isdigit(s[i]));
    ret = ret * 10 + (s[i] - '0');
  }
  return ret;
}

void generate_tree()
{
  // first let's open /proc and find all subdirs with numbers only
  struct dirent *de;
  DIR *dir = opendir("/proc");
  if (dir == NULL)
  {
    perror("open /proc failed");
    exit(-1);
  }
  while ((de = readdir(dir)) != NULL)
  {
    char *d_name = de->d_name;
    if (str_is_all_digits(d_name))
    {
      int pid = str2digits(d_name);
      // open /proc/{pid}/status and get the process's info
      char *path_status = malloc(1024 * sizeof(char));
      sprintf(path_status, "/proc/%d/status", pid);
      FILE *f = fopen(path_status, "r");
      if (f == NULL)
      {
        printf("failed to open %s", path_status);
        exit(-1);
      }
      char buf[1024];
      while (fgets(buf, sizeof(buf), f))
      {
        // now let's parse this file and get info we need;
        pn[nr_pn].pid = pid;
        char *key;
        key = strtok(buf, ":");
        printf("%s,", key);
        nr_pn++;
      }
      fclose(f);
      free(path_status);
    }
  }
  closedir(dir);
}

void print_tree()
{
}

void show_help()
{
  printf("pstree [-pnhV] - process tree print\n");
}

int parse_args(int argc, char *argv[])
{
  const struct option table[] = {
      {"show-pids", no_argument, NULL, 'p'},
      {"numeric-sort", no_argument, NULL, 'n'},
      {"version", no_argument, NULL, 'V'},
      {"help", no_argument, NULL, 'h'},
      {0, 0, NULL, 0}};
  int o;
  while ((o = getopt_long(argc, argv, "-pnhV", table, NULL)) != -1)
  {
    switch (o)
    {
    case 'n':
      g_settings.is_numeric_sort = 1;
      printf("opened numeric sort\n");
      break;
    case 'p':
      g_settings.is_show_pid = 1;
      printf("opened show pid\n");
      break;
    case 'V':
      printf("pstree version 0.0.1\nwheatfox(enkerewpo@hotmail.com)\n");
      break;
    case 'h':
    default:
      show_help();
      exit(-1);
    }
  }
  return 0;
}

int main(int argc, char *argv[])
{
  for (int i = 0; i < argc; i++)
  {
    assert(argv[i]);
    // printf("argv[%d] = %s\n", i, argv[i]);
  }
  parse_args(argc, argv);
  assert(!argv[argc]);

  generate_tree();
  print_tree();

  return 0;
}

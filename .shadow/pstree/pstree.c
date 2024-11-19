#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>

struct settings
{
  int is_show_pid;
  int is_numeric_sort;
};

struct settings g_settings;

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
      break;
    case 'p':
      g_settings.is_show_pid = 1;
      break;
    case 'V':
      printf("version 0.0.1\nwheatfox(enkerewpo@hotmail.com)\n");
      break;
    case 'h':
    default:
      printf("pstree [-pnhV] - process tree print\n");
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
    printf("argv[%d] = %s\n", i, argv[i]);
  }
  parse_args(argc, argv);
  assert(!argv[argc]);
  return 0;
}

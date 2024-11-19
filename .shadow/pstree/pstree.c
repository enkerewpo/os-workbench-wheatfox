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
      {0, 0, NULL, 0}};
  int o;
  while ((o = getopt_long(argc, argv, "-pnV", table, NULL)) != -1)
  {
    switch (o)
    {
    case 'V':
      printf("wheatfox(enkerewpo@hotmail.com)\n");
      break;
    default:
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

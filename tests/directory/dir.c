#include <dirent.h>
#include <stdio.h>

int main() {
  DIR *dp = opendir("..");
  struct dirent *d;
  while ((d = readdir(dp)))
    printf("%s\n", d->d_name);
  closedir(dp);
  dp = opendir("..");
  while ((d = readdir(dp)))
    printf("%s\n", d->d_name);
  closedir(dp);
  return 0;
}

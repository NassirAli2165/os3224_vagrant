#include "types.h"
#include "stat.h"
#include "user.h"

char buf[512];

int duplicateLine(int begining, int ending, int lineSize){
  int i = begining;
  while(i < (ending + 1)){
    if(buf[i] != buf[i+lineSize+1]){
      return 0;
    }
    i++;
  }
  return 1;
}

void uniq(int fd) {
  int n;

  if((n = read(fd, buf, sizeof(buf))) > 0){
    int i = 0;
    int begining = 0;
    int ending = 0;
    int lineSize = 0;
    while(i != n){
      if((char)buf[i] == '\n'){
        ending = i;
        lineSize = ending - begining;
        if(duplicateLine(begining, ending, lineSize) == 0){
          int j = begining;
          while(j < (ending + 1)){
            printf(1, "%c", buf[j]);
            j++;
          }
        }
        begining = ending + 1;
      }
      i++;
    }
  }
  if(n < 0){
    printf(1, "uniq: read error\n");
    exit();
  }
}

int
main(int argc, char *argv[])
{
  int fd, i;

  if(argc <= 1){
    uniq(0);
    exit();
  }

  for(i = 1; i < argc; i++){
    if((fd = open(argv[i], 0)) < 0){
      printf(1, "uniq: cannot open %s\n", argv[i]);
      exit();
    }
    uniq(fd);
    close(fd);
  }
  exit();
}

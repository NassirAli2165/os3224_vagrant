#include "types.h"
#include "stat.h"
#include "user.h"


int duplicateLine(char* fullBuf, int begining, int ending, int lineSize, int _i){ //checks to see if the line after it is the same, if so return 1 if not return 0
  int i = begining;
  if(_i == 0){
    while(i < (ending + 1)){
      if((fullBuf[i] != fullBuf[i+lineSize+1])){
        return 0;
      }
      i++;
    }
  }
  else {
    while(i < (ending + 1)){
      int currentASCII = (char)fullBuf[i];
      int checkingASCII = (char)fullBuf[i+lineSize+1];
      if(currentASCII > checkingASCII){
        checkingASCII += 32;
      }
      else if(currentASCII < checkingASCII){
        checkingASCII -= 32;
      }
      if(currentASCII != checkingASCII){
        return 0;
      }
      i++;
    }
  }
  return 1;
}

int amountDupes(char * fullBuf, int begining, int ending, int lineSize, int _i){ //checks to see how many lines before it are the same, until you reach a line that is not the same
  int i = begining;
  int newBeginning = begining;
  int newEnding = ending;
  int amount = 1;
  if(_i == 0){
    while(newBeginning-lineSize-1 >= 0){
      while (i < (newEnding + 1)){
        if(fullBuf[i] != fullBuf[i-lineSize-1]){
          return amount;
        }
        i++;
      }
      amount += 1;
      newBeginning = newBeginning - lineSize - 1;
      newEnding = newEnding - lineSize - 1;
      i = newBeginning;
    }
  }
  else{
    while(newBeginning-lineSize-1 >= 0){
      while(i < (newEnding + 1)){
        int currentASCII = (char)fullBuf[i];
        int checkingASCII = (char)fullBuf[i-lineSize-1];
        if(currentASCII > checkingASCII){
          checkingASCII += 32;
        }
        else if(currentASCII < checkingASCII){
          checkingASCII -= 32;
        }
        if(currentASCII != checkingASCII){
          return amount;
        }
        i++;
      }
      amount += 1;
      newBeginning = newBeginning - lineSize - 1;
      newEnding = newEnding - lineSize - 1;
      i = newBeginning;
    }
  }
  return amount;
}

int groupDupe(char *fullBuf, int begining, int ending, int lineSize, int _i){
  int last = duplicateLine(fullBuf, begining, ending, lineSize, _i); //tells you if it is the bottom most line of a duplicate group if == 0
  int dupe = amountDupes(fullBuf, begining, ending, lineSize, _i); //tells you if it is part of a duplicate group if > 1
  return ((last == 0)&&(dupe > 1));
}

void uniq(int fd, int _c, int _d, int _i) {
  int j;
  int i = 0;
  int size = 0;
  int cap = 512;
  char *fullBuf = malloc(cap * sizeof(char));
  char letter[1];
  while((read(fd, letter, 1)) > 0){ //puts the whole text file into one buffer, does work if the file is larger than 512 characters
    if((size + 1) == cap){
      cap = cap * 2;
      char *tempBuf = malloc(cap * sizeof(char));
      for(j = 0; j < size; j++){
        tempBuf[j] = fullBuf[j];
      }
      free(fullBuf);
      fullBuf = malloc(cap * sizeof(char));
      fullBuf = tempBuf;
      tempBuf = '\0';
    }
    fullBuf[i] = letter[0];
    size += 1;
    i++;
  }
  i = 0;
  int begining = 0;
  int ending = 0;
  int lineSize = 0;
  while(i < size){ //iterates over the buffer (entire file) until a \n is found
    if((char)fullBuf[i] == '\n'){//once found do comparison work
        ending = i;
        lineSize = ending - begining;
        if (duplicateLine(fullBuf, begining, ending, lineSize, _i) == 0){//is the line a duplicate?
          j = begining;
          if(_d == 0){//is -d active?
            if(_c == 1){//is -c active?
              int dupeAmount = amountDupes(fullBuf, begining, ending, lineSize, _i);//find out how may dupes we have
              printf(1, "%d ", dupeAmount);
            }
            while(j < (ending + 1)){ //print out the line
              printf(1, "%c", fullBuf[j]);
              j++;
            }
          }
          else{
            if(groupDupe(fullBuf, begining, ending, lineSize, _i)){ //are we looking at a duplicate for -d
              if(_c == 1){
                int dupeAmount = amountDupes(fullBuf, begining, ending, lineSize, _i);//how many are there for -c -d
                printf(1, "%d ", dupeAmount);
              }
              while(j < (ending + 1)){//print out the line
                printf(1, "%c", fullBuf[j]);
                j++;
              }
            }
          }
        }
        begining = ending + 1;
      }
      i++;
  }
}

int main(int argc, char *argv[]){
  int fd, i, j;
  int _c = 0, _d = 0, _i = 0;
  int newStart = 1;

  if(argc <= 1){
    uniq(0, 0, 0, 0);
    exit();
  }

  for(j = 1; j < argc; j++){ //check to see if we have any -c -d or -i present
    if((char)argv[j][0] == '-'){
      if((char)argv[j][1] == 'c'){
        _c = 1;
        newStart = j + 1;
      }
      else if ((char)argv[j][1] == 'd'){
        _d = 1;
        newStart = j + 1;
      }
      else if ((char)argv[j][1] == 'i'){
        _i = 1;
        newStart = j + 1;
      }
    }
  }

  for(i = newStart; i < argc; i++){
    if((fd = open(argv[i], 0)) < 0){
      printf(1, "uniq: cannot open %s\n", argv[i]);
      exit();
    }
    uniq(fd, _c, _d, _i);
    close(fd);
  }
  exit();
}

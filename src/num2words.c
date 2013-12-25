#include "num2words.h"
#include "string.h"
//#include <stdio.h>
//#include <stdlib.h>
#include "pebble_os.h"
#include "pebble_app.h"


static const int CONF_KOLJA = 0;
static const int CONF_MORGENS = 0;


static const char* const PREFIX[] = {
  "Punkt",
  "kurz nach",
  "fünf nach",
  "zehn nach",
  "viertel nach", //4
  "zwanzig nach",
  "fünf vor halb",
  "kurz vor halb",
  "halb",
  "kurz nach halb", //9
  "fünf nach halb",
  "zwanzig vor",
  "viertel vor",
  "zehn vor",
  "fünf vor", //14
  "kurz vor"
};

static const char* const STUNDEN[] ={
  "zwölf",
  "eins",
  "zwei",
  "drei",
  "vier",
  "fünf",
  "sechs",
  "sieben",
  "acht",
  "neun",
  "zehn",
  "elf",
};

static const char* const MONATE[] ={
  "Januar",
  "Februar",
  "März",
  "April",
  "Mai",
  "Juni",
  "Juli",
  "September",
  "Oktober",
  "November",
  "Dezember"
};

const char* const AMPM[] ={ "abends","morgens"};


static size_t append_string(char* buffer, const size_t length, const char* str) {
  strncat(buffer, str, length);

  size_t written = strlen(str);
  return (length > written) ? written : length;
}

char internalBuf[5];

int date_to_words(int day, int month, int year, char *words, size_t length) {
  memset(words, 0, length);
  size_t remaining = length;
  memset(internalBuf, 0, 5);
  snprintf(internalBuf, 5, "%d", day);
  remaining -= append_string(words, remaining, internalBuf);
  remaining -= append_string(words, remaining, ". ");
  remaining -= append_string(words, remaining, MONATE[month]);
  remaining -= append_string(words, remaining, " ");
  memset(internalBuf, 0, 5);
  snprintf(internalBuf, 5, "%d", year);
  remaining -= append_string(words, remaining, internalBuf);
  return remaining;
}

int fuzzy_time_to_words(int hours, int minutes, char* words, size_t length) {
  int fuzzy_hours = hours;
  int fuzzy_minutes = minutes;
  int morgens = 1;
  if ( minutes >= 24 ){
    fuzzy_hours++;
  }
  if(fuzzy_hours>=12){
    fuzzy_hours-=12;
    morgens=0;
  }


  size_t remaining = length;
  memset(words, 0, length);

  if(fuzzy_minutes == 0){
    remaining -= append_string(words, remaining, PREFIX[0]);
  }else if(fuzzy_minutes <= 5){
    remaining -= append_string(words, remaining, PREFIX[1]);
  }else if(fuzzy_minutes <= 7){
    remaining -= append_string(words, remaining, PREFIX[2]);
  }else if(fuzzy_minutes <= 12){
    remaining -= append_string(words, remaining, PREFIX[3]);
  }else if(fuzzy_minutes <= 17){
    remaining -= append_string(words, remaining, PREFIX[4]);
  }else if(fuzzy_minutes <= 23){
    remaining -= append_string(words, remaining, PREFIX[5]);
  }else if(fuzzy_minutes <= 26){
    remaining -= append_string(words, remaining, PREFIX[6]);
  }else if(fuzzy_minutes < 30){
    remaining -= append_string(words, remaining, PREFIX[7]);
  }else if(fuzzy_minutes == 30){
    remaining -= append_string(words, remaining, PREFIX[8]);
  }else if(fuzzy_minutes <= 32){
    remaining -= append_string(words, remaining, PREFIX[9]);
  }else if(fuzzy_minutes == 35){
    remaining -= append_string(words, remaining, PREFIX[10]);
  }else if(fuzzy_minutes <= 42){
    remaining -= append_string(words, remaining, PREFIX[11]);
  }else if(fuzzy_minutes <= 47){
    remaining -= append_string(words, remaining, PREFIX[12]);
  }else if(fuzzy_minutes <= 54){
    remaining -= append_string(words, remaining, PREFIX[13]);
  }else if(fuzzy_minutes == 55){
    remaining -= append_string(words, remaining, PREFIX[14]);
  }else {
    remaining -= append_string(words, remaining, PREFIX[15]);
  }

  
    
  
  remaining -= append_string(words, remaining, " ");
  if( (fuzzy_hours%12 == 0) && (morgens)){
    remaining -= append_string(words, remaining, "mitternacht" );
  }else{
    remaining -= append_string(words, remaining, STUNDEN[fuzzy_hours % 12] );
  }
  if(CONF_MORGENS){
    remaining -= append_string(words, remaining, " ");
    remaining -= append_string(words, remaining, AMPM[morgens] );
  }
  if(CONF_KOLJA){
    remaining -= append_string(words, remaining, " kolja!");
  }

  return remaining;
}

/*
void find_max_buf(void){
  char *str = malloc(35);
  int h=0;
  int m=0;
  int max=0;
  int min=256;
  for(h=0; h<=24; h++){
    for(m=0; m<60; m++){
      int r = fuzzy_time_to_words(h, m, str, 35);
      printf("Remaining: %d  for %s\n", r, str);
      if(r<min) min=r;
      if(strlen(str)>max) max = strlen(str);
    }}

  printf("Max: %d, Min remaining: %d\n", max, min);
}

int main(void){
  find_max_buf();
}
*/
/*
  char *str = malloc(256);
  
  fuzzy_time_to_words(6, 23, str, 256);
  printf("%s\n", str);
  fuzzy_time_to_words(0, 2, str, 256);
  printf("%s\n", str);
  fuzzy_time_to_words(12, 13, str, 256);
  printf("%s\n", str);
  fuzzy_time_to_words(18, 8, str, 256);
  printf("%s\n", str);
  fuzzy_time_to_words(23, 54, str, 256);
  printf("%s\n", str);
  fuzzy_time_to_words(23, 56, str, 256);
  printf("%s\n", str);
  fuzzy_time_to_words(1, 1, str, 256);
  printf("%s\n", str);
  fuzzy_time_to_words(7, 13, str, 256);
  printf("%s\n", str);
  fuzzy_time_to_words(7, 11, str, 256);
  printf("%s\n", str);

  fuzzy_time_to_words(0, 0, str, 256);
  printf("%s\n", str);
}

**/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "scanner.h"
#include "error_codes.h"
#include "str.h"

int main(int argc, char **argv)
{
  FILE *f;
  if (argc != 2)
  {
    printf("wrong arguments\n");
    return INTER_ERROR;
  }
  if ((f = fopen(argv[1], "r")) == NULL)
    {
      printf("Soubor se nepodarilo otevrit\n");
      return INTER_ERROR;
    }
    
    setSourceFile(f);
    
    token smth;
    //printf("%d \n", TO_EQUAL);

    while (1)
    {
    //printf ("%d\n", TO_MULTIPLY);
    smth = getToken();
    printf ("%d \n", smth.type);
    if (smth.type == T_INT)
    {
      printf ("%d \n", smth.attribute.inumber);
    }
    else if (smth.type == T_ERROR)
    {
      printf("%d \n", smth.attribute.inumber);
      fclose(f);
      return smth.attribute.inumber;
    }
    else if (smth.type == T_EOF)
      break;
    else if (smth.type == T_STRING || smth.type == T_ID || smth.type == T_QUALID)
    {
      printf("%s\n", smth.attribute.str);
      free(smth.attribute.str);
    }
    else if (smth.type == T_DOUBLE)
    {
      printf("%f\n", smth.attribute.dnumber);
    }
    else
    {
      ;
    }
    }

    fclose(f);
}

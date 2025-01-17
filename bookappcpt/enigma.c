/*
 * engima simulation
 * 
 * author: Henry Tieman
 * 
 * references: 
 *     "How Polish Mathematicians Deciphered the Enigma", Marian Rejewski,
 *     Annals of the History of Computing, Vol 3, no 3, July 1981, Pg 213 ff
 *     appendix by C. A. Devours.
 *
 *     "Machine Cryptography and Modern Cryptanalysis", C.A. Deavours and L.
 *     Kurth, Artech House, Dedham, Mass 1985.
 */

#include <stdio.h>
#include <ctype.h>
#include <string.h>

#ifndef TRUE
#define TRUE  1
#define FALSE 0
#endif

#define LINE_LEN 80

/*
 * rotor data
 *       reference "Machine Cryptography and Modern Cryptanalysis" pg. 100
 *
 * note: rotor stepping is associated with each @PROGCODE = rotor instead
 *       of position or being constant.
 */

#define NUM_ROTORS 5

char ref_rotor[27] = "YRUHQSLDPXNGOKMIEBFZCWVJAT";

char rotor[NUM_ROTORS][27] = { /* pre defined rotors */
  "EKMFLGDQVZNTOWYHXUSPAIBRCJ",
  "AJDKSIRUXBLHWTMCQGZNPYFVOE",
  "BDFHJLCPRTXVZNYEIWGAKMUSQO",
  "ESOVPZJAYQUIRHXLNFTGKDCMWB",
  "VZBRGITYUPSDNHLXAWMJQOFECK",
};

int step_data[NUM_ROTORS] = {
  16,  4, 21,  9, 25           /* steps at: q, e, v, j, z */
};

/* 
 * engima key default settings
 */

int order[3] = { 0, 1, 2};     /* rotor order, user input is +1 */
char ring[8] = {               /* ring settings */
  '\0', 'A',  'A',  'A',       /* default: AAA */
  '\0', '\0', '\0', '\0' };

int n_plugs = 0;               /* number of plugs */
char plugs[80] = "";           /* plug string */
int pos[3] = { 0, 0, 0 };      /* rotor positions */

/*
 * simulation data and machine state data 
 */

int data[8][26];               /* working array for machine */
int step[3];                   /* steps corresponding to rotors */
int double_step;               /* rotor 2 step twice */

/*
 * encipher - C implementation of the engima cipher function
 */

int
encipher(int c)
{
  int j;                        /* index for counting */
  int idx;                      /* rotor index */

  if (isalpha(c))
    {

      pos[0] = (pos[0] + 1) % 26;  /* first, advances the rotors */
      if (pos[0] == step[0])
        pos[1] = (pos[1] + 1) % 26;
      if (double_step)
        {
          pos[1] = (pos[1] + 1) % 26;
          pos[2] = (pos[2] + 1) % 26;
          double_step = FALSE;
        }
      if (pos[1] == step[1])
        double_step = TRUE;

      c -= 'A';                 /* start to encipher */
      if (n_plugs != 0)
        c = data[0][c];
      for (j=0;j<3;j++)         /* do rotors forward */
        {
          idx = (c + pos[j]) % 26;
          c   = (c + data[j+1][idx]) % 26;
        }
      c = (data[4][c]) % 26;    /* reflecting rotor */
      for (j=0;j<3;j++)         /* do rotors reverse */
        {
          idx = (c + pos[2-j]) % 26;
          c   = (c + data[j+5][idx]) % 26;
        }
      if (n_plugs != 0)
        c = data[0][c];
      c += 'A';
    }
  return(c);
}

/*
 * encipher_file - open and encipher a file
 */

void
encipher_file(char *file_name)
{
  FILE *fp;                     /* plaintext/ciphertext FILE pointer */
  char line[LINE_LEN + 1];      /* input data line, inc. '\n' */
  char *ret_val;                /* value from fgets(), used for EOF check */
  char c;                       /* character from data line */
  int len;                      /* length of data line */
  int idx;                      /* index/counter */

  fp = fopen(file_name, "r");
  ret_val = fgets(line, LINE_LEN, fp);
  while(ret_val != NULL)
    {
      len = strlen(line);
      for (idx=0;idx<len;idx++)
        {
          c = line[idx];
          if (isalpha(c))
            {
              c = encipher((int)(toupper(c)));
              line[idx] = c;
            }
        }
      printf("%s", line);
      ret_val = fgets(line, LINE_LEN, fp);
    }
  fclose(fp);
}

/*
 * init_mach - set up data according to the input data
 */

void
init_mach( void )
{
  int i, j;                      /* indexes */
  int ds;                        /* used during ring settings */
  int u, v;                      /* temps for plugboard input */

  /* setup rotor data */
  for (j=0;j<26;j++)
    data[4][j] = ((int)ref_rotor[j]-'A'+26)%26;

  for (i=1;i<4;i++)
    {
      step[i-1] = step_data[order[i-1]];
      for (j=0;j<26;j++)
        {
          data[i][j] = ((int)(rotor[order[i-1]][j])-'A' + 26) % 26;
          data[8-i][data[i][j]] = j;
        }
    }

  /* setup ring settings */
  ring[7] = ring[1];
  ring[6] = ring[2];
  ring[5] = ring[3];
  for (i=1;i<8;i++)
    if (i != 4)
      {
        ds = (int)(ring[i]) - 'A';
        if (ds != 0)
          {
            for (j=0;j<26;j++)
              data[0][j] = data[i][j];
            for (j=0;j<26;j++)
              data[i][j] = data[0][(26-ds+j)%26];
          }
      }

  /* setup plug data */
  if (n_plugs != 0)
    {
      j = 0;
      for (i=0;i<26;i++)
        data[0][i] = i;
      for (i=0;i<n_plugs;i++)
        {
          while (!isalpha(plugs[j]))
            {
              j++;
              if (plugs[j] == '\0')
                break;
            }
          u = toupper(plugs[j++]) - 'A';
          v = toupper(plugs[j++]) - 'A';
          data[0][u] = v;
          data[0][u] = u;
        }
    }

  /* convert all moving rotor data to displacements */
  for (i=1;i<8;i++)
    {
      if (i!=4)
        for (j=0;j<26;j++)
          data[i][j] = (data[i][j] - j + 26) % 26;
    }

  /* setup rotor starting positions */
  double_step = FALSE;              /* no previous rotor position */
  /* input function has already done the rotor positions */
}

/* 
 * read_keyfile - a simple function to read in the key file 
 */

void
read_keyfile( char *str )
{
  FILE *kf;                         /* input key FILE pointer */
  int num;                          /* dummy returned from fscanf() */
  int idx;                          /* index/counter */
  char a[3];                        /* dummy for input */

  kf = fopen(str, "r");
  num = fscanf(kf, "%d %d %d\n", &order[0], &order[1], &order[2]);
  num = fscanf(kf, "%c %c %c\n", &ring[1], &ring[2], &ring[3]);
  num = fscanf(kf, "%d\n", &n_plugs);

  if (n_plugs != 0)
    num = fscanf(kf, "%[^\n]\n", plugs);
  num = fscanf(kf, "%c %c %c\n", &a[0], &a[1], &a[2]);

  for (idx = 0; idx < 3; idx++)
    {
      (order[idx])--;
      ring[idx+1] = toupper(ring[idx+1]);
      pos[idx] = toupper(a[idx]) - 'A';
    }
  fclose(kf);
}

/*
 * usage - function to print out the correct usage of the program
 */

void
usage( char *str )
{
  fprintf(stderr, "usage: %s [<keyfile>] <infile>\n\n", str);
  fprintf(stderr, "\tkeyfile has the form:\n");
  fprintf(stderr, "\t\tn n n\t\t- for rotor order, 1 <= n <= 5\n");
  fprintf(stderr, "\t\tx x x\t\t- for ring settings, x alpha\n");
  fprintf(stderr, "\t\tn\t\t- number of plugs, 0 <= n <= 13\n");
  fprintf(stderr, "\t\txx xx xx ...\t- plug letter pairs, one pair"
          " for each n\n");
  fprintf(stderr, "\t\tx x x\t\t- initial rotor position, x alpha\n\n");
  fprintf(stderr, "\toutput is stdout\n");
}

/*
 * main - the main function, nothing special here
 */

int
main( int argc, char ** argv )
{
  char *infile;            /* plaintext/ciphertext file name ptr */

  if ((argc < 2) || (argc > 3))
    {
      usage(argv[0]);
      return 1;
    }

  if (argc == 2)
    {
      infile = argv[1];
    }
  else
    {
      infile = argv[2];
      read_keyfile(argv[1]);
    }

  init_mach();
  encipher_file(infile);

  return 0;
}

/*
 * end of engima simulation
 */


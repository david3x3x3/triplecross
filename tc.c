#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdint.h>

uint32_t *perma2num, *num2perma;
uint32_t *permb2num, *num2permb;
unsigned char num2permc[720][7];
uint32_t permc_tbl1[128], permc2num[46656];
char permc_tbl2[] = "1234ab";
uint32_t counta, countb, countc=0, startnum;

/* Function to swap values at two pointers */
void
swap(char* x, char* y) {
    char temp;
    temp = *x;
    *x = *y;
    *y = temp;
}
 
/* Function to print permutations of string
This function takes three parameters:
1. String
2. Starting index of the string
3. Ending index of the string. */

void
permute(char* a, uint32_t l, uint32_t r) {
  uint32_t i, j;
  if (l == r) {
    strcpy(num2permc[countc], a);
    for(i=j=0;i<6;i++) {
      j = j*6 + permc_tbl1[a[i]];
    }
    permc2num[j] = countc;
    countc++;
    //printf("%s\n", a);
  } else {
    for (i = l; i <= r; i++) {
      swap((a + l), (a + i));
      permute(a, l + 1, r);
      swap((a + l), (a + i)); // backtrack
    }
  }
}

uint32_t
next_set(uint32_t x) {
   uint32_t smallest, ripple, new_smallest, ones;

   if (x == 0) return 0;
   smallest     = (x & -x);
   ripple       = x + smallest;
   new_smallest = (ripple & -ripple);
   ones         = ((new_smallest/smallest) >> 1) - 1;
   return ripple | ones;
}

int
make_tables(uint32_t n, uint32_t m, uint32_t **tablein, uint32_t **tableout) {
  uint32_t i, num;
  unsigned x;
  
  num = 0;
  for(x = (1<<n)-1; x < (1<<m); x = next_set(x)) {
    num++;
  }
  *tablein = calloc(1<<m, sizeof(uint32_t));
  *tableout = calloc(num, sizeof(uint32_t));
  num = 0;
  for(x = (1<<n)-1; x < (1<<m); x = next_set(x)) {
    (*tablein)[x] = num;
    (*tableout)[num] = x;
    num++;
  }
  return num;
}

//    ab
//cdefghi
//jklmnop
// qr

char *mvnames[] = { "rucd", "lucd", "urdc", "uldc" };
char *mvseq0[][2] = {
  {"cdelrqj", "bafmnoh"},
  {"fedkqrm", "abipong"},
  {"jqrledc", "honmfab"},
  {"mrqkdef", "gnopiba"}};

// move0() helps create mvseq[] from mvseq0[]
char *
move0(unsigned char *s, uint32_t n) {
  unsigned char *s2 = malloc(18);
  uint32_t i, j;
  
  for(i=0;i<18;i++) {
    s2[i] = s[i];
  }
  for(j=0;j<2;j++) {
    for(i=0;i<7;i++) {
      s2[mvseq0[n][j][(i+1)%7]-'a'] = s[mvseq0[n][j][(i)%7]-'a'];
    }
  }
  return s2;
}

unsigned char *mvseq[4];

// take a move number and make s2 as the representation of the puzzle
// state after performing the move on s
char *
move(char *s, char *s2, uint32_t n) {
  uint32_t i, j;
  //printf("move(%d)\n", n);
  for(i=0;i<18;i++) {
    //printf("%d comes from %d\n", i, mvseq[n][i]);
    s2[i] = s[mvseq[n][i]];
  }
  s2[18] = '\0';
  return s2;
}

void
prt(char *s) {
  uint32_t i;
  printf("    %c%c\n", s[0], s[1]);
  for(i=0;i<7;i++) {
    printf("%c", s[i+2]);
  }
  printf("\n");
  for(i=0;i<7;i++) {
    printf("%c", s[i+9]);
  }
  printf("\n");
  printf(" %c%c\n\n", s[16], s[17]);
}

unsigned char *dists;

// get a 2-bit number from an array of bytes containing 4 2-bit numbers
int
distget(uint32_t n) {
  return 3 & (dists[n/4]>>((n%4)*2));
}

// save a 2-bit number in an array of bytes containing 4 2-bit numbers
void
distput(uint32_t n, uint32_t newval) {
  uint32_t n1 = n/4;
  uint32_t n2 = n%4;
  dists[n1] = ((0xff^(3<<(2*n2))) & dists[n1]) ^ (newval<<(2*n2));
}

// convert a number into a string representing a position (inverse of pos2num)
char *
num2pos(uint32_t n, char *res) {
  uint32_t a, b, bn;
  int32_t an;
  char *c, *res0=res;
  
  c = num2permc[n%720]; n /= 720;
  b = num2permb[n%84];  n /= 84;
  a = num2perma[n];
  
  bn = 8;
  for (an=17;an>=0;an--) {
    if (((1<<an) & a) != 0) {
      if (((1<<bn) & b) != 0) {
	*res++ = *c++;
      } else {
	*res++ = '-';
      }
      bn--;
    } else {
      *res++ = '.';
    }
  }
  *res = '\0';
  return res0;
}

// convert the string representation of a position into a distinct
// number between 0 and the number of possible positions
int
pos2num(char *s) {
  uint32_t a=0, b=0, c=0, i;

  while(*s) {
    if (*s == '.') {
      a = a*2;
    } else if(*s == '-') {
      a = a*2 + 1;
      b = b*2;
    } else {
      a = a*2 + 1;
      b = b*2 + 1;
      c = c * 6 + permc_tbl1[*s];
    }
    s++;
  }
  return (perma2num[a]*84+permb2num[b])*720+permc2num[c];
}

// the 7 positions that are furthest (24 moves) from solved
uint32_t hardest[] = {
  235789132,  375314002, 1380654175, 1785985569,
  1864000965, 2660541737, 2723912530 };

// take the number of a position and print the moves to return to the
// starting position
void
solve(uint32_t num) {
  uint32_t i, num2, d, d2;
  unsigned char buf[19], buf2[19];

  d = distget(num);
  while(num != startnum) {
    num2pos(num, buf);
    for(i=0;i<4;i++) {
      num2 = pos2num(move(buf, buf2, i));
      d2 = distget(num2);
      if((int[]){999, 3, 1, 2}[d] == d2) {
	printf("%s ", mvnames[i]);
	num = num2;
	d = d2;
	break;
      }
    }
  }
  printf("\n");
}

int
main(int argc, char *argv[]) {
  uint32_t i, j, k, depth, count, total=0, done, num, num2, d, d2;
  unsigned char buf[19], buf2[19], start[19];
  time_t start_time, total_time;
  FILE *fpin, *fpout;

  start_time = time(NULL);
  
  // build permutation tables
  counta = make_tables(9, 18, &perma2num, &num2perma);
  countb = make_tables(6,  9, &permb2num, &num2permb);
  for(i=0;i<6;i++) {
    permc_tbl1[permc_tbl2[i]] = i;
  }
  strcpy(buf, permc_tbl2);
  permute(buf, 0, strlen(buf)-1);
  //printf("counta = %d\n", counta);
  //printf("countb = %d\n", countb);
  //printf("countc = %d\n", countc);
  //printf("distsize = %u/4 = %u\n",
  //       counta*countb*countc, (counta*countb*countc)/4);
  dists = calloc((counta*countb*countc)/4, sizeof(unsigned char));

  // build mvseq arrays
  for(j=0;j<4;j++) {
    for(i=0;i<18;i++) {
      buf[i] = i;
    }
    mvseq[j] = move0(buf, j);
  }

  strcpy(buf, "..---..12.....34ba");
  i = pos2num(buf);
  //printf("start = %u\n", i);
  //fflush(stdout);
  distput(i, 1);

  if (fpin = fopen("dists.bin", "rb")) {
    fread(dists, 1, (counta*countb*countc)/4, fpin);
    fclose(fpin);
  } else {
    fpout = fopen("prev0.txt", "w");
    fprintf(fpout, "%u\n", i);
    fclose(fpout);
    depth=1;
    while(1) {
      count=0;
      done=1;
      sprintf(buf, "prev%d.txt", (depth+1)%2);
      fpin = fopen(buf, "r");
      sprintf(buf, "prev%d.txt", depth%2);
      fpout = fopen(buf, "w");
      fgets(buf, sizeof(buf), fpin);
      while(1) {
	if(feof(fpin)) {
	  break;
	}
	buf[strlen(buf)-1] = '\0';
	i = strtoul(buf, NULL, 10);
	count++;
	num2pos(i, buf);
	for(j=0;j<4;j++) {
	  move(buf, buf2, j);
	  k = pos2num(buf2);
	  if(distget(k) == 0) {
	    done=0;
	    distput(k, (depth%3)+1);
	    fprintf(fpout, "%u\n", k);
	  }
	}
	fgets(buf, sizeof(buf), fpin);
      }
      fclose(fpin);
      fclose(fpout);
      total += count;
      total_time = time(NULL)-start_time;
      printf("depth %2u - count %10u total %10u %u:%02u:%02u\n", depth-1, count,
	     total, total_time/3600, (total_time/60)%60, total_time%60);
      fflush(stdout);
      depth++;
      if(done) {
	break;
      }
    }
    fpout = fopen("dists.bin", "wb");
    i = fwrite(dists, 1, (counta*countb*countc)/4, fpout);
    fclose(fpout);
  }
  startnum = pos2num("..---..12.....34ba");
  if (argc > 1) {
    solve(pos2num(argv[1]));
  } else {
    for(j=0;j<7;j++) {
      num = hardest[j];
      prt(num2pos(num, buf));
      solve(num);
      printf("\n");
    }
  }
  exit(0);
}

   #include <stdio.h>
   #include <math.h>
   #include <time.h>
   
   #define len 100
   #define maxname 40
   
   // 0
   float map[len];
   float diff[len];
   float sroots[len];
   float qroots[len];
   
   // staff
   int writefile(char path[maxname], void (*worker)(FILE*)) {
       FILE *fptr = fopen(path, "w+");
       if (fptr == NULL) {
           printf("Could not open file\n");
           for(int i = 0; i < 5; i++) { printf("\n !?"); }
           return 0;
           }
       else {
           worker(fptr);
           fclose(fptr);
           return 1;
           }
       };

   void showtime(float (*expression)(float), int depth, char name[maxname], int shms) {
       clock_t begin, end;
       double time_spent;
       begin = clock();
       for(int i = 0; i < depth; i++) { expression(i); }
       end = clock();
       time_spent = ( (double)(end - begin)/depth );
       if(shms) {
           time_spent /= (CLOCKS_PER_SEC/1000);
           printf("%s average time: %.8f ms\n", name, time_spent);
           }
       else { 
           printf("%s average time: %.8f pt\n", name, time_spent);
           }
       };

   float Q_rsqrt( float number ) {
       int i;
       float x2, y;
       const float threehalfs = 1.5F;
       
       x2 = number * 0.5F;
       y  = number;
       i  = * ( int * ) &y;                       // bit level hacking
       i  = 0x5f3759df - ( i >> 1 );               // what a hell?
       y  = * ( float * ) &i;
       y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st newton iteration
       
       return y;
       };

   float wrap(float x) {
       return 1/ Q_rsqrt(x);
       };

   void CSVwriter(FILE *pointer) {
       fprintf(pointer,"%s\n", "x || abs_accuracy");
       for (int i = 0; i < len; i++) {
           fprintf(pointer,"%d || %f\n", i+1, diff[i]);
           }
       };

   int main(){
       //1
       for(int x = 1; x < len; x++) {
           map[x-1] = x;
           sroots[x-1] = wrap(x);
           qroots[x-1] = Q_rsqrt(x);
           diff[x-1] = 1/sqrtf(x) - Q_rsqrt(x);
           }
       
       //2
       int scs = writefile("res.csv", CSVwriter);
       
       if(scs) { printf("CSV created sucsessfully.\n"); }
       else { printf("Filepath error or something else went wrong.\n"); }
       
       // 3
       showtime(sqrtf, len, "Library", 0);
       showtime(wrap, len, "FISR (Quake 3)", 0);
       
       return 1;
       };

   

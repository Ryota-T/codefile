#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mecab.h>

#define CODE_SIZE 10
#define ARRAY_SIZE 100

char P_code[CODE_SIZE];   // 郵便番号
char Add1[ARRAY_SIZE];    // 都道府県名
char Add2[ARRAY_SIZE];    // 市区町村名
char Add3[ARRAY_SIZE];    // 町域名

char Input[100];          // MeCab入力データ
int Count1 = 0;           // NEologdに含まれる単語数
int Count2 = 0;           // NEologdに含まれない単語数

FILE *FPcsv;              // csvファイル
FILE *FP1;                // NEologdに含まれる単語に関するデータ
FILE *FP2;                // NEologdに含まれない単語に関するデータ

char *Csvname = "all.csv";
char *Fname1 = "Data.txt";
char *Fname2 = "NoData.txt";
int Ret;

/* csvファイルより住所を生成 */
void
str_join(char *str1, char *str2, char *str3)
{
  char *ptr = &Input[0];
  while (*str1++ != '\0') 
    if (*str1 != '\"') *ptr++ = *str1;
  *ptr--;
  while (*str2++ != '\0') 
    if (*str2 != '\"') *ptr++ = *str2;
  *ptr--;
  if (strstr(str3, "\"以下に掲載がない場合\"") == NULL) {
    while (*str3++ != '\0') 
      if (*str3 != '\"') *ptr++ = *str3;
  }
}

/* MeCab実行・ファイルへの書き込み */
void
mecab(char *str, char *p_code, FILE *fp1, FILE *fp2)
{
  MeCab::Tagger *mt = MeCab::createTagger("-Ochasen -d /usr/local/lib/mecab/dic/mecab-ipadic-neologd/");
  const char *res = mt->parse(str);
  //printf("%s\n", res);
  if (strstr(res, str) != NULL) {
    Count1++;
    fprintf(fp1, "Input Data: %s\n", str);
    fprintf(fp1, "Mecab Result:\n%s", res);
    fprintf(fp1, "Postal code: %s\n\n\n", p_code);
  } else {
    Count2++;
    fprintf(fp2, "Input Data: %s\n", str);
    fprintf(fp2, "Mecab Result:\n%s", res);
    fprintf(fp2, "Postal code: %s\n\n\n", p_code);
  }
  delete mt;
}

int
main(void)
{
  /* ファイルオープン */
  FPcsv = fopen(Csvname, "r");
  FP1 = fopen(Fname1, "w");
  FP2 = fopen(Fname2, "w");
  if (FPcsv == NULL) {
    printf("csv file open error./n");
    exit(1);
  }
  if ((FP1 == NULL) || (FP2 == NULL)) { 
    printf("write file open error./n");
    exit(1);
  }

  /* csvファイルの読み込み・MeCab実行 */ 
  while ((Ret = fscanf(FPcsv, "%*d, %*[^,], %[^,], %*[^,], %*[^,], %*[^,], %[^,], %[^,], %[^,], %*d, %*d, %*d, %*d, %*d, %*d", P_code, Add1, Add2, Add3)) != EOF) {
    str_join(Add1, Add2, Add3);
    mecab(Input, P_code, FP1, FP2);
  }

  /* メタ情報総数の書き込み */
  fprintf(FP1, "Total Number of Data: %d\n\n", Count1);
  fprintf(FP2, "Total Number of Data: %d\n\n", Count2);
  
  /* ファイルクローズ */
  fclose(FPcsv);  fclose(FP1);  fclose(FP2);

  return 0;
}

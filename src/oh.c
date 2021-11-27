#include "user.h"
#include <time.h>

DIR_LIST* dirfound(char* fdir);
char* prtpwd();
int cntfound();
int acc_inode();
int acc_data();

/*
?΄λ¦?    : mycd ?¨?
??±?  : ?€κ·λΉ
κΈ°λ₯    : ?? ?? ? λ¦¬μ κ²½λ‘λ₯? λ³?κ²½ν?€
λ°λκ°?  : κ²½λ‘ λ¬Έμ?΄
λ¦¬ν΄κ°?  : X
*/
void mycd (char* path)
{
    if(path == NULL)
    {//path ?Έ?κ°? ???
        rear_dir_list_ptr == front_dir_list_ptr;//?? ?? ?°λ¦¬λ?? ??? ?°λ¦¬λ‘ ?€? 
    }

    else if (path != NULL)
    {
        int first = 0;//μ²? / ??Ό?΄κΈ?
        DIR_LIST *tmp_ptr;
        char *nm_ptr = strtok(path, "/");
        while (nm_ptr != NULL)//?΄λ¦μ΄ NULLκ°μ΄ ???Όλ©?
        {
         if (first == 0)//???? ? λ¦¬κ?? . , .. , /? μ€? ??  . ??? ..?? ??? ?? κ²½μ° ??¨
         {
          if (nm_ptr == ".") //.?? ??
          {
          tmp_ptr = rear_dir_list_ptr;
          nm_ptr = strtok(NULL, "/");
          first++;
          }
          else if (nm_ptr == "..")//..?? ??
          {
           int i = 0;
           tmp_ptr = front_dir_list_ptr;
           while (i < cntfound()-1){
               tmp_ptr = tmp_ptr -> next_ptr;
           } // tmp_ptr?΄ ??¬ ?? ?°λ¦? λ¦¬μ€?Έ?? (λ§μ??λ§?-1)λ²μ§Έ ?? ? λ¦¬λ?? κ°?λ¦¬ν€?λ‘? ?¨.
           tmp_ptr = rear_dir_list_ptr;
           nm_ptr = strtok(NULL, "/");
           first++;
          }
         }
         else if (first != 0)
         {
          DIR_LIST* new_dir = malloc(sizeof(DIR_LIST)); 
          new_dir-> name = nm_ptr;
          char* s = strcat("/",nm_ptr);//s = /'?€? ?? ? λ¦?'
          new_dir-> inode = path_to_inode(strcat(prtpwd(),s));
          tmp_ptr -> next_ptr = new_dir;
          nm_ptr = strtok(NULL, "/");
         }
        }
    }
}

/*
?΄λ¦?    : mycpto ?¨?
??±?  : ?€κ·λΉ
κΈ°λ₯    : MY ??Ό??€?? ?? ??Ό? ?Έ?€?Έ μ»΄ν¨?°λ‘? λ³΅μ¬??€.
λ°λκ°?  : ??Όλͺ? 1,2
λ¦¬ν΄κ°?  : λ¦¬ν΄κ°?
*/
void mycpto (const char* source_file, const char* dest_file  )
{
    FILE *ofp;
    FILE *myfs;
    int c,d;
    int i = 0;
    int inode = 1;

    char *tmp_file_string_ptr = (char *)malloc(sizeof(char) * 8); //?? ? λ¦¬μ datablock?? μΆμΆ? ??Όλͺμ κ°?λ¦¬ν¬ ?¬?Έ?°
    int *tmp_inode_ptr = (int *)malloc(sizeof(int)); //?? ? λ¦¬μ datablock?? μΆμΆ? inode λ²νΈλ₯? κ°?λ¦¬ν¬ ?¬?Έ?°
    INODE *inode_ptr = (INODE *)malloc(sizeof(INODE));// inode ?¬?Έ?°

    myfs = fopen("myfs", "rb");
    fseek(myfs, BOOT_BLOCK_SIZE+SUPER_BLOCK_SIZE+(sizeof(INODE)*(path_to_inode(prtpwd())-1)),SEEK_SET);//??¬ ?? ? λ¦¬μ ?°?΄?°λΈλ‘ ??Όλ‘? ?¬?Έ?° ?΄?
    fread(inode_ptr, sizeof(INODE), 1, myfs);
    int tmp_datablock = (int)(inode_ptr->dir_1 + 1);
    fread(tmp_file_string_ptr, sizeof(char) * 8, 1, myfs); // ?¬?Έ?°λ‘? ??Όλͺ? ??Έ

    while(!strcmp(tmp_file_string_ptr, source_file))
    {//μ°Ύλ ??Όκ³? ??Έ? ??Ό? ?΄λ¦μ΄ κ°μ?? ??€λ©?
        if((c = getchar()) != EOF)
        {//myfs??Ό? ? ??Έ
            fseek(myfs, -1, SEEK_CUR);
            fseek(myfs,sizeof(int),SEEK_CUR);//??¬ ?¬?Έ?° ?μΉλ‘λΆ??° int? ?¬κΈ°λ§?Ό ?΄?
            fread(tmp_file_string_ptr, sizeof(char) * 8, 1, myfs);
        }
        else
        {
        fprintf(stderr,"?€λ₯? : %s ??Ό?΄ ??΅??€.\n", source_file); //??Όλͺμ ??¬ ?? ? λ¦¬μ? λͺ»μ°Ύ?Όλ©? ?€λ₯? ??°κΈ?
        exit(1);
        }
    }
    fread(tmp_inode_ptr, sizeof(int), 1, myfs);
    inode =*tmp_inode_ptr;

    if ((ofp = fopen(dest_file, "wb")) == NULL)
    {
        fprintf(stderr, "?€λ₯? : %s ??Ό? ?΄ ? ??΅??€. \n", source_file);
        exit(1);
    }

    fseek(myfs, BOOT_BLOCK_SIZE+SUPER_BLOCK_SIZE+(sizeof(INODE)*128)+(DATA_BLOCK_SIZE*((inode_ptr->dir_1)- 1)),SEEK_SET);//?λ‘μ΄ ??Ό? λ³΅μ¬
    while(i != DATA_BLOCK_SIZE)
    {
        d = getchar();
        putc(d,ofp);
        i++;
    }
    free(tmp_file_string_ptr);
    free(tmp_inode_ptr);
    free(inode_ptr);
    fclose(ofp);
    fclose(myfs);

    return;
}

/*
?΄λ¦?    : mycpfrom ?¨?
??±?  : ?€κ·λΉ
κΈ°λ₯    : ?Έ?€?Έ μ»΄ν¨?°? ?? ??Ό? MY ??Ό??€??Όλ‘? λ³΅μ¬??€.
λ°λκ°?  : ??Όλͺ? 1,2
λ¦¬ν΄κ°?  : λ¦¬ν΄κ°?
*/

void mycpfrom (const char* source_file, const char* dest_file )
{
    FILE *ifp;
    FILE *myfs;
    int c,size_F;
    time_t Time;
    struct tm* TimeInfo;
 
    Time = time(NULL);                // ??¬ ?κ°μ λ°μ
    TimeInfo = localtime(&Time); 

    myfs = fopen("myfs", "wb");
    char *tmp_dir_string_ptr = (char *)malloc(sizeof(char) * 8); //?? ? λ¦¬μ datablock?? μΆμΆ? ?? ? λ¦¬λͺ? κ°?λ¦¬ν¬ ?¬?Έ?°
    int *tmp_inode_ptr = (int *)malloc(sizeof(int)); //?? ? λ¦¬μ datablock?? μΆμΆ? inode λ²νΈλ₯? κ°?λ¦¬ν¬ ?¬?Έ?°
    INODE *inode_data_ptr = (INODE *)malloc(sizeof(INODE));
    
    int new_inode = acc_inode();
    int new_data = acc_data();
    ifp = fopen(source_file,"rb");
    fseek(myfs, BOOT_BLOCK_SIZE+SUPER_BLOCK_SIZE+(sizeof(INODE)*128)+(DATA_BLOCK_SIZE*(new_data-1)),SEEK_SET);//?λ‘μ΄ ??Ό? λ³΅μ¬
    while ((c = getc(ifp)) != EOF)
    {
      size_F++;
      putchar(c);
    }

    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + (sizeof(INODE) * (new_inode - 1)),SEEK_SET);//INODELIST μ±μ°κΈ?
    fread(inode_data_ptr, sizeof(INODE), 1, myfs);
    inode_data_ptr -> type = 0;
    inode_data_ptr -> year = TimeInfo ->tm_year+1900;
    inode_data_ptr-> size = size_F;
    inode_data_ptr -> month = TimeInfo ->tm_mon+1;
    inode_data_ptr -> date = TimeInfo ->tm_mday;
    inode_data_ptr -> hour = TimeInfo ->tm_hour;
    inode_data_ptr -> minute = TimeInfo ->tm_min;
    inode_data_ptr -> second = TimeInfo ->tm_sec;
    inode_data_ptr -> dir_1 = new_data;    
    inode_data_ptr -> dir_2 = 0;
    inode_data_ptr -> dir_3 = 0;
    inode_data_ptr -> dir_4 = 0;
    inode_data_ptr -> dir_5 = 0;
    inode_data_ptr -> dir_6 = 0;
    inode_data_ptr -> dir_7 = 0;
    inode_data_ptr -> dir_8 = 0;
    inode_data_ptr -> indir = 0; 
    fwrite(inode_data_ptr,sizeof(INODE),1,myfs);

    

    fclose(ifp);
    fclose(myfs);

    return;
}

/*
?΄λ¦?    : mycp ?¨?
??±?  : ?€κ·λΉ
κΈ°λ₯    : MY ??Ό??€?? ?? ??Ό? λ³΅μ¬?? λͺλ Ή?΄
λ°λκ°?  : ??Όλͺ? 1,2
λ¦¬ν΄κ°?  : λ¦¬ν΄κ°?
*/

void mycp(const char* source_file, const char* dest_file  )
{
    FILE *myfs;
    int c,size_F;;
    int i = 0;
    int inode = 1;
    char* d;
    time_t Time;
    struct tm* TimeInfo;
 
    Time = time(NULL);                // ??¬ ?κ°μ λ°μ
    TimeInfo = localtime(&Time); 

    char *tmp_file_string_ptr; //?? ? λ¦¬μ datablock?? μΆμΆ? ??Όλͺμ κ°?λ¦¬ν¬ ?¬?Έ?°
    int *tmp_inode_ptr = (int *)malloc(sizeof(int)); //?? ? λ¦¬μ datablock?? μΆμΆ? inode λ²νΈλ₯? κ°?λ¦¬ν¬ ?¬?Έ?°
    INODE *inode_data_ptr = (INODE *)malloc(sizeof(INODE));// inode ?¬?Έ?°
    char *tmp_data_string; //?? ? λ¦¬μ datablock?? μΆμΆ? ?°?΄?°λ₯? κ°?λ¦¬ν¬ ?¬?Έ?°

    myfs = fopen("myfs", "rb");
    fseek(myfs, BOOT_BLOCK_SIZE+SUPER_BLOCK_SIZE+(sizeof(INODE)*(path_to_inode(prtpwd())-1)),SEEK_SET);//??¬ ?? ? λ¦¬μ ?°?΄?°λΈλ‘ ??Όλ‘? ?¬?Έ?° ?΄?
    fread(inode_data_ptr, sizeof(INODE), 1, myfs);
    int tmp_datablock = (int)(inode_data_ptr->dir_1 + 1);
    fread(tmp_file_string_ptr, sizeof(char) * 8, 1, myfs); // ?¬?Έ?°λ‘? ??Όλͺ? ??Έ

    while(!strcmp(tmp_file_string_ptr, source_file))
    {//μ°Ύλ ??Όκ³? ??Έ? ??Ό? ?΄λ¦μ΄ κ°μ?? ??€λ©?
        if((c = getchar()) != EOF)
        {//myfs??Ό? ? ??Έ
            fseek(myfs, -1, SEEK_CUR);
            fseek(myfs,sizeof(int),SEEK_CUR);//??¬ ?¬?Έ?° ?μΉλ‘λΆ??° int? ?¬κΈ°λ§?Ό ?΄?
            fread(tmp_file_string_ptr, sizeof(char) * 8, 1, myfs);
        }
        else
        {
        fprintf(stderr,"?€λ₯? : %s ??Ό?΄ ??΅??€.\n", source_file); //??Όλͺμ ??¬ ?? ? λ¦¬μ? λͺ»μ°Ύ?Όλ©? ?€λ₯? ??°κΈ?
        exit(1);
        }
    }
    fread(tmp_inode_ptr, sizeof(int), 1, myfs);
    inode =*tmp_inode_ptr;

    fseek(myfs, BOOT_BLOCK_SIZE+SUPER_BLOCK_SIZE+(sizeof(INODE)*128)+(DATA_BLOCK_SIZE*((inode_data_ptr->dir_1)- 1)),SEEK_SET);//λ¬Έμ?΄ ?¬?Έ?°? λ³΅μ¬?΄?κΈ?
    while(i != DATA_BLOCK_SIZE)
    {
        d = getchar();
        strcat(tmp_data_string,d);
        i++;
    }

    fclose(myfs);

    int new_inode = acc_inode();
    int new_data = acc_data();
    
    myfs = fopen("myfs","wb");
    rewind(myfs);
    fseek(myfs, BOOT_BLOCK_SIZE+SUPER_BLOCK_SIZE+(sizeof(INODE)*128)+(DATA_BLOCK_SIZE*(new_data-1)),SEEK_SET);//?λ‘μ΄ ??Ό? λ³΅μ¬
    int k = 0;
    int j = sizeof(tmp_data_string);
    while (k < j);
    {
      size_F++;
      putchar(c);
      k++;
    }

    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + (sizeof(INODE) * (new_inode - 1)),SEEK_SET);//INODELIST μ±μ°κΈ?
    fread(inode_data_ptr, sizeof(INODE), 1, myfs);
    inode_data_ptr -> type = 0;
    inode_data_ptr -> year = TimeInfo ->tm_year+1900;
    inode_data_ptr-> size = size_F;
    inode_data_ptr -> month = TimeInfo ->tm_mon+1;
    inode_data_ptr -> date = TimeInfo ->tm_mday;
    inode_data_ptr -> hour = TimeInfo ->tm_hour;
    inode_data_ptr -> minute = TimeInfo ->tm_min;
    inode_data_ptr -> second = TimeInfo ->tm_sec;
    inode_data_ptr -> dir_1 = new_data;    
    inode_data_ptr -> dir_2 = 0;
    inode_data_ptr -> dir_3 = 0;
    inode_data_ptr -> dir_4 = 0;
    inode_data_ptr -> dir_5 = 0;
    inode_data_ptr -> dir_6 = 0;
    inode_data_ptr -> dir_7 = 0;
    inode_data_ptr -> dir_8 = 0;
    inode_data_ptr -> indir = 0; 
    fwrite(inode_data_ptr,sizeof(INODE),1,myfs);

    
    free(tmp_file_string_ptr);
    free(tmp_inode_ptr);
    fclose(myfs);

    return;
}

/*
?΄λ¦?    : myrm ?¨?
??±?  : ?€κ·λΉ
κΈ°λ₯    : ??Ό? ?­? ?? λͺλ Ή?΄
λ°λκ°?  : ??Όλͺ?
λ¦¬ν΄κ°?  : λ¦¬ν΄κ°?
*/

void myrm(const char* file)
{
    FILE *myfs;
    int c,i=0;
    int inode = 1;

    char *tmp_file_string_ptr = (char *)malloc(sizeof(char) * 8); //?? ? λ¦¬μ datablock?? μΆμΆ? ??Όλͺμ κ°?λ¦¬ν¬ ?¬?Έ?°
    int *tmp_inode_ptr = (int *)malloc(sizeof(int)); //?? ? λ¦¬μ datablock?? μΆμΆ? inode λ²νΈλ₯? κ°?λ¦¬ν¬ ?¬?Έ?°
    INODE *inode_ptr = (INODE *)malloc(sizeof(INODE));// inode ?¬?Έ?°

    myfs = fopen("myfs", "rb");
    if (myfs == NULL)
    {
        printf("myrm() ?¨? : ??Ό ?΄κΈ°μ ?€?¨??΅??€.\n");
        abort();
    }
    fseek(myfs, BOOT_BLOCK_SIZE+SUPER_BLOCK_SIZE+(sizeof(INODE)*128)+(DATA_BLOCK_SIZE*((path_to_inode(prtpwd()))-1)),SEEK_SET);//??¬ ?? ? λ¦¬μ ?°?΄?°λΈλ‘ ??Όλ‘? ?¬?Έ?° ?΄?
    fread(tmp_file_string_ptr, sizeof(char) * 8, 1, myfs); // ?¬?Έ?°λ‘? ??Όλͺ? ??Έ

    while(!strcmp(tmp_file_string_ptr, file))
    {//μ°Ύλ ??Όκ³? ??Έ? ??Ό? ?΄λ¦μ΄ κ°μ?? ??€λ©?
        if((c = getchar()) != EOF)
        {//myfs??Ό? ? ??Έ
            fseek(myfs, -1, SEEK_CUR);
            fseek(myfs,sizeof(int),SEEK_CUR);//??¬ ?¬?Έ?° ?μΉλ‘λΆ??° int? ?¬κΈ°λ§?Ό ?΄?
            fread(tmp_file_string_ptr, sizeof(char) * 8, 1, myfs);
        }
        else
        {
        fprintf(stderr,"?€λ₯? : %s ??Ό?΄ ??΅??€.\n", file); //??Όλͺμ ??¬ ?? ? λ¦¬μ? λͺ»μ°Ύ?Όλ©? ?€λ₯? ??°κΈ?
        exit(1);
        }
    }
    fread(tmp_inode_ptr, sizeof(int), 1, myfs);
    inode =*tmp_inode_ptr;
    fclose(myfs);

    myfs = fopen("myfs", "wb");
    rewind(myfs);
    fseek(myfs, BOOT_BLOCK_SIZE + (inode- 1) ,SEEK_SET);//INODELIST 
    putchar(0);
    fseek(myfs, BOOT_BLOCK_SIZE + 128 + ((inode_ptr -> dir_1) - 1),SEEK_SET);//DATABLOCK
    putchar(0);
    fseek(myfs, BOOT_BLOCK_SIZE + (sizeof(INODE)*128)+(DATA_BLOCK_SIZE)*((inode_ptr -> dir_1) - 1) ,SEEK_SET);
    while(i != DATA_BLOCK_SIZE)
    {
        putchar(0);
        i++;
    }
}

/*
?΄λ¦?    : cntfound ?¨?
??±?  : ?€κ·λΉ
κΈ°λ₯    : dir_list?? ??¬ ?΄?΄μ§? ?? ? λ¦? ? λ¦¬ν΄
λ°λκ°?  : X
λ¦¬ν΄κ°?  : countκ°?
*/

int cntfound() 
{
    int cnt = 0;
    DIR_LIST* tmp_dir = front_dir_list_ptr;
    while ((tmp_dir->next_ptr) != NULL)
    {
        {
            cnt++;
            tmp_dir = tmp_dir->next_ptr;
        }
    }
    return cnt;//?? ?€?¨?

}

/*
?΄λ¦?    : prtpwd ?¨?
??±?  : ?€κ·λΉ
κΈ°λ₯    : pwdλ₯? λ¬Έμ?΄λ‘? λ¦¬ν΄
λ°λκ°?  : X
λ¦¬ν΄κ°?  : pwd λ¬Έμ?΄κ°?
*/
char* prtpwd()
{
    char* s;
    char* pwd;
    DIR_LIST *tmp_ptr = front_dir_list_ptr;

    while (1)
    {
        s = tmp_ptr->name;
        pwd = strcat(pwd,s);
        if (tmp_ptr->next_ptr == NULL)
        {
            break;
        }
        else
        {
            pwd = strcat(pwd,"/");
            tmp_ptr = tmp_ptr->next_ptr;
        }
    }
    return pwd;
}

/*
?΄λ¦?    : acc_inode ?¨?
??±?  : ?€κ·λΉ
κΈ°λ₯    : κ°??©μ€μ΄μ§? ???? inode λ²νΈλ₯? κ°?? Έ?€κ³?, κ·? inodeλ²νΈ? ?΄?Ή?? ??ΌλΈλ‘? 1λ‘? μ±μ΄?€.
λ°λκ°?  : X
λ¦¬ν΄κ°?  : κ°??©μ€μ΄μ§? ???? inode λ²νΈ
*/

int acc_inode()
{
    FILE* myfs;
    int t = 1;

    myfs = fopen("myfs", "rb");
    if (myfs == NULL)
    {
        printf("acc_inode() ?¨? : ??Ό ?΄κΈ°μ ?€?¨??΅??€.\n");
        abort();
    }

    SUPERBLOCK *sb_ptr = (SUPERBLOCK *)malloc(sizeof(SUPERBLOCK));
    fseek(myfs, BOOT_BLOCK_SIZE, SEEK_SET);
    fread(sb_ptr, sizeof(SUPERBLOCK), 1, myfs);//?¬?©μ€μΈ ??ΌλΈλ‘ ??Έ??¬ κ°??©μ€μ΄μ§? ???? ??΄?Έ?? ??Ό ?£κΈ? ??¨.

    unsigned mask;
    while(1){ // λΉ? inodeκ°? ??? κΉμ?? λ¬΄ν λ°λ³΅
    if (t < 33) // inode_1? λΉ? inodeκ°? ?? κ²½μ°
    {
        mask = 1 << (t - 1);
        if ((sb_ptr->inode_1 & mask) == 0) //λΉ? ??΄?Έ?κ°? ??? κ²½μ°
        {
            sb_ptr->inode_1 = sb_ptr->inode_1 | 1;//λΉ? ??΄?Έ? μ±μ°κΈ?
            break;
        }
        t++;
    }
    else if (t < 65) // inode_2? λΉ? inodeκ°? ?? κ²½μ°
    {
        mask = 1 << ((t- 32) - 1);
        if ((sb_ptr->inode_2 & mask) == 0)
        {
            sb_ptr->inode_2 = sb_ptr->inode_2 | 1;
            break;
        }
        t++;
    }
    else if (t < 97) // inode_3? λΉ? inodeκ°? ?? κ²½μ°
    {
        mask = 1 << ((t - 64) - 1);
        if ((sb_ptr->inode_3 & mask) == 0)
        {
          sb_ptr->inode_3 = sb_ptr->inode_3 | 1;
          break;
        }
        t++;
    }
    else // inode_4? λΉ? inodeκ°? ?? κ²½μ°
    {
        mask = 1 << ((t - 96) - 1);
        if ((sb_ptr->inode_4 & mask) == 0)
        {
            sb_ptr->inode_4 = sb_ptr->inode_4 | 1;
            break;
        }
        t++;
    }
    }
    return t;
}

/*
?΄λ¦?    : acc_data ?¨?
??±?  : ?€κ·λΉ
κΈ°λ₯    : κ°??©μ€μ΄μ§? ???? datablock λ²νΈλ₯? κ°?? Έ?€κ³?, κ·? inodeλ²νΈ? ?΄?Ή?? ??ΌλΈλ‘? 1λ‘? μ±μ΄?€.
λ°λκ°?  : X
λ¦¬ν΄κ°?  : κ°??©μ€μ΄μ§? ???? datablock λ²νΈ
*/
int acc_data()
{
    FILE* myfs;
    int t = 1;

    myfs = fopen("myfs", "rb");
    if (myfs == NULL)
    {
        printf("acc_data() ?¨? : ??Ό ?΄κΈ°μ ?€?¨??΅??€.\n");
        abort();
    }

    SUPERBLOCK *sb_ptr = (SUPERBLOCK *)malloc(sizeof(SUPERBLOCK));
    fseek(myfs, BOOT_BLOCK_SIZE+128, SEEK_SET);
    fread(sb_ptr, sizeof(SUPERBLOCK), 1, myfs);//?¬?©μ€μΈ ??ΌλΈλ‘ ??Έ??¬ κ°??©μ€μ΄μ§? ???? ?°?΄?°λΈλ‘? ??Ό ?£κΈ? ??¨.

    unsigned mask;
    while(1)
    { // λΉ? ?°?΄?°λΈλ‘?΄ ??? κΉμ?? λ¬΄ν λ°λ³΅
    if (t < 33) // data_block_1? λΉ? data_block?΄ ?? κ²½μ°
    {
        mask = 1 << (t - 1);
        if ((sb_ptr->data_block_1 & mask) == 0) //λΉ? ??ΌλΈλ‘?΄ ??? κ²½μ°
        {
            sb_ptr->data_block_1 = sb_ptr->data_block_1 | 1;//λΉ? ??ΌλΈλ‘ μ±μ°κΈ?
            break;
        }
        t++;
    }
    else if (t < 65) // data_block_2? λΉ? data_block?΄ ?? κ²½μ°
    {
        mask = 1 << (t - 1);
        if ((sb_ptr->data_block_2 & mask) == 0) //λΉ? ??ΌλΈλ‘?΄ ??? κ²½μ°
        {
            sb_ptr->data_block_2 = sb_ptr->data_block_2 | 1;//λΉ? ??ΌλΈλ‘ μ±μ°κΈ?
            break;
        }
        t++;
    }
    else if (t < 97) // data_block_3? λΉ? data_block?΄ ?? κ²½μ°
    {
        mask = 1 << (t - 1);
        if ((sb_ptr->data_block_3 & mask) == 0) //λΉ? ??ΌλΈλ‘?΄ ??? κ²½μ°
        {
            sb_ptr->data_block_3 = sb_ptr->data_block_3 | 1;//λΉ? ??ΌλΈλ‘ μ±μ°κΈ?
            break;
        }
        t++;
    }
    else if (t < 129) // data_block_4? λΉ? data_block?΄ ?? κ²½μ°
    {
        mask = 1 << (t - 1);
        if ((sb_ptr->data_block_4 & mask) == 0) //λΉ? ??ΌλΈλ‘?΄ ??? κ²½μ°
        {
            sb_ptr->data_block_4 = sb_ptr->data_block_4 | 1;//λΉ? ??ΌλΈλ‘ μ±μ°κΈ?
            break;
        }
        t++;
    }
    else if (t < 161) // data_block_5? λΉ? data_block?΄ ?? κ²½μ°
    {
        mask = 1 << (t - 1);
        if ((sb_ptr->data_block_5 & mask) == 0) //λΉ? ??ΌλΈλ‘?΄ ??? κ²½μ°
        {
            sb_ptr->data_block_5 = sb_ptr->data_block_5 | 1;//λΉ? ??ΌλΈλ‘ μ±μ°κΈ?
            break;
        }
        t++;
    }
    else if (t < 193) // data_block_6? λΉ? data_block?΄ ?? κ²½μ°
    {
        mask = 1 << (t - 1);
        if ((sb_ptr->data_block_6 & mask) == 0) //λΉ? ??ΌλΈλ‘?΄ ??? κ²½μ°
        {
            sb_ptr->data_block_6 = sb_ptr->data_block_6 | 1;//λΉ? ??ΌλΈλ‘ μ±μ°κΈ?
            break;
        }
        t++;
    }
    else if (t < 225) // data_block_7? λΉ? data_block?΄ ?? κ²½μ°
    {
        mask = 1 << (t - 1);
        if ((sb_ptr->data_block_7 & mask) == 0) //λΉ? ??ΌλΈλ‘?΄ ??? κ²½μ°
        {
            sb_ptr->data_block_7 = sb_ptr->data_block_7 | 1;//λΉ? ??ΌλΈλ‘ μ±μ°κΈ?
            break;
        }
        t++;
    }
    else // data_block_3? λΉ? data_block?΄ ?? κ²½μ°
    {
        mask = 1 << (t - 1);
        if ((sb_ptr->data_block_8 & mask) == 0) //λΉ? ??ΌλΈλ‘?΄ ??? κ²½μ°
        {
            sb_ptr->data_block_8 = sb_ptr->data_block_8 | 1;//λΉ? ??ΌλΈλ‘ μ±μ°κΈ?
            break;
        }
        t++;
    }
    }
    return t;
}


#include "user.h"
#include <time.h>

DIR_LIST* dirfound(char* fdir);
char* prtpwd();
int cntfound();
int acc_inode();
int acc_data();

/*
?���?    : mycd ?��?��
?��?��?��  : ?��규빈
기능    : ?��?�� ?��?��?��리의 경로�? �?경한?��
받는�?  : 경로 문자?��
리턴�?  : X
*/
void mycd (char* path)
{
    if(path == NULL)
    {//path ?��?���? ?��?��?��
        rear_dir_list_ptr == front_dir_list_ptr;//?��?�� ?��?��?��리�?? ?��?��?��?��리로 ?��?��
    }

    else if (path != NULL)
    {
        int first = 0;//�? / ?��?��?���?
        DIR_LIST *tmp_ptr;
        char *nm_ptr = strtok(path, "/");
        while (nm_ptr != NULL)//?��름이 NULL값이 ?��?��?���?
        {
         if (first == 0)//?��?��?��?��?��리�?? . , .. , /?�� �? ?��?��  . ??? ..?��?�� ?��?��?��?��?�� 경우 ?��?��
         {
          if (nm_ptr == ".") //.?��?�� ?��?��
          {
          tmp_ptr = rear_dir_list_ptr;
          nm_ptr = strtok(NULL, "/");
          first++;
          }
          else if (nm_ptr == "..")//..?��?�� ?��?��
          {
           int i = 0;
           tmp_ptr = front_dir_list_ptr;
           while (i < cntfound()-1){
               tmp_ptr = tmp_ptr -> next_ptr;
           } // tmp_ptr?�� ?��?�� ?��?��?���? 리스?��?��?�� (마�??�?-1)번째 ?��?��?��리�?? �?리키?���? ?��.
           tmp_ptr = rear_dir_list_ptr;
           nm_ptr = strtok(NULL, "/");
           first++;
          }
         }
         else if (first != 0)
         {
          DIR_LIST* new_dir = malloc(sizeof(DIR_LIST)); 
          new_dir-> name = nm_ptr;
          char* s = strcat("/",nm_ptr);//s = /'?��?�� ?��?��?���?'
          new_dir-> inode = path_to_inode(strcat(prtpwd(),s));
          tmp_ptr -> next_ptr = new_dir;
          nm_ptr = strtok(NULL, "/");
         }
        }
    }
}

/*
?���?    : mycpto ?��?��
?��?��?��  : ?��규빈
기능    : MY ?��?��?��?��?��?�� ?��?�� ?��?��?�� ?��?��?�� 컴퓨?���? 복사?��?��.
받는�?  : ?��?���? 1,2
리턴�?  : 리턴�?
*/
void mycpto (const char* source_file, const char* dest_file  )
{
    FILE *ofp;
    FILE *myfs;
    int c,d;
    int i = 0;
    int inode = 1;

    char *tmp_file_string_ptr = (char *)malloc(sizeof(char) * 8); //?��?��?��리의 datablock?��?�� 추출?�� ?��?��명을 �?리킬 ?��?��?��
    int *tmp_inode_ptr = (int *)malloc(sizeof(int)); //?��?��?��리의 datablock?��?�� 추출?�� inode 번호�? �?리킬 ?��?��?��
    INODE *inode_ptr = (INODE *)malloc(sizeof(INODE));// inode ?��?��?��

    myfs = fopen("myfs", "rb");
    fseek(myfs, BOOT_BLOCK_SIZE+SUPER_BLOCK_SIZE+(sizeof(INODE)*(path_to_inode(prtpwd())-1)),SEEK_SET);//?��?�� ?��?��?��리의 ?��?��?��블록 ?��?���? ?��?��?�� ?��?��
    fread(inode_ptr, sizeof(INODE), 1, myfs);
    int tmp_datablock = (int)(inode_ptr->dir_1 + 1);
    fread(tmp_file_string_ptr, sizeof(char) * 8, 1, myfs); // ?��?��?���? ?��?���? ?��?��

    while(!strcmp(tmp_file_string_ptr, source_file))
    {//찾는 ?��?���? ?��?��?�� ?��?��?�� ?��름이 같�?? ?��?���?
        if((c = getchar()) != EOF)
        {//myfs?��?��?�� ?�� ?��?��
            fseek(myfs, -1, SEEK_CUR);
            fseek(myfs,sizeof(int),SEEK_CUR);//?��?�� ?��?��?�� ?��치로�??�� int?�� ?��기만?�� ?��?��
            fread(tmp_file_string_ptr, sizeof(char) * 8, 1, myfs);
        }
        else
        {
        fprintf(stderr,"?���? : %s ?��?��?�� ?��?��?��?��.\n", source_file); //?��?��명을 ?��?�� ?��?��?��리에?�� 못찾?���? ?���? ?��?���?
        exit(1);
        }
    }
    fread(tmp_inode_ptr, sizeof(int), 1, myfs);
    inode =*tmp_inode_ptr;

    if ((ofp = fopen(dest_file, "wb")) == NULL)
    {
        fprintf(stderr, "?���? : %s ?��?��?�� ?�� ?�� ?��?��?��?��. \n", source_file);
        exit(1);
    }

    fseek(myfs, BOOT_BLOCK_SIZE+SUPER_BLOCK_SIZE+(sizeof(INODE)*128)+(DATA_BLOCK_SIZE*((inode_ptr->dir_1)- 1)),SEEK_SET);//?��로운 ?��?��?�� 복사
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
?���?    : mycpfrom ?��?��
?��?��?��  : ?��규빈
기능    : ?��?��?�� 컴퓨?��?�� ?��?�� ?��?��?�� MY ?��?��?��?��?��?���? 복사?��?��.
받는�?  : ?��?���? 1,2
리턴�?  : 리턴�?
*/

void mycpfrom (const char* source_file, const char* dest_file )
{
    FILE *ifp;
    FILE *myfs;
    int c,size_F;
    time_t Time;
    struct tm* TimeInfo;
 
    Time = time(NULL);                // ?��?�� ?��간을 받음
    TimeInfo = localtime(&Time); 

    myfs = fopen("myfs", "wb");
    char *tmp_dir_string_ptr = (char *)malloc(sizeof(char) * 8); //?��?��?��리의 datablock?��?�� 추출?�� ?��?��?��리명?�� �?리킬 ?��?��?��
    int *tmp_inode_ptr = (int *)malloc(sizeof(int)); //?��?��?��리의 datablock?��?�� 추출?�� inode 번호�? �?리킬 ?��?��?��
    INODE *inode_data_ptr = (INODE *)malloc(sizeof(INODE));
    
    int new_inode = acc_inode();
    int new_data = acc_data();
    ifp = fopen(source_file,"rb");
    fseek(myfs, BOOT_BLOCK_SIZE+SUPER_BLOCK_SIZE+(sizeof(INODE)*128)+(DATA_BLOCK_SIZE*(new_data-1)),SEEK_SET);//?��로운 ?��?��?�� 복사
    while ((c = getc(ifp)) != EOF)
    {
      size_F++;
      putchar(c);
    }

    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + (sizeof(INODE) * (new_inode - 1)),SEEK_SET);//INODELIST 채우�?
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
?���?    : mycp ?��?��
?��?��?��  : ?��규빈
기능    : MY ?��?��?��?��?��?�� ?��?�� ?��?��?�� 복사?��?�� 명령?��
받는�?  : ?��?���? 1,2
리턴�?  : 리턴�?
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
 
    Time = time(NULL);                // ?��?�� ?��간을 받음
    TimeInfo = localtime(&Time); 

    char *tmp_file_string_ptr; //?��?��?��리의 datablock?��?�� 추출?�� ?��?��명을 �?리킬 ?��?��?��
    int *tmp_inode_ptr = (int *)malloc(sizeof(int)); //?��?��?��리의 datablock?��?�� 추출?�� inode 번호�? �?리킬 ?��?��?��
    INODE *inode_data_ptr = (INODE *)malloc(sizeof(INODE));// inode ?��?��?��
    char *tmp_data_string; //?��?��?��리의 datablock?��?�� 추출?�� ?��?��?���? �?리킬 ?��?��?��

    myfs = fopen("myfs", "rb");
    fseek(myfs, BOOT_BLOCK_SIZE+SUPER_BLOCK_SIZE+(sizeof(INODE)*(path_to_inode(prtpwd())-1)),SEEK_SET);//?��?�� ?��?��?��리의 ?��?��?��블록 ?��?���? ?��?��?�� ?��?��
    fread(inode_data_ptr, sizeof(INODE), 1, myfs);
    int tmp_datablock = (int)(inode_data_ptr->dir_1 + 1);
    fread(tmp_file_string_ptr, sizeof(char) * 8, 1, myfs); // ?��?��?���? ?��?���? ?��?��

    while(!strcmp(tmp_file_string_ptr, source_file))
    {//찾는 ?��?���? ?��?��?�� ?��?��?�� ?��름이 같�?? ?��?���?
        if((c = getchar()) != EOF)
        {//myfs?��?��?�� ?�� ?��?��
            fseek(myfs, -1, SEEK_CUR);
            fseek(myfs,sizeof(int),SEEK_CUR);//?��?�� ?��?��?�� ?��치로�??�� int?�� ?��기만?�� ?��?��
            fread(tmp_file_string_ptr, sizeof(char) * 8, 1, myfs);
        }
        else
        {
        fprintf(stderr,"?���? : %s ?��?��?�� ?��?��?��?��.\n", source_file); //?��?��명을 ?��?�� ?��?��?��리에?�� 못찾?���? ?���? ?��?���?
        exit(1);
        }
    }
    fread(tmp_inode_ptr, sizeof(int), 1, myfs);
    inode =*tmp_inode_ptr;

    fseek(myfs, BOOT_BLOCK_SIZE+SUPER_BLOCK_SIZE+(sizeof(INODE)*128)+(DATA_BLOCK_SIZE*((inode_data_ptr->dir_1)- 1)),SEEK_SET);//문자?�� ?��?��?��?�� 복사?��?���?
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
    fseek(myfs, BOOT_BLOCK_SIZE+SUPER_BLOCK_SIZE+(sizeof(INODE)*128)+(DATA_BLOCK_SIZE*(new_data-1)),SEEK_SET);//?��로운 ?��?��?�� 복사
    int k = 0;
    int j = sizeof(tmp_data_string);
    while (k < j);
    {
      size_F++;
      putchar(c);
      k++;
    }

    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + (sizeof(INODE) * (new_inode - 1)),SEEK_SET);//INODELIST 채우�?
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
?���?    : myrm ?��?��
?��?��?��  : ?��규빈
기능    : ?��?��?�� ?��?��?��?�� 명령?��
받는�?  : ?��?���?
리턴�?  : 리턴�?
*/

void myrm(const char* file)
{
    FILE *myfs;
    int c,i=0;
    int inode = 1;

    char *tmp_file_string_ptr = (char *)malloc(sizeof(char) * 8); //?��?��?��리의 datablock?��?�� 추출?�� ?��?��명을 �?리킬 ?��?��?��
    int *tmp_inode_ptr = (int *)malloc(sizeof(int)); //?��?��?��리의 datablock?��?�� 추출?�� inode 번호�? �?리킬 ?��?��?��
    INODE *inode_ptr = (INODE *)malloc(sizeof(INODE));// inode ?��?��?��

    myfs = fopen("myfs", "rb");
    if (myfs == NULL)
    {
        printf("myrm() ?��?�� : ?��?�� ?��기에 ?��?��?��?��?��?��.\n");
        abort();
    }
    fseek(myfs, BOOT_BLOCK_SIZE+SUPER_BLOCK_SIZE+(sizeof(INODE)*128)+(DATA_BLOCK_SIZE*((path_to_inode(prtpwd()))-1)),SEEK_SET);//?��?�� ?��?��?��리의 ?��?��?��블록 ?��?���? ?��?��?�� ?��?��
    fread(tmp_file_string_ptr, sizeof(char) * 8, 1, myfs); // ?��?��?���? ?��?���? ?��?��

    while(!strcmp(tmp_file_string_ptr, file))
    {//찾는 ?��?���? ?��?��?�� ?��?��?�� ?��름이 같�?? ?��?���?
        if((c = getchar()) != EOF)
        {//myfs?��?��?�� ?�� ?��?��
            fseek(myfs, -1, SEEK_CUR);
            fseek(myfs,sizeof(int),SEEK_CUR);//?��?�� ?��?��?�� ?��치로�??�� int?�� ?��기만?�� ?��?��
            fread(tmp_file_string_ptr, sizeof(char) * 8, 1, myfs);
        }
        else
        {
        fprintf(stderr,"?���? : %s ?��?��?�� ?��?��?��?��.\n", file); //?��?��명을 ?��?�� ?��?��?��리에?�� 못찾?���? ?���? ?��?���?
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
?���?    : cntfound ?��?��
?��?��?��  : ?��규빈
기능    : dir_list?��?�� ?��?�� ?��?���? ?��?��?���? ?�� 리턴
받는�?  : X
리턴�?  : count�?
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
    return cnt;//?��?�� ?��?��?��

}

/*
?���?    : prtpwd ?��?��
?��?��?��  : ?��규빈
기능    : pwd�? 문자?���? 리턴
받는�?  : X
리턴�?  : pwd 문자?���?
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
?���?    : acc_inode ?��?��
?��?��?��  : ?��규빈
기능    : �??��중이�? ?��??? inode 번호�? �??��?���?, �? inode번호?�� ?��?��?��?�� ?��?��블록?�� 1�? 채운?��.
받는�?  : X
리턴�?  : �??��중이�? ?��??? inode 번호
*/

int acc_inode()
{
    FILE* myfs;
    int t = 1;

    myfs = fopen("myfs", "rb");
    if (myfs == NULL)
    {
        printf("acc_inode() ?��?�� : ?��?�� ?��기에 ?��?��?��?��?��?��.\n");
        abort();
    }

    SUPERBLOCK *sb_ptr = (SUPERBLOCK *)malloc(sizeof(SUPERBLOCK));
    fseek(myfs, BOOT_BLOCK_SIZE, SEEK_SET);
    fread(sb_ptr, sizeof(SUPERBLOCK), 1, myfs);//?��?��중인 ?��?��블록 ?��?��?��?�� �??��중이�? ?��??? ?��?��?��?��?�� ?��?�� ?���? ?��?��.

    unsigned mask;
    while(1){ // �? inode�? ?��?��?�� 까�?? 무한 반복
    if (t < 33) // inode_1?�� �? inode�? ?��?�� 경우
    {
        mask = 1 << (t - 1);
        if ((sb_ptr->inode_1 & mask) == 0) //�? ?��?��?��?���? ?��?��?�� 경우
        {
            sb_ptr->inode_1 = sb_ptr->inode_1 | 1;//�? ?��?��?��?�� 채우�?
            break;
        }
        t++;
    }
    else if (t < 65) // inode_2?�� �? inode�? ?��?�� 경우
    {
        mask = 1 << ((t- 32) - 1);
        if ((sb_ptr->inode_2 & mask) == 0)
        {
            sb_ptr->inode_2 = sb_ptr->inode_2 | 1;
            break;
        }
        t++;
    }
    else if (t < 97) // inode_3?�� �? inode�? ?��?�� 경우
    {
        mask = 1 << ((t - 64) - 1);
        if ((sb_ptr->inode_3 & mask) == 0)
        {
          sb_ptr->inode_3 = sb_ptr->inode_3 | 1;
          break;
        }
        t++;
    }
    else // inode_4?�� �? inode�? ?��?�� 경우
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
?���?    : acc_data ?��?��
?��?��?��  : ?��규빈
기능    : �??��중이�? ?��??? datablock 번호�? �??��?���?, �? inode번호?�� ?��?��?��?�� ?��?��블록?�� 1�? 채운?��.
받는�?  : X
리턴�?  : �??��중이�? ?��??? datablock 번호
*/
int acc_data()
{
    FILE* myfs;
    int t = 1;

    myfs = fopen("myfs", "rb");
    if (myfs == NULL)
    {
        printf("acc_data() ?��?�� : ?��?�� ?��기에 ?��?��?��?��?��?��.\n");
        abort();
    }

    SUPERBLOCK *sb_ptr = (SUPERBLOCK *)malloc(sizeof(SUPERBLOCK));
    fseek(myfs, BOOT_BLOCK_SIZE+128, SEEK_SET);
    fread(sb_ptr, sizeof(SUPERBLOCK), 1, myfs);//?��?��중인 ?��?��블록 ?��?��?��?�� �??��중이�? ?��??? ?��?��?��블록?�� ?��?�� ?���? ?��?��.

    unsigned mask;
    while(1)
    { // �? ?��?��?��블록?�� ?��?��?�� 까�?? 무한 반복
    if (t < 33) // data_block_1?�� �? data_block?�� ?��?�� 경우
    {
        mask = 1 << (t - 1);
        if ((sb_ptr->data_block_1 & mask) == 0) //�? ?��?��블록?�� ?��?��?�� 경우
        {
            sb_ptr->data_block_1 = sb_ptr->data_block_1 | 1;//�? ?��?��블록 채우�?
            break;
        }
        t++;
    }
    else if (t < 65) // data_block_2?�� �? data_block?�� ?��?�� 경우
    {
        mask = 1 << (t - 1);
        if ((sb_ptr->data_block_2 & mask) == 0) //�? ?��?��블록?�� ?��?��?�� 경우
        {
            sb_ptr->data_block_2 = sb_ptr->data_block_2 | 1;//�? ?��?��블록 채우�?
            break;
        }
        t++;
    }
    else if (t < 97) // data_block_3?�� �? data_block?�� ?��?�� 경우
    {
        mask = 1 << (t - 1);
        if ((sb_ptr->data_block_3 & mask) == 0) //�? ?��?��블록?�� ?��?��?�� 경우
        {
            sb_ptr->data_block_3 = sb_ptr->data_block_3 | 1;//�? ?��?��블록 채우�?
            break;
        }
        t++;
    }
    else if (t < 129) // data_block_4?�� �? data_block?�� ?��?�� 경우
    {
        mask = 1 << (t - 1);
        if ((sb_ptr->data_block_4 & mask) == 0) //�? ?��?��블록?�� ?��?��?�� 경우
        {
            sb_ptr->data_block_4 = sb_ptr->data_block_4 | 1;//�? ?��?��블록 채우�?
            break;
        }
        t++;
    }
    else if (t < 161) // data_block_5?�� �? data_block?�� ?��?�� 경우
    {
        mask = 1 << (t - 1);
        if ((sb_ptr->data_block_5 & mask) == 0) //�? ?��?��블록?�� ?��?��?�� 경우
        {
            sb_ptr->data_block_5 = sb_ptr->data_block_5 | 1;//�? ?��?��블록 채우�?
            break;
        }
        t++;
    }
    else if (t < 193) // data_block_6?�� �? data_block?�� ?��?�� 경우
    {
        mask = 1 << (t - 1);
        if ((sb_ptr->data_block_6 & mask) == 0) //�? ?��?��블록?�� ?��?��?�� 경우
        {
            sb_ptr->data_block_6 = sb_ptr->data_block_6 | 1;//�? ?��?��블록 채우�?
            break;
        }
        t++;
    }
    else if (t < 225) // data_block_7?�� �? data_block?�� ?��?�� 경우
    {
        mask = 1 << (t - 1);
        if ((sb_ptr->data_block_7 & mask) == 0) //�? ?��?��블록?�� ?��?��?�� 경우
        {
            sb_ptr->data_block_7 = sb_ptr->data_block_7 | 1;//�? ?��?��블록 채우�?
            break;
        }
        t++;
    }
    else // data_block_3?�� �? data_block?�� ?��?�� 경우
    {
        mask = 1 << (t - 1);
        if ((sb_ptr->data_block_8 & mask) == 0) //�? ?��?��블록?�� ?��?��?�� 경우
        {
            sb_ptr->data_block_8 = sb_ptr->data_block_8 | 1;//�? ?��?��블록 채우�?
            break;
        }
        t++;
    }
    }
    return t;
}


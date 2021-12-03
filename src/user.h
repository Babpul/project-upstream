//�������
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>



//��ũ��
//block �� ũ��(����Ʈ ����)
#define     BOOT_BLOCK_SIZE             2               //boot block�� �� ũ��
#define     SUPER_BLOCK_SIZE            48              //super block�� �� ũ��
#define     INODE_LIST_SIZE             (20 * 128)      //inode list�� �� ũ��(inode �ϳ��� ũ�� �ƴ�)

#define     DATA_BLOCK_SIZE             256             //data block�� block �ϳ��� ũ��

//��ɾ� ���ڿ��� ũ�� ����
#define     COM_SEP_SIZE                50              //���� �Է��ϴ� ��ɾ�� �� �ܾ��� �ִ� ũ��(NULL ���� ����)
#define     COM_SEP_NUM                 4               //���� �Է��ϴ� ��ɾ ���� �� �ִ� �ܾ��� �ִ� ����



//����� ������
//super block ����ü
typedef struct
{
    //inode �κ�
    unsigned inode_1 : 32;
    unsigned inode_2 : 32;
    unsigned inode_3 : 32;
    unsigned inode_4 : 32;
    
    //data block �κ�
    unsigned data_block_1 : 32;
    unsigned data_block_2 : 32;
    unsigned data_block_3 : 32;
    unsigned data_block_4 : 32;
    unsigned data_block_5 : 32;
    unsigned data_block_6 : 32;
    unsigned data_block_7 : 32;
    unsigned data_block_8 : 32;
}SUPERBLOCK;

//inode ����ü
typedef struct
{
    unsigned size : 32;     //���� ũ��

    unsigned type : 8;      //���� ����
    
    unsigned year : 8;      //���� ���� ����
    unsigned month : 8;     //���� ���� ����
    unsigned date : 8;      //���� ���� ����
    unsigned hour : 8;      //���� ���� ����
    unsigned minute : 8;    //���� ���� ����
    unsigned second : 8;    //���� ���� ����

    unsigned dir_1 : 8;     //direct �� ��ȣ
    unsigned dir_2 : 8;     //direct �� ��ȣ
    unsigned dir_3 : 8;     //direct �� ��ȣ
    unsigned dir_4 : 8;     //direct �� ��ȣ
    unsigned dir_5 : 8;     //direct �� ��ȣ
    unsigned dir_6 : 8;     //direct �� ��ȣ
    unsigned dir_7 : 8;     //direct �� ��ȣ
    unsigned dir_8 : 8;     //direct �� ��ȣ

    unsigned indir : 8;     //single indirect �� ��ȣ
}INODE;

//�۾� ���丮�� ���� ��θ� ������ node ����ü
typedef struct dir_list
{
    char *name;
    int inode;
    struct dir_list *next_ptr;
}DIR_LIST;



//�������� ����
extern DIR_LIST *front_dir_list_ptr;
extern DIR_LIST *rear_dir_list_ptr;



//�Լ� ���� ����
//mainfile
void shell(void);

//jhun
void mypwd(void);
void myinode(const char *ptr);
void mydatablock(const char *ptr);
void command(void);
int int_pow(int a, int b);
void mytree(const char *path_ptr);
int path_to_inode(const char *path_ptr);
void dir_print(int inode, FILE *myfs);
char *current_dir_find(int inode, int high_inode, FILE *myfs);
void mymkfs(void);
void mymv(char *file_1, char *file_2);
void mv_to_dir(FILE *myfs, int file_1_inode, int file_2_inode, char *file_1_name, int count);

//insuk
int checkbit(int number, unsigned block);
void change_superblock(int saveinumber, int savedbnumber, SUPERBLOCK *sb_data);
void bit_print(unsigned a);
void mymkdir(char *dir_name);
void myrmdir(char *givenname);
void mystate(void);
void mytouch(char *givenname);

//jin
int stringtoint(char *byte);
void myls(const char *ptr);
void mycat(char *givename);
void myshowfile(char *startbyte, char *endbyte, char *givenname);

//oh
void mycd (char* path);
void mycpto (const char* source_file, const char* dest_file);
void mycpfrom (char* source_file, char* dest_file);
void mycp(char* source_file, char* dest_file);
void myrm(const char* file);
char* prtpwd();
int cntfound();
int acc_inode();
int acc_data();

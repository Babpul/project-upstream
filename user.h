//�������
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



//��ũ��
//block �� ũ��(����Ʈ ����)
#define     BOOT_BLOCK_SIZE             2       //boot block�� �� ũ��
#define     SUPER_BLOCK_SIZE            12      //super block�� �� ũ��
#define     INODE_LIST_SIZE             2176    //inode list�� �� ũ��(inode �ϳ��� ũ�� �ƴ�)

#define     DATA_BLOCK_SIZE             256     //data block�� block �ϳ��� ũ��

//��ɾ� ���ڿ��� ũ�� ����
#define     COM_SEP_SIZE                30      //���� �Է��ϴ� ��ɾ�� �� �ܾ��� �ִ� ũ��
#define     COM_SEP_NUM                 4       //���� �Է��ϴ� ��ɾ ���� �� �ִ� �ܾ��� �ִ� ����



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
    unsigned type : 8;      //���� ����
    
    unsigned year : 8;      //���� ���� ����
    unsigned month : 8;     //���� ���� ����
    unsigned date : 8;      //���� ���� ����
    unsigned hour : 8;      //���� ���� ����
    unsigned minute : 8;    //���� ���� ����
    unsigned second : 8;    //���� ���� ����

    unsigned size : 32;     //���� ũ��

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

//�۾� ���丮 ��θ� ������ node ����ü
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

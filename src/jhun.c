#include "user.h"


/*
�̸�    : mypwd �Լ�
�ۼ���  : ������
���    : �۾� ���丮�� �����θ� ����Ѵ�
�޴°�  : X
���ϰ�  : X
*/
void mypwd(void)
{
    DIR_LIST *tmp_ptr = front_dir_list_ptr;

    int first = 0;

    while(1)
    {
        printf("%s", tmp_ptr->name);

        if(tmp_ptr->next_ptr == NULL)
        {
            break;
        }

        tmp_ptr = tmp_ptr->next_ptr;

        if(first == 0)
        {
            first++;
        }
        else
        {
            printf("/");
        }
    }

    return;
}


/*
�̸�    : myinode �Լ�
�ۼ���  : ������
���    : ������ inode�� ������ ����Ѵ�
�޴°�  : X
���ϰ�  : X
*/
void myinode(int inode)
{
    //���� ����
    FILE *myfs;
    myfs = fopen("myfs.bin", "rb");

    //inode ���� �б�
    INODE *inode_data_ptr = (INODE *)malloc(sizeof(INODE)); //inode ������ ������ �޸� ������ ����Ű�� ������

    fseek(myfs, BOOT_BLOCK_SIZE + (sizeof(INODE) * (inode - 1)), SEEK_SET);
    fread(inode_data_ptr, sizeof(INODE), 1, myfs);

    //inode ���� ����ϱ�
    //����
    printf("���� : ");
    if((inode_data_ptr->type) == 0)
    {
        printf("���丮\n");
    }
    else
    {
        printf("����\n");
    } 
    //��������
    printf("�������� : ");
    if(inode_data_ptr->year > 50)
    {
        printf("19%d/", inode_data_ptr->year);
    }
    else
    {
        printf("20%d/", inode_data_ptr->year);
    }
    printf("%02d/%02d ", inode_data_ptr->month, inode_data_ptr->date);
    printf("%02d:%02d:%02d\n", inode_data_ptr->hour, inode_data_ptr->minute, inode_data_ptr->second);

    //���⼭���� �̾ ¥��-------------------------------------------
    //ũ��
    //test
    printf("ũ�� : %d\n", inode_data_ptr->size);
    //��
    printf("���� ��� ��� : \n");




    //���� �ݱ�
    fclose(myfs);

    return;
}
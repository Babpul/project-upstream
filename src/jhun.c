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

    if (tmp_ptr == NULL)
    {
        printf("pwd�� ���� linked list�� �������� �ʽ��ϴ�.\n");
        abort();
    }

    int first = 0;

    while (1)
    {
        printf("%s", tmp_ptr->name);

        if (tmp_ptr->next_ptr == NULL)
        {
            break;
        }
        else
        {
            tmp_ptr = tmp_ptr->next_ptr;
        }

        if (first == 0)
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
void myinode(char *ptr)
{
    // ptr�� ����Ű�� ���ڿ��� ������ ��ȯ, �ùٸ� ������ �˻�
    int inode = 0;  // inode ��ȣ
    int figure = 0; // ptr�� ����Ű�� �ִ� ���ڿ��� �ڸ���
    int ptr_offset = 0;

    while (1)
    {
        if (*(ptr + ptr_offset) == 0)
        {
            break;
        }
        else
        {
            if ((*(ptr + ptr_offset) < 48) || (*(ptr + ptr_offset) > 57)) //���� ���ڰ� �ƴ� ���ڰ� �ִ� ���
            {
                printf("inode ��ȣ�� �߸��Ǿ����ϴ�.\n");
                return;
            }
            else
            {
                ptr_offset++;
                figure++;
            }
        }
    }

    ptr_offset = 0;

    for (int i = (figure - 1); i >= 0; i--)
    {
        inode += (*(ptr + ptr_offset) - 48) * ((int)pow(10, i));

        ptr_offset++;
    }

    if ((inode > 128) || (inode < 1)) // 1~128�� ������ �ƴ� ���
    {
        printf("inode ��ȣ�� �߸��Ǿ����ϴ�.\n");
        return;
    }

    //���� ����
    FILE *myfs;
    myfs = fopen("myfs.bin", "rb");
    if (myfs == NULL)
    {
        printf("myinode() �Լ� : ���� ���⿡ �����߽��ϴ�.\n");
        abort();
    }

    // myfs ���� �б�
    INODE *inode_data_ptr = (INODE *)malloc(sizeof(INODE)); // inode ������ ������ �޸� ������ ����Ű�� ������
    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + (sizeof(INODE) * (inode - 1)), SEEK_SET);
    fread(inode_data_ptr, sizeof(INODE), 1, myfs);

    SUPERBLOCK *sb_ptr = (SUPERBLOCK *)malloc(sizeof(SUPERBLOCK));
    fseek(myfs, BOOT_BLOCK_SIZE, SEEK_SET);
    fread(sb_ptr, sizeof(SUPERBLOCK), 1, myfs);

    // inode ��� ���� Ȯ���ϱ�
    unsigned mask;

    if (inode < 33) // inode_1�� ������ ����ִ� ���
    {
        mask = 1 << (inode - 1);
        if ((sb_ptr->inode_1 & mask) == 0)
        {
            printf("�ش� inode�� ��� ���� �ƴմϴ�.\n");
            return;
        }
    }
    else if (inode < 65) // inode_2�� ������ ����ִ� ���
    {
        mask = 1 << ((inode - 32) - 1);
        if ((sb_ptr->inode_2 & mask) == 0)
        {
            printf("�ش� inode�� ��� ���� �ƴմϴ�.\n");
            return;
        }
    }
    else if (inode < 97) // inode_3�� ������ ����ִ� ���
    {
        mask = 1 << ((inode - 64) - 1);
        if ((sb_ptr->inode_3 & mask) == 0)
        {
            printf("�ش� inode�� ��� ���� �ƴմϴ�.\n");
            return;
        }
    }
    else // inode_4�� ������ ����ִ� ���
    {
        mask = 1 << ((inode - 96) - 1);
        if ((sb_ptr->inode_4 & mask) == 0)
        {
            printf("�ش� inode�� ��� ���� �ƴմϴ�.\n");
            return;
        }
    }

    // inode ���� ����ϱ�
    //����
    printf("���� : ");
    if (inode_data_ptr->type == 0) //���� ������ 0�� ���
    {
        printf("���丮\n");
    }
    else //���� ������ 1�� ���
    {
        printf("����\n");
    }
    //��������
    printf("�������� : ");
    if (inode_data_ptr->year > 50)
    {
        printf("19%d/", inode_data_ptr->year);
    }
    else
    {
        printf("20%d/", inode_data_ptr->year);
    }
    printf("%02d/%02d ", inode_data_ptr->month, inode_data_ptr->date);
    printf("%02d:%02d:%02d\n", inode_data_ptr->hour, inode_data_ptr->minute, inode_data_ptr->second);

    //ũ��
    printf("ũ�� : %d\n", inode_data_ptr->size);

    // direct
    printf("���� ��� ��� : \n");

    if (inode_data_ptr->dir_1 != 0)
    {
        printf(" #0 ���� ������ ��� : %d\n", inode_data_ptr->dir_1 + 1);
    }
    if (inode_data_ptr->dir_2 != 0)
    {
        printf(" #1 ���� ������ ��� : %d\n", inode_data_ptr->dir_2 + 1);
    }
    if (inode_data_ptr->dir_3 != 0)
    {
        printf(" #2 ���� ������ ��� : %d\n", inode_data_ptr->dir_3 + 1);
    }
    if (inode_data_ptr->dir_4 != 0)
    {
        printf(" #3 ���� ������ ��� : %d\n", inode_data_ptr->dir_4 + 1);
    }
    if (inode_data_ptr->dir_5 != 0)
    {
        printf(" #4 ���� ������ ��� : %d\n", inode_data_ptr->dir_5 + 1);
    }
    if (inode_data_ptr->dir_6 != 0)
    {
        printf(" #5 ���� ������ ��� : %d\n", inode_data_ptr->dir_6 + 1);
    }
    if (inode_data_ptr->dir_7 != 0)
    {
        printf(" #6 ���� ������ ��� : %d\n", inode_data_ptr->dir_7 + 1);
    }
    if (inode_data_ptr->dir_8 != 0)
    {
        printf(" #7 ���� ������ ��� : %d\n", inode_data_ptr->dir_8 + 1);
    }

    // single indirect
    printf("���� ��� ��ȣ : %d\n", inode_data_ptr->indir);

    //���� �޸� �Ҵ� ���� �ݳ�
    free(inode_data_ptr);
    free(sb_ptr);

    //���� �ݱ�
    fclose(myfs);

    return;
}
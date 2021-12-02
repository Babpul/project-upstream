#include "user.h"


/*
�̸�    : myls �Լ�
�ۼ���  : �̿���
���    : ���� ��� �ڼ��� ����
�޴°�  : X
���ϰ�  : X
*/
void myls(const char *ptr)
{
    int inode = 0;  // inode ��ȣ
    int num = 0; // ptr�� ����Ű�� �ִ� ���ڿ��� �ڸ���
    int ptr_a = 0;

    while (1)
    {
        if (*(ptr + ptr_a) == 0)
        {
            break;
        }
        else
        {
            if ((*(ptr + ptr_a) < 48) || (*(ptr + ptr_a) > 57)) // ���� ���ڰ� �ƴ� ���ڰ� �ִ� ���
            {
                printf("inode ��ȣ�� �߸��Ǿ����ϴ�.\n");
                return;
            }
            else
            {
                ptr_a++;
                num++;
            }
        }
    }

    ptr_a = 0; // ���� ������ ���� �ʱ�ȭ

    for (int i = (num - 1); i >= 0; i--)
    {
        inode += (*(ptr + ptr_a) - 48) * (int_pow(10, i));

        ptr_a;
    }

    if ((inode < 1) || (inode > 128)) // 1~128�� ������ �ƴ� ���
    {
        printf("inode ��ȣ�� �߸��Ǿ����ϴ�.\n");
        return;
    }

    // ���� ����
    FILE *myfs;
    myfs = fopen("myfs", "rb");

    if(myfs == NULL)
    {
        printf("���� ���⿡ �����߽��ϴ�.\n");
        abort();
    }

    // superblock ���� �б�
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
            free(sb_ptr);
            fclose(myfs);
            return;
        }
    }
    else if (inode < 65) // inode_2�� ������ ����ִ� ���
    {
        mask = 1 << ((inode - 32) - 1);
        if ((sb_ptr->inode_2 & mask) == 0)
        {
            printf("�ش� inode�� ��� ���� �ƴմϴ�.\n");
            free(sb_ptr);
            fclose(myfs);
            return;
        }
    }
    else if (inode < 97) // inode_3�� ������ ����ִ� ���
    {
        mask = 1 << ((inode - 64) - 1);
        if ((sb_ptr->inode_3 & mask) == 0)
        {
            printf("�ش� inode�� ��� ���� �ƴմϴ�.\n");
            free(sb_ptr);
            fclose(myfs);
            return;
        }
    }
    else // inode_4�� ������ ����ִ� ���
    {
        mask = 1 << ((inode - 96) - 1);
        if ((sb_ptr->inode_4 & mask) == 0)
        {
            printf("�ش� inode�� ��� ���� �ƴմϴ�.\n");
            free(sb_ptr);
            fclose(myfs);
            return;
        }
    }


    while(inode < 128 + 1 ) // ���� ������Ʈ �ð�, ���� ����, inode ��ȣ, ���� ũ��
    {
        // inode list ���� �б�
        INODE *inode_ptr = (INODE *)malloc(sizeof(INODE)); 
        fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + (sizeof(INODE) * (inode - 1)), SEEK_SET);
        fread(inode_ptr, sizeof(INODE), 1, myfs);


        // ��������
        printf("    �������� : ");
        
        if (inode_ptr->year > 50)
        {
            printf(" %d/", inode_ptr->year);
        }
        else
        {
            printf(" %d/", inode_ptr->year);
        }
        printf("%d/%d ", inode_ptr->month, inode_ptr->date);
        printf("%d:%d:%d", inode_ptr->hour, inode_ptr->minute, inode_ptr->second);


        // ����
        printf("���� : ");

        if (inode_ptr->type == 0) // ���� ���� 0 = ���丮
        {
            printf(" ���丮");
        }
        else // ���� ���� 1 = �Ϲ� ����
        {
            printf(" ����");
        }


        // inode ��ȣ

        printf("    inode��ȣ : %d", inode);


        // ũ��
        printf("    ũ�� : %d\n", inode_ptr->size);

        free(inode_ptr);

        inode++;
    }
    
    free(sb_ptr);
    fclose(myfs);

    return;
}




/*
�̸�    : mycat �Լ�
�ۼ���  : �̿���
���    : ���� ���� ��� ��ɾ�
�޴°�  : X
���ϰ�  : X
*/
void mycat(char *file)
{
    FILE *myfs;
    myfs = fopen("myfs", "rb");

    INODE *inode_data = (INODE *)malloc(sizeof(INODE));
    int num = rear_dir_list_ptr->inode; //���� ���丮�� inode ��ȣ�� ������ ����
    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + sizeof(INODE) * (num - 1), SEEK_SET);
    fread(inode_data, sizeof(INODE), 1, myfs);
    int file_num = (inode_data->size / (8 + sizeof(int)));
        

    // ���� ã��
    int dir_inode = 0;

    INODE *f_inode = (INODE *)malloc(sizeof(INODE)); //���� ���丮 ���� ������ inode ������ ������ ����
    char *fn = (char *)malloc(sizeof(char) * 8);  // filename
    int *in = (int *)malloc(sizeof(int));// inodenumber

    for(int i = 0; i < file_num; i++) 
    {
        fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (DATA_BLOCK_SIZE * inode_data->dir_1) + (i * (8 + sizeof(int))), SEEK_SET);
        fread(fn, 8, 1, myfs); 
        fread(in, sizeof(int), 1, myfs); 

        if(strcmp(file, fn) == 0)
        {
            fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + (sizeof(INODE) * (*in - 1)), SEEK_SET);
            fread(f_inode, sizeof(INODE), 1, myfs);

            if(f_inode->type == 0) //���丮�� ���
            {
                printf("%s ��/�� ���丮�Դϴ�.\n", file);

                free(inode_data);
                free(fn);
                free(in);
                free(f_inode);

                return;
            }
            else //�Ϲ� ������ ���
            {
                int number = *in;
                if(number == 0)
                {
                    printf("%s ��/�� �������� �ʽ��ϴ�.\n", file);

                    return;
                }

                else // ���
                {
                    char *test;
                    fread(test,sizeof(file),1,myfs);
                    printf("%s", test);
                }


                break;   
            }
        }
    }

    free(inode_data);
    free(fn);
    free(in);
    free(f_inode);

    fclose(myfs);
    
    return;
}


/*
�̸�    : myshowfile �Լ�
�ۼ���  : �̿���
���    : ���� ������ �κ� ��� ��ɾ�
�޴°�  : X
���ϰ�  : X
*/
void myshowfile(char *num1, char *num2, char *file)
{
    FILE *myfs;
    myfs = fopen("myfs", "rb");

    INODE *inode_data = (INODE *)malloc(sizeof(INODE));

    int num = rear_dir_list_ptr->inode; //���� ���丮�� inode ��ȣ�� ������ ����
    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + sizeof(INODE) * (num - 1), SEEK_SET);
    fread(inode_data, sizeof(INODE), 1, myfs);
    int file_num = (inode_data->size / (8 + sizeof(int)));
        

    // ���� ã��
    int dir_inode = 0;

    INODE *f_inode = (INODE *)malloc(sizeof(INODE)); //���� ���丮 ���� ������ inode ������ ������ ����
    char *fn = (char *)malloc(sizeof(char) * 8);  // filename
    int *in = (int *)malloc(sizeof(int));// inodenumber

    for(int i = 0; i < file_num; i++) 
    {
        fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (DATA_BLOCK_SIZE * inode_data->dir_1) + (i * (8 + sizeof(int))), SEEK_SET);
        fread(fn, 8, 1, myfs); 
        fread(in, sizeof(int), 1, myfs); 

        if(strcmp(file, fn) == 0)
        {
            fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + (sizeof(INODE) * (*in - 1)), SEEK_SET);
            fread(f_inode, sizeof(INODE), 1, myfs);

            if(f_inode->type == 0) //���丮�� ���
            {
                printf("%s ��/�� ���丮�Դϴ�.\n", file);

                free(inode_data);
                free(fn);
                free(in);
                free(f_inode);

                return;
            }
            else //�Ϲ� ������ ���
            {
                int number = *in;

                if(number == 0) 
                {
                    printf("%s �� �������� �ʽ��ϴ�.\n", file);

                    return;
                }

                else //�κ� ���
                {
                    int first_num = atoi(num1);
                    int end_num = atoi(num2);

                    fseek(myfs,  first_num - 1, SEEK_CUR);
                    char *test;

                    fread(test, (end_num - first_num), 1, myfs);
                    printf("%s", test);
                }
                break;
            }
        }
    }

    free(inode_data);
    free(fn);
    free(in);
    free(f_inode);
    fclose(myfs);
    
    return;
}
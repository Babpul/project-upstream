#include "user.h"
/*
�̸� : stringtoint�Լ�
�ۼ��� : ���μ�
��� : ���ڿ� �����κ�ȯ
�޴°� : ���ڿ�
��°� : ����
*/
int stringtoint(char *byte)
{
    int n;
    if(strlen(byte) == 1)
    {
        n = (*(byte) - 48);
    }
    else if(strlen(byte) == 2)
    {
        n = ((*(byte) - 48) * 10) + (*(byte + 1) - 48);
    }
    else if(strlen(byte) == 3)
    {
        n = ((*(byte) - 48) * 100) + ((*(byte + 1) - 48) * 10) + (*(byte + 2) - 48);
    }
    else if(strlen(byte) == 4)
    {
        n = ((*(byte) - 48) * 1000) + ((*(byte + 1) - 48) * 100) + ((*(byte + 2) - 48) * 10) + (*(byte + 3) - 48);
    }
    return n;
}

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
�ۼ���  : ���μ�
���    : ���� ���� ��� ��ɾ�
�޴°�  : �����̸�
���ϰ�  : X
*/
void mycat(char *givenname)
{
    //����ó��
    if (givenname == NULL)
    {
        printf("���ϸ��� �ʿ��մϴ�.\n");
        return;
    }

    if (strlen(givenname) > 7)
    {
        printf("���ڿ��� �ʹ� ��ϴ�. (�ִ� 7����)\n");

        return;
    }

    FILE *myfs;
    myfs = fopen("myfs", "rb+");
    
    //���� ���͸� ���̳�� �ޱ�
    int saveinode = rear_dir_list_ptr->inode; //���� ���丮�� inode ��ȣ ����
    //���� ���丮�� inode ���� �ҷ�����
    INODE *presenti_data = (INODE *)malloc(sizeof(INODE));
    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + 20 * (saveinode - 1), SEEK_SET);
    fread(presenti_data, sizeof(INODE), 1, myfs);
    
    //���ϸ��� ���� ������ �ִ��� �˻�
    char *filename = (char *)malloc(sizeof(char) * 8);
    int *inodenumber = (int *)malloc(sizeof(int));
    int saveinumber;
    unsigned count = 0;

    int dir_file_num = presenti_data->size / (8 + sizeof(int));

    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (DATA_BLOCK_SIZE * presenti_data->dir_1), SEEK_SET);
    for (int i = 0; i < dir_file_num; i++)
    {
        fread(filename, 8, 1, myfs);
        fread(inodenumber, sizeof(int), 1, myfs);

        if (strcmp(givenname, filename) == 0)
        {
            saveinumber = *inodenumber;
            break;
        }
        else
        {
            count++;
        }
    }
    //���� ������ ���̳�� �ޱ�
    INODE *i_data = (INODE *)malloc(sizeof(INODE));
    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + 20 * (saveinumber - 1), SEEK_SET);
    fread(i_data, sizeof(INODE), 1, myfs);

    //�������ϳ��� �о ���
    char *datablock_ptr = (char *)malloc(sizeof(char));
    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (DATA_BLOCK_SIZE * (i_data-> dir_1)), SEEK_SET);
    for(int i = 0; i < i_data-> size; i++)
    {
        fread(datablock_ptr, sizeof(char), 1, myfs);

        if(*datablock_ptr == -1)
        {
            break;
        }
        else
        {
            printf("%c", *datablock_ptr);
        }
    }
    
    free(presenti_data);
    free(filename);
    free(inodenumber);
    free(i_data);
    free(datablock_ptr);
    return;
}

/*
�̸�    : myshowfile �Լ�
�ۼ���  : ���μ�
���    : ���� ������ �κ� ��� ��ɾ�
�޴°�  : ����Ʈ���۰�, ����Ʈ���ᰪ, ���ϸ�
���ϰ�  : X
*/
void myshowfile(char *startbyte, char *endbyte, char *givenname)
{
    //���ڿ� ������ �ٲٱ�
    int intstartbyte = stringtoint(startbyte);
    int intendbyte = stringtoint(endbyte);
    printf("%d\n%d\n", intstartbyte, intendbyte);

    //����ó��
    if (givenname == NULL)
    {
        printf("���ϸ��� �ʿ��մϴ�.\n");
        return;
    }

    if (strlen(givenname) > 7)
    {
        printf("���ڿ��� �ʹ� ��ϴ�. (�ִ� 7����)\n");

        return;
    }

    FILE *myfs;
    myfs = fopen("myfs", "rb+");
    
    //���� ���͸� ���̳�� �ޱ�
    int saveinode = rear_dir_list_ptr->inode; //���� ���丮�� inode ��ȣ ����
    //���� ���丮�� inode ���� �ҷ�����
    INODE *presenti_data = (INODE *)malloc(sizeof(INODE));
    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + 20 * (saveinode - 1), SEEK_SET);
    fread(presenti_data, sizeof(INODE), 1, myfs);
    
    //���ϸ��� ���� ������ �ִ��� �˻�
    char *filename = (char *)malloc(sizeof(char) * 8);
    int *inodenumber = (int *)malloc(sizeof(int));
    int saveinumber;
    unsigned count = 0;

    int dir_file_num = presenti_data->size / (8 + sizeof(int));

    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (DATA_BLOCK_SIZE * presenti_data->dir_1), SEEK_SET);
    for (int i = 0; i < dir_file_num; i++)
    {
        fread(filename, 8, 1, myfs);
        fread(inodenumber, sizeof(int), 1, myfs);

        if (strcmp(givenname, filename) == 0)
        {
            saveinumber = *inodenumber;
            break;
        }
        else
        {
            count++;
        }
    }
    //���� ������ ���̳�� �ޱ�
    INODE *i_data = (INODE *)malloc(sizeof(INODE));
    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + 20 * (saveinumber - 1), SEEK_SET);
    fread(i_data, sizeof(INODE), 1, myfs);

    //Ư���κ� ���� �� ���
    char *datablock_ptr = (char *)malloc(sizeof(char));
    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (DATA_BLOCK_SIZE * (i_data-> dir_1)) + (intstartbyte - 1), SEEK_SET);
    for(int i = intstartbyte; i < intendbyte; i++)
    {
        fread(datablock_ptr, sizeof(char), 1, myfs);

        if(*datablock_ptr == -1)
        {
            break;
        }
        else
        {
            printf("%c", *datablock_ptr);
        }
    }
    
    free(presenti_data);
    free(filename);
    free(inodenumber);
    free(i_data);
    free(datablock_ptr);
    return;
}
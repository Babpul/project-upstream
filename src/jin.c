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
�޴°�  : ���� ��
���ϰ�  : X
*/
void myls(const char* file)
{
    // ���� ����
    FILE *myfs;
    myfs = fopen("myfs", "rb");

    //���� ���丮���� ���ڷ� �Է¹��� ���丮���� inode ��ȣ ã��

    int saveinode, count, saveinumber;
    int *fileinode = (int *)malloc(sizeof(int));
    saveinode = rear_dir_list_ptr->inode;

    INODE *presenti_data = (INODE *)malloc(sizeof(INODE));
    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + 20 * (saveinode - 1), SEEK_SET);
    fread(presenti_data, sizeof(INODE), 1, myfs);

    int n = presenti_data->size / (8 + sizeof(int));
    char *filename = (char *)malloc(sizeof(char) * 8);

    int none_tmp = 0;

    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (DATA_BLOCK_SIZE * (presenti_data->dir_1)), SEEK_SET);
    for (int i = 0; i < n; i++)
    {
        fread(filename, 8, 1, myfs);
        fread(fileinode, sizeof(int), 1, myfs);

        if (strcmp(file, filename) == 0)
        {
            count = i;
            break;
        }
        else
        {
            none_tmp++;
        }
    }

    if (none_tmp == n) //�ش� �̸��� ������ �������� �ʴ� ���
    {
        printf("�ش� �̸��� ������ �������� �ʽ��ϴ�.\n");

        return;
    }
    else //�ش� �̸��� ������ �����ϴ� ���
    {
        //�ش� ������ �Ϲ� �������� �˻�
        INODE *file_inode_tmp_ptr = (INODE *)malloc(sizeof(INODE));
        fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + sizeof(INODE) * (*fileinode - 1), SEEK_SET);
        fread(file_inode_tmp_ptr, sizeof(INODE), 1, myfs);
        if (file_inode_tmp_ptr->type == 1) //�ش� ������ �Ϲ� ������ ���
        {
            printf("�ش� ������ ���丮�� �ƴմϴ�.\n");

            free(file_inode_tmp_ptr);
            free(fileinode);
            free(presenti_data);
            free(filename);

            return;
        }
        else //�ش� ������ ���丮�� ���
        {
            free(file_inode_tmp_ptr);
        }
    }

    //�ش� inode�� datablock ��ȣ ã��

    INODE *file_d = (INODE *)malloc(sizeof(INODE));
    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + sizeof(INODE) * (*fileinode - 1), SEEK_SET);
    fread(file_d, sizeof(INODE), 1, myfs);
    int data_num = (int)(file_d->dir_1 + 1);


    // datablock���� ������ ���� ã�Ƴ��� �� ���� ��� ������ inode�� ���λ��� ����

    n = file_d->size / (8 + sizeof(int));


    for(int i = count; i < n; i++)
    {
        int i = 0;
        fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + ((DATA_BLOCK_SIZE * (data_num)) + i), SEEK_SET);
        fread(filename, 8, 1, myfs);
        fread(fileinode, sizeof(int), 1, myfs);

        // ���ϸ�
        printf("%s" ,filename);

        // inode list ���� �б�
        fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + (sizeof(INODE) * (*fileinode - 1)), SEEK_SET);
        INODE *inode_ptr = (INODE *)malloc(sizeof(INODE));
        fread(inode_ptr, sizeof(INODE), 1, myfs);

        // ��������
        printf("    �������� : ");
        
        if (inode_ptr->year > 50)
        {
            printf(" 19%d/", inode_ptr->year);
        }
        else
        {
            printf(" 20%d/", inode_ptr->year);
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
        printf("    inode��ȣ : %d", inode_ptr);


        // ũ��
        printf("    ũ�� : %d\n", inode_ptr->size);

        fseek(myfs, -1, SEEK_CUR);
        fseek(myfs,sizeof(int),SEEK_CUR);

        i++;
    }


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